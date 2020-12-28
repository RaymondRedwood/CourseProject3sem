#include "mtetris.h"

QString getPlayerName()
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
       name = qgetenv("USERNAME");
    
    return name;
}

QColor getColorFromCode(char colorcode)
{
    switch (colorcode) 
    {
        case '1': return QColor(0, 255, 255);
        case '2': return QColor(255, 255, 0);
        case '3': return QColor(125, 0, 125);
        case '4': return QColor(0, 255, 0);
        case '5': return QColor(255, 0, 0);
        case '6': return QColor(0, 0, 255);
        case '7': return QColor(255, 125, 0);
        default: return Qt::black;
    }
}

STetrisFigure2::STetrisFigure2(std::string pattern, int x, int y)
{
    std::vector<std::string> *entries = this->splitStr(pattern, " ");
    
    if (entries->size() < 1)
    {
        if (entries != nullptr) delete entries;
        throw "Invalid pattern detected!";
    }
    
    size_t lastlen = (*entries)[0].size();
    for (size_t i = 1; i < entries->size(); ++i)
    {
       if (lastlen != (*entries)[i].size())
       {
           if (entries != nullptr) delete entries;
           throw "Invalid pattern detected!";//std::exception("Invalid pattern detected!");
       }
    }
    
    this->figure = new std::vector<std::vector<char> *>;
    for (auto entry: *entries)
    {
        std::vector<char> *tmp = new std::vector<char>(); 
        for (char c: entry)
        {
            tmp->push_back(c);
        }
        this->figure->push_back(tmp);
    }
    
    
    
    if (entries != nullptr) delete entries;
    
    this->x = x;
    this->y = y;
}

STetrisFigure2::STetrisFigure2(const STetrisFigure2 &source)
{
    this->figure = new std::vector<std::vector<char> *>(*source.figure);
    for (int i = 0; i < static_cast<int>(this->figure->size()); ++i)
    {
        (*this->figure)[static_cast<unsigned int>(i)] 
                = new std::vector<char>(*(*source.figure)[static_cast<unsigned int>(i)]);  
    }
    this->x = source.x;
    this->y = source.y;
}

STetrisFigure2& STetrisFigure2::operator=(const STetrisFigure2 &source)
{
   if (this == &source) return *this;
   
   if (this->figure != nullptr)
   {
       for (unsigned int i = this->figure->size() - 1; static_cast<int>(i) >= 0; --i)
       {
           if ((*this->figure)[i] != nullptr)
            if ((*this->figure)[i]->size() > 0) 
            {
                delete (*this->figure)[i];
                this->figure->pop_back();
            }
       }
       delete this->figure;
   }
   
   this->figure = new std::vector<std::vector<char> *>(*source.figure); 
   this->x = source.x;
   this->y = source.y;
   
   return *this;
}

std::vector<std::string>* STetrisFigure2::splitStr(std::string input, std::string delimeter)
{
    std::vector<std::string> *result = new std::vector<std::string>();
    
    char* pTempStr = strdup(input.c_str());      
    char* pWord = strtok(pTempStr, delimeter.c_str());       
    while (pWord != nullptr)
    {
       result->push_back(pWord);
       pWord = strtok(nullptr, delimeter.c_str());        
    }
        
    free(pTempStr); 
    
    return result;
}

void STetrisFigure2::RotateLeft()
{
    std::vector<std::vector<char> *> *tmp = new std::vector<std::vector<char> *>();
    
    for (unsigned int j = (*this->figure)[0]->size() - 1; static_cast<int>(j) >= 0; --j)
    {
        std::vector<char> *tmp2 = new std::vector<char>();
        for (unsigned int i = 0; i < this->figure->size(); ++i)
        {
            tmp2->push_back((*(*this->figure)[i])[j]);
        }
        tmp->push_back(tmp2);
    }
    
    if (this->figure != nullptr)
    {
        for (unsigned int i = this->figure->size() - 1; static_cast<int>(i) >= 0; --i)
        {
            if ((*this->figure)[i] != nullptr)
             if ((*this->figure)[i]->size() > 0) 
             {
                 delete (*this->figure)[i];
                 this->figure->pop_back();
             }
        }
        delete this->figure;
    }
    
    this->figure = tmp;
}

