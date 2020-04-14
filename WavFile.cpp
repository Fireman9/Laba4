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
	fread(&this->numChannels, sizeof(this->numChannels), 1, in);
	fread(&this->sampleRate, sizeof(this->sampleRate), 1, in);
	fread(&this->byteRate, sizeof(this->byteRate), 1, in);
	fread(&this->blockAlign, sizeof(this->blockAlign), 1, in);
	fread(&this->bitsPerSample, sizeof(this->bitsPerSample), 1, in);
	fread(&this->subchunk2Id, sizeof(this->subchunk2Id), 1, in);
	fread(&this->subchunk2Size, sizeof(this->subchunk2Size), 1, in);
	this->data = new int8_t[this->subchunk2Size];
	for (int i = 0; i < this->subchunk2Size; i++) {
		int8_t temp;
		fread(&temp, sizeof(temp), 1, in);
		this->data[i] = temp;
	}
	fclose(in);
}

WavFile::~WavFile() {
	delete newData;
	delete data;
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
	for (int i = 0; i < this->subchunk2Size; i++) {
		fwrite(&this->data[i], sizeof(this->data[i]), 1, out);
	}
	fclose(out);
}

void WavFile::interpolation(double koef) {
	this->newSize = this->subchunk2Size * koef;
	this->newData = new int8_t[this->newSize];
	std::vector<int> newIndex;
	int x0, x1, y0, y1, y, x, temp;
	for (int i = 0; i < this->subchunk2Size; i++) {
		temp = i * koef;
		this->newData[temp] = this->data[i];
		newIndex.push_back(temp);
	}
	for (int i = 0; i < newIndex.size() - 1; i++) {
		for (int j = newIndex[i] + 1; j < newIndex[i + 1]; j++) {
			y = j;
			x0 = i;
			x1 = i + 1;
			y0 = newIndex[i];
			y1 = newIndex[i + 1];
			if ((y1 - y0) != 0) {
				x = x0 + (((y - y0) * (x1 - x0)) / (y1 - y0));
				this->newData[y] = this->data[x];
			}
			else {
				cout << " We have divide for 0" << endl;
			}
		}
	}
}
