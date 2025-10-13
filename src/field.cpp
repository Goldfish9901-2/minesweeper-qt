#include "mainwindow.h"
#include "static.h"
#include <QRandomGenerator>
#include <QString>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <random>
#include <chrono>
#include <iostream>
#include <queue>


Field::Field(QWidget* parent)
    : Field{10, 10, 10, "easy", parent}
{
}


Field::Field(
    const unsigned short rows,
    const unsigned short cols,
    const unsigned short mines,
    const std::string& mode,
    QWidget* parent)
    : //basic setup
    QWidget{parent}, ui(new Ui_Field),
    //initializing variables
    rows(rows), cols(cols), mines(mines),
    secs(0),
    started(false),
    grids(std::vector<Grid*>()), finishedGrids(std::set<Grid*>()),
    timer(new QTimer(this)), mode(QString::fromStdString(mode))
{
    ui->setupUi(this);

    // 设置窗口自适应屏幕大小
    const QScreen* screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int screenWidth = screenGeometry.width();
    const int screenHeight = screenGeometry.height();
    this->resize(
        static_cast<int>(screenWidth * 0.8),
        static_cast<int>(screenHeight * 0.8));
    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);

    mw->fields.push_back(this);
    ui->lMode->setText(this->mode);
    setWindowTitle(this->mode);
    this->ui->gridLayout->setRowMinimumHeight(rows, rows);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grids.push_back(new Grid(i, j, this));
        }
    }

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            Grid* temp = grids.at(r * cols + c);
            connect(temp, &Grid::check, this, &Field::check);
            for (int ri = r - 1; ri <= r + 1; ri++)
            {
                for (int ci = c - 1; ci <= c + 1; ci++)
                {
                    if (ri < 0 || ri >= rows)
                        continue;
                    if (ci < 0 || ci >= cols)
                        continue;
                    if (ri == r && ci == c)
                        continue;
                    Grid* neighbor = this->grids.at(flatLoc(ri, ci));
                    temp->addNeighbor(neighbor);
                }
            }
        }
    }
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
        {
            this->ui->gridLayout->addWidget(
                grids.at(r * cols + c), r, c
            );
        }
}

Field::~Field()
{
    // delete random;
    for (const auto& grid : grids)
    {
        delete grid;
    }
    grids.clear();
    delete ui;
    const auto it = std::find(mw->fields.begin(), mw->fields.end(), this);
    mw->fields.erase(it);
}

void Field::generateMines(Grid* start)
{
    if (started)
        return;
    const auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed); // Mersenne Twister engine

    std::uniform_int_distribution<>
        rDist(0, rows - 1),
        wDist(0, cols - 1);

    int minesGenerated = 0;
    while (minesGenerated < mines)
    {
        const int r = rDist(rng);
        const int c = wDist(rng);
        const int pos = this->flatLoc(r, c);
        Grid* temp = grids.at(pos);
        if (temp == start)
        {
            continue;
        }
        if (temp->isNeighbor(start))
        {
            continue;
        }
        if (temp->setMine())
        {
            minesGenerated++;
        }
    }
    for (const auto& grid : grids)
    {
        grid->countSurroundings();
        grid->updateDisplay();
    }
    connect(timer, &QTimer::timeout, this, &Field::updateTime);
    timer->start(1000);
    started = true;
}

bool Field::lose(const Grid* grid)
{
    if (!started)
        return false;
    // 如果是雷，游戏结束
    if (!grid->getMine())
        return false;

    timer->stop();
    //delete timer;
    for (const auto& t : this->grids)
    {
        t->reveal();
    }
    if (started)
        QMessageBox::information(this, "Game Over", "You Lost");
    // delete this;
    return true;
}

void Field::check(Grid* grid)
{
    if (lose(grid)) return;

    // 添加到已完成的格子集合中
    finishedGrids.insert(grid);
    qDebug() << QString(" %1 mines finished").arg(finishedGrids.size());

    // 检查是否所有非雷格子都已经打开
    if (finishedGrids.size() == rows * cols - mines)
    {
        /*
         * nothing more to do
         * hooray! win！
         */
        timer->stop();
        //delete timer;
        for (const auto& t : this->grids)
        {
            t->reveal();
        }
        QMessageBox::information(this, "Game Over", "You Won");
        // delete this;
    }
}


