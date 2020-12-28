#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "mtetris.h"
#include <vector>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QColor>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>
#include <QtXml>
#include <QXmlStreamWriter>
#include <QXmlSimpleReader>
#include <QtCore>

#define GAME_UPDATE_INTERVAL 500
#define STAT_UPDATE_INTERVAL 250

#define SCORE_TITLE "Score: "
#define ROWSC_TITLE "Completed rows count: "
#define LEVEL_TITLE "Current level: "

#define CBUTTON_STARTTEXT "Start"
#define CBUTTON_STOPTEXT "Stop"
#define PBUTTON_PAUSETEXT "Pause"
#define PBUTTON_RESUMETEXT "Resume"

#define WINDOW_TITLE_IDLE "QTETRIS"
#define WINDOW_TITLE_GAME "QTETRIS - GAME IN PROGRESS"
#define WINDOW_TITLE_PAUSE "QTETRIS - PAUSE"

namespace Ui {
    class MainWindow;
}

typedef struct __ST2_RECORD_ENTRY
{
    QString RName;
    QString RScore;
    QString RLevel;
    QString RCRC;
} ST2_RECORD_ENTRY;

class MainWindow : public QMainWindow
{
        Q_OBJECT
        
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        
    private:
        Ui::MainWindow *ui;
        QGraphicsScene *gamefieldr;
        STetrisField2  *gamefield;
        STetrisFigure2 *figure;
        STetrisFigure2Generator *gen;
        QTimer *gametimer;
        QTimer *stattimer;
        QList<ST2_RECORD_ENTRY> *recs;
        
        bool gamestateflag = false;
        bool pausestateflag = false;
        bool iku = false;
        int currentscore = 0;
        int currentlevel = 0;
        int crc = 0;
        
    private slots:
        void keyPressEvent(QKeyEvent *e);
        void gameTimerUpdate();
        void statTimerUpdate();
        void on_pushButton_clicked();
        void on_pushButton_2_clicked();
        void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
