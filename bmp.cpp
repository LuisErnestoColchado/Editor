#include "bmp.hpp"
bmp::bmp(){}
bmp::bmp(std::vector<unsigned char> pData,std::string pFormat,int pWidth, int pHeight, int pSizeFile){
    this->data = pData;
    this->format = pFormat;
    this->height = pHeight;
    this->width = pWidth;
    this->size = pSizeFile;
}
void bmp::setParameters(std::vector<unsigned char> pData,std::string pFormat,int pWidth, int pHeight, int pSizeFile){
    this->data = pData;
    this->format = pFormat;
    this->height = pHeight;
    this->width = pWidth;
    this->size = pSizeFile;
}