// 新增方法：统一处理邻居格子的打开
void Field::onOpenRequest(Grid* grid)
{
    // 使用队列进行广度优先遍历，避免递归
    std::queue<Grid*> toVisit;
    std::set<Grid*> visited;

    toVisit.push(grid);
    // visited.insert(grid);

    while (!toVisit.empty())
    {
        Grid* current = toVisit.front();
        toVisit.pop();
        switch (const auto openResult = current->open())
        {
        case Grid::GridOpenResult::SURROUNDING_AVAILABLE:
        case Grid::GridOpenResult::SURROUNDING_UNFLAGGED_AVAILABLE:
            for (const auto& neighbor : current->getNeighbors())
            {
                if (visited.find(neighbor) != visited.end())
                    continue;
                visited.insert(neighbor);
                if (openResult == Grid::GridOpenResult::SURROUNDING_UNFLAGGED_AVAILABLE
                    && (neighbor->getFlagged() || neighbor->getOpened()))
                    continue;
                // if (neighbor->getSurroundingMines())
                //     continue;
                toVisit.push(neighbor);
                displayField();
            }
        case Grid::GridOpenResult::SINGLE:
            registerOpened(current);
            break;
        case Grid::GridOpenResult::GAME_ENDED:
            check(grid);
            break;

        default: break;
        }
        current->updateDisplay();
        displayField();
    }
}

int Field::flatLoc(const int r, const int c) const
{
    const int res = r * cols + c;
    if (res >= cols * rows || r < 0 || c < 0)
    {
        std::cerr << "[" << r << ',' << c << "] out of bounds";
        exit(1);
    }
    if (const Grid* grid = grids.at(res);
        grid->getRow() != r || grid->getColumn() != c)
    {
        std::cerr << "[" << r << ',' << c << "] out of bounds";
        exit(1);
    }
    return res;
}

bool Field::contains(Grid* grid) const
{
    const auto it = std::find(grids.begin(), grids.end(), grid);
    return it != grids.end();
}

//static Field* Field::container(Grid* grid)

bool Field::isStarted() const
{
    return started;
}

void Field::updateFlags() const
{
    int flags = 0;
    for (const Grid* grid : grids)
    {
        if (grid->getFlagged())
        {
            flags++;
        }
    }
    ui->lFlag->setText(QString::number(flags));
}

void Field::updateTime()
{
    secs++;
    ui->lTime->setText(QString::number(secs));
}

StartOpenResult Field::openGrid(Grid* start)
{
    // 检查游戏是否已开始

    if (!start)
        return StartOpenResult::NULLPTR;
    // 如果游戏已经结束，则不执行任何操作
    if (!timer->isActive() && started)
    {
        return StartOpenResult::GAME_ALREADY_ENDED;
    }

    // 如果点击的是已标记的格子，则不允许打开
    if (start->getFlagged())
    {
        return StartOpenResult::FLAGGED_GRID;
    }

    if (!started)
    {
        // 第一次点击时生成地雷
        generateMines(start);
        // 单击打开格子
        onOpenRequest(start);
        emit start->check(start);
        started = true;
        return StartOpenResult::SUCCESS;
    }
    // 如果是已经开始的状态，检查周围旗子数量是否匹配

    const int flagCount = start->countFlag();
    const int surroundingMines = start->getSurroundingMines();

    if (start->getOpened())
    {
        if (flagCount < surroundingMines)
        {
            return StartOpenResult::NOT_ENOUGH_FLAGS;
        }

        if (flagCount > surroundingMines)
        {
            return StartOpenResult::TOO_MANY_FLAGS;
        }
    }

    onOpenRequest(start);
    emit start->check(start);
    return StartOpenResult::SUCCESS;
}

void Field::registerOpened(Grid* grid)
{
    finishedGrids.insert(grid);
    qDebug() <<
        QString(" %1 grids finished")
        .arg(finishedGrids.size());
}

void Field::displayField() const
{
    std::string row = "";
    char buf[20];
    for (const auto& grid : grids)
    {
        if (grid->getColumn() == 0)
        {
            qDebug() << row;
            row = "";
        }
        snprintf(
            buf, sizeof(buf) - 1,
            "%3d [%3d] ",
            static_cast<int>(grid->updateState(false)),
            grid->getMine() ? -1 : grid->getSurroundingMines()
        );
        row += buf;
    }
}
