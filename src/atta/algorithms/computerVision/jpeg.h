//--------------------------------------------------
// Atta Algorithms - Computer Vision
// jpeg.h
// Date: 2021-07-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <cstdint>
#include <cstddef>

namespace atta::cv
{
	class BitReader;
	class JPEG {
		public:
			JPEG();
			const std::vector<uint8_t>& decode(const std::vector<uint8_t>& data);

			unsigned getWidth() const { return _width; }
			unsigned getHeight() const { return _height; }

		private: 
			struct QuantizationTable {
				unsigned table[64] = {0};
				bool defined = false;
			};

			struct HuffmanTable {
				uint8_t offsets[17] = {0};
				uint8_t symbols[162] = {0};
				unsigned codes[162] = {0};
				bool defined = false;
			};

			struct ColorComponent {
				uint8_t horizontalSamplingFactor = 1;
				uint8_t verticalSamplingFactor = 1;
				size_t quantizationTableId = 0;
				size_t huffmanDCTableId = 0;
				size_t huffmanACTableId = 0;
				bool defined = false;
			};

			// TODO An MCU can have more than 64 values when using sampling
			// Maybe change the name to block
			struct MCU {
				union {
					int y[64] = {0};
					int r[64];
				};
				union {
					int cb[64] = {0};
					int g[64];
				};
				union {
					int cr[64] = {0};
					int b[64];
				};

				int* operator[](unsigned i) {
					switch(i) {
						case 0:
							return y;
						case 1:
							return cb;
						case 2:
							return cr;
						default:
							return nullptr;
					}
				}
			};

			bool readApplicationSegment(const std::vector<uint8_t>& data, size_t& i);
			bool readComment(const std::vector<uint8_t>& data, size_t& i);
			bool readQuantizationTable(const std::vector<uint8_t>& data, size_t& i);
			bool readHuffmanTable(const std::vector<uint8_t>& data, size_t& i);
			bool readStartOfScan(const std::vector<uint8_t>& data, size_t& i);
			bool readStartOfFrame(const std::vector<uint8_t>& data, size_t& i);
			bool readRestartInterval(const std::vector<uint8_t>& data, size_t& i);
			bool loopScan(const std::vector<uint8_t>& data, size_t& i);

			bool decodeHuffmanData();
			void generateHuffmanCodes(HuffmanTable& table);
			bool decodeMCUComponent(BitReader &b, int* const component, int& previousDC, const HuffmanTable& dcTable, const HuffmanTable& acTable);
			uint8_t getNextSymbol(BitReader &b, const HuffmanTable& table);

			void dequantizeMCUs();
			void dequantizeMCUComponent(const QuantizationTable& qTable, int* const component);
			void inverseDCT();
			void inverseDCTComponent(int* const component);
			void YCbCrToRBG();
			void YCbCrToRGBMCU(MCU& mcu, const MCU& cbcr, const size_t v, const size_t h);
			void MCUToDecoded();

			unsigned _width;
			unsigned _height;
			unsigned _numComponents;
			QuantizationTable _quantizationTables[4];
			ColorComponent _colorComponents[4];
			HuffmanTable _huffmanDCTables[4];
			HuffmanTable _huffmanACTables[4];
			unsigned _restartInterval;
			bool _zeroBased;
			uint8_t _startOfSelection;
			uint8_t _endOfSelection;
			uint8_t _successiveApproximationHigh;
			uint8_t _successiveApproximationLow;
			uint8_t _horizontalSamplingFactor;
			uint8_t _verticalSamplingFactor;

			std::vector<uint8_t> _huffmanData;
			std::vector<MCU> _mcus;
			unsigned _mcuWidth;
			unsigned _mcuHeight;
			unsigned _mcuWidthReal;
			unsigned _mcuHeightReal;
			std::vector<uint8_t> _decoded;

			// Definitions
			enum Marker {
				MARKER_START_OF_IMAGE     = 0xffd8,
				MARKER_END_OF_IMAGE       = 0xffd9,
				MARKER_QUANTIZATION_TABLE = 0xffdb,
				MARKER_START_OF_FRAME     = 0xffc0,
				MARKER_HUFFMAN_TABLE      = 0xffc4,
				MARKER_START_OF_SCAN      = 0xffda,
				MARKER_RESTART_INTERVAL   = 0xffdd,
				MARKER_NUMBER_OF_LINES    = 0xffdc,
				MARKER_HIERARCHICAL_PROGRESSION = 0xffde,
				MARKER_EXPAND_REFERENCE_COMP    = 0xffdf,
				// Application segments
				MARKER_APP0  = 0xffe0,// JFIF JPEG Image; AVI1 – Motion JPEG (MJPG
				MARKER_APP1  = 0xffe1,// EXIF Metadata; TIFF IFD format
				MARKER_APP2  = 0xffe2,// ICC color profile
				MARKER_APP3  = 0xffe3,
				MARKER_APP4  = 0xffe4,
				MARKER_APP5  = 0xffe5,
				MARKER_APP6  = 0xffe6,
				MARKER_APP7  = 0xffe7,
				MARKER_APP8  = 0xffe8,
				MARKER_APP9  = 0xffe9,
				MARKER_APP10 = 0xffea,
				MARKER_APP11 = 0xffeb,
				MARKER_APP12 = 0xffec,
				MARKER_APP13 = 0xffed,
				MARKER_APP14 = 0xffee,
				MARKER_APP15 = 0xffef,
				// Reset markers
				MARKER_RESET0  = 0xffd0,
				MARKER_RESET1  = 0xffd1,
				MARKER_RESET2  = 0xffd2,
				MARKER_RESET3  = 0xffd3,
				MARKER_RESET4  = 0xffd4,
				MARKER_RESET5  = 0xffd5,
				MARKER_RESET6  = 0xffd6,
				MARKER_RESET7  = 0xffd7,
				// Misc Markers
				MARKER_JPG0  = 0xfff0,
				MARKER_JPG1  = 0xfff1,
				MARKER_JPG2  = 0xfff2,
				MARKER_JPG3  = 0xfff3,
				MARKER_JPG4  = 0xfff4,
				MARKER_JPG5  = 0xfff5,
				MARKER_JPG6  = 0xfff6,
				MARKER_JPG7  = 0xfff7,
				MARKER_JPG8  = 0xfff8,
				MARKER_JPG9  = 0xfff9,
				MARKER_JPG10 = 0xfffa,
				MARKER_JPG11 = 0xfffb,
				MARKER_JPG12 = 0xfffc,
				MARKER_JPG13 = 0xfffd,
				MARKER_COMMENT = 0xfffe,// Comment
				MARKER_TEM = 0xff01,
			};

	};

	class BitReader
	{
		public:	
			BitReader(const std::vector<uint8_t>& d);

			int readBit();
			int readBits(const unsigned length);
			void align();

		private:
			unsigned _nextByte;
			unsigned _nextBit;
			const std::vector<uint8_t>& _data;
	};

}
