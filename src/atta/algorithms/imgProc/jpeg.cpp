//--------------------------------------------------
// Atta Algorithms - Image Processing
// jpeg.cpp
// Date: 2021-07-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/algorithms/imgProc/jpeg.h>
#include <atta/helpers/log.h>

namespace atta::imgproc {

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
	_successiveApproximationHigh(0), _successiveApproximationLow(0)
	{

	}

const std::vector<uint8_t>& JPEG::decode(const std::vector<uint8_t>& data) {
    size_t i = 0;// Current data offset

    bool finished = false;
    while(!finished && i<data.size()) {
        unsigned marker = data[i++]<<8 | data[i++];
        Log::debug("imgproc::JPEG", "Marker $0", (int)data[i-1]);
		if(marker == MARKER_START_OF_IMAGE) {
            Log::debug("imgproc::JPEG", "Marker MARKER_START_OF_IMAGE");
		} else if(marker == MARKER_END_OF_IMAGE) {
            Log::debug("imgproc::JPEG", "Marker MARKER_END_OF_IMAGE");
			finished = true;
		} else if(marker >= MARKER_APP0 && marker <= MARKER_APP15) {
			if(!readApplicationSegment(data, i)) {
				Log::error("imgproc::JPEG", "Could not read application segment");
				return {};
			}
		} else if(marker == MARKER_QUANTIZATION_TABLE) {
			if(!readQuantizationTable(data, i)) {
				Log::error("imgproc::JPEG", "Could not read quantization table");
				return {};
			}
		} else if(marker == MARKER_HUFFMAN_TABLE) {
			if(!readHuffmanTable(data, i)) {
				Log::error("imgproc::JPEG", "Could not read huffman table");
				return {};
			}
		} else if(marker == MARKER_START_OF_SCAN) {
			if(!readStartOfScan(data, i)) {
				Log::error("imgproc::JPEG", "Could not read start of scan segment");
				return {};
			}
			loopScan(data, i);
		} else if(marker == MARKER_START_OF_FRAME) {
			if(!readStartOfFrame(data, i)) {
				Log::error("imgproc::JPEG", "Could not read start of frame segment");
				return {};
			}
		} else if(marker == MARKER_RESTART_INTERVAL) {
			if(!readRestartInterval(data, i)) {
				Log::error("imgproc::JPEG", "Could not read restart interval segment");
				return {};
			}
		} else if((marker >= MARKER_JPG0 && marker <= MARKER_JPG13) ||
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
			std::cout << std::hex;
			Log::warning("imgproc::JPEG", "Unknown marker found: $0", marker);
			std::cout << std::dec;
			finished = true;
			return {};
		}
    }

    return _decoded;
}

bool JPEG::readQuantizationTable(const std::vector<uint8_t>& data, size_t& i) {
	Log::debug("imgproc::JPEG", "Marker MARKER_QUANTIZATION_TABLE");
	int length = data[i++]<<8 | data[i++];
	length-=2;
	while(length > 0) {
		uint8_t infoQT = data[i++];
		length--;
		size_t idxQT = infoQT&0x0f;
		if(idxQT > 3) {
			Log::error("imgproc::JPEG", "Invalid quantization table id: $0", idxQT);
			return false;
		}
		_quantizationTables[idxQT].defined = true;
		Log::debug("imgproc::JPEG", "Reading quantization table $0", idxQT);

		if(infoQT>>4 != 0) {
			for(size_t j=0;j<64;j++)
				_quantizationTables[idxQT].table[zigZagMap[j]] = data[i++]<<8 | data[i++];
			length-=128;
		} else {
			for(size_t j=0;j<64;j++)
				_quantizationTables[idxQT].table[zigZagMap[j]] = data[i++];
			length-=64;
		}
	}
	if(length != 0) {
		Log::error("imgproc::JPEG", "Unexpected quantization table length");
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
	Log::debug("imgproc::JPEG", "Marker MARKER_APPLICATION_SEGMENT");
	unsigned length = data[i++]<<8 | data[i++];
	i+=length-2;
	return true;
}

bool JPEG::readComment(const std::vector<uint8_t>& data, size_t& i) {
	Log::debug("imgproc::JPEG", "Marker MARKER_COMMENT (or ignored)");
	unsigned length = data[i++]<<8 | data[i++];
	i+=length-2;
	return true;
}

bool JPEG::readHuffmanTable(const std::vector<uint8_t>& data, size_t& i) {
	int length = data[i++]<<8 | data[i++];
	length-=2;

	while(length > 0) {
		uint8_t infoHT = data[i++];
		uint8_t idxHT = infoHT & 0x0f;// 0->Luma; 1->Chroma
		bool ACTable = (infoHT & 0xf0)>>4;// false->DC table; true->AC table

		if(idxHT > 3) {
			Log::error("imgproc::JPEG", "Invalid huffman table index: $0", idxHT);
			return false;
		}

		HuffmanTable* hTable = ACTable ? &_huffmanACTables[idxHT] : &_huffmanDCTables[idxHT];
		hTable->defined = true;

		// Populate offsets (starting index of symbols of each length)
		uint8_t allSymbols = 0;
		for(size_t j=1;j<=16;j++) {
			allSymbols += data[i++];
			hTable->offsets[j] = allSymbols;
		}
		if(allSymbols>162) {
			Log::error("imgproc::JPEG", "Too many symbols in Huffman table: $0", allSymbols);
			return false;
		}

		for(size_t j=0;j<allSymbols;j++)
			hTable->symbols[j] = data[i++];

		length -= 17 + allSymbols;
	}
	if(length<0) {
		Log::error("imgproc::JPEG", "More bytes than expected on the Huffman Table segment");
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
	Log::debug("imgproc::JPEG", "Marker MARKER_DEFINE_HUFFMAN_TABLE \n$0", symbolsStr);
	return true;
}

bool JPEG::readStartOfScan(const std::vector<uint8_t>& data, size_t& i) {
	Log::debug("imgproc::JPEG", "Marker MARKER_START_OF_SCAN");
	if(_numComponents == 0) {
		Log::error("imgproc::JPEG", "Start of Scan before Start of Frame");
		return false;
	}
	unsigned length = data[i++]<<8 | data[i++];
	for(size_t j = 0; j < _numComponents; j++)
		_colorComponents[j].defined = false;

	uint8_t numComponents = data[i++];
	for(size_t j = 0; j < numComponents; j++) {
		size_t componentId = data[i++];
		if(_zeroBased) componentId++;

		if(componentId > _numComponents) {
			Log::error("imgproc::JPEG", "Invalid color component Id: $0", componentId);
			return false;
		}

		ColorComponent* component = &_colorComponents[componentId-1];
		if(component->defined) {
			Log::error("imgproc::JPEG", "Duplicate component: $0", componentId);
			return false;
		}
		component->defined = true;

		uint8_t huffmanTableId = data[i++];
		component->huffmanDCTableId = huffmanTableId >> 4;
		component->huffmanACTableId = huffmanTableId & 0x0f;

		if(component->huffmanDCTableId > 3) {
			Log::error("imgproc::JPEG", "Invalid DC Table index: $0", component->huffmanDCTableId);
			return false;
		}

		if(component->huffmanACTableId > 3) {
			Log::error("imgproc::JPEG", "Invalid AC Table index: $0", component->huffmanACTableId);
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
		Log::error("imgproc::JPEG", "Invalid spectral selection");
		return false;
	}
	if(_successiveApproximationHigh != 0 || _successiveApproximationLow != 0) {
		Log::error("imgproc::JPEG", "Invalid successive approximation");
		return false;
	}

	if(length != 6+2*numComponents) {
		Log::error("imgproc::JPEG", "Invalid start of scan length");
		return false;
	}

	return true;
}

bool JPEG::readStartOfFrame(const std::vector<uint8_t>& data, size_t& i) {
	Log::debug("imgproc::JPEG", "Marker MARKER_START_OF_FRAME");
	if(_numComponents != 0) {
		Log::error("imgproc::JPEG", "Multiple start of frame found!");
		return false;
	}

	unsigned length = data[i++]<<8 | data[i++];

	//----- Precision -----//
	unsigned precision = data[i++];
	if(precision != 8) {
		Log::error("imgproc::JPEG", "Invalid precision: $0", precision);
		return false;
	}

	//----- Width/Height -----//
	_height = data[i++]<<8 | data[i++];
	_width = data[i++]<<8 | data[i++];
	if(_height == 0 || _width == 0) {
		Log::error("imgproc::JPEG", "Invalid size: $0x$1", _width, _height);
		return false;
	}

	//----- Number of components -----//
	_numComponents = data[i++];
	if(_numComponents == 4) {
		Log::error("imgproc::JPEG", "CMYK color mode not supported");
		return false;
	}
	if(_numComponents == 0) {
		Log::error("imgproc::JPEG", "Number of color components must not be zero");
		return false;
	}

	for(size_t j=0;j<_numComponents;j++) {
		size_t componentId = data[i++];
		if(componentId == 4 || componentId == 5) {
			Log::error("imgproc::JPEG", "YIQ color mode not supported");
			return false;
		}

		if(componentId == 0) _zeroBased = true;
		if(_zeroBased) componentId ++;

		if(componentId > 3) {
			Log::error("imgproc::JPEG", "Invalid color component Id: $0", componentId);
			return false;
		}

		ColorComponent* component = &_colorComponents[componentId-1];
		if(component->defined) {
			Log::error("imgproc::JPEG", "Duplicate component definition: $0", componentId);
			return false;
		}
		component->defined = true;
		uint8_t samplingFactor = data[i++];
		component->horizontalSamplingFactor = samplingFactor>>4;
		component->verticalSamplingFactor = samplingFactor&0x0f;
		component->quantizationTableId = data[i++];
		if(component->quantizationTableId>3) {
			Log::error("imgproc::JPEG", "Invalid quantization table id($0) in frame components($1)", component->quantizationTableId, componentId);
			return false;
		}
		Log::debug("imgproc::JPEG", "ComponentId: $0, hor:$1, ver:$2, table:$3 $4", componentId, 
			component->horizontalSamplingFactor, component->verticalSamplingFactor, component->quantizationTableId, _numComponents);
	}

	if(length != (8+(3*_numComponents))) {
		Log::error("imgproc::JPEG", "Invalid length for SOF (start of frame) segment $0!=$1", (int)length, (int)(8+(3*_numComponents)));
		return false;
	}

	return true;
}

bool JPEG::readRestartInterval(const std::vector<uint8_t>& data, size_t& i) {
	unsigned length = data[i++]<<8 | data[i++];
	if(_restartInterval > 0) {
		Log::error("imgproc::JPEG", "Duplicated restart interval");
		return false;
	}

	_restartInterval = data[i++]<<8 | data[i++];

	if(_restartInterval == 0) {
		Log::error("imgproc::JPEG", "Restart interval must not be zero");
		return false;
	}

	Log::debug("imgproc::JPEG", "Marker MARKER_RESTART_INTERVAL value:$0", _restartInterval);
	return true;
}

bool JPEG::loopScan(const std::vector<uint8_t>& data, size_t& i) {
	uint8_t last, current;
	current = data[i++];
	while(true)	{
		std::cout << i << data.size()<< std::endl;	

		last = current;
		current = data[i++];

		if(last == 0xff) {
			if(current == unsigned(MARKER_END_OF_IMAGE&0x00ff)) {
				std::cout << "end of file" << std::endl;
				i-=2;
				return true;
			} else if(current == 0x00) {
				_huffmanData.push_back(last);
				std::cout << "ff00" << std::endl;
			} else if(current >= (MARKER_RESET0&0x00ff) && current <= (MARKER_RESET7&0x00ff)) {
				std::cout << "Reset marker" << std::endl;
				current = data[i++];
			} else if(current == 0xff) {
				std::cout << "ffff" << std::endl;
				continue;
			} else {
				Log::error("imgproc::JPEG", "Invalid marker during compressed data scan: $0", current);
				return false;
			}
		} else {
			_huffmanData.push_back(last);
		}

		if(i==data.size()) {
			Log::error("imgproc::JPEG", "JPEG data finished before End of File marker $0 $1", last, current);
			return false;
		}
	}
	return true;
}

}