void STetrisFigure2::RotateRight()
{
    std::vector<std::vector<char> *> *tmp = new std::vector<std::vector<char> *>();
    
    for (unsigned int j = 0; j < (*this->figure)[0]->size(); ++j)
    {
        std::vector<char> *tmp2 = new std::vector<char>();
        for (unsigned int i = this->figure->size() - 1; static_cast<int>(i) >= 0; --i)
        {
            tmp2->push_back((*(*this->figure)[i])[j]);
        }
        tmp->push_back(tmp2);
    }
    
    if (this->figure != nullptr)
    {
        for (unsigned int i = this->figure->size() - 1; static_cast<int>(i) >= 0; --i)
        {
            if ((*this->figure)[i] != nullptr)
             if ((*this->figure)[i]->size() > 0) 
             {
                 delete (*this->figure)[i];
                 this->figure->pop_back();
             }
        }
        delete this->figure;
    }
    
    this->figure = tmp;
}

int STetrisFigure2::getX()
{
    return this->x;
}

int STetrisFigure2::getY()
{
    return this->y;
}

void STetrisFigure2::setX(int value)
{
    this->x = value;
}

void STetrisFigure2::setY(int value)
{
    this->y = value;
}

std::vector<std::vector<char> *>* STetrisFigure2::getFigureStructure()
{
    return this->figure;
}

int STetrisFigure2::getFigureHeight()
{
    return static_cast<int>(this->figure->size());
}

int STetrisFigure2::getFigureWidth()
{
    return static_cast<int>((*this->figure)[0]->size());
}


STetrisFigure2::~STetrisFigure2()
{
    if (this->figure != nullptr)
    {
        for (unsigned int i = this->figure->size() - 1; static_cast<int>(i) >= 0; --i)
        {
            if ((*this->figure)[i] != nullptr)
             if ((*this->figure)[i]->size() > 0) 
             {
                 delete (*this->figure)[i];
                 this->figure->pop_back();
             }
        }
        delete this->figure;
    }
}

STetrisField2::STetrisField2(QGraphicsScene *scene, int w, int h)
{
    this->frenderer = scene;
    this->fieldheight = h;
    this->fieldwidth = w;
    
    this->fb = nullptr;
    this->fblines = nullptr;
    this->blocksCF = nullptr;
    this->blocksR = nullptr;
    this->currentfigure = nullptr;
    this->completedrowscount = 0;
    
    //now we need to initialize blocks array of the field
    this->blocks = new QList<QList<char> *>();
    for (int i = 0; i <= this->fieldwidth; ++i)
    {
        QList<char> *tmp = new QList<char>();
        for (int j = 0; j <= this->fieldheight; ++j)
        {
            tmp->push_back(STF2_ZERO_BLOCK);
        }
        this->blocks->push_back(tmp);
    }
    
    this->drawField();
}

void STetrisField2::addCurrentFigureToField()
{
    if (this->currentfigure == nullptr)
        return;
    
    if (this->blocks == nullptr)
        return;
    
    auto pos = this->currentfigure->getFigureStructure();
    int x = this->currentfigure->getX();
    int y = this->currentfigure->getY();
    int w = this->fieldwidth;
    int h = this->fieldheight;
    
    for (int i = 0; i < static_cast<int>(pos->size()); ++i)
    {
        for (int j = 0; j < static_cast<int>((*pos)[static_cast<unsigned int>(i)]->size()); ++j)
        {
            if ((*(*pos)[static_cast<unsigned int>(i)])[static_cast<unsigned int>(j)] != STF2_ZERO_BLOCK)
            {
                if ((x + i >= 0) && (y + j >= 0) && (x + i < w) && (y + j < h))
                    (*(*this->blocks)[x + i])[y + j] = (*(*pos)[static_cast<unsigned int>(i)])[static_cast<unsigned int>(j)];
            }
        }
    }
}

