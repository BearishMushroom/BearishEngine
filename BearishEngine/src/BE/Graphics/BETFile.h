#pragma once

#include <BE/Types.h>
#include <stdio.h>
#include <cassert>
#include <map>
#include <vector>

/*
* Header: 'B''E''T'[version]
* Width: u32
* Height: u32
* dataSize: u32
* [data] ->
*   while toread ->
*     col = u8 * numchannels
*     num = u8
*	  toread -= num + col
*     data = col num times
*/

#define BIT(x) 1 << x

namespace Bearish { namespace Graphics {
	enum BETFileChannel : u8 {
		BETFileChannel_R = BIT(0),
		BETFileChannel_G = BIT(1),
		BETFileChannel_B = BIT(2),
		BETFileChannel_A = BIT(3),
	};

	class BETFile {
	public:
		u8 header[4];
		u8 channels;
		u32 width;
		u32 height;
		u8* data;
		u32 dataSize;
		u32 bytesWritten;

		u32 numColors;

		BETFile() {
			header[0] = 'B';
			header[1] = 'E';
			header[2] = 'T';
			header[3] = 1;
		}

		~BETFile() {
		}

		u8* ConvertToBET() {
			numColors = 0;
			if (data == 0) {
				return 0;
			}

			std::vector<std::pair<u32, u8>> colors;

			u32 bytes = width * height * 4;
			u32 pos = 0;
			u32 col = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
			u8 num = 0;
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
			data = (u8*)malloc(colors.size() * 5);
			dataSize = (u32)colors.size() * 5;

			pos = 0;
			for (auto& pair : colors) {
				data[pos + 0] = (pair.first & 0xFF000000) >> 24;
				data[pos + 1] = (pair.first & 0x00FF0000) >> 16;
				data[pos + 2] = (pair.first & 0x0000FF00) >> 8;
				data[pos + 3] = (pair.first & 0x000000FF);
				data[pos + 4] = pair.second;
				pos += 5;
				numColors++;
			}

			return data;
		}

		u8* ConvertToRaw() {
			if (data == 0) {
				return 0;
			}

			std::vector<std::pair<u32, u8>> colors;

			u32 pos = 0;
			while (pos < dataSize) {
				colors.push_back(std::make_pair(
					data[pos + 0] << 24 | data[pos + 1] << 16 | data[pos + 2] << 8 | data[pos + 3],
					data[pos + 4]
					));
				pos += 5;
			}

			dataSize = 0;
			for (auto& pair : colors) {
				dataSize += pair.second * 4;
			}

			free(data);
			data = (u8*)malloc(dataSize);

			pos = 0;
			for (auto& pair : colors) {
				for (i32 i = 0; i < pair.second; i++) {
					data[pos + 0] = (pair.first & 0xFF000000) >> 24;
					data[pos + 1] = (pair.first & 0x00FF0000) >> 16;
					data[pos + 2] = (pair.first & 0x0000FF00) >> 8;
					data[pos + 3] = (pair.first & 0x000000FF);
					pos += 4;
				}
			}

			return data;
		}

		void WriteBytes(FILE* file, u8* bytes, u32 size, u32 element) {
			fwrite(bytes, element, size, file);
			bytesWritten += size * element;
		}

		u8* ReadBytes(FILE* file, u32 size, u32 element) {
			u8* buffer = (u8*)malloc(size * element);
			fread(buffer, element, size, file);
			return buffer;
		}

		void WriteToFile(string filename) {
			bytesWritten = 0;
			FILE* output;
			fopen_s(&output, filename.c_str(), "wb");
			printf("Writing %d pixels and %d colors to %s.\n", width * height, numColors, filename.c_str());

			printf("Writing header...                 ");
			WriteBytes(output, header, 4, 1);
			WriteBytes(output, (u8*)&width, 1, sizeof(u32));
			WriteBytes(output, (u8*)&height, 1, sizeof(u32));
			WriteBytes(output, (u8*)&dataSize, 1, sizeof(u32));
			printf("%d\n", bytesWritten);

			u32 preBytes = bytesWritten;
			printf("Writing data...                   ");
			WriteBytes(output, data, dataSize, sizeof(u8));
			printf("%d\n", bytesWritten - preBytes);

			printf("%d bytes written.\n", bytesWritten);

			fclose(output);
		}

		void ReadFromFile(string filename) {
			FILE* file;
			fopen_s(&file, filename.c_str(), "rb");

			u8* header = ReadBytes(file, 4, 1);
			assert(memcmp(header, this->header, 4) == 0);

			width = *(u32*)ReadBytes(file, sizeof(u32), 1);
			height = *(u32*)ReadBytes(file, sizeof(u32), 1);
			dataSize = *(u32*)ReadBytes(file, sizeof(u32), 1);

			data = ReadBytes(file, dataSize, sizeof(u8));

			fclose(file);
		}
	};
} }