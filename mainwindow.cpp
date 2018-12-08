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

    beta = slider->value();
    brightness();
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
    readBMP(cfilename);
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
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == oBmp.width){
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
        int npad8 = (oBmp.sizeRaw / oBmp.height) - oBmp.width;
        for (int j8 = oBmp.height-1; j8 >= 0 ; j8--)
        {
            for (int i8 = 0; i8 < oBmp.width; i8++)
            {
            pic.setPixel(i8,j8,palette [((int)oBmp.data[nindex8]&0xff)]);
            nindex8++;
            }
            nindex8 += npad8;
       }


        /*QVector<float> colorTable(256);
        //for(int i=0;i<256;i++)
        //    colorTable[i] = qRgb(i,i,i);
        for(int i = 0; i < colorTable.size(); i++){
             float Lin = (i+0.5) / (float) (colorTable.size()-1);
             if (Lin <= 0.0f)
                 colorTable[i] = 0.0f;
             else if (Lin >= 1.0f)
                 colorTable[i] = 1.0f;
             else if (Lin < 0.0031308f)
                 colorTable[i] = Lin * 12.92f;
             else
                 colorTable[i] = std::pow(Lin, 1.0f / 2.4f) * 1.055f - 0.055f;
             //std::cout << colorTable[i] << std::endl;
        }

        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGB888);
        /*for(int i = 0;i<255;i++){
            value = qRgb(i,0,0);
            pic.setColor(i,value);
        }
        int countRows=oBmp.height-1;
        int countColumns=0;
        std::cout << "ddd" << std::endl;
        for(int i = 0;i<oBmp.width*oBmp.height;i+=1){
            int k = oBmp.data[i];
            //std::cout << k << std::endl;
            value = qRgb(colorTable[k],colorTable[k],colorTable[k]);
            pic.setPixel(countColumns,countRows,value);
            countColumns++;
            if(countColumns == oBmp.width){
                //std::cout << r << "-" << g << "-" << b << std::endl;
                countRows--;
                countColumns=0;
            }
        }*/
        QGraphicsPixmapItem* item2 = new QGraphicsPixmapItem(QPixmap::fromImage(pic));
        scene->clear();
        scene->addItem(item2);
        graphicsView->setScene(scene);
        graphicsView->show();
    }

}

void MainWindow::readBMP(char* filename){
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

void MainWindow::darken()
{
    groupBox->setHidden(false);
}

void MainWindow::brightness()
{
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

void MainWindow::upContrast()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::downContrast()
{
    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
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

    darkenAct = new QAction(tr("&Darken the image"), this);
    darkenAct->setStatusTip(tr("Undo the last operation"));
    connect(darkenAct, &QAction::triggered, this, &MainWindow::darken);

    brightnessAct = new QAction(tr("&Add bright to image"), this);
    brightnessAct->setStatusTip(tr("Redo the last operation"));
    connect(brightnessAct, &QAction::triggered, this, &MainWindow::brightness);

    upContrastAct = new QAction(tr("&Add contrast to image"), this);
    upContrastAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(upContrastAct, &QAction::triggered, this, &MainWindow::upContrast);

    downContrastAct = new QAction(tr("&Down contrast to image"), this);
    downContrastAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(downContrastAct, &QAction::triggered, this, &MainWindow::downContrast);

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

    filtersMenu = menuBar()->addMenu(tr("&Filters"));
    filtersMenu->addAction(darkenAct);
    filtersMenu->addAction(brightnessAct);
    filtersMenu->addSeparator();
    filtersMenu->addAction(upContrastAct);
    filtersMenu->addAction(downContrastAct);
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