bool STetrisField2::validateFigurePosition(STetrisFigure2 *figure)
{
    if (this->blocks == nullptr)
        return false;
    
    if (figure == nullptr)
        return false;
    
    auto pos = figure->getFigureStructure();
    int x = figure->getX();
    int y = figure->getY();
    int w = this->fieldwidth;
    int h = this->fieldheight;
    
    for (int i = 0; i < static_cast<int>(pos->size()); ++i)
    {
        for (int j = 0; j < static_cast<int>((*pos)[static_cast<unsigned int>(i)]->size()); ++j)
        {
            if ((*(*pos)[static_cast<unsigned int>(i)])[static_cast<unsigned int>(j)] != STF2_ZERO_BLOCK)
            {
                if ((x + i >= 0) && (y + j >= 0) && (x + i < w) && (y + j < h))
                {
                    if ((*(*this->blocks)[x + i])[y + j] != STF2_ZERO_BLOCK)
                    {
                        return false;
                    }
                }
                else
                   return false;
            }
        }
    }
    
    return true;
}

int STetrisField2::getRealRenderSquareHeight()
{
    return static_cast<int>(this->frenderer->height()) / this->fieldheight;
}

int STetrisField2::getRealRenderSquareWidth()
{
    return static_cast<int>(this->frenderer->width()) / this->fieldwidth;
}

int STetrisField2::translateFieldXToSceneX(int FieldX)
{
    return FieldX * (this->getRealRenderSquareWidth());
}

int STetrisField2::translateFieldYToSceneY(int FieldY)
{
    return FieldY * (this->getRealRenderSquareHeight());
}

void STetrisField2::drawField()
{
    if ((this->fblines != nullptr) || (this->fb != nullptr))
        return;
    
    //step 1: we need to calculate real height and width of one square;
    
    int realheight = this->getRealRenderSquareHeight();
    int realwidth = this->getRealRenderSquareWidth();
    
    //step 2: fill all scene by black color;
    
    QRectF rect(0, 0, this->frenderer->width(), this->frenderer->height());
    QPen bgpen;
    QColor bgcolor;
    bgcolor.setRgb(0, 0, 0);
    bgpen.setColor(bgcolor);
    bgpen.setWidth(1);
    QBrush bgbrush;
    bgbrush.setStyle(Qt::SolidPattern);
    bgbrush.setColor(bgcolor);
    this->fb = new QGraphicsRectItem(rect);
    fb->setBrush(bgbrush);
    fb->setPen(bgpen);
    this->frenderer->addItem(this->fb);
    
    //step 3: draw grid
    
    QColor bglcolor;
    bglcolor.setRgb(0, 125, 0);
    QPen bglpen;
    bglpen.setColor(bglcolor);
    bglpen.setWidth(1);
    this->fblines = new QList<QGraphicsLineItem *>();
    for (int i = 0; i <= this->frenderer->width(); i+=realwidth)
    {
        for (int j = 0; j <= this->frenderer->height(); j+=realheight)
        {
            QGraphicsLineItem *gridline = new QGraphicsLineItem();
            gridline->setPen(bglpen);
            QLine tmp(i, 0, i, j);
            gridline->setLine(tmp);
            this->frenderer->addItem(gridline);
                    
            QGraphicsLineItem *gridline2 = new QGraphicsLineItem();
            gridline2->setPen(bglpen);        
            QLine tmp2(0, j, i, j);
            gridline2->setLine(tmp2);
            this->frenderer->addItem(gridline2);
        }
    }
}

void STetrisField2::eraseField()
{
    if ((this->fblines == nullptr) || (this->fb = nullptr))
        return;
    
    for (int i = this->fblines->size() - 1; i >= 0; --i)
    {
        if (((*this->fblines)[i]) != nullptr)
        {
            this->frenderer->items().removeAt(this->frenderer->items().indexOf((*this->fblines)[i]));
            delete (*this->fblines)[i];
        }
                
        this->fblines->removeLast();
    }
    
    this->frenderer->items().removeAt(this->frenderer->items().indexOf(this->fb));
    
    frenderer->update(0, 0, this->frenderer->width(), this->frenderer->height());
    
    delete this->fblines;
    delete this->fb;
}

void STetrisField2::drawCurrentFigure()
{
    if (this->blocksCF != nullptr)
        eraseCurrentFigure();
    
    if (this->currentfigure != nullptr)
    {
        int realheight = this->getRealRenderSquareHeight();
        int realwidth = this->getRealRenderSquareWidth();
        
        int realx = this->translateFieldXToSceneX(this->currentfigure->getX());
        int realy = this->translateFieldYToSceneY(this->currentfigure->getY());
        
        auto fg = this->currentfigure->getFigureStructure();
        
        this->blocksCF = new QList<QList<QGraphicsRectItem *> *>();
        
        for (unsigned int i = 0; i < fg->size(); ++i)
        {
            auto tmp = new QList<QGraphicsRectItem *>();
            this->blocksCF->push_back(tmp);
            for (unsigned int j = 0; j < (*fg)[i]->size(); ++j)
            {
                if (((*(*fg)[i])[j]) != STF2_ZERO_BLOCK)
                {
                    QRectF actualblock(realx + (realwidth * static_cast<int>(i)), realy + (realheight * static_cast<int>(j)), realwidth, realheight);
                    QBrush actualblockbrush;
                    actualblockbrush.setColor(getColorFromCode((*(*fg)[i])[j]));
                    actualblockbrush.setStyle(Qt::SolidPattern);
                    QPen actualblockpen;
                    actualblockpen.setColor(getColorFromCode((*(*fg)[i])[j]));
                    actualblockpen.setWidth(1);
                    QGraphicsRectItem *item = new QGraphicsRectItem(actualblock);
                    item->setBrush(actualblockbrush);
                    item->setPen(actualblockpen);
                    tmp->push_back(item);
                    this->frenderer->addItem(item);
                }
            }
        }
    }
}

void STetrisField2::updateCurrentFigure(STF2_UPDATE_MODE direction)
{
    if (this->blocksCF == nullptr)
        return;
    
    int realw = this->getRealRenderSquareWidth();
    int realh = this->getRealRenderSquareHeight();
    
    switch (direction)
    {
       case STF2_UPDATE_NONE: return;
       case STF2_UPDATE_DOWN:
        {
            for (auto row: *this->blocksCF)
            {
                for (auto column: *row)
                {
                    QRectF pos = column->rect();
                    pos.setRect(pos.x(), pos.y() + realh, pos.width(), pos.height());
                    column->setRect(pos);
                }
            }
            this->frenderer->update();
        } break;
       case STF2_UPDATE_LEFT:
        {
            for (auto row: *this->blocksCF)
            {
                for (auto column: *row)
                {
                    QRectF pos = column->rect();
                    pos.setRect(pos.x() - realw, pos.y(), pos.width(), pos.height());
                    column->setRect(pos);
                }
            } 
            this->frenderer->update();
        } break;
       case STF2_UPDATE_RIGHT:
        {
            for (auto row: *this->blocksCF)
            {
                for (auto column: *row)
                {
                    QRectF pos = column->rect();
                    pos.setRect(pos.x() + realw, pos.y(), pos.width(), pos.height());
                    column->setRect(pos);
                }
            } 
            this->frenderer->update();
        } break;
    }
}

void STetrisField2::eraseCurrentFigure()
{
    if (this->blocksCF == nullptr)
        return;
    
    for (int i = this->blocksCF->size() - 1; i >= 0; --i)
    {
        for (int j = (*this->blocksCF)[i]->size() - 1; j >= 0; --j)
        {
            this->frenderer->removeItem((*(*this->blocksCF)[i])[j]);
            delete (*(*this->blocksCF)[i])[j];
            (*this->blocksCF)[i]->removeLast();
        }
        delete (*this->blocksCF)[i];
        (*this->blocksCF).removeLast();
    }
    
   delete this->blocksCF;
   this->blocksCF = nullptr;
   this->frenderer->update();
}

void STetrisField2::drawFieldBlocks()
{
    if (this->blocksR != nullptr)
        return;
    
    int rw = this->getRealRenderSquareWidth();
    int rh = this->getRealRenderSquareHeight();
    
    this->blocksR = new QList<QList<QGraphicsRectItem *> *>;
    for (int i = 0; i < this->blocks->size(); ++i)
    {
        QList<QGraphicsRectItem *> *tmp = new QList<QGraphicsRectItem *>;
        for (int j = 0; j < (*this->blocks)[i]->size(); ++j)
        {
            if ((*(*this->blocks)[i])[j] != STF2_ZERO_BLOCK)
            {
                int rx = this->translateFieldXToSceneX(i);
                int ry = this->translateFieldYToSceneY(j);
                
                QColor blockcolor = getColorFromCode((*(*this->blocks)[i])[j]);
                QPen pen(blockcolor);
                pen.setWidth(1);
                QBrush brush(blockcolor, Qt::SolidPattern);
                QRectF blockrect(rx, ry, rw, rh);
                QGraphicsRectItem *bri = new QGraphicsRectItem();
                bri->setRect(blockrect);
                bri->setPen(pen);
                bri->setBrush(brush);
                this->frenderer->addItem(bri);
                tmp->push_back(bri);
            }
        }
        this->blocksR->push_back(tmp);
    }
   
    this->frenderer->update();
}

void STetrisField2::eraseFieldBlocks()
{
    if (this->blocksR == nullptr)
        return;
    
    for (int i = this->blocksR->size() - 1; i >= 0; --i)
    {
        for (int j = (*this->blocksR)[i]->size() - 1; j >= 0; --j)
        {
            this->frenderer->removeItem((*(*this->blocksR)[i])[j]);
            delete (*(*this->blocksR)[i])[j];
            (*this->blocksR)[i]->removeLast();
        }
        delete (*this->blocksR)[i];
        this->blocksR->removeLast();
    }
    
    delete this->blocksR;
    this->blocksR = nullptr;
    this->frenderer->update();
}

bool STetrisField2::addFigure(STetrisFigure2 *figure)
{
    if (this->currentfigure != nullptr)
        return false;
    
    if (!this->validateFigurePosition(figure))
        return false;
    
    this->currentfigure = figure;
    this->drawCurrentFigure();
    
    return true;
}

bool STetrisField2::moveCurrentFigureLeft()
{
    if (this->currentfigure == nullptr)
        return false;
    
    //STetrisFigure2 tmp = STetrisFigure2(*this->currentfigure);
    //tmp.setX(tmp.getX() - 1);
    //if (!this->validateFigurePosition(&tmp))
    //    return false;
    
    this->currentfigure->setX(this->currentfigure->getX() - 1);
    if (!this->validateFigurePosition(this->currentfigure))
    {
        this->currentfigure->setX(this->currentfigure->getX() + 1);
        return false;
    }
    this->updateCurrentFigure(STF2_UPDATE_LEFT);
    
    return true;
}

bool STetrisField2::moveCurrentFigureRight()
{
    if (this->currentfigure == nullptr)
        return false;
    
    //STetrisFigure2 tmp = STetrisFigure2(*this->currentfigure);
    //tmp.setX(tmp.getX() + 1);
    //if (!this->validateFigurePosition(&tmp))
    //    return false;
    
    this->currentfigure->setX(this->currentfigure->getX() + 1);
    if (!this->validateFigurePosition(this->currentfigure))
    {
        this->currentfigure->setX(this->currentfigure->getX() - 1);
        return false;
    }
    this->updateCurrentFigure(STF2_UPDATE_RIGHT);
    
    return true;
}

bool STetrisField2::moveCurrentFigureDown()
{
    if (this->currentfigure == nullptr)
        return false;
    
    //STetrisFigure2 tmp = STetrisFigure2(*this->currentfigure);
    //tmp.setY(tmp.getY() + 1);
    //if (!this->validateFigurePosition(&tmp))
    //    return false;
    
    this->currentfigure->setY(this->currentfigure->getY() + 1);
    if (!this->validateFigurePosition(this->currentfigure))
    {
        this->currentfigure->setY(this->currentfigure->getY() - 1);
        return false;
    }
    this->updateCurrentFigure(STF2_UPDATE_DOWN);
    
    return true;
}

bool STetrisField2::rotateCurrentFigureLeft()
{
    if (this->currentfigure == nullptr)
        return false;
    
    STetrisFigure2 tmp = STetrisFigure2(*this->currentfigure);
    tmp.RotateLeft();
    if (!this->validateFigurePosition(&tmp))
        return false;
    
    this->currentfigure->RotateLeft();
    this->eraseCurrentFigure();
    this->drawCurrentFigure();
    
    return true;
}

