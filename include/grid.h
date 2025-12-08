#ifndef GRID_H
#define GRID_H
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSvgRenderer>
#ifdef ANDROID
#include <android/log.h>
#endif
#include "haptic.h"
#include <qevent.h>
class Field;
class Grid final : public QPushButton, public Haptic {
    Q_OBJECT

public:
    enum class GridOpenResult {
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

    enum class State { TRIGGERED, FLAGGED, UNOPENED, OPENED };

    explicit Grid(Field *parent = nullptr);
    Grid(int row, int col, Field *parent = nullptr);
    ~Grid() override;
    Grid &operator=(const Grid &grid);
    bool addNeighbor(Grid *grid);
    [[nodiscard]] bool setMine();
    void updateDisplay();
    void updateDisplay(bool reveal);
    bool isNeighbor(const Grid *grid) const;
    void countSurroundings();
    [[nodiscard]] int countFlag() const;
    [[nodiscard]] int getSurroundingMines() const;
    [[nodiscard]] GridOpenResult open(); // 将open方法移到public部分
    [[nodiscard]] int getRow() const;
    [[nodiscard]] int getColumn() const;
    [[nodiscard]] bool getMine() const;
    [[nodiscard]] bool getOpened() const;
    [[nodiscard]] bool getFlagged() const;
    [[nodiscard]] std::vector<Grid *> getNeighbors() const;
    bool isMine() const;
    bool isOpened() const;
    bool isFlagged() const;
    void reveal();
    State updateState(bool reveal);

private:
    Field *parent;
    QElapsedTimer pressTime; // 记录按下时间

    unsigned short row, col, surroundingMines;
    bool mine;
    bool opened;
    bool flagged;
    std::vector<Grid *> neighbors;

    // SVG renderer for dynamic icon rendering
    void renderIcon();
    void renderIcon(const QSize &size);

signals:
    void check(Grid *grid);

private:
    void flag();

    State currentState;

private:
    bool _pressing = false;
    QTimer *_longPressTimer;

private slots:
    void mousePressEvent(QMouseEvent *event) override;
#ifdef ANDROID
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif
    void resizeEvent(QResizeEvent *event) override;

protected:
    // bool event(QEvent *event) override;
    void onTap(QMouseEvent *event);
    void onLongPress();
};

#ifdef ANDROID
#ifndef LOG_TAG
#define LOG_TAG "MineSweeperQT" // Define a tag for your logs
#endif

#define LOGI(...)                                                              \
__android_log_print(ANDROID_LOG_INFO, LOG_TAG,                               \
                    __VA_ARGS__) // Info-level macro
#define LOGV(...)                                                              \
__android_log_print(ANDROID_LOG_INFO, LOG_TAG,                               \
                    __VA_ARGS__) // Info-level macro
#endif

#endif // GRID_H
