#include "static.h"
#include <QRandomGenerator>
#include <QString>
#include <QMessageBox>



Field::Field(QWidget* parent)
    : Field{10, 10, 10, "easy", parent}
{}


Field::Field(int rows, int cols, int mines, std::string mode, QWidget* parent)
    ://basic setup
     QWidget{parent}, ui(new Ui_Field),
     //initializing variables
     rows(rows), cols(cols), mines(mines),
    grids(QList<Grid*>()),
     started(false), mode(QString::fromStdString(mode)),
     random(new QRandomGenerator()),
     timer(new QTimer(this)), secs(0)
{
    ui->setupUi(this);

    mw->fields.append(this);
    ui->lMode->setText(this->mode);
    setWindowTitle(this->mode);
    this->ui->gridLayout->setRowMinimumHeight(rows, rows);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            grids.append(new Grid(i, j, this));
        }
    }

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            Grid* temp = grids.at(r * cols + c);
            connect(temp, &Grid::check, this, &Field::check);
            for(int ri = r - 1; ri <= r + 1; ri++)
            {
                for(int ci = c - 1; ci <= c + 1; ci++)
                {
                    if(ri < 0 || ri >= rows)
                        continue;
                    if(ci < 0 || ci >= cols)
                        continue;
                    if(ri == r && ci == c)
                        continue;
                    Grid *neighbor = this->grids.at(ri * cols + ci);
                    temp->addNeighbor(neighbor);

                }
            }

        }
    }
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
        {
            this->ui->gridLayout->addWidget(
                grids.at(r * cols + c), r, c
            );
        }
}

Field::~Field()
{
    delete random;
    for(auto& grid : grids)
    {
        delete grid;
    }
    grids.clear();
    delete ui;
    mw->fields.removeOne(this);
}

void Field::generateMines(Grid* start)
{
    int minesGenerated = 0;
    random->seed(time(NULL));
    while(minesGenerated < mines)
    {
        int r = random->bounded(rows);
        int c = random->bounded(cols);
        Grid* temp = grids.at(r * cols + c);
        if(temp == start)
        {
            continue;
        }
        if(temp->isNeighbor(start))
        {
            continue;
        }
        if(temp->setMine())
        {
            minesGenerated++;
        }
    }
    for(int r = 0; r < this->rows; r++)
        for(int c = 0; c < this->cols; c++)
        {
            Grid* temp = grids.at(r * cols + c);
            temp->countSurroundings();
            temp->update();
        }
    connect(timer, &QTimer::timeout, this, &Field::updateTime);
    timer->start(1000);
}
void Field::check()
{
    bool allOpened = true;
    for(Grid*& grid : this->grids)
    {
        if(grid->getMine())
        {
            if(grid->getOpened())
            {
                timer->stop();
                delete timer;
                for(Grid*& t : this->grids)
                {
                    t->reveal();
                }
                QMessageBox::information(this, "Game Over", "You Lost");
                delete this;
                return;
            }
        }
        else if(!grid->getOpened())
        {
            allOpened = false;
        }
    }
    if(allOpened)
    {
        timer->stop();
        delete timer;
        for(Grid*& t : this->grids)
        {
            t->reveal(true);
        }
        QMessageBox::information(this, "Game Over", "You Won");
        delete this;
    }


}
bool Field::contains(Grid* grid)
{
    return grids.contains(grid);
}
//static Field* Field::container(Grid* grid)

bool Field::isStarted()
{
    if(!started)
    {
        started = true;
        return false;
    }
    return true;
}
void Field::updateFlags()
{
    int flags = 0;
    for(Grid* grid : grids)
    {
        if(grid->getFlagged())
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


