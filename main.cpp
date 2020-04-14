#include "WavFile.h"

using namespace std;

int main(int argc, char* argv[]) {
    char testFileNameInput[] = { "input.wav" };
    char testFileNameOutput[] = { "output.wav" };
    WavFile wavFile(testFileNameInput);
    /*wavFile->interpolation(1.35);*/
    wavFile.writeWav(testFileNameOutput);
    return 0;
}
