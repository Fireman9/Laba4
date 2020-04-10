#include "WavFile.h"

using namespace std;

WavFile::WavFile(std::string fileName) {
	FILE* in;
	errno_t err;
	err = fopen_s(&in, "input.wav", "rb");
	if (err) {
		cout << "Failed open file, error " << err;
		return;
	}
	fread_s(&this->chunkId, sizeof(this->chunkId), sizeof(this->chunkId), 1, in);
	fread_s(&this->chunkSize, sizeof(this->chunkSize), sizeof(this->chunkSize), 1, in);
	fread_s(&this->format, sizeof(this->format), sizeof(this->format), 1, in);
	fread_s(&this->subchunk1Id, sizeof(this->subchunk1Id), sizeof(this->subchunk1Id), 1, in);
	fread_s(&this->subchunk1Size, sizeof(this->subchunk1Size), sizeof(this->subchunk1Size), 1, in);
	fread_s(&this->audioFormat, sizeof(this->audioFormat), sizeof(this->audioFormat), 1, in);
	fread_s(&this->numChannels, sizeof(this->numChannels), sizeof(this->numChannels), 1, in);
	fread_s(&this->sampleRate, sizeof(this->sampleRate), sizeof(this->sampleRate), 1, in);
	fread_s(&this->byteRate, sizeof(this->byteRate), sizeof(this->byteRate), 1, in);
	fread_s(&this->blockAlign, sizeof(this->blockAlign), sizeof(this->blockAlign), 1, in);
	fread_s(&this->bitsPerSample, sizeof(this->bitsPerSample), sizeof(this->bitsPerSample), 1, in);
	fread_s(&this->subchunk2Id, sizeof(this->subchunk2Id), sizeof(this->subchunk2Id), 1, in);
	fread_s(&this->subchunk2Size, sizeof(this->subchunk2Size), sizeof(this->subchunk2Size), 1, in);
	for (int i = 0; i < this->subchunk2Size; i++) {
		int8_t temp;
		fread_s(&temp, sizeof(temp), sizeof(temp), 1, in);
		this->data.push_back(temp);
	}
	fclose(in);
}