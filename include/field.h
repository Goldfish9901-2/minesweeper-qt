#pragma once
#include <QTimer>
#include <set>

#include "grid.h"
#include "Localeable.h"
#include "ui_field.h"
class MainWindow;
class Field;
#ifndef MINESWEEPER_FIELD_H
#define MINESWEEPER_FIELD_H

class Field final : public QWidget, public Localeable
{
public:
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

    enum class GameMode
    {
        EASY,
        MEDIUM,
        HARD,
        CUSTOM
    };

    Q_OBJECT

public:
    explicit Field(MainWindow* parent = nullptr)
        : Field(10, 10, 10, GameMode::EASY, parent)
    {
    }

    Field(unsigned short rows,
          unsigned short cols,
          unsigned short mines,
          GameMode mode = GameMode::CUSTOM, MainWindow* parent = nullptr);
    ~Field() override;

    void generateMines(const Grid* start);
    bool lose(const Grid* grid);
    void win();
    [[nodiscard]] bool isStarted() const;
    bool contains(const Grid* grid) const;
    void retranslate() override { ui->retranslateUi(this); }
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

    bool isGaming() const
    {
        return isStarted() && timer != nullptr && timer->isActive();
    }

    void retranslateUi();

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
    MainWindow* mainWindow;

public:
    [[nodiscard]] MainWindow* getParent() const
    {
        return mainWindow;
    }

private:
    [[nodiscard]] int flatLoc(int r, int c) const;

    void traverseRow_Cols(const std::function<void(int, int)>& receiver) const;
};


#endif // MINESWEEPER_FIELD_H
