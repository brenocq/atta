//--------------------------------------------------
// Atta Algorithms - Image Processing
// jpeg.cpp
// Date: 2021-07-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/computerVision/jpeg.h>

#define _USE_MATH_DEFINES
#ifdef ATTA_OS_WINDOWS
#define M_PI 3.14159265368979
#endif

#include <cmath>

namespace atta::cv {

// IDCT scaling factors
const float m0 = 2.0*std::cos(1.0/16.0*2.0*M_PI);
const float m1 = 2.0*std::cos(2.0/16.0*2.0*M_PI);
const float m3 = 2.0*std::cos(2.0/16.0*2.0*M_PI);
const float m5 = 2.0*std::cos(3.0/16.0*2.0*M_PI);
const float m2 = m0-m5;
const float m4 = m0+m5;

const float s0 = std::cos(0.0/16.0*M_PI)/std::sqrt(8);
const float s1 = std::cos(1.0/16.0*M_PI)/2.0;
const float s2 = std::cos(2.0/16.0*M_PI)/2.0;
const float s3 = std::cos(3.0/16.0*M_PI)/2.0;
const float s4 = std::cos(4.0/16.0*M_PI)/2.0;
const float s5 = std::cos(5.0/16.0*M_PI)/2.0;
const float s6 = std::cos(6.0/16.0*M_PI)/2.0;
const float s7 = std::cos(7.0/16.0*M_PI)/2.0;

const size_t zigZagMap [] = {
	0,  1,   8, 16,  9,  2,  3, 10,
	17,	24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63
};

JPEG::JPEG()
	: _width(0), _height(0), _numComponents(0), _restartInterval(0), _zeroBased(false),
	_startOfSelection(0), _endOfSelection(63),
	_successiveApproximationHigh(0), _successiveApproximationLow(0),
	_horizontalSamplingFactor(1), _verticalSamplingFactor(1),
	_mcuWidth(0), _mcuHeight(0), _mcuWidthReal(0), _mcuHeightReal(0)
{

}

const std::vector<uint8_t>& JPEG::decode(const std::vector<uint8_t>& data) {
    size_t i = 0;// Current data offset

    bool finished = false;
    while(!finished && i<data.size()) {
        unsigned marker = data[i++]<<8 | data[i++];
        //LOG_DEBUG("imgproc::JPEG", "Marker $0", marker);
		if(marker == MARKER_START_OF_IMAGE) {
            //LOG_DEBUG("imgproc::JPEG", "Marker MARKER_START_OF_IMAGE");
		} else if(marker == MARKER_END_OF_IMAGE) {
            //LOG_DEBUG("imgproc::JPEG", "Marker MARKER_END_OF_IMAGE");
			finished = true;
		} else if(marker >= MARKER_APP0 && marker <= MARKER_APP15) {
			if(!readApplicationSegment(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not read application segment");
				return _decoded;
			}
		} else if(marker == MARKER_QUANTIZATION_TABLE) {
			if(!readQuantizationTable(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not read quantization table");
				return _decoded;
			}
		} else if(marker == MARKER_HUFFMAN_TABLE) {
			if(!readHuffmanTable(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not read huffman table");
				return _decoded;
			}
		} else if(marker == MARKER_START_OF_SCAN) {
			if(!readStartOfScan(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not read start of scan segment");
				return _decoded;
			}
			if(!loopScan(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not run loop scan");
				return _decoded;
			}
		} else if(marker == MARKER_START_OF_FRAME) {
			if(!readStartOfFrame(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not read start of frame segment");
				return _decoded;
			}
		} else if(marker == MARKER_RESTART_INTERVAL) {
			if(!readRestartInterval(data, i)) {
				LOG_ERROR("imgproc::JPEG", "Could not read restart interval segment");
				return _decoded;
			}
		} else if((marker >= MARKER_JPG0 && marker <= MARKER_JPG13) ||
				marker == MARKER_COMMENT ||
				marker == MARKER_NUMBER_OF_LINES ||
				marker == MARKER_HIERARCHICAL_PROGRESSION ||
				marker == MARKER_EXPAND_REFERENCE_COMP) {
			readComment(data, i);
		} else if(marker == MARKER_TEM){
			// No size
		} else if(marker == 0xffff){
			// Read next marker starting with ff
			i--;
		} else {
			LOG_WARN("imgproc::JPEG", "Unknown marker found: $0", marker);
			finished = true;
			return _decoded;
		}
    }

	if(_numComponents != 1 && _numComponents != 3) {
		LOG_ERROR("imgproc::JPEG", "$0 color components given, must be 1 or 3", _numComponents);
		return _decoded;
	}

	for(size_t j = 0; j < _numComponents; j++) {
		if(_quantizationTables[_colorComponents[j].quantizationTableId].defined == false) {
			LOG_ERROR("imgproc::JPEG", "Color component $0 using unitialized Quantization table", _numComponents);
			return _decoded;
		}
		if(_huffmanDCTables[_colorComponents[j].huffmanDCTableId].defined == false) {
			LOG_ERROR("imgproc::JPEG", "Color component $0 using unitialized Huffman DC table", _numComponents);
			return _decoded;
		}
		if(_huffmanACTables[_colorComponents[j].huffmanACTableId].defined == false) {
			LOG_ERROR("imgproc::JPEG", "Color component $0 using unitialized Huffman AC table", _numComponents);
			return _decoded;
		}
	}

	if(!decodeHuffmanData()) {
		LOG_ERROR("imgproc::JPEG", "Could not decode Huffman data");
		return _decoded;
	}

	// Dequantize MCU coefficients
	dequantizeMCUs();

	// Inverse Discrete Cosine Transform
	inverseDCT();

	// Color conversion YCbCr -> RGB
	YCbCrToRBG();

	// MCUs to decoded
	MCUToDecoded();

    return _decoded;
}

bool JPEG::readQuantizationTable(const std::vector<uint8_t>& data, size_t& i) {
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_QUANTIZATION_TABLE");
	int length = (data[i++]<<8) | data[i++];
	length-=2;

	while(length > 0) {
		uint8_t infoQT = data[i++];
		length--;
		size_t idxQT = infoQT&0x0f;
		if(idxQT > 3) {
			LOG_ERROR("imgproc::JPEG", "Invalid quantization table id: $0", idxQT);
			return false;
		}
		_quantizationTables[idxQT].defined = true;
		//LOG_DEBUG("imgproc::JPEG", "Reading quantization table $0", idxQT);

		if(infoQT>>4 != 0) {
			for(size_t j = 0; j < 64; j++)
				_quantizationTables[idxQT].table[zigZagMap[j]] = (data[i++]<<8) | data[i++];
			length-=128;
		} else {
			for(size_t j = 0; j < 64; j++)
				_quantizationTables[idxQT].table[zigZagMap[j]] = data[i++];
			length-=64;
		}
	}

	if(length != 0) {
		LOG_ERROR("imgproc::JPEG", "Unexpected quantization table length");
		return false;
	}

	/*for(size_t j=0;j<4;j++)
		if(_quantizationTables[j].defined) {
			std::cout << "Table " << j << ":" << std::endl;
			for(size_t k=0;k<64;k++) {
				if(k%8==0) std::cout << std::endl;
				std::cout << (int)_quantizationTables[j].table[k] << " ";
			}
			std::cout << std::endl;
		}*/

	return true;
}

bool JPEG::readApplicationSegment(const std::vector<uint8_t>& data, size_t& i) {
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_APPLICATION_SEGMENT");
	unsigned length = (data[i++]<<8) | data[i++];
	i+=length-2;
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_APPLICATION_SEGMENT $0", (data[i]<<8)|data[i+1]);
	return true;
}

bool JPEG::readComment(const std::vector<uint8_t>& data, size_t& i) {
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_COMMENT (or ignored)");
	unsigned length = (data[i++]<<8) | data[i++];
	i+=length-2;
	return true;
}

bool JPEG::readHuffmanTable(const std::vector<uint8_t>& data, size_t& i) {
	int length = (data[i++]<<8) | data[i++];
	length -= 2;

	while(length > 0) {
		uint8_t infoHT = data[i++];
		uint8_t idxHT = infoHT & 0x0f;// 0->Luma; 1->Chroma
		bool ACTable = infoHT >> 4;// false->DC table; true->AC table

		if(idxHT > 3) {
			LOG_ERROR("imgproc::JPEG", "Invalid huffman table index: $0", idxHT);
			return false;
		}

		HuffmanTable* hTable = ACTable ? &_huffmanACTables[idxHT] : &_huffmanDCTables[idxHT];
		hTable->defined = true;

		// Populate offsets (starting index of symbols of each length)
		hTable->offsets[0] = 0;
		uint8_t allSymbols = 0;
		for(size_t j=1;j<=16;j++) {
			allSymbols += data[i++];
			hTable->offsets[j] = allSymbols;
		}
		if(allSymbols>162) {
			LOG_ERROR("imgproc::JPEG", "Too many symbols in Huffman table: $0", allSymbols);
			return false;
		}

		for(size_t j=0;j<allSymbols;j++)
			hTable->symbols[j] = data[i++];

		length -= 17 + allSymbols;
	}
	if(length < 0) {
		LOG_ERROR("imgproc::JPEG", "More bytes than expected on the Huffman Table segment");
		return false;
	}

	// Create symbols string
	std::string symbolsStr;
	/*for(size_t j=0;j<4;j++) {
		if(_huffmanDCTables[j].defined) {
			symbolsStr += "DC "+std::to_string(j)+"\nSymbols:[";
			for(size_t k=0;k<16;k++) {
				for(size_t l=_huffmanDCTables[j].offsets[k];l<_huffmanDCTables[j].offsets[k+1]; l++)
					symbolsStr += std::to_string(_huffmanDCTables[j].symbols[l])+" ";
				symbolsStr += "\n";
			}
			symbolsStr += "]\n";
		}
		if(_huffmanACTables[j].defined) {
			symbolsStr += "AC "+std::to_string(j)+"\nSymbols:[";
			for(size_t k=0;k<16;k++) {
				for(size_t l=_huffmanACTables[j].offsets[k];l<_huffmanACTables[j].offsets[k+1]; l++)
					symbolsStr += std::to_string(_huffmanACTables[j].symbols[l])+" ";
				symbolsStr += "\n";
			}
			symbolsStr += "]\n";
		}
	}*/
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_DEFINE_HUFFMAN_TABLE \n$0", symbolsStr);
	return true;
}

bool JPEG::readStartOfScan(const std::vector<uint8_t>& data, size_t& i) {
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_START_OF_SCAN");
	if(_numComponents == 0) {
		LOG_ERROR("imgproc::JPEG", "Start of Scan before Start of Frame");
		return false;
	}
	unsigned length = (data[i++]<<8) | data[i++];
	for(size_t j = 0; j < _numComponents; j++)
		_colorComponents[j].defined = false;

	uint8_t numComponents = data[i++];
	for(size_t j = 0; j < numComponents; j++) {
		size_t componentId = data[i++];
		if(_zeroBased) componentId++;

		if(componentId > _numComponents) {
			LOG_ERROR("imgproc::JPEG", "Invalid color component Id: $0", componentId);
			return false;
		}

		ColorComponent* component = &_colorComponents[componentId-1];
		if(component->defined) {
			LOG_ERROR("imgproc::JPEG", "Duplicate component: $0", componentId);
			return false;
		}
		component->defined = true;

		uint8_t huffmanTableId = data[i++];
		component->huffmanDCTableId = huffmanTableId >> 4;
		component->huffmanACTableId = huffmanTableId & 0x0f;

		if(component->huffmanDCTableId > 3) {
			LOG_ERROR("imgproc::JPEG", "Invalid DC Table index: $0", component->huffmanDCTableId);
			return false;
		}

		if(component->huffmanACTableId > 3) {
			LOG_ERROR("imgproc::JPEG", "Invalid AC Table index: $0", component->huffmanACTableId);
			return false;
		}
	}

	_startOfSelection = data[i++];
	_endOfSelection = data[i++];
	uint8_t successiveApproximation = data[i++];
	_successiveApproximationHigh = successiveApproximation >> 4;
	_successiveApproximationLow = successiveApproximation & 0x0f;

	// Baseline JPEGs don't use spectral selection or successive approximation
	if(_startOfSelection != 0 || _endOfSelection != 63) {
		LOG_ERROR("imgproc::JPEG", "Invalid spectral selection");
		return false;
	}
	if(_successiveApproximationHigh != 0 || _successiveApproximationLow != 0) {
		LOG_ERROR("imgproc::JPEG", "Invalid successive approximation");
		return false;
	}

	if(length != 6+2*numComponents) {
		LOG_ERROR("imgproc::JPEG", "Invalid start of scan length");
		return false;
	}

	return true;
}

bool JPEG::readStartOfFrame(const std::vector<uint8_t>& data, size_t& i) {
	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_START_OF_FRAME");
	if(_numComponents != 0) {
		LOG_ERROR("imgproc::JPEG", "Multiple start of frame found!");
		return false;
	}

	unsigned length = (data[i++]<<8) | data[i++];

	//----- Precision -----//
	unsigned precision = data[i++];
	if(precision != 8) {
		LOG_ERROR("imgproc::JPEG", "Invalid precision: $0", precision);
		return false;
	}

	//----- Width/Height -----//
	_height = data[i++]<<8 | data[i++];
	_width = data[i++]<<8 | data[i++];
	_mcuHeight = (_height+7)/8;
	_mcuWidth = (_width+7)/8;
	_mcuHeightReal = _mcuHeight;
	_mcuWidthReal = _mcuWidth;
	//Log::info("imgproc::JPEG", "Start of frame: $0x$1", _width, _height);

	if(_height == 0 || _width == 0) {
		LOG_ERROR("imgproc::JPEG", "Invalid size: $0x$1", _width, _height);
		return false;
	}

	//----- Number of components -----//
	_numComponents = data[i++];
	if(_numComponents == 4) {
		LOG_ERROR("imgproc::JPEG", "CMYK color mode not supported");
		return false;
	}
	if(_numComponents == 0) {
		LOG_ERROR("imgproc::JPEG", "Number of color components must not be zero");
		return false;
	}

	for(size_t j=0;j<_numComponents;j++) {
		size_t componentId = data[i++];
		if(componentId == 4 || componentId == 5) {
			LOG_ERROR("imgproc::JPEG", "YIQ color mode not supported");
			return false;
		}

		if(componentId == 0) _zeroBased = true;
		if(_zeroBased) componentId++;

		if(componentId > 3) {
			LOG_ERROR("imgproc::JPEG", "Invalid color component Id: $0", componentId);
			return false;
		}

		ColorComponent* component = &_colorComponents[componentId-1];
		if(component->defined) {
			LOG_ERROR("imgproc::JPEG", "Duplicate component definition: $0", componentId);
			return false;
		}
		component->defined = true;
		uint8_t samplingFactor = data[i++];
		component->horizontalSamplingFactor = samplingFactor>>4;
		component->verticalSamplingFactor = samplingFactor&0x0f;
		component->quantizationTableId = data[i++];

		if(componentId == 1) {
			if((component->horizontalSamplingFactor != 1 && component->horizontalSamplingFactor != 2) ||
				(component->verticalSamplingFactor != 1 && component->verticalSamplingFactor != 2)) {
				LOG_ERROR("imgproc::JPEG", "Luma sampling factors not supported: h:$0 v:$1", component->horizontalSamplingFactor, component->verticalSamplingFactor);
				return false;
			}
			// Add padding if necessary
			if(component->horizontalSamplingFactor == 2 && _mcuWidth%2 == 1)
				_mcuWidthReal++;
			if(component->verticalSamplingFactor == 2 && _mcuHeight%2 == 1)
				_mcuHeightReal++;
			_horizontalSamplingFactor = component->horizontalSamplingFactor;
			_verticalSamplingFactor = component->verticalSamplingFactor;
		} else {
			if(component->horizontalSamplingFactor != 1 || component->verticalSamplingFactor != 1) {
				LOG_ERROR("imgproc::JPEG", "Chroma channel sampling factors not supported: h:$0 v:$1", component->horizontalSamplingFactor, component->verticalSamplingFactor);
				return false;
			}
		}

		if(component->quantizationTableId > 3) {
			LOG_ERROR("imgproc::JPEG", "Invalid quantization table id($0) in frame components($1)", component->quantizationTableId, componentId);
			return false;
		}

		//LOG_DEBUG("imgproc::JPEG", "ComponentId: $0, hor:$1, ver:$2, table:$3 $4", componentId, 
			//component->horizontalSamplingFactor, component->verticalSamplingFactor, component->quantizationTableId, _numComponents);
	}

	if(length != (8+(3*_numComponents))) {
		LOG_ERROR("imgproc::JPEG", "Invalid length for SOF (start of frame) segment $0!=$1", (int)length, (int)(8+(3*_numComponents)));
		return false;
	}

	return true;
}

bool JPEG::readRestartInterval(const std::vector<uint8_t>& data, size_t& i) {
	unsigned length = (data[i++]<<8) | data[i++];
	if(_restartInterval > 0) {
		LOG_ERROR("imgproc::JPEG", "Duplicated restart interval");
		return false;
	}

	_restartInterval = (data[i++]<<8) | data[i++];

	//if(_restartInterval == 0) {
	//	_restartInterval = (unsigned)-1;
	//	LOG_WARN("imgproc::JPEG", "Restart interval read as 0");
	//	//return false;
	//}

	//LOG_DEBUG("imgproc::JPEG", "Marker MARKER_RESTART_INTERVAL value:$0", _restartInterval);
	return true;
}

bool JPEG::loopScan(const std::vector<uint8_t>& data, size_t& i) {
	uint8_t last, current;
	current = data[i++];
	while(true)	{
		last = current;
		current = data[i++];

		if(last == 0xff) {
			if(current == uint8_t(MARKER_END_OF_IMAGE&0x00ff)) {
				i-=2;
				////Log::info("imgproc::JPEG", "Loop scan end");
				return true;
			} else if(current == 0x00) {
				_huffmanData.push_back(last);
				// Overwrite 0x00 with next byte
				current = data[i++];
				////Log::info("imgproc::JPEG", "Loop scan actual ff");
			}else if(current >= (MARKER_RESET0&0x00ff) && current <= (MARKER_RESET7&0x00ff)) {
				////Log::info("imgproc::JPEG", "Loop scan marker found");
				current = data[i++];
			} else if(current == 0xff) {
				////Log::info("imgproc::JPEG", "Loop scan ignore multiple ff");
				continue;
			} else {
				//LOG_ERROR("imgproc::JPEG", "Invalid marker during compressed data scan: $0", current);
				return false;
			}
		} else {
			_huffmanData.push_back(last);
		}

		if(i==data.size()) {
			LOG_ERROR("imgproc::JPEG", "JPEG data finished before End of File marker $0 $1", last, current);
			return false;
		}
	}

	return true;
}

bool JPEG::decodeHuffmanData() {
	// Decode all Huffman data and fill all MCUs
	_mcus.resize(_mcuHeightReal*_mcuWidthReal);

	for(size_t i = 0; i < 4; i++) {
		if(_huffmanDCTables[i].defined)
			generateHuffmanCodes(_huffmanDCTables[i]);
		if(_huffmanACTables[i].defined)
			generateHuffmanCodes(_huffmanACTables[i]);
	}

	int previousDCs[3] = {0};

	BitReader b(_huffmanData);

	// Restarting taking into account MCU blocks bigger than 8x8
	unsigned restartInterval = _restartInterval*_horizontalSamplingFactor*_verticalSamplingFactor;

	//LOG_DEBUG("JPEG", "Num mcus: $0", _mcus.size());
	for(size_t y = 0; y < _mcuHeight; y+=_verticalSamplingFactor) {
		for(size_t x = 0; x < _mcuWidth; x+=_horizontalSamplingFactor) {
			////LOG_DEBUG("JPEG", "curr mcu: $0", (y*_mcuWidthReal+x));
			// Restart previousDC
			if(restartInterval != 0 && (y*_mcuWidthReal+x)%restartInterval == 0) {
				////LOG_DEBUG("JPEG", "Reset now");
				previousDCs[0] = 0;
				previousDCs[1] = 0;
				previousDCs[2] = 0;
				b.align();
			}

			for(size_t i = 0; i < _numComponents; i++) {
				for(size_t v = 0; v < _colorComponents[i].verticalSamplingFactor; v++) {
					for(size_t h = 0; h < _colorComponents[i].horizontalSamplingFactor; h++) {
						if(!decodeMCUComponent(
							b, 
							_mcus[(y+v)*_mcuWidthReal+(x+h)][i],
							previousDCs[i],
							_huffmanDCTables[_colorComponents[i].huffmanDCTableId],
							_huffmanACTables[_colorComponents[i].huffmanACTableId])) {
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

void JPEG::generateHuffmanCodes(HuffmanTable& table) {
	unsigned code = 0;
	for(size_t i = 0; i < 16; i++)
	{
		for(size_t j = table.offsets[i]; j < table.offsets[i+1]; j++) {
			table.codes[j] = code;
			code++;
		}
		code <<= 1;
	}
}

bool JPEG::decodeMCUComponent(BitReader &b, int* const component, int& previousDC, const HuffmanTable& dcTable, const HuffmanTable& acTable) {
	// Fill the coefficients of an MCU component base on Huffman codes read from BitReader
	// Get DC value for this MCU component
	uint8_t length = getNextSymbol(b, dcTable);
	if(length == (uint8_t)-1) {
		LOG_ERROR("imgproc::JPEG", "Invalid DC length");
		return false;
	}
	if(length > 11) {
		LOG_ERROR("imgproc::JPEG", "DC coefficient length greater than 11");
		return false;
	}
	
	// Read DC coefficient
	int coeff = b.readBits(length);
	if(coeff == -1) {
		LOG_ERROR("imgproc::JPEG", "Invalid DC value");
		return false;
	}
	if(length != 0 && coeff < (1<<(length-1))) {
		coeff -= (1<<length)-1;
	}
	component[0] = coeff+previousDC;
	previousDC = component[0];

	// Read 63 AC coefficients
	unsigned i = 1;
	while(i<64) {
		uint8_t symbol = getNextSymbol(b, acTable);
		if(symbol == (uint8_t)-1) {
			LOG_ERROR("imgproc::JPEG", "Invalid AC length");
			return false;
		}
		
		// Special symbol 0x00 means fill remainder of component with 0
		if(symbol == 0x00) {
			for(; i < 64; i++) {
				component[zigZagMap[i]] = 0;
			}
			return true;
		}

		// Otherwise, read next component coefficient
		uint8_t numZeros = symbol >> 4;
		uint8_t coeffLength = symbol & 0x0f;
		coeff = 0;

		// Special symbol 0xf0 means skip 16 0's
		if(symbol == 0xf0)
			numZeros = 16;
		
		if(i+numZeros >= 64) {
			LOG_ERROR("imgproc::JPEG", "Zero run-length exceeded MCU");
			return false;
		}

		// Fill zeros
		for(size_t j = 0; j < numZeros; j++, i++)
			component[zigZagMap[i]] = 0;
		
		if(coeffLength > 10) {
			LOG_ERROR("imgproc::JPEG", "AC coefficient length greater than 10");
			return false;
		}

		// Read AC value
		if(coeffLength != 0) {
			coeff = b.readBits(coeffLength);
			if(coeff == -1) {
				LOG_ERROR("imgproc::JPEG", "Could not read AC value");
				return false;
			}
			if(coeff < (1<<(coeffLength-1)))
				coeff -= (1<<coeffLength)-1;
			component[zigZagMap[i]] = coeff;
			i++;
		}
	}
	return true;
}

uint8_t JPEG::getNextSymbol(BitReader &b, const HuffmanTable& table) {
	// Return the symbol from Huffman table that corresponds to the next Huffman code read from the BitReader
	unsigned currentCode = 0;
	// Test each length
	for(size_t i = 0; i < 16; i++) {
		int bit = b.readBit();
		if(bit == -1) {
			LOG_WARN("imgproc::JPEG", "Could not read bit");
			return -1;
		}
		currentCode = (currentCode<<1)|bit;
		// Test if found a match with that length
		for(size_t j = table.offsets[i]; j < table.offsets[i+1]; j++) {
			if(currentCode == table.codes[j]) {
				//Log::success("imgproc::JPEG", "Found $0", currentCode);
				return table.symbols[j];
			}
		}
	}

	LOG_WARN("imgproc::JPEG", "Could not match the symbol $0", currentCode);
	return -1;
}

void JPEG::dequantizeMCUs() {
	for(size_t y = 0; y < _mcuHeight; y+=_verticalSamplingFactor)
		for(size_t x = 0; x < _mcuWidth; x+=_horizontalSamplingFactor)
			for(size_t i = 0; i < _numComponents; i++)
				for(size_t v = 0; v < _colorComponents[i].verticalSamplingFactor; v++)
					for(size_t h = 0; h < _colorComponents[i].horizontalSamplingFactor; h++)
						dequantizeMCUComponent(_quantizationTables[_colorComponents[i].quantizationTableId], _mcus[(y+v)*_mcuWidthReal+(x+h)][i]);
}

void JPEG::dequantizeMCUComponent(const QuantizationTable& qTable, int* const component) {
	for(size_t i = 0; i < 64; i++)
		component[i] *= qTable.table[i];
}

void JPEG::inverseDCT() {
	for(size_t y = 0; y < _mcuHeight; y+=_verticalSamplingFactor)
		for(size_t x = 0; x < _mcuWidth; x+=_horizontalSamplingFactor)
			for(size_t i = 0; i < _numComponents; i++)
				for(size_t v = 0; v < _colorComponents[i].verticalSamplingFactor; v++)
					for(size_t h = 0; h < _colorComponents[i].horizontalSamplingFactor; h++)
						inverseDCTComponent(_mcus[(y+v)*_mcuWidthReal+(x+h)][i]);
}

void JPEG::inverseDCTComponent(int* const component) {
	float result[64] = {0};
	// Compute inverse DCT using AAN algorithm
	
	// For each column
	for(size_t i = 0; i < 8; i++) {
		const float g0 = component[0*8+i]*s0;
		const float g1 = component[4*8+i]*s4;
		const float g2 = component[2*8+i]*s2;
		const float g3 = component[6*8+i]*s6;
		const float g4 = component[5*8+i]*s5;
		const float g5 = component[1*8+i]*s1;
		const float g6 = component[7*8+i]*s7;
		const float g7 = component[3*8+i]*s3;

		const float f0 = g0;
		const float f1 = g1;
		const float f2 = g2;
		const float f3 = g3;
		const float f4 = g4-g7;
		const float f5 = g5+g6;
		const float f6 = g5-g6;
		const float f7 = g4+g7;

		const float e0 = f0;
		const float e1 = f1;
		const float e2 = f2-f3;
		const float e3 = f2+f3;
		const float e4 = f4;
		const float e5 = f5-f7;
		const float e6 = f6;
		const float e7 = f5+f7;
		const float e8 = f4+f6;

		const float d0 = e0;
		const float d1 = e1;
		const float d2 = e2*m1;
		const float d3 = e3;
		const float d4 = e4*m2;
		const float d5 = e5*m3;
		const float d6 = e6*m4;
		const float d7 = e7;
		const float d8 = e8*m5;

		const float c0 = d0+d1;
		const float c1 = d0-d1;
		const float c2 = d2-d3;
		const float c3 = d3;
		const float c4 = d4+d8;
		const float c5 = d5+d7;
		const float c6 = d6-d8;
		const float c7 = d7;
		const float c8 = c5-c6;

		const float b0 = c0+c3;
		const float b1 = c1+c2;
		const float b2 = c1-c2;
		const float b3 = c0-c3;
		const float b4 = c4-c8;
		const float b5 = c8;
		const float b6 = c6-c7;
		const float b7 = c7;

		component[0*8+i] = b0+b7;
		component[1*8+i] = b1+b6;
		component[2*8+i] = b2+b5;
		component[3*8+i] = b3+b4;
		component[4*8+i] = b3-b4;
		component[5*8+i] = b2-b5;
		component[6*8+i] = b1-b6;
		component[7*8+i] = b0-b7;
	}

	// For each row
	for(size_t i = 0; i < 8; i++) {
		const float g0 = component[i*8+0]*s0;
		const float g1 = component[i*8+4]*s4;
		const float g2 = component[i*8+2]*s2;
		const float g3 = component[i*8+6]*s6;
		const float g4 = component[i*8+5]*s5;
		const float g5 = component[i*8+1]*s1;
		const float g6 = component[i*8+7]*s7;
		const float g7 = component[i*8+3]*s3;

		const float f0 = g0;
		const float f1 = g1;
		const float f2 = g2;
		const float f3 = g3;
		const float f4 = g4-g7;
		const float f5 = g5+g6;
		const float f6 = g5-g6;
		const float f7 = g4+g7;

		const float e0 = f0;
		const float e1 = f1;
		const float e2 = f2-f3;
		const float e3 = f2+f3;
		const float e4 = f4;
		const float e5 = f5-f7;
		const float e6 = f6;
		const float e7 = f5+f7;
		const float e8 = f4+f6;

		const float d0 = e0;
		const float d1 = e1;
		const float d2 = e2*m1;
		const float d3 = e3;
		const float d4 = e4*m2;
		const float d5 = e5*m3;
		const float d6 = e6*m4;
		const float d7 = e7;
		const float d8 = e8*m5;

		const float c0 = d0+d1;
		const float c1 = d0-d1;
		const float c2 = d2-d3;
		const float c3 = d3;
		const float c4 = d4+d8;
		const float c5 = d5+d7;
		const float c6 = d6-d8;
		const float c7 = d7;
		const float c8 = c5-c6;

		const float b0 = c0+c3;
		const float b1 = c1+c2;
		const float b2 = c1-c2;
		const float b3 = c0-c3;
		const float b4 = c4-c8;
		const float b5 = c8;
		const float b6 = c6-c7;
		const float b7 = c7;

		component[i*8+0] = b0+b7;
		component[i*8+1] = b1+b6;
		component[i*8+2] = b2+b5;
		component[i*8+3] = b3+b4;
		component[i*8+4] = b3-b4;
		component[i*8+5] = b2-b5;
		component[i*8+6] = b1-b6;
		component[i*8+7] = b0-b7;
	}
}

void JPEG::YCbCrToRBG() {
	for(size_t y = 0; y < _mcuHeight; y+=_verticalSamplingFactor) { 
		for(size_t x = 0; x < _mcuWidth; x+=_horizontalSamplingFactor) {
			const MCU& cbcr = _mcus[y*_mcuWidthReal+x];
			for(size_t v = _verticalSamplingFactor-1; v<_verticalSamplingFactor; v--) {
				for(size_t h = _horizontalSamplingFactor-1; h<_horizontalSamplingFactor; h--) {
					MCU& mcu = _mcus[(y+v)*_mcuWidthReal+(x+h)];
					YCbCrToRGBMCU(mcu, cbcr, v, h);
				}
			}
		}
	}
}

void JPEG::YCbCrToRGBMCU(MCU& mcu, const MCU& cbcr, const size_t v, const size_t h) {
	for(unsigned y = 7; y < 8; y--) {
		for(unsigned x = 7; x < 8; x--) {
			const size_t pixel = y*8+x;
			const size_t cbcrPixelRow = y/_verticalSamplingFactor + 4*v;
			const size_t cbcrPixelColumn = x/_horizontalSamplingFactor + 4*h;
			const size_t cbcrPixel = cbcrPixelRow*8 + cbcrPixelColumn;


			int r = mcu.y[pixel]                             + 1.402f*cbcr.cr[cbcrPixel] + 128;
			int g = mcu.y[pixel] - 0.344f*cbcr.cb[cbcrPixel] - 0.714f*cbcr.cr[cbcrPixel] + 128;
			int b = mcu.y[pixel] + 1.772f*cbcr.cb[cbcrPixel]                             + 128;

			if(r < 0) r = 0;
			if(g < 0) g = 0;
			if(b < 0) b = 0;
			if(r > 255) r = 255;
			if(g > 255) g = 255;
			if(b > 255) b = 255;

			mcu.r[pixel] = r;
			mcu.g[pixel] = g;
			mcu.b[pixel] = b;
		}
	}
}

void JPEG::MCUToDecoded() {
	_decoded.resize(_width*_height*_numComponents);
	for(size_t y = 0; y < _height; y++)
		for(size_t x = 0; x < _width; x++) {
			MCU& mcu = _mcus[(y/8)*_mcuWidthReal+(x/8)];
			unsigned i = x%8 + y%8*8;
			_decoded[y*_width*3+x*3] = mcu.r[i];
			_decoded[y*_width*3+x*3+1] = mcu.g[i];
			_decoded[y*_width*3+x*3+2] = mcu.b[i];
		}
}

BitReader::BitReader(const std::vector<uint8_t>& d)
	: _data(d), _nextByte(0), _nextBit(0) 
{

}

int BitReader::readBit() {
	if(_nextByte >= _data.size())
		return -1;

	int bit = (_data[_nextByte]>>(7-_nextBit))&1;
	_nextBit++;
	if(_nextBit == 8) {
		_nextBit = 0;
		_nextByte++;
	}
	return bit;
}

int BitReader::readBits(const unsigned length) {
	int bits = 0;
	for(size_t i = 0; i < length; i++) {
		int bit = readBit();
		if(bit == -1) {
			LOG_WARN("imgproc::JPEG", "Trying to read bits from the end of the file");
			bits = -1;
			break;
		}
		bits = (bits<<1)|bit;
	}
	return bits;
}

void BitReader::align() {
	if(_nextByte >= _data.size())
		return;
	if(_nextBit != 0) {
		_nextBit = 0;
		_nextByte++;
	}
}

}
