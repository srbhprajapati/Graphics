#pragma once

#include <iostream>
#include <fstream>
#include <string>

struct BmpHeader {
	char bitmapSignatureBytes[2] = { 'B', 'M' };
	uint32_t sizeOfBitmapFile = 54 + 786432; // total size of bitmap file
	uint32_t reservedBytes = 0;
	uint32_t pixelDataOffset = 54;
} bmpHeader;

struct BmpInfoHeader {
	uint32_t sizeOfThisHeader = 40;
	int32_t width = 512; // in pixels
	int32_t height = 512; // in pixels
	uint16_t numberOfColorPlanes = 1; // must be 1
	uint16_t colorDepth = 24;
	uint32_t compressionMethod = 0;
	uint32_t rawBitmapDataSize = 0; // generally ignored
	int32_t horizontalResolution = 3780; // in pixel per meter
	int32_t verticalResolution = 3780; // in pixel per meter
	uint32_t colorTableEntries = 0;
	uint32_t importantColors = 0;
} bmpInfoHeader;

struct Pixel {
	uint8_t blue = 255;
	uint8_t green = 255;
	uint8_t red = 0;
} pixel;


void WriteTestBitmapFile(std::string filePath)
{
	std::ofstream fout(filePath.c_str(), std::ios::binary);

	
	fout.write(bmpHeader.bitmapSignatureBytes, 2);
	fout.write((char*)&bmpHeader.sizeOfBitmapFile, sizeof(uint32_t));
	fout.write((char*)&bmpHeader.reservedBytes, sizeof(uint32_t));
	fout.write((char*)&bmpHeader.pixelDataOffset, sizeof(uint32_t));
	
	fout.write((char*)&bmpInfoHeader.sizeOfThisHeader, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.width, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.height, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.numberOfColorPlanes, sizeof(uint16_t));
	fout.write((char*)&bmpInfoHeader.colorDepth, sizeof(uint16_t));
	fout.write((char*)&bmpInfoHeader.compressionMethod, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.rawBitmapDataSize, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.horizontalResolution, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.verticalResolution, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.colorTableEntries, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.importantColors, sizeof(uint32_t));
	

	// writing pixel data
	size_t numberOfPixels = (size_t)bmpInfoHeader.width * (size_t)bmpInfoHeader.height;
	for (int i = 0; i < numberOfPixels; i++) {
		fout.write((char*)&pixel, 3);
	}
	fout.close();
}


void WriteBitmapFile(int32_t width, int32_t height, std::string filePath, char* data)
{
	std::ofstream fout(filePath.c_str(), std::ios::binary);


	bmpHeader.sizeOfBitmapFile = 54 + (width * height * 3);
	bmpInfoHeader.width = width;
	bmpInfoHeader.height = height;

	fout.write(bmpHeader.bitmapSignatureBytes, 2);
	fout.write((char*)&bmpHeader.sizeOfBitmapFile, sizeof(uint32_t));
	fout.write((char*)&bmpHeader.reservedBytes, sizeof(uint32_t));
	fout.write((char*)&bmpHeader.pixelDataOffset, sizeof(uint32_t));

	fout.write((char*)&bmpInfoHeader.sizeOfThisHeader, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.width, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.height, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.numberOfColorPlanes, sizeof(uint16_t));
	fout.write((char*)&bmpInfoHeader.colorDepth, sizeof(uint16_t));
	fout.write((char*)&bmpInfoHeader.compressionMethod, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.rawBitmapDataSize, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.horizontalResolution, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.verticalResolution, sizeof(int32_t));
	fout.write((char*)&bmpInfoHeader.colorTableEntries, sizeof(uint32_t));
	fout.write((char*)&bmpInfoHeader.importantColors, sizeof(uint32_t));


	// writing pixel data
	char* dataLocal = data;
	size_t numberOfPixels = (size_t)bmpInfoHeader.width * (size_t)bmpInfoHeader.height;
	for (int i = 0; i < numberOfPixels; i++) {
		char* R = (char*)dataLocal; dataLocal++;
		char* G = (char*)dataLocal; dataLocal++;
		char* B = (char*)dataLocal; dataLocal++;
		dataLocal++;

		fout.write(B, 1);
		fout.write(G, 1);
		fout.write(R, 1);
	}
	fout.close();
}