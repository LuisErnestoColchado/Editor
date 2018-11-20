#include "bmp.hpp"
bmp::bmp(){}
bmp::bmp(std::vector<unsigned char> pData,std::string pFormat,int pWidth, int pHeight, int pSizeFile,int pNumberColor,std::vector<unsigned char> pBufferColors){
    this->data = pData;
    this->format = pFormat;
    this->height = pHeight;
    this->width = pWidth;
    this->size = pSizeFile;
    this->numberColors = pNumberColor;
    this->bufferColors = pBufferColors;
}
void bmp::setParameters(std::vector<unsigned char> pData,std::string pFormat,int pWidth, int pHeight, int pSizeFile,int pNumberColor){
    this->data = pData;
    this->format = pFormat;
    this->height = pHeight;
    this->width = pWidth;
    this->size = pSizeFile;
    this->numberColors = pNumberColor;
}

void bmp::setParameters(std::vector<unsigned char> pData,std::string pFormat,int pWidth, int pHeight, int pSizeFile,int pNumberColor,std::vector<unsigned char> pBufferColors,int pSizeRaw){
    this->data = pData;
    this->format = pFormat;
    this->height = pHeight;
    this->width = pWidth;
    this->size = pSizeFile;
    this->numberColors = pNumberColor;
    this->bufferColors = pBufferColors;
    this->sizeRaw = pSizeRaw;
}
