#include "WavFile.h"

using namespace std;

WavFile::WavFile(char fileName[]) {
	FILE* in;
	errno_t err;
	err = fopen_s(&in, fileName, "rb");
	if (err) {
		cout << "Failed to open input file, error: " << err;
		return;
	}
	fread(&this->chunkId, sizeof(this->chunkId), 1, in);
	fread(&this->chunkSize, sizeof(this->chunkSize), 1, in);
	fread(&this->format, sizeof(this->format), 1, in);
	fread(&this->subchunk1Id, sizeof(this->subchunk1Id), 1, in);
	fread(&this->subchunk1Size, sizeof(this->subchunk1Size), 1, in);
	fread(&this->audioFormat, sizeof(this->audioFormat), 1, in);
	if (this->audioFormat != 1) {
		cout << "File was compressed!" << endl;
	}
	fread(&this->numChannels, sizeof(this->numChannels), 1, in);
	if (this->numChannels != 1) {
		cout << "Works only with MONO!" << endl;
	}
	fread(&this->sampleRate, sizeof(this->sampleRate), 1, in);
	fread(&this->byteRate, sizeof(this->byteRate), 1, in);
	fread(&this->blockAlign, sizeof(this->blockAlign), 1, in);
	if (this->blockAlign != 2) {
		cout << "Works only with bitrate: 16" << endl;
	}
	fread(&this->bitsPerSample, sizeof(this->bitsPerSample), 1, in);
	if (this->bitsPerSample != 16) {
		cout << "Works only with bitrate: 16" << endl;
	}
	fread(&this->subchunk2Id, sizeof(this->subchunk2Id), 1, in);
	fread(&this->subchunk2Size, sizeof(this->subchunk2Size), 1, in);
	this->data = new int16_t[this->subchunk2Size / this->blockAlign];
	for (int i = 0; i < this->subchunk2Size/ this->blockAlign; i++) {
		int16_t temp;
		fread(&temp, sizeof(this->blockAlign), 1, in);
		this->data[i] = temp;
	}
	fclose(in);
}

WavFile::~WavFile() {
	delete newData;
	delete data;
	cout << "Done";
}

void WavFile::writeWav(char fileName[]) {
	FILE* out;
	errno_t err;
	err = fopen_s(&out, fileName, "wb");
	if (err) {
		cout << "Failed to write a file, error: " << err;
		return;
	}
	fwrite(&this->chunkId, sizeof(this->chunkId), 1, out);
	fwrite(&this->chunkSize, sizeof(this->chunkSize), 1, out);
	fwrite(&this->format, sizeof(this->format), 1, out);
	fwrite(&this->subchunk1Id, sizeof(this->subchunk1Id), 1, out);
	fwrite(&this->subchunk1Size, sizeof(this->subchunk1Size), 1, out);
	fwrite(&this->audioFormat, sizeof(this->audioFormat), 1, out);
	fwrite(&this->numChannels, sizeof(this->numChannels), 1, out);
	fwrite(&this->sampleRate, sizeof(this->sampleRate), 1, out);
	fwrite(&this->byteRate, sizeof(this->byteRate), 1, out);
	fwrite(&this->blockAlign, sizeof(this->blockAlign), 1, out);
	fwrite(&this->bitsPerSample, sizeof(this->bitsPerSample), 1, out);
	fwrite(&this->subchunk2Id, sizeof(this->subchunk2Id), 1, out);
	fwrite(&this->subchunk2Size, sizeof(this->subchunk2Size), 1, out);
	for (int i = 0; i < this->subchunk2Size / this->blockAlign; i++) {
		fwrite(&this->newData[i], sizeof(this->newData[i]), 1, out);
	}
	fclose(out);
}

void WavFile::interpolation(double koef) {
	this->newSize = this->subchunk2Size * koef;
	this->newData = new int16_t[this->newSize];
	int temp;
	int32_t x0, x1, y0, y1, y, x;
	for (int i = 0; i < this->subchunk2Size / this->blockAlign; i++) {
		temp = i * koef;
		this->newData[temp] = this->data[i];
	}
	for (int i = 0; i < this->subchunk2Size / this->blockAlign - 1; i++) {
		x0 = i * koef;
		x1 = (i + 1) * koef;
		y0 = this->newData[x0];
		y1 = this->newData[x1];
		temp = x1 - x0 - 1;
		for (int j = 1; j < temp + 1; j++) {
			y = ((y1 - y0) * j) / (x1 - x0) + y0;
			x = j + x0;
			this->newData[x] = y;
		}
	}
	this->chunkSize += this->newSize - this->subchunk2Size;
	this->subchunk2Size = this->newSize;
}