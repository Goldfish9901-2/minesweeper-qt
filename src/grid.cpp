#include "static.h"
#include <QMessageBox>

//initializers
//1. default constructor
Grid::Grid(QWidget* parent)
    : Grid{0, 0, parent}
{
}

//2. parameterized constructor
Grid::~Grid()
{
    neighbors.clear();
}

//3. copy constructor
Grid& Grid::operator=(const Grid& grid)
{
    this->row = grid.row;
    this->col = grid.col;
    this->mine = grid.mine;
    this->opened = grid.opened;
    this->flagged = grid.flagged;
    return *this;
}

//4
bool Grid::addNeighbor(Grid* grid)
{
    if (grid == nullptr)
    {
        return false;
    }
    if (isNeighbor(grid))
    {
        return false;
    }
    neighbors.push_back(grid);
    return true;
}

//5
Grid::Grid(const int row, const int col, QWidget* parent)
    : QPushButton{parent}
      , row(row), col(col)
      , surroundingMines(0), mine(false), opened(false)
      , neighbors(std::vector<Grid*>())
      , flagged(false)
{
    // 根据父窗口大小动态设置格子大小
    if (parent)
    {
        int gridSize = qMin(parent->width(), parent->height()) / qMax(((Field*)parent)->cols, ((Field*)parent)->rows);
        gridSize = qBound(20, gridSize, 40); // 限制在20-40像素之间
        setFixedSize(gridSize, gridSize);
    }
    else
    {
        setFixedSize(32, 32);
    }
    setFont(QFont("Arial", 20, QFont::Bold));

    updateDisplay();
    show();
}

bool Grid::isNeighbor(Grid* grid) const
{
    return std::find(neighbors.begin(), neighbors.end(), grid) != neighbors.end();
}

bool Grid::setMine()
{
    if (mine)
    {
        return false;
    }
    mine = true;
    return true;
}

void Grid::countSurroundings()
{
    surroundingMines = 0;
    for (const auto& neighbor : neighbors)
    {
        if (neighbor->mine)
        {
            surroundingMines++;
        }
    }
}

int Grid::countFlag() const
{
    int count = 0;
    for (const auto& neighbor : neighbors)
    {
        if (neighbor->flagged)
        {
            count++;
        }
    }
    return count;
}

int Grid::getSurroundingMines() const
{
    return surroundingMines;
}

std::vector<Grid*> Grid::getNeighbors() const
{
    return neighbors;
}

// getters of private members showing the state of the grid
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
    Field* f = container(this);
    if (event->button() == Qt::RightButton)
    {
        this->flag();
        f->updateFlags();
    }
    else if (event->button() == Qt::LeftButton)
    {
        // 使用Field类统一调度打开格子的操作
        StartOpenResult result = f->openGrid(this);
        qDebug() << tr("open result") << static_cast<int>(result);

        // 根据返回结果显示相应的消息
        switch (result)
        {
        case StartOpenResult::GAME_ALREADY_ENDED:
            QMessageBox::information(f, "游戏已结束", "游戏已经结束");
            break;
        case StartOpenResult::FLAGGED_GRID:
            QMessageBox::information(f, "无法打开", "不能打开已标记的格子");
            break;
        case StartOpenResult::NOT_ENOUGH_FLAGS:
            {
                const QString message = QString("还需要 %1 个旗子").arg(surroundingMines - countFlag());
                QMessageBox::information(f, "旗子不足", message);
            }
            break;
        case StartOpenResult::TOO_MANY_FLAGS:
            {
                const QString message = QString("多了 %1 个旗子").arg(countFlag() - surroundingMines);
                QMessageBox::information(f, "旗子过多", message);
            }
            break;
        case StartOpenResult::SUCCESS:
            break;
        default:
            // 操作成功，无需显示消息
            break;
        }
    }
}

//2
Grid::GridOpenResult Grid::open()
{
    if (Field* field = container(this); field && !field->isStarted())
    {
        field->generateMines(this);
    }
    if (this->flagged)
    {
        return GridOpenResult::INVALID;
    }
    if (this->opened)
    {
        if (countFlag() == surroundingMines)
        {
            if (surroundingMines != 0)
                return GridOpenResult::SURROUNDING_UNFLAGGED_AVAILABLE;
            return GridOpenResult::SURROUNDING_AVAILABLE;
        }
        return GridOpenResult::INVALID;
    }

    this->opened = true;
    updateDisplay();
    if (surroundingMines != 0)
    {
        return GridOpenResult::SINGLE;
    }
    return GridOpenResult::SURROUNDING_AVAILABLE;
}

int Grid::getRow() const
{
    return row;
}

int Grid::getColumn() const
{
    return col;
}

//3
void Grid::flag()
{
    if (this->getOpened())
    {
        return;
    }
    this->flagged = !this->flagged;
    updateDisplay();
    show();
}

Grid::State Grid::updateState(const bool reveal)
{
    if (flagged)
    {
        return currentState = State::FLAGGED;
    }
    if (!getOpened() && !reveal)
    {
        return currentState = State::UNOPENED;
    }
    if (mine)
    {
        return currentState = State::TRIGGERED;
    }
    return currentState = State::OPENED;
}


// functions for foreground
void Grid::updateDisplay()
{
    updateDisplay(false);
}

void Grid::updateDisplay(const bool reveal)
{
    switch (updateState(reveal))
    {
    case State::OPENED:
        setIcon(surroundingMines == 0 ? mw->noAroundIcon : mw->surroundingMineIcons[surroundingMines - 1]);
        break;
    case State::FLAGGED:
        setIcon(mw->flagIcon);
        break;
    case State::TRIGGERED:
        setIcon(mw->mineIcon);
        break;
    case State::UNOPENED:
        setIcon(mw->blankIcon);
        break;
    }
}

void Grid::reveal()
{
    if (!mine)return;
    updateDisplay(true);
}

