


#ifndef FIELD_H
#define FIELD_H
#include <QWidget>
#include <QTimer>
#include <set>

#include "grid.h"
#include "ui_field.h"
class MainWindow;

enum class StartOpenResult
{
    NULLPTR,
    SUCCESS,
    GAME_ALREADY_ENDED,
    FLAGGED_GRID,
    NOT_ENOUGH_FLAGS,
    TOO_MANY_FLAGS,
    MINE_TRIGGERED
};

class Field final : public QWidget
{
public:
    enum class GameMode
    {
        EASY,
        MEDIUM,
        HARD,
        CUSTOM
    };

    Q_OBJECT

public:
    explicit Field(QWidget* parent = nullptr) : Field(10, 10, 10)
    {
    }

    Field(unsigned short rows,
          unsigned short cols,
          unsigned short mines,
          GameMode mode = GameMode::CUSTOM, QWidget* parent = nullptr);
    Field(unsigned short rows,
          unsigned short cols,
          unsigned short mines,
          GameMode mode,
          Field* parentField);
    ~Field() override;

    void generateMines(Grid* start);
    bool lose(const Grid* grid);
    [[nodiscard]] bool isStarted() const;
    bool contains(Grid* grid) const;

    void updateFlags() const;
    StartOpenResult openGrid(Grid* start);

    // Public interface for rendering icons through MainWindow

private slots:
    void check(Grid* grid);
    void updateTime();

private:
    // friend class Grid;
    Ui_Field* ui;
    unsigned short rows, cols, mines;

public:
    [[nodiscard]] unsigned short getRows() const
    {
        return rows;
    }

    [[nodiscard]] unsigned short getCols() const
    {
        return cols;
    }

private:
    int secs;
    bool started;
    std::vector<Grid*> grids;
    std::set<Grid*> finishedGrids;
    QTimer* timer;
    GameMode mode;

    void onOpenRequest(Grid* grid);
    void registerOpened(Grid* grid);
    void displayField() const;

    [[nodiscard]] int flatLoc(int r, int c) const;
};


#endif // FIELD_H
