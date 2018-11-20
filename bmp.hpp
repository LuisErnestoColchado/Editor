#ifndef BMP_H
#define BMP_H
#include <iostream>
#include <vector>

class bmp
{

public:
    std::vector<unsigned char> bufferColors;
    std::vector<unsigned char> data;
    std::string format;
    int width;
    int height;
    int size;
    int numberColors;
    int sizeRaw;
    bmp();
    bmp(std::vector<unsigned char>,std::string,int, int, int,int,std::vector<unsigned char>);
    void setParameters(std::vector<unsigned char>,std::string,int, int, int,int);
    void setParameters(std::vector<unsigned char>,std::string,int, int, int,int,std::vector<unsigned char>,int);
};

#endif // BMP_H
