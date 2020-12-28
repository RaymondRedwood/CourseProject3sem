#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(640, 480);
    this->setWindowTitle(WINDOW_TITLE_IDLE);
    this->grabKeyboard();
    
    this->recs = new QList<ST2_RECORD_ENTRY>();
    QFile rtfile(QCoreApplication::applicationDirPath() + "/records.xml");
    if (rtfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader reader(&rtfile);
        while (reader.readNextStartElement())
        {
            while (reader.readNextStartElement())
            {
                if (reader.name() == "recordentry")
                {
                    QString rname = "";
                    QString rscore = "";
                    QString rlevel = "";
                    QString rcrc = "";
                    for (auto entry: reader.attributes())
                    {
                        if (entry.name() == "rname")
                        {
                            rname = entry.value().toString();
                        }
                        if (entry.name() == "rscore")
                        {
                            rscore = entry.value().toString();
                        }
                        if (entry.name() == "rlevel")
                        {
                            rlevel = entry.value().toString();
                        }
                        if (entry.name() == "rcrc")
                        {
                            rcrc = entry.value().toString();
                        }
                    }
                    ST2_RECORD_ENTRY ritem;
                    ritem.RName = rname;
                    ritem.RScore = rscore;
                    ritem.RLevel = rlevel;
                    ritem.RCRC = rcrc;
                    recs->push_back(ritem);
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(rname));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(rscore));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(rlevel));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(rcrc));
                }
            }
        }
        rtfile.close();
    }
    
    this->crc = 0;
    this->currentscore = 0;
    this->currentlevel = 0;
    this->gamestateflag = false;
    this->pausestateflag = false;
    this->iku = false;
    
    gamefieldr = new QGraphicsScene(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(gamefieldr);
    gamefield = new STetrisField2(gamefieldr, 15, 23);
    
    gen = new STetrisFigure2Generator;
    
    gametimer = new QTimer();
    gametimer->setInterval(GAME_UPDATE_INTERVAL);
    connect(gametimer, SIGNAL(timeout()), this, SLOT(gameTimerUpdate()));
    
    stattimer = new QTimer();
    stattimer->setInterval(STAT_UPDATE_INTERVAL);
    connect(stattimer, SIGNAL(timeout()), this, SLOT(statTimerUpdate()));
}

void MainWindow::statTimerUpdate()
{
    if (!this->pausestateflag)
    {
        if (this->crc != this->gamefield->getComplitedRowsCount())
        {
            switch (this->gamefield->getComplitedRowsCount() - this->crc)
            {
                case 1: 
                {
                    this->currentscore += 1000;
                    this->currentlevel += 1;
                    if (this->gametimer->interval() > 250)
                        this->gametimer->setInterval(this->gametimer->interval() - 25);
                } break;
                case 2: 
                {
                   this->currentscore += 2000;
                   this->currentlevel += 2;
                   for (int i = 0; i < 2000; i += 1000)
                   {
                       if (this->gametimer->interval() > 250)
                           this->gametimer->setInterval(this->gametimer->interval() - 25);
                   }
                } break;
                case 3: 
                {
                   this->currentscore += 3000;
                   this->currentlevel += 3;
                   for (int i = 0; i < 3000; i += 1000)
                   {
                       if (this->gametimer->interval() > 250)
                           this->gametimer->setInterval(this->gametimer->interval() - 25);
                   }
                } break;
                case 4: 
                {
                   this->currentscore += 4000;
                   this->currentlevel += 4;
                   for (int i = 0; i < 4000; i += 1000)
                   {
                       if (this->gametimer->interval() > 250)
                           this->gametimer->setInterval(this->gametimer->interval() - 25);
                   }
                } break;
                default:
                {
                   this->currentscore += 10000;
                   this->currentlevel += 10;
                   for (int i = 0; i < 10000; i += 1000)
                   {
                       if (this->gametimer->interval() > 250)
                           this->gametimer->setInterval(this->gametimer->interval() - 25);
                   }
                } break;
            }
            this->crc = this->gamefield->getComplitedRowsCount();
            this->ui->label->setStyleSheet("QLabel {color: red}");
            this->ui->label_2->setStyleSheet("QLabel {color: red}");
        }
        else
        {
            this->ui->label->setStyleSheet("QLabel {color: black}");
            this->ui->label_2->setStyleSheet("QLabel {color: black}");
        }
        
        this->ui->label->setText(QString(SCORE_TITLE) + QString::number(this->currentscore));
        this->ui->label_2->setText(QString(ROWSC_TITLE) + QString::number(this->crc));
        this->ui->label_3->setText(QString(LEVEL_TITLE) + QString::number(this->currentlevel));
    }
}

