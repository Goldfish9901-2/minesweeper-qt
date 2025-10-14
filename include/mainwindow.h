#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grid.h"
#include "field.h"

#include <QMainWindow>
#include <QSvgRenderer>
#include <functional>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    std::vector<Field*> fields;
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    const std::string resourcePrefix, mineNumberPrefix;

    // Public interface for rendering icons
    void renderIcon(Grid::State state, int surroundingMines, QPushButton* button) const;
    void renderIcon(Grid::State state, int surroundingMines, QPushButton* button, const QSize& size) const;
    // void renderIcon(const QString& iconName, QPushButton* button) const;
    // void renderIcon(const QString& iconName, QPushButton* button, const QSize& size) const;

    [[nodiscard]] int getOffset() const;
    static void easy();
    static void medium();
    static void hard();
    static void custom();
    // 将Difficulty枚举转换为std::string
    static QString difficultyToStringStandard(Field::GameMode difficulty);

private:
    static QIcon loadSvg(const QString& path);
    Ui::MainWindow* ui;
    int offset;

    // SVG renderers stored as private const members
    const std::unique_ptr<QSvgRenderer> blankIconRenderer;
    const std::unique_ptr<QSvgRenderer> noAroundIconRenderer;
    const std::unique_ptr<QSvgRenderer> flagIconRenderer;
    const std::unique_ptr<QSvgRenderer> mineIconRenderer;
    const std::unique_ptr<QSvgRenderer> mineTriggeredIconRenderer;
    const std::vector<std::unique_ptr<QSvgRenderer>> surroundingMineIconRenderers; // 1-8数字图标渲染器

    void testIcons();
    void loadTestIcon(const QString& iconName, int r, int c);

    // 根据难度创建游戏
    static void createGame(Field::GameMode difficulty);

    // Individual renderer methods - 修改为直接设置图标到按钮
    void setBlankIcon(QPushButton* button) const;
    void setNoAroundIcon(QPushButton* button) const;
    void setFlagIcon(QPushButton* button) const;
    void setMineIcon(QPushButton* button) const;
    void setMineTriggeredIcon(QPushButton* button) const;
    void setSurroundingMineIcon(int number, QPushButton* button) const;
};

#endif // MAINWINDOW_H
