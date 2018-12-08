#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bmp.hpp"
#include <vector>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QGraphicsView;
class QGraphicsScene;
class QSlider;
class QGroupBox;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    std::vector<unsigned int> currentPixels;
    void readBMP(char* filename);
    bmp oBmp;
    unsigned char* data;
    QGraphicsView * graphicsView;
    QGraphicsScene *scene;
    QSlider *slider;
    QGroupBox *groupBox;
    int beta;
private slots:
    void changeSlider();
    void newFile();
    void open();
    void save();
    void print();
    void darken();
    void brightness();
    void upContrast();
    void downContrast();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *filtersMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *darkenAct;
    QAction *brightnessAct;
    QAction *upContrastAct;
    QAction *downContrastAct;

    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;

};

#endif
