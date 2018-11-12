#include <QtWidgets>
#include "mainwindow.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    scene = new QGraphicsScene(this);
    graphicsView = new QGraphicsView(this);
    scene->setSceneRect(QRectF(2,2,800,800));

    QString fileName = "/Users/luisernestocolchado/bmp_file_99151/Bmp_file_Icon_256.png";
    QImage image(fileName);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    graphicsView->setScene(scene);
    graphicsView->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);

    layout->addWidget(graphicsView);
    widget->setLayout(layout);

    createActions();
    createMenus();

    setWindowTitle(tr("Processing Image"));
    showMaximized();
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
        QVector<QRgb> colorTable(256);
        for(int i=0;i<256;i++)
            colorTable[i] = qRgb(i,i,i);
        QImage pic = QImage(oBmp.width, oBmp.height, QImage::Format_RGB888);
        /*for(int i = 0;i<255;i++){
            value = qRgb(i,0,0);
            pic.setColor(i,value);
        }*/
        int countRows=oBmp.height-1;
        int countColumns=0;
        for(int i = 0;i<oBmp.width*oBmp.height;i+=1){
            int k = oBmp.data[i];
            pic.setPixel(countColumns,countRows,colorTable[k]);
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
    int raw = int(info[34] << 24 | info[33] << 16 | info[32] << 8 | info[31]);
    int start = int(info[13] << 24 | info[12] << 16 | info[11] << 8 | info[10]);

    std::cout << "sizefile " << sizeFile << std::endl;
    std::cout << "width  " << width << std::endl;
    std::cout << "height " << height<< std::endl;
    std::cout << "raw " << raw << std::endl;
    std::cout << "size " << size1 << std::endl;
    std::cout << "start " << start << std::endl;

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
        oBmp.setParameters(dataRgb,"24",width,height,sizeFile);
    }
    else if(sizeFile == 8){
        int size = width * height;
        std::vector<unsigned char> fullData (size);
        std::vector<unsigned char> dataRgb (width * height);

        fread(&fullData[0], sizeof(unsigned char), size, f); // read the rest of the data at once
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
        oBmp.setParameters(dataRgb,"8",width,height,sizeFile);
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

void MainWindow::undo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::redo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
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

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

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

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    formatMenu = editMenu->addMenu(tr("&Format"));
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
