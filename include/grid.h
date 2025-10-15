#ifndef GRID_H
#define GRID_H
#include <QPushButton>
#include <QRandomGenerator>
#include <QMenu>
#include <QMouseEvent>
#include <QSvgRenderer>
class Field;
class Grid final : public QPushButton
{
    Q_OBJECT

public:
    enum class GridOpenResult
    {
        /*
         * opened. no further action needed
         */
        SINGLE,
        /*
         * open surrounding grids
         */
        SURROUNDING_AVAILABLE,
        /*
         * open unflagged surrounding grids
         */
        SURROUNDING_UNFLAGGED_AVAILABLE,
        /*
         * nothing changed, no further action needed
         */
        INVALID,
        /*
         * mine triggered
         */
        GAME_ENDED,
        SIZE
    };

    enum class State
    {
        TRIGGERED,
        FLAGGED,
        UNOPENED,
        OPENED
    };

    explicit Grid(Field* parent = nullptr);
    Grid(int row, int col, Field* parent = nullptr);
    ~Grid() override;
    Grid& operator=(const Grid& grid);
    bool addNeighbor(Grid* grid);
    [[nodiscard]] bool setMine();
    void updateDisplay();
    void updateDisplay(bool reveal);
    bool isNeighbor(const Grid* grid) const;
    void countSurroundings();
    [[nodiscard]] int countFlag() const;
    [[nodiscard]] int getSurroundingMines() const;
    [[nodiscard]] GridOpenResult open(); // 将open方法移到public部分
    [[nodiscard]] int getRow() const;
    [[nodiscard]] int getColumn() const;
    [[nodiscard]] bool getMine() const;
    [[nodiscard]] bool getOpened() const;
    [[nodiscard]] bool getFlagged() const;
    [[nodiscard]] std::vector<Grid*> getNeighbors() const;
    bool isMine() const;
    bool isOpened() const;
    bool isFlagged() const;
    void reveal();
    State updateState(bool reveal);

private:
    Field* parent;
    unsigned short row, col, surroundingMines;
    bool mine;
    bool opened;
    bool flagged;
    std::vector<Grid*> neighbors;
    
    // SVG renderer for dynamic icon rendering
    void renderIcon();
    void renderIcon(const QSize& size);

signals:
    void check(Grid* grid);

private:
    void flag();

    State currentState;

private slots:
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};

#endif // GRID_H