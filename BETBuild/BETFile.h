#ifndef _BETBUILD_BETFILE_H_
#define _BETBUILD_BETFILE_H_

#include <stdio.h>
#include <cassert>
#include <map>
#include <vector>

/*
* Header: 'B''E''T'[version]
* Width: unsigned int
* Height: unsigned int
* dataSize: unsigned int
* [data] ->
*   while toread ->
*     col = unsigned char * numchannels
*     num = unsigned char
*	  toread -= num + col
*     data = col num times
*/

#define BIT(x) 1 << x

namespace Bearish {namespace Core {namespace BET {
	enum BETFileChannel : unsigned char {
		BETFileChannel_R = BIT(0),
		BETFileChannel_G = BIT(1),
		BETFileChannel_B = BIT(2),
		BETFileChannel_A = BIT(3),
	};

	class BETFile {
	public:
		unsigned char header[4];
		unsigned char channels;
		unsigned int width;
		unsigned int height;
		unsigned char* data;
		unsigned int dataSize;
		unsigned int bytesWritten;

		unsigned int numColors;

		BETFile() {
			header[0] = 'B';
			header[1] = 'E';
			header[2] = 'T';
			header[3] = 1;
		}

		unsigned char* ConvertToBET() {
			numColors = 0;
			if (data == 0) {
				return 0;
			}

			std::vector<std::pair<unsigned int, unsigned char>> colors;

			unsigned int bytes = width * height * 4;
			unsigned int pos = 0;
			unsigned int col = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
			unsigned char num = 0;
			while (bytes) {
				if (col == (data[pos + 0] << 24 | data[pos + 1] << 16 | data[pos + 2] << 8 | data[pos + 3]) && num < 255) {
					pos += 4;
					num++;
				}
				else {
					bytes -= num * 4;
					colors.push_back(std::make_pair(col, num));
					col = data[pos + 0] << 24 | data[pos + 1] << 16 | data[pos + 2] << 8 | data[pos + 3];
					num = 0;
				}
			}

			free(data);
			data = (unsigned char*)malloc(colors.size() * 5);
			dataSize = (unsigned int)colors.size() * 5;

			pos = 0;
			for (auto& pair : colors) {
				data[pos + 0] = (pair.first & 0xFF000000) >> 24;
				data[pos + 1] = (pair.first & 0x00FF0000) >> 16;
				data[pos + 2] = (pair.first & 0x0000FF00) >>  8;
				data[pos + 3] = (pair.first & 0x000000FF);
				data[pos + 4] = pair.second;
				pos += 5;
				numColors++;
			}

			return data;
		}

		unsigned char* ConvertToRaw() {
			if (data == 0) {
				return 0;
			}

			std::vector<std::pair<unsigned int, unsigned char>> colors;

			unsigned int pos = 0;
			while (pos < dataSize) {
				colors.push_back(std::make_pair(
					data[pos+0] << 24 | data[pos+1] << 16 | data[pos+2] << 8 | data[pos+3],
					data[pos+4]
					));
				pos += 5;
			}

			dataSize = 0;
			for (auto& pair : colors) {
				dataSize += pair.second * 4;
			}

			free(data);
			data = (unsigned char*)malloc(dataSize);

			pos = 0;
			for (auto& pair : colors) {
				for (int i = 0; i < pair.second; i++) {
					data[pos + 0] = (pair.first & 0xFF000000) >> 24;
					data[pos + 1] = (pair.first & 0x00FF0000) >> 16;
					data[pos + 2] = (pair.first & 0x0000FF00) >>  8;
					data[pos + 3] = (pair.first & 0x000000FF);
					pos += 4;
				}
			}

			return data;
		}

		void WriteBytes(FILE* file, unsigned char* bytes, unsigned int size, unsigned int element) {
			fwrite(bytes, element, size, file);
			bytesWritten += size * element;
		}

		unsigned char* ReadBytes(FILE* file, unsigned int size, unsigned int element) {
			unsigned char* buffer = (unsigned char*)malloc(size * element);
			fread(buffer, element, size, file);
			return buffer;
		}

		void WriteToFile(std::string filename) {
			bytesWritten = 0;
			FILE* output;
			fopen_s(&output, filename.c_str(), "wb");
			printf("Writing %d pixels and %d colors to %s.\n", width * height, numColors, filename.c_str());

			printf("Writing header...                 ");
			WriteBytes(output, header, 4, 1);
			WriteBytes(output, (unsigned char*)&width, 1, sizeof(unsigned int));
			WriteBytes(output, (unsigned char*)&height, 1, sizeof(unsigned int));
			WriteBytes(output, (unsigned char*)&dataSize, 1, sizeof(unsigned int));
			printf("%d\n", bytesWritten);

			unsigned int preBytes = bytesWritten;
			printf("Writing data...                   ");
			WriteBytes(output, data, dataSize, sizeof(unsigned char));
			printf("%d\n", bytesWritten - preBytes);

			printf("%d bytes written.\n", bytesWritten);

			fclose(output);
		}

		void ReadFromFile(std::string filename) {
			FILE* file;
			fopen_s(&file, filename.c_str(), "rb");

			unsigned char* header = ReadBytes(file, 4, 1);
			assert(memcmp(header, this->header, 4) == 0);

			width = *(unsigned int*)ReadBytes(file, sizeof(unsigned int), 1);
			height = *(unsigned int*)ReadBytes(file, sizeof(unsigned int), 1);
			dataSize = *(unsigned int*)ReadBytes(file, sizeof(unsigned int), 1);

			data = ReadBytes(file, dataSize, sizeof(unsigned char));

			fclose(file);
		}
	};
} } }

#undef BIT

#endif // _BETBUILD_BETFILE_H_