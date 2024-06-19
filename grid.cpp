#include "static.h"

//initializers
//1. default constructor
Grid::Grid(QWidget* parent)
    : Grid{0, 0, parent}
{}
//2. parameterized constructor
Grid::~Grid()
{
    neighbors.clear();
}
//3. copy constructor
void Grid::operator=(const Grid& grid)
{
    this->row = grid.row;
    this->col = grid.col;
    this->mine = grid.mine;
    this->opened = grid.opened;
    this->flagged = grid.flagged;
}
//4
bool Grid::addNeighbor(Grid* grid)
{
    if(grid == nullptr)
    {
        return false;
    }
    if(neighbors.contains(grid))
    {
        return false;
    }
    neighbors.append(grid);
    return true;
}
//5
Grid::Grid(int row, int col, QWidget* parent)
    : QPushButton{parent}
    ,row(row),col(col)
          ,mine(false),opened(false),flagged(false),
          surroundingMines(0)
{
    setFixedSize(0x20,0x20);
    setFont(QFont("Arial", 20));

    update();
    show();
}

bool Grid::isNeighbor(Grid* grid)
{
    return neighbors.contains(grid);
}

bool Grid::setMine()
{
    if(mine)
    {
        return false;
    }
    mine = true;
    return true;
}
void Grid::countSurroundings()
{
    surroundingMines = 0;
    for(auto neighbor : neighbors)
    {
        if(neighbor->mine)
        {
            surroundingMines++;
        }
    }
}
int Grid::countFlag()
{
    int count = 0;
    for(auto neighbor : neighbors)
    {
        if(neighbor->flagged)
        {
            count++;
        }
    }
    return count;
}

//getters of private members showing the state of the grid
bool Grid::getMine() const
{
    return mine;
}

bool Grid::getOpened() const
{
    return opened;
}

bool Grid::getFlagged() const
{
    return flagged;
}
//event listeners
//1
void Grid::mousePressEvent(QMouseEvent* event)
{
    Field* f=container(this);
    if(event->button() == Qt::RightButton)
    {
        this->flag();
        f->updateFlags();
    }
    else if(event->button() == Qt::LeftButton)
    {
        this->open();
        emit check();
    }

}
//2
void Grid::open()
{
    Field* field = container(this);
    if(!field->isStarted())
    {
        field->generateMines(this);
    }
    if(this->opened)
    {
        if(countFlag() == surroundingMines)
        {
            for(auto neighbor : neighbors)
            {
                if(!neighbor->opened)
                {
                    neighbor->open();
                }
            }
        }
    }
    else
    {
        if(this->flagged)
        {
            return;
        }
        this->opened = true;
        update();
        if(surroundingMines != 0)
        {
            return;
        }
        for(auto neighbor : neighbors)
        {
            neighbor->open();
        }
    }
}
//3
void Grid::flag()
{
    if(this->opened)
    {
        return;
    }
    this->flagged = !this->flagged;
    update();
    show();
}
// functions for foreground
void Grid::update(){
    update(false);
}

void Grid::update(bool reveal)
{
    std::string text = para();
    if(text=="F")
    {   
        changeTheme(flagIcon);
    }

    else if(text=="..."){
        changeTheme(text);
    }
    else if(text=="*")
    {
        changeTheme(reveal?(mineIcon):(mineTriggeredIcon));
    }
    else if(text==" ")
    {
        changeTheme(nullptr);
    }
    else
    {
        std::string sheet="color:"+numberColors.at(this->surroundingMines-1)+";";
        setStyleSheet(QString::fromStdString(sheet));
        changeTheme(text);
    }
    // setStyleSheet(QString::fromStdString("background-color: "+opened? "white" : "gray"));
}
void Grid::changeTheme(QIcon *icon){
    if(icon == nullptr)
    {
        setIcon(QIcon());
    }
    else
        setIcon(*icon);
    setText("");

}

void Grid::changeTheme(std::string theme)
{
    setIcon(QIcon());
    setText(QString::fromStdString(theme));
}
std::string Grid::para()
{
    if(opened)
    {
        if(mine)
        {
            return "*";
        }
        if(surroundingMines == 0)
        {
            return " ";
        }
        return std::to_string(surroundingMines);
    }
    else
    {
        if(flagged)
        {
            return "F";
        }
        return "...";
    }
}
void Grid::reveal()
{
    reveal(false);
}
void Grid::reveal(bool success){
    if(success)
    {
        if(mine)
        {
            changeTheme(mineIcon);
        }
    }
    else
    {
        if(mine)
        {
            changeTheme(mineTriggeredIcon);
        }
    }

}