void MainWindow::gameTimerUpdate()
{
    if (!this->pausestateflag)
    {
        if (gamefield->getCurrentFigure() == nullptr)
        {
            if (!gamefield->addFigure(gen->makeFigure(0, 0)))
            {
                ui->pushButton->click();
            }
        }
        else
        {
            if (!gamefield->moveCurrentFigureDown())
            {
                gamefield->placeCurrentFigure();
                this->currentscore += 100;
                gamefield->completeRows();
                gamefield->completeRows();
                if (this->currentscore % 1000 == 0)
                {
                    this->currentlevel += 1;
                    if (this->gametimer->interval() > 250)
                        this->gametimer->setInterval(this->gametimer->interval() - 25);
                }
            }
        }
    }       
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if ((this->gamestateflag) && (!this->pausestateflag))
    {
        this->iku = true;
        switch (e->key())
        {
            case Qt::Key_Left:
            {
                gamefield->moveCurrentFigureLeft();        
            } break;
            case Qt::Key_Right:
            {
                gamefield->moveCurrentFigureRight();
            } break;
            case Qt::Key_Space:
            {
                if (gamefield->getCurrentFigure() != nullptr)
                {
                    if (!gamefield->moveCurrentFigureDown())
                    {
                        gamefield->placeCurrentFigure();
                        this->currentscore += 100;
                        gamefield->completeRows();
                        if (this->currentscore % 1000 == 0)
                        {
                            this->currentlevel += 1;
                            if (this->gametimer->interval() > 250)
                                this->gametimer->setInterval(this->gametimer->interval() - 25);
                        }
                    }
                }
            } break;
            case Qt::Key_Up:
            {
                gamefield->rotateCurrentFigureLeft();
            } break;
            case Qt::Key_Down:
            {
                gamefield->rotateCurrentFigureRight();
            } break;
        }
        this->iku = false;
    }
}

MainWindow::~MainWindow()
{
    stattimer->stop();
    delete stattimer;
    
    gametimer->stop();
    delete gametimer;
    
    delete gen;
    
    delete gamefield;
    delete gamefieldr;
    
    QFile rtfile(QCoreApplication::applicationDirPath() + "/records.xml");
    if (rtfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter xmlsaver(&rtfile);
        xmlsaver.setAutoFormatting(true);
        xmlsaver.writeStartDocument();
        xmlsaver.writeStartElement("records");
        for (auto entr: *recs)
        {
            xmlsaver.writeStartElement("recordentry");
            xmlsaver.writeAttribute("rname", entr.RName);
            xmlsaver.writeAttribute("rscore", entr.RScore);
            xmlsaver.writeAttribute("rlevel", entr.RLevel);
            xmlsaver.writeAttribute("rcrc", entr.RCRC);
            xmlsaver.writeEndElement();
        }
        xmlsaver.writeEndElement();
        xmlsaver.writeEndDocument();
        rtfile.close();
    }
    
    delete recs;
    
    this->releaseKeyboard();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (!gamestateflag)
    {
        gamestateflag = true;
        setWindowTitle(WINDOW_TITLE_GAME);
        ui->pushButton_2->setEnabled(true);
        stattimer->start();
        gametimer->setInterval(GAME_UPDATE_INTERVAL);
        gametimer->start();
        ui->pushButton->setText(QString(CBUTTON_STOPTEXT));
    }
    else
    {
        gamestateflag = false;
        ui->pushButton_2->setEnabled(false);
        gametimer->stop();
        stattimer->stop();
        iku = false;
        ui->pushButton->setText(CBUTTON_STARTTEXT);
        ui->label->setText(QString(SCORE_TITLE));
        ui->label->setStyleSheet("QLabel {color: black}");
        ui->label_2->setText(QString(ROWSC_TITLE));
        ui->label_2->setStyleSheet("QLabel {color: black}");
        ui->label_3->setText(QString(LEVEL_TITLE));
        ui->label_3->setStyleSheet("QLabel {color: black}");
        
        QMessageBox msgbox(QMessageBox::Information, 
                           "Game Over", 
                           "Game is over, your score is: " + QString::number(this->currentscore), 
                           QMessageBox::Ok);
        msgbox.exec();
        
        int maxscore = currentscore;
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
        {
            if (ui->tableWidget->itemAt(i, 1)->text().toInt() > maxscore)
                maxscore = ui->tableWidget->itemAt(i, 1)->text().toInt();
        }
        
        if (maxscore == currentscore)
        {
            QMessageBox msgbox2(QMessageBox::Information, 
                               "New Record!", 
                               "New High Score is: " + QString::number(this->currentscore), 
                               QMessageBox::Ok);
            msgbox2.exec();
            
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(getPlayerName()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::number(currentscore)));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::number(currentlevel)));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::number(crc)));
            ST2_RECORD_ENTRY ritem;
            ritem.RName = getPlayerName();
            ritem.RScore = QString::number(currentscore);
            ritem.RLevel = QString::number(currentlevel);
            ritem.RCRC = QString::number(crc);
            recs->push_back(ritem);
        }
        
        currentscore = 0;
        crc = 0;
        currentlevel = 0;
        
        setWindowTitle(WINDOW_TITLE_IDLE);
        gamefield->resetField();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!pausestateflag)
    {
        pausestateflag = true;
        ui->pushButton->setEnabled(false);
        this->setWindowTitle(WINDOW_TITLE_PAUSE);
    }
    else
    {
        pausestateflag = false;
        ui->pushButton->setEnabled(true);
        this->setWindowTitle(WINDOW_TITLE_GAME);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    this->ui->tableWidget->setRowCount(0);
    recs->clear();
}
