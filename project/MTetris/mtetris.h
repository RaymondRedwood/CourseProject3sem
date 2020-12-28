#ifndef MTETRIS_H
#define MTETRIS_H

#include <string>
#include <vector>
#include <exception>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>

#define STF2_ZERO_BLOCK '0'
#define STF2_CYAN_BLOCK '1'
#define STF2_YELLOW_BLOCK '2'
#define STF2_PURPLE_BLOCK '3'
#define STF2_GREEN_BLOCK '4'
#define STF2_RED_BLOCK '5'
#define STF2_BLUE_BLOCK '6'
#define STF2_ORANGE_BLOCK '7'

QString getPlayerName();
QColor getColorFromCode(char colorcode);

class STetrisFigure2
{
    private:
        std::vector<std::vector<char> *> *figure;
        int x, y;
        
        std::vector<std::string>* splitStr(std::string input, std::string delimeter);
    public:
        STetrisFigure2(std::string pattern, int x, int y);
        STetrisFigure2(const STetrisFigure2 &source);
        STetrisFigure2& operator=(const STetrisFigure2 &source);
        
        void RotateLeft();
        void RotateRight();
        
        int getX();
        int getY();
        void setX(int value);
        void setY(int value);
        
        std::vector<std::vector<char> *>* getFigureStructure();
        int getFigureHeight();
        int getFigureWidth();
        
        ~STetrisFigure2();
};

typedef enum __STF2_UPDATE_MODE
{
    STF2_UPDATE_NONE,
    STF2_UPDATE_DOWN,
    STF2_UPDATE_LEFT,
    STF2_UPDATE_RIGHT
} STF2_UPDATE_MODE;

class STetrisField2
{
    private:
        //tetris field background 
        QList<QGraphicsLineItem *> *fblines;
        QGraphicsRectItem *fb;
        void eraseField();
        void drawField();
        
        //tetris field figures
        QList<QList<char> *> *blocks;
        QList<QList<QGraphicsRectItem *> *> *blocksR;
        void eraseFieldBlocks();
        void drawFieldBlocks();
        
        //common fields:
        int fieldheight;
        int fieldwidth;
        QGraphicsScene *frenderer;
        
        //current figure
        QList<QList<QGraphicsRectItem *> *> *blocksCF;
        STetrisFigure2 *currentfigure;
        void eraseCurrentFigure();
        void updateCurrentFigure(STF2_UPDATE_MODE direction);
        void drawCurrentFigure(); 
        
        //internal funcs
        void addCurrentFigureToField();
        
        bool validateFigurePosition(STetrisFigure2 *figure);
        
        int getRealRenderSquareWidth();
        int getRealRenderSquareHeight();
        int translateFieldXToSceneX(int FieldX);
        int translateFieldYToSceneY(int FieldY);
        
        void deleteRows(QList<int> *rows);
        
        //gamestats
        int completedrowscount;
    public:
        STetrisField2(QGraphicsScene *scene, int w, int h);
        
        //figure managment
        bool addFigure(STetrisFigure2 *figure);
        bool moveCurrentFigureLeft();
        bool moveCurrentFigureRight();
        bool moveCurrentFigureDown();
        bool rotateCurrentFigureLeft();
        bool rotateCurrentFigureRight();
        void placeCurrentFigure();
        
        //rows managment
        void completeRows();
        
        //gamestats
        int getComplitedRowsCount();
        
        STetrisFigure2* getCurrentFigure();
        
        void resetField();
        ~STetrisField2();
};

class STetrisFigure2Generator
{
    public:
        STetrisFigure2* makeFigure(int x, int y);
};

#endif // MTETRIS_H