bool STetrisField2::rotateCurrentFigureRight()
{
    if (this->currentfigure == nullptr)
        return false;
    
    //STetrisFigure2 tmp = STetrisFigure2(*this->currentfigure);
    //tmp.RotateRight();
    //if (!this->validateFigurePosition(&tmp))
    //    return false;
    
    this->currentfigure->RotateRight();
    if (!this->validateFigurePosition(this->currentfigure))
    {
        this->currentfigure->RotateLeft();
        return false;
    }
    this->eraseCurrentFigure();
    this->drawCurrentFigure();
    
    return true;
}

void STetrisField2::placeCurrentFigure()
{
    if (this->currentfigure == nullptr)
        return;
    
    this->addCurrentFigureToField();
    this->eraseCurrentFigure();
    
    delete this->currentfigure;
    this->currentfigure = nullptr;
    
    this->eraseFieldBlocks();
    this->drawFieldBlocks();
}

void STetrisField2::deleteRows(QList<int> *rows)
{
    for (auto row: *rows)
    {
        for (int i = 0; i < this->blocks->size(); ++i)
        {
            (*this->blocks)[i]->removeAt(row);
            (*this->blocks)[i]->push_front(STF2_ZERO_BLOCK);
        }
    }
}

void STetrisField2::completeRows()
{
    QList<int> cry;
    
    //for (int j = 0; j < (*this->blocks)[0]->size(); ++j)
    for (int j = (*this->blocks)[0]->size() - 2; j >= 0; --j)
    {
        bool irc = true;
        for (int i = 0; i < this->blocks->size() - 1; ++i)
            if ((*(*this->blocks)[i])[j] == STF2_ZERO_BLOCK) irc = false;
        if (irc) cry.push_back(j);
    }
    
    if (cry.size() > 0)
    {
        this->deleteRows(&cry);
        this->completedrowscount += cry.size();
        
        this->eraseFieldBlocks();
        this->drawFieldBlocks();
    }
}

int STetrisField2::getComplitedRowsCount()
{
    return this->completedrowscount;
}

STetrisFigure2* STetrisField2::getCurrentFigure()
{
    return this->currentfigure;
}

void STetrisField2::resetField()
{
    this->eraseCurrentFigure();
    
    if (this->currentfigure != nullptr)
        delete this->currentfigure;
    this->currentfigure = nullptr;
    
    this->eraseFieldBlocks();
    
    for (int i = this->blocks->size() - 1; i >= 0; --i)
    {
        for (int j = (*this->blocks)[i]->size() - 1; j >= 0; --j)
        {
            (*(*this->blocks)[i])[j] = STF2_ZERO_BLOCK;
        }
    }
}

STetrisField2::~STetrisField2()
{
    this->eraseCurrentFigure();
    
    if (this->currentfigure != nullptr)
        delete this->currentfigure;
    
    this->eraseFieldBlocks();
    
    //now we must to destroy array of field blocks
    for (int i = this->blocks->size() - 1; i >= 0; --i)
    {
        for (int j = (*this->blocks)[i]->size() - 1; j >= 0; --j)
        {
            (*this->blocks)[i]->pop_back();
        }
        delete (*this->blocks)[i];
        this->blocks->pop_back();
    }
    delete this->blocks;
    
    this->eraseField();
}

STetrisFigure2* STetrisFigure2Generator::makeFigure(int x, int y)
{
    std::mt19937::result_type seed = static_cast<unsigned int>(time(nullptr));   
    auto int_rnd = std::bind(std::uniform_int_distribution<int>(1, 7), std::mt19937(seed));
    
    STetrisFigure2 *result;
    switch (int_rnd())
    {
       case 1: result = new STetrisFigure2("1111", x, y); break;
       case 2: result = new STetrisFigure2("22 22", x, y); break;
       case 3: result = new STetrisFigure2("030 333", x, y); break;
       case 4: result = new STetrisFigure2("044 440", x, y); break;
       case 5: result = new STetrisFigure2("550 055", x, y); break;
       case 6: result = new STetrisFigure2("666 600", x, y); break;
       case 7: result = new STetrisFigure2("700 777", x, y); break;
       default: result = new STetrisFigure2("123 456 765", x, y); break;
    }
    
    return result;
}
