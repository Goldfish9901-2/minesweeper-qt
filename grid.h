
#include <QPushButton>

#include <QList>
#include <QPushButton>
#include <QRandomGenerator>
#include <QMenu>
#include <QMouseEvent>
#include <QIcon>

#ifndef GRID_H
#define GRID_H

class Grid : public QPushButton
{
    Q_OBJECT
public:
    explicit Grid(QWidget* parent = nullptr);
    Grid(int row, int col, QWidget* parent = nullptr);
    ~Grid();
    void operator=(const Grid& grid);
    bool addNeighbor(Grid* grid);
    bool setMine();
    std::string para();
    void update();
    void update(bool reveal);
    bool isNeighbor(Grid* grid);
    void countSurroundings();
    int countFlag();

    bool getMine() const;
    bool getOpened() const;
    bool getFlagged() const;
    void reveal();
    void reveal(bool success);

private:
    int row;
    int col;
    int surroundingMines;
    bool mine;
    bool opened;
    bool flagged;
    QList<Grid*> neighbors;
signals:
    void check();
private:
    void open();
    void flag();
    void changeTheme(QIcon* icon);
    void changeTheme(std::string theme);

private slots:
    void mousePressEvent(QMouseEvent* event)override;
};

#endif // GRID_H
