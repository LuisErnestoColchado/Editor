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
    void readBMP(char* filename,bmp&);
    bmp oBmp;
    bmp oBmpCombine;
    unsigned char* data;
    QGraphicsView * graphicsView;
    QGraphicsScene *scene;
    QSlider *slider;
    QGroupBox *groupBox;
    int beta;
    double alpha;
    unsigned int numOpe;
    unsigned int numCom;
private slots:
    void changeSlider();
    void newFile();
    void open();
    void save();
    void print();
    void brightness();
    void changeBrightness();
    void constrast();
    void changeConstrast();
    void combinationAdd();
    void combinationOr();
    void hor7();
    void matrix3x3();
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
    QMenu *cbMenu;
    QMenu *filtersMenu;
    QMenu *combinationMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *brightnessAct;
    QAction *constrastAct;
    QAction *combinationAndAct;
    QAction *combinationOrAct;
    QAction *hor7Act;
    QAction *matrix3x3Act;

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
