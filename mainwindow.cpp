#include <QtWidgets>
#include "mainwindow.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
typedef std::bitset<8> byte;
MainWindow::MainWindow()
{
    QLCDNumber *lcd = new QLCDNumber(3);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    scene = new QGraphicsScene(this);
    graphicsView = new QGraphicsView(this);
    scene->setSceneRect(QRectF(2,2,800,800));

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(-255, 255);
    slider->setValue(0);
    groupBox = new QGroupBox(tr("Adjust Brightness"));

    QString fileName = "/Users/luisernestocolchado/bmp_file_99151/Bmp_file_Icon_256.png";
    QImage image(fileName);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    graphicsView->setScene(scene);
    graphicsView->setAlignment(Qt::AlignCenter);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(slider);
    groupBox->setLayout(vbox);
    groupBox->setHidden(true);
    QVBoxLayout *layout = new QVBoxLayout;
    //QPushButton *button = new QPushButton("&Download", this);
    connect(slider, SIGNAL(valueChanged(int)),lcd, SLOT(display(int)));
    connect(slider,SIGNAL(sliderReleased()), SLOT(changeSlider()));

    layout->setMargin(5);
    layout->addWidget(graphicsView);
    layout->addWidget(groupBox);
    widget->setLayout(layout);
    createActions();
    createMenus();

    setWindowTitle(tr("Processing Image"));
    showMaximized();
}

void MainWindow::changeSlider(){

    if(numOpe == 1){
        beta = slider->value();
        changeBrightness();
    }
    else{
        if(slider->value() >= 0 ){
            alpha = slider->value();
            changeConstrast();
        }
        else{
            alpha = -1.0/(double)slider->value();
            std::cout << "ddddd" << std::endl;
            changeConstrast();
        }

   }
}

void MainWindow::newFile()
{
    infoLabel->setText(tr("Invoked <b>File|New</b>"));
}
std::string show_binary(unsigned char u)
{
  int t;
  std::string cadena="";
  for(t=128; t>0; t = t/2)
    if(u & t) cadena += "1 ";
    else cadena += "0 ";
    std::cout << cadena << std::endl;
}
void MainWindow::open()
{
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/Users/luisernestocolchado", tr("Image Files (*.bmp)"));
    QByteArray ba = fileName.toLocal8Bit();
    char* cfilename = ba.data();
    readBMP(cfilename,oBmp);
    //data = ;
    QRgb value;
    int r,g,b;

    int countRows=oBmp.height-1;
    int countColumns=0;
    if(oBmp.size == 24){
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        for(int i = 0;i<oBmp.width*oBmp.height*3;i+=3){
            r = oBmp.data[i];
            g = oBmp.data[i+1];
            b = oBmp.data[i+2];
            //std::cout << data[i] << std::endl;
            currentPixels.push_back(r);
            currentPixels.push_back(g);
            currentPixels.push_back(b);
            value = qRgb(r,g,b);
            //std::cout << r << " " << g  << " " << b << " ";
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == oBmp.width){
               // std::cout << std::endl;
                //std::cout << r << "-" << g << "-" << b << std::endl;
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
    else if(oBmp.size == 8){
        int* palette = new int [oBmp.numberColors];
        //byte* bpalette = new byte[numberColor*4];
        //f.read (bpalette, 0, numberColor*4);
        int rgb8bpp;
        int nindex8 = 0;
        QRgb rgb;
        for (int n = 0; n < oBmp.numberColors; n++)
        {
            palette[n] = (255&0xff)<<24
                        | (((int)oBmp.bufferColors[nindex8+2]&0xff)<<16)
                        | (((int)oBmp.bufferColors[nindex8+1]&0xff)<<8)
                        | (int)oBmp.bufferColors[nindex8]&0xff;
            nindex8+=4;
        }
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        nindex8 = 0;
        int npad8 = (oBmp.sizeRaw / oBmp.height) - oBmp.width;
        for (int j8 = oBmp.height-1; j8 >= 0 ; j8--)
        {
            for (int i8 = 0; i8 < oBmp.width; i8++)
            {

                rgb8bpp = (int)oBmp.data[nindex8]&0xff;
                //std::cout << rgb8bpp << std::endl;
                int red = (palette[rgb8bpp] & 0x00ff0000) >> 16;
                int green = (palette[rgb8bpp] & 0x0000ff00) >> 8;
                int blue = (palette[rgb8bpp] & 0x000000ff);

                /*char ar[9] = {0};
                std::string v8_str = redb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ar);

                char ag[9] = {0};
                v8_str = greenb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ag);

                char ab[9] = {0};
                v8_str = blueb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ab);

                int c1 = static_cast<int>(std::stoi(ar, nullptr, 2) + 64);
                int c2 = static_cast<int>(std::stoi(ag, nullptr, 2) + 64);
                int c3 = static_cast<int>(std::stoi(ab, nullptr, 2) + 64);
                //int value = int(c1 );*/
                currentPixels.push_back(red);
                currentPixels.push_back(green);
                currentPixels.push_back(blue);
                //rgb = qRgb(c1,c2,c3);
                //std::cout << (int)red << " . " << (int)green << " . " << (int)blue << std::endl;
                rgb = qRgb(red,green,blue);
                //unsigned int value = c1 << 5 || c2 << 2 || c3 << 0;
                //std::cout << value << std::endl;
                pic.setPixel(i8,j8,rgb);
                nindex8++;
            }
            nindex8 += npad8;
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }

}

void MainWindow::readBMP(char* filename,bmp& oBmp){
    size_t i;
    FILE* f = fopen(filename, "rb");

    //unsigned char info[54];
    //fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    std::vector<unsigned char> info (54);
    fread(&info[0], sizeof(unsigned char), 54, f); // read the 54-byte header

    //extract width, height, and size file
    int width = int(info[21] << 24 | info[20] << 16 | info[19] << 8 | info[18]);
    int height = int(info[25] << 24 | info[24] << 16 | info[23] << 8 | info[22]);
    int sizeFile = int(info[29] << 8 | info[28]);
    int size1 = int(info[6] << 24 | info[5] << 16 | info[4] << 8 | info[3]);
    //int raw = int(info[34] << 24 | info[33] << 16 | info[32] << 8 | info[31]);
    int start = int(info[13] << 24 | info[12] << 16 | info[11] << 8 | info[10]);
    int nsize = int(info[37] << 24 | info[36] << 16 | info[35] << 8 | info[34]);
    int pal = int(info[49] << 24 | info[48] << 16 | info[47] << 8 | info[46]);
    int ppMeterX = int(info[41] << 24 | info[40] << 16 | info[39] << 8 | info[38]);
    int ppMeterY = int(info[45] << 24 | info[44] << 16 | info[43] << 8 | info[42]);

    std::cout << "sizefile " << sizeFile << std::endl;
    std::cout << "width  " << width << std::endl;
    std::cout << "height " << height<< std::endl;
    std::cout << "raw " << nsize << std::endl;
    std::cout << "size " << size1 << std::endl;
    std::cout << "start " << start << std::endl;
    std::cout << "pal " << pal << std::endl;
    std::cout << "ppX " << ppMeterX << std::endl;
    std::cout << "ppY " << ppMeterY << std::endl;

    //std::vector<unsigned char> info2 (start-54);
    //fread(&info2[0], sizeof(unsigned char), start-54, f); // read the 54-byte header

    int numberColor = 0;
    if(pal > 0){
        numberColor = pal;
    }
    else{
        numberColor = (1&0xff) << 8;
    }


    if(sizeFile == 24){
        int size = 3*width * height;
        std::vector<unsigned char> fullData (size);
        std::vector<unsigned char> dataRgb (3*width * height);

        fread(&fullData[0], sizeof(unsigned char), size, f); // read the rest of the data at once
        fclose(f);
        i = 0;

        for(int j =0;j<size;j+=3){
            dataRgb[i+2] = fullData[j];
            dataRgb[i+1] = fullData[j+1];
            dataRgb[i] = fullData[j+2];
            i+=3;
        }
        //stored in object bmp
        oBmp.setParameters(dataRgb,"24",width,height,sizeFile,numberColor*4);
    }
    else if(sizeFile == 8){
        if (nsize == 0)
                   {
                   nsize = ((((width*8)+31) & ~31 ) >> 3);
                   nsize *= height;
                   }
        int size = width * height;
        std::vector<unsigned char> fullData (size);
        std::vector<unsigned char> dataRgb (width * height);
        std::vector<unsigned char> bufferColors(numberColor*4);
        fread(&bufferColors[0],sizeof(unsigned char),numberColor*4,f);
        int npad8 = (nsize / height) - width;
        fread(&fullData[0], sizeof(unsigned char), (width+npad8)*height, f); // read the rest of the data at once
        fclose(f);
        i = 0;
        dataRgb= fullData;
        /*for(int j =0;j<size;j+=3){
            dataRgb[i+2] = fullData[j];
            dataRgb[i+1] = fullData[j+1];
            dataRgb[i] = fullData[j+2];
            i+=3;
        }*/
        //stored in object bmp
        oBmp.setParameters(dataRgb,"8",width,height,sizeFile,numberColor,bufferColors,nsize);
    }

}
void MainWindow::save()
{
    infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void MainWindow::print()
{
    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::constrast()
{
    numOpe = 2;
    groupBox->setTitle("Adjust Constrast");
    slider->setRange(-10,10);
    slider->setValue(0);
    groupBox->setHidden(false);

}

void MainWindow::changeConstrast(){
    //double factorAlpha = (259.0 * (alpha + 255.0)) / (255.0 * (259.0 - alpha));
    std::vector<unsigned> changedPixels;
    std::cout << alpha << std::endl;
    //unsigned int beta = 100;
    for(int i=0;i<currentPixels.size();i++)
    {
        if ((int)(currentPixels[i] * alpha) <= 255 && (int)(currentPixels[i] * alpha) >=0)
            changedPixels.push_back((int)(currentPixels[i] * alpha));
        else
        {
            if (alpha >= 0)
                changedPixels.push_back(255);
            else
                changedPixels.push_back(0);
        }
    }
    if(oBmp.size == 24){
        int countRows=oBmp.height-1;
        int countColumns=0;
        QRgb valueRGB;
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        for(int i = 0;i<oBmp.width*oBmp.height*3;i+=3){
            valueRGB = qRgb(changedPixels[i],changedPixels[i+1],changedPixels[i+2]);
            pic.setPixel(countColumns,countRows,valueRGB);
            countColumns++;
            if(countColumns == oBmp.width){
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
    else{
            QRgb rgb;
            int* palette = new int [oBmp.numberColors];
                    //byte* bpalette = new byte[numberColor*4];
                    //f.read (bpalette, 0, numberColor*4);
                    int nindex8 = 0;
                    for (int n = 0; n < oBmp.numberColors; n++)
                    {
                        palette[n] = (255&0xff)<<24
                                    | (((int)oBmp.bufferColors[nindex8+2]&0xff)<<16)
                                    | (((int)oBmp.bufferColors[nindex8+1]&0xff)<<8)
                                    | (int)oBmp.bufferColors[nindex8]&0xff;
                        nindex8+=4;
                    }

                    QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGB888);
                    nindex8 = 0;
                    int i = 0;
                    int npad8 = (oBmp.sizeRaw / oBmp.height) - oBmp.width;
                    for (int j8 = oBmp.height-1; j8 >= 0 ; j8--)
                    {
                        for (int i8 = 0; i8 < oBmp.width; i8++)
                        {

                            int valuecharR = changedPixels[i];
                            int valuecharG = changedPixels[i+1];
                            int valuecharB = changedPixels[i+2];
                            rgb = qRgb(valuecharR,valuecharG,valuecharB);
                            pic.setPixel(i8,j8,rgb);
                            //std::cout << palette [changedPixels[i]] << std::endl;
                            nindex8++;
                            i+=3;
                        }
                        nindex8 += npad8;
                    }

                    QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
                    scene->clear();
                    scene->addItem(item2);
                    graphicsView->setScene(scene);
                    graphicsView->show();
        }

}
void MainWindow::brightness()
{
    numOpe = 1;
    //if(oBmp.size == 8){
    //    slider->setRange(0,oBmp.numberColors);
    //}
    groupBox->setHidden(false);
}

void MainWindow::changeBrightness(){
    std::vector<unsigned> changedPixels;
    //unsigned int beta = 100;
    for(int i=0;i<currentPixels.size();i++)
    {
        if (currentPixels[i] + beta <= 255 && currentPixels[i] + beta >=0)
            changedPixels.push_back(currentPixels[i] + beta);
        else
            if (beta >= 0)
                changedPixels.push_back(255);
            else
                changedPixels.push_back(0);
    }
    if(oBmp.size == 24){
        int countRows=oBmp.height-1;
        int countColumns=0;
        QRgb valueRGB;
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        for(int i = 0;i<oBmp.width*oBmp.height*3;i+=3){
            valueRGB = qRgb(changedPixels[i],changedPixels[i+1],changedPixels[i+2]);
            pic.setPixel(countColumns,countRows,valueRGB);
            countColumns++;
            if(countColumns == oBmp.width){
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
    else{
        QRgb rgb;
        int* palette = new int [oBmp.numberColors];
                //byte* bpalette = new byte[numberColor*4];
                //f.read (bpalette, 0, numberColor*4);
                int nindex8 = 0;
                for (int n = 0; n < oBmp.numberColors; n++)
                {
                    palette[n] = (255&0xff)<<24
                                | (((int)oBmp.bufferColors[nindex8+2]&0xff)<<16)
                                | (((int)oBmp.bufferColors[nindex8+1]&0xff)<<8)
                                | (int)oBmp.bufferColors[nindex8]&0xff;
                    nindex8+=4;
                }

                QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGB888);
                nindex8 = 0;
                int i = 0;
                int npad8 = (oBmp.sizeRaw / oBmp.height) - oBmp.width;
                for (int j8 = oBmp.height-1; j8 >= 0 ; j8--)
                {
                    for (int i8 = 0; i8 < oBmp.width; i8++)
                    {

                        int valuecharR = changedPixels[i];
                        int valuecharG = changedPixels[i+1];
                        int valuecharB = changedPixels[i+2];
                        rgb = qRgb(valuecharR,valuecharG,valuecharB);
                        pic.setPixel(i8,j8,rgb);
                        //std::cout << palette [changedPixels[i]] << std::endl;
                        nindex8++;
                        i+=3;
                    }
                    nindex8 += npad8;
                }

                QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
                scene->clear();
                scene->addItem(item2);
                graphicsView->setScene(scene);
                graphicsView->show();
    }

}

void MainWindow::combinationAdd(){
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Choose image to Combine"), "/Users/luisernestocolchado", tr("Image Files (*.bmp)"));
    QByteArray ba = fileName.toLocal8Bit();
    char* cfilename = ba.data();
    readBMP(cfilename,oBmpCombine);
    //data = ;
    QRgb value;
    int r,g,b;


    int maxWidth;
    int maxHeight;
    int minWidth;
    int minHeight;

    if(oBmpCombine.width > oBmp.width){
        maxWidth = oBmpCombine.width;
        minWidth = oBmp.width;
    }
    else{
        maxWidth = oBmp.width;
        minWidth = oBmpCombine.width;
    }
    if(oBmpCombine.height > oBmp.height){
        maxHeight = oBmpCombine.height;
        minHeight = oBmp.height;
    }
    else{
        maxHeight = oBmp.height;
        minHeight = oBmpCombine.height;
    }
    int countRows=maxHeight-1;
    int countColumns=0;
    std::vector<unsigned int> pixelsMax(maxWidth*maxHeight*3);
    std::vector<unsigned int> value1(currentPixels.size());
    std::vector<unsigned int> value2(oBmpCombine.data.size());
    int j=oBmp.width*3-1;
    int p=oBmp.width*3;
    for(int i =currentPixels.size()-1;i>=0;i--){
        value1[j] = currentPixels[i];
        j--;
        if(j < (p-(oBmp.width*3))){
            p+=oBmp.width*3;
            j = p-1;

        }
    }
    j=oBmpCombine.width*3-1;
    p=oBmpCombine.width*3;
    for(int i =oBmpCombine.data.size()-1;i>=0;i--){
        value2[j] = (unsigned int)oBmpCombine.data[i];
        j--;
        if(j < (p-(oBmpCombine.width*3))){
            p+=oBmpCombine.width*3;
            j = p-1;
        }
    }

    QRgb valuergb1;
    std::vector<unsigned int> intRgb(maxWidth*3,0);
    std::vector<std::vector<unsigned int>> image1(maxHeight);
    std::vector<std::vector <unsigned int>> image2(maxHeight);
    std::vector<std::vector <unsigned int>> image3(maxHeight);
    int count=0;
    for(int i =0;i<oBmp.height;i++){
        for(int j=0;j<oBmp.width*3;j+=3){
            intRgb[j] = value1[count];
            intRgb[j+1] = value1[count+1];
            intRgb[j+2] = value1[count+2];
            count+=3;
        }
        image1[i] = intRgb;
    }
    if(oBmp.height < maxHeight){
        for(int h = oBmp.height; h < maxHeight;h++){
            std::cout << " h " << h << std::endl;
            for(int j=0;j<oBmp.width*3;j+=3){
                intRgb[j] = 0;
                intRgb[j+1] = 0;
                intRgb[j+2] = 0;
            }
            image1[h] = intRgb;
        }

    }

    count=0;
    for(int i =0;i<oBmpCombine.height;i++){

        for(int j=0;j<oBmpCombine.width*3;j+=3){
            intRgb[j] = value2[count];
            intRgb[j+1] = value2[count+1];
            intRgb[j+2] = value2[count+2];
            count+=3;
        }
        image2[i] = intRgb;

    }
    if(oBmpCombine.height < maxHeight){
        std::cout << "here" << std::endl;
        for(int h = oBmpCombine.height; h < maxHeight;h++){
            std::cout << " h " << h << std::endl;
            for(int j=0;j<oBmpCombine.width*3;j+=3){
                intRgb[j] = 0;
                intRgb[j+1] = 0;
                intRgb[j+2] = 0;
            }
            image2[h] = intRgb;
        }
    }

    count =0;
    for(int i = maxHeight - 1;i>=0;i--){
        for(int k = 0;k < maxWidth*3;k++){
            pixelsMax[count]  = image1[i][k] + image2[i][k];
            count++;
        }
    }

    if(oBmpCombine.size == 24){
        QImage pic = QImage(maxWidth, maxHeight, QImage::Format_RGBA8888);
        for(int i = 0;i<maxWidth*maxHeight*3;i+=3){
            r = pixelsMax[i];
            g = pixelsMax[i+1];
            b = pixelsMax[i+2];
            if(r > 255){
                r = 255;
            }
            if(g > 255){
                g = 255;
            }
            if(b > 255){
                b = 255;
            }
            value = qRgb(r,g,b);
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == maxWidth){
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
    else if(oBmp.size == 8){
        int* palette = new int [oBmp.numberColors];
        //byte* bpalette = new byte[numberColor*4];
        //f.read (bpalette, 0, numberColor*4);
        int rgb8bpp;
        int nindex8 = 0;
        QRgb rgb;
        for (int n = 0; n < oBmp.numberColors; n++)
        {
            palette[n] = (255&0xff)<<24
                        | (((int)oBmp.bufferColors[nindex8+2]&0xff)<<16)
                        | (((int)oBmp.bufferColors[nindex8+1]&0xff)<<8)
                        | (int)oBmp.bufferColors[nindex8]&0xff;
            nindex8+=4;
        }
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        nindex8 = 0;
        int npad8 = (oBmp.sizeRaw / oBmp.height) - oBmp.width;
        for (int j8 = oBmp.height-1; j8 >= 0 ; j8--)
        {
            for (int i8 = 0; i8 < oBmp.width; i8++)
            {

                rgb8bpp = (int)oBmp.data[nindex8]&0xff;
                //std::cout << rgb8bpp << std::endl;
                int red = (palette[rgb8bpp] & 0x00ff0000) >> 16;
                int green = (palette[rgb8bpp] & 0x0000ff00) >> 8;
                int blue = (palette[rgb8bpp] & 0x000000ff);

                /*char ar[9] = {0};
                std::string v8_str = redb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ar);

                char ag[9] = {0};
                v8_str = greenb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ag);

                char ab[9] = {0};
                v8_str = blueb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ab);

                int c1 = static_cast<int>(std::stoi(ar, nullptr, 2) + 64);
                int c2 = static_cast<int>(std::stoi(ag, nullptr, 2) + 64);
                int c3 = static_cast<int>(std::stoi(ab, nullptr, 2) + 64);
                //int value = int(c1 );*/
                currentPixels.push_back(red);
                currentPixels.push_back(green);
                currentPixels.push_back(blue);
                //rgb = qRgb(c1,c2,c3);
                //std::cout << (int)red << " . " << (int)green << " . " << (int)blue << std::endl;
                rgb = qRgb(red,green,blue);
                //unsigned int value = c1 << 5 || c2 << 2 || c3 << 0;
                //std::cout << value << std::endl;
                pic.setPixel(i8,j8,rgb);
                nindex8++;
            }
            nindex8 += npad8;
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
}

void MainWindow::combinationOr(){
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Choose image to Combine"), "/Users/luisernestocolchado", tr("Image Files (*.bmp)"));
    QByteArray ba = fileName.toLocal8Bit();
    char* cfilename = ba.data();
    readBMP(cfilename,oBmpCombine);
    //data = ;
    QRgb value;
    int r,g,b;


    int maxWidth;
    int maxHeight;
    int minWidth;
    int minHeight;

    if(oBmpCombine.width > oBmp.width){
        maxWidth = oBmpCombine.width;
        minWidth = oBmp.width;
    }
    else{
        maxWidth = oBmp.width;
        minWidth = oBmpCombine.width;
    }
    if(oBmpCombine.height > oBmp.height){
        maxHeight = oBmpCombine.height;
        minHeight = oBmp.height;
    }
    else{
        maxHeight = oBmp.height;
        minHeight = oBmpCombine.height;
    }
    int countRows=maxHeight-1;
    int countColumns=0;
    std::vector<unsigned int> pixelsMax(maxWidth*maxHeight*3);
    std::vector<unsigned int> value1(currentPixels.size());
    std::vector<unsigned int> value2(oBmpCombine.data.size());
    int j=oBmp.width*3-1;
    int p=oBmp.width*3;
    for(int i =currentPixels.size()-1;i>=0;i--){
        value1[j] = currentPixels[i];
        j--;
        if(j < (p-(oBmp.width*3))){
            p+=oBmp.width*3;
            j = p-1;

        }
    }
    j=oBmpCombine.width*3-1;
    p=oBmpCombine.width*3;
    for(int i =oBmpCombine.data.size()-1;i>=0;i--){
        value2[j] = (unsigned int)oBmpCombine.data[i];
        j--;
        if(j < (p-(oBmpCombine.width*3))){
            p+=oBmpCombine.width*3;
            j = p-1;
        }
    }

    QRgb valuergb1;
    std::vector<unsigned int> intRgb(maxWidth*3,1);
    std::vector<std::vector<unsigned int>> image1(maxHeight);
    std::vector<std::vector <unsigned int>> image2(maxHeight);
    std::vector<std::vector <unsigned int>> image3(maxHeight);
    int count=0;
    for(int i =0;i<oBmp.height;i++){
        for(int j=0;j<oBmp.width*3;j+=3){
            intRgb[j] = value1[count];
            intRgb[j+1] = value1[count+1];
            intRgb[j+2] = value1[count+2];
            count+=3;
        }
        image1[i] = intRgb;
    }
    if(oBmp.height < maxHeight){
        for(int h = oBmp.height; h < maxHeight;h++){
            std::cout << " h " << h << std::endl;
            for(int j=0;j<oBmp.width*3;j+=3){
                intRgb[j] = 1;
                intRgb[j+1] = 1;
                intRgb[j+2] = 1;
            }
            image1[h] = intRgb;
        }

    }

    count=0;
    for(int i =0;i<oBmpCombine.height;i++){

        for(int j=0;j<oBmpCombine.width*3;j+=3){
            intRgb[j] = value2[count];
            intRgb[j+1] = value2[count+1];
            intRgb[j+2] = value2[count+2];
            count+=3;
        }
        image2[i] = intRgb;

    }
    if(oBmpCombine.height < maxHeight){
        std::cout << "here" << std::endl;
        for(int h = oBmpCombine.height; h < maxHeight;h++){
            std::cout << " h " << h << std::endl;
            for(int j=0;j<oBmpCombine.width*3;j+=3){
                intRgb[j] = 1;
                intRgb[j+1] = 1;
                intRgb[j+2] = 1;
            }
            image2[h] = intRgb;
        }
    }

    count =0;
    for(int i = maxHeight - 1;i>=0;i--){
        for(int k = 0;k < maxWidth*3;k++){
            pixelsMax[count]  = image1[i][k] * image2[i][k];
            count++;
        }
    }

    if(oBmpCombine.size == 24){
        QImage pic = QImage(maxWidth, maxHeight, QImage::Format_RGBA8888);
        for(int i = 0;i<maxWidth*maxHeight*3;i+=3){
            r = pixelsMax[i];
            g = pixelsMax[i+1];
            b = pixelsMax[i+2];
            if(r > 255){
                r = 255;
            }
            if(g > 255){
                g = 255;
            }
            if(b > 255){
                b = 255;
            }
            value = qRgb(r,g,b);
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == maxWidth){
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
    else if(oBmp.size == 8){
        int* palette = new int [oBmp.numberColors];
        //byte* bpalette = new byte[numberColor*4];
        //f.read (bpalette, 0, numberColor*4);
        int rgb8bpp;
        int nindex8 = 0;
        QRgb rgb;
        for (int n = 0; n < oBmp.numberColors; n++)
        {
            palette[n] = (255&0xff)<<24
                        | (((int)oBmp.bufferColors[nindex8+2]&0xff)<<16)
                        | (((int)oBmp.bufferColors[nindex8+1]&0xff)<<8)
                        | (int)oBmp.bufferColors[nindex8]&0xff;
            nindex8+=4;
        }
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        nindex8 = 0;
        int npad8 = (oBmp.sizeRaw / oBmp.height) - oBmp.width;
        for (int j8 = oBmp.height-1; j8 >= 0 ; j8--)
        {
            for (int i8 = 0; i8 < oBmp.width; i8++)
            {

                rgb8bpp = (int)oBmp.data[nindex8]&0xff;
                //std::cout << rgb8bpp << std::endl;
                int red = (palette[rgb8bpp] & 0x00ff0000) >> 16;
                int green = (palette[rgb8bpp] & 0x0000ff00) >> 8;
                int blue = (palette[rgb8bpp] & 0x000000ff);

                /*char ar[9] = {0};
                std::string v8_str = redb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ar);

                char ag[9] = {0};
                v8_str = greenb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ag);

                char ab[9] = {0};
                v8_str = blueb.to_string();
                std::copy(v8_str.begin(), v8_str.end(), ab);

                int c1 = static_cast<int>(std::stoi(ar, nullptr, 2) + 64);
                int c2 = static_cast<int>(std::stoi(ag, nullptr, 2) + 64);
                int c3 = static_cast<int>(std::stoi(ab, nullptr, 2) + 64);
                //int value = int(c1 );*/
                currentPixels.push_back(red);
                currentPixels.push_back(green);
                currentPixels.push_back(blue);
                //rgb = qRgb(c1,c2,c3);
                //std::cout << (int)red << " . " << (int)green << " . " << (int)blue << std::endl;
                rgb = qRgb(red,green,blue);
                //unsigned int value = c1 << 5 || c2 << 2 || c3 << 0;
                //std::cout << value << std::endl;
                pic.setPixel(i8,j8,rgb);
                nindex8++;
            }
            nindex8 += npad8;
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
}

void MainWindow::hor7(){
    std::vector<unsigned int> intRgb((oBmp.width*3)+18,0);
    std::vector<std::vector<unsigned int>> image1(oBmp.height);
    std::vector<unsigned int> value1(currentPixels.size());
    std::vector<int> kernel = {1,1,1,1,1,1,1};
    int j=oBmp.width*3-1;
    int p=oBmp.width*3;
    std::vector<std::vector<unsigned int>> imageResult(oBmp.height);
    std::vector<unsigned int> intRgbR((oBmp.width*3),0);
    for(int i =currentPixels.size()-1;i>=0;i--){
        value1[j] = currentPixels[i];
        j--;
        if(j < (p-(oBmp.width*3))){
            p+=oBmp.width*3;
            j = p-1;

        }
    }
    int count=0;
    for(int i =0;i<oBmp.height;i++){
        for(int j=9;j<oBmp.width*3+9;j+=3){
            intRgb[j] = value1[count];
            intRgb[j+1] = value1[count+1];
            intRgb[j+2] = value1[count+2];
            count+=3;
        }
        image1[i] = intRgb;
        for(auto i : intRgb){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    std::cout << " ******* " << std::endl;
    for(int i =0;i<image1.size();i++){

        for(int j=9;j<image1[i].size()-18;j+=3){
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for(int k =0;k<7;k++){
                sumR += image1[i][(j-9)+(k*3)] * kernel[k];
                sumG += image1[i][((j-9)+1)+(k*3)] * kernel[k];
                sumB += image1[i][((j-9)+2)+(k*3)] * kernel[k];
            }
            sumR/=7;
            sumG/=7;
            sumB/=7;
            intRgbR[j-9] = sumR;
            intRgbR[(j-9)+1] = sumG;
            intRgbR[(j-9)+2] = sumB;
        }
        imageResult[i] = intRgbR;
        for(auto i : intRgbR){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    count =0;
    std::vector<unsigned int> pixelsMax(oBmp.height*oBmp.width*3);
    for(int i = imageResult.size() - 1;i>=0;i--){
        for(int k = 0;k < imageResult[i].size();k++){
            pixelsMax[count]  = imageResult[i][k];
            count++;
        }
    }
    unsigned r,g,b;
    QRgb value;
    int countColumns = 0;
    int countRows = oBmp.height - 1;
    if(oBmp.size == 24){
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        for(int i = 0;i<oBmp.width*oBmp.height*3;i+=3){
            r = pixelsMax[i];
            g = pixelsMax[i+1];
            b = pixelsMax[i+2];
            if(r > 255){
                r = 255;
            }
            if(g > 255){
                g = 255;
            }
            if(b > 255){
                b = 255;
            }
            value = qRgb(r,g,b);
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == oBmp.width){
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
}

void MainWindow::matrix3x3(){
    std::vector<unsigned int> intRgb((oBmp.width*3)+6,0);
    std::vector<std::vector<unsigned int>> image1(oBmp.height+2);
    std::vector<unsigned int> value1(currentPixels.size());

    std::vector<int> rowKernel1 = {1,2,1};
    std::vector<int> rowKernel2 = {2,4,2};
    std::vector<int> rowKernel3 = {1,2,1};
    std::vector<std::vector<int>> kernel = {rowKernel1,rowKernel2,rowKernel3};

    int j=oBmp.width*3-1;
    int p=oBmp.width*3;
    std::vector<std::vector<unsigned int>> imageResult(oBmp.height);
    std::vector<unsigned int> intRgbR((oBmp.width*3),0);
    for(int i =currentPixels.size()-1;i>=0;i--){
        value1[j] = currentPixels[i];
        j--;
        if(j < (p-(oBmp.width*3))){
            p+=oBmp.width*3;
            j = p-1;

        }
    }
    int count=0;
    for(int i =0;i<oBmp.height+2;i++){
        if(i == 0 || i == oBmp.height+1){
            image1[i] = intRgb;
            for(auto i : intRgb){
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        else{
            for(int j=3;j<oBmp.width*3+3;j+=3){
                intRgb[j] = value1[count];
                intRgb[j+1] = value1[count+1];
                intRgb[j+2] = value1[count+2];
                count+=3;
            }
            image1[i] = intRgb;
            for(auto i : intRgb){
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << " ******* " << std::endl;
    for(int i =1;i<image1.size()-1;i++){
        for(int j=3;j<image1[i].size()-3;j+=3){
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            int count = 0;
            for(int r =0;r<3;r++){
                for(int c =0;c<3;c++){
                    sumR += image1[i+(r-1)][(j-3)+(c*3)] * kernel[r][c];
                    sumG += image1[i+(r-1)][((j-3)+1)+(c*3)] * kernel[r][c];
                    sumB += image1[i+(r-1)][((j-3)+2)+(c*3)] * kernel[r][c];
                }
            }
            sumR/=9;
            sumG/=9;
            sumB/=9;
            intRgbR[j-3] = sumR;
            intRgbR[(j-3)+1] = sumG;
            intRgbR[(j-3)+2] = sumB;
        }
        imageResult[i] = intRgbR;
        for(auto i : intRgbR){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    count =0;
    std::vector<unsigned int> pixelsMax(oBmp.height*oBmp.width*3);
    for(int i = imageResult.size() - 1;i>=0;i--){
        for(int k = 0;k < imageResult[i].size();k++){
            pixelsMax[count]  = imageResult[i][k];
            count++;
        }
    }
    unsigned r,g,b;
    QRgb value;
    int countColumns = 0;
    int countRows = oBmp.height - 1;
    if(oBmp.size == 24){
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGBA8888);
        for(int i = 0;i<oBmp.width*oBmp.height*3;i+=3){
            r = pixelsMax[i];
            g = pixelsMax[i+1];
            b = pixelsMax[i+2];
            if(r > 255){
                r = 255;
            }
            if(g > 255){
                g = 255;
            }
            if(b > 255){
                b = 255;
            }
            value = qRgb(r,g,b);
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == oBmp.width){
                countRows--;
                countColumns=0;
            }
        }
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }
}

void MainWindow::paste()
{
    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::bold()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void MainWindow::italic()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void MainWindow::leftAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void MainWindow::rightAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void MainWindow::justify()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
}

void MainWindow::center()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void MainWindow::setLineSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::setParagraphSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}

void MainWindow::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
               "menu-bar menus and context menus."));
}

void MainWindow::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    constrastAct = new QAction(tr("&Adjust Constrast"), this);
    constrastAct->setStatusTip(tr("Undo the last operation"));
    connect(constrastAct, &QAction::triggered, this, &MainWindow::constrast);

    brightnessAct = new QAction(tr("&Adjust Brightness"), this);
    brightnessAct->setStatusTip(tr("Redo the last operation"));
    connect(brightnessAct, &QAction::triggered, this, &MainWindow::brightness);

    combinationAndAct = new QAction(tr("&Combination AND"), this);
    combinationAndAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(combinationAndAct, &QAction::triggered, this, &MainWindow::combinationAdd);

    combinationOrAct = new QAction(tr("&Combination OR"), this);
    combinationOrAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(combinationOrAct, &QAction::triggered, this, &MainWindow::combinationOr);

    hor7Act = new QAction(tr("&Horizontal 7"), this);
    hor7Act->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(hor7Act, &QAction::triggered, this, &MainWindow::hor7);

    matrix3x3Act = new QAction(tr("&Matrix 3x3"), this);
    matrix3x3Act->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(matrix3x3Act, &QAction::triggered, this, &MainWindow::matrix3x3);


    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, &QAction::triggered, this, &MainWindow::bold);

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, &QAction::triggered, this, &MainWindow::italic);

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, &QAction::triggered, this, &MainWindow::setLineSpacing);

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, &QAction::triggered,
            this, &MainWindow::setParagraphSpacing);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, &QAction::triggered, this, &MainWindow::leftAlign);

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, &QAction::triggered, this, &MainWindow::rightAlign);

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, &QAction::triggered, this, &MainWindow::justify);

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, &QAction::triggered, this, &MainWindow::center);

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    cbMenu = menuBar()->addMenu(tr("&Change constrast and brightness"));
    cbMenu->addAction(constrastAct);
    cbMenu->addAction(brightnessAct);
    cbMenu->addSeparator();

    combinationMenu = menuBar()->addMenu(tr("&Combination"));
    combinationMenu->addAction(combinationAndAct);
    combinationMenu->addAction(combinationOrAct);
    combinationMenu->addSeparator();

    filtersMenu = menuBar()->addMenu(tr("&Filters"));
    filtersMenu->addAction(hor7Act);
    filtersMenu->addAction(matrix3x3Act);
    filtersMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    formatMenu = filtersMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
}
