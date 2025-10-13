#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grid.h"
#include "field.h"

#include <QMainWindow>

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
    const QIcon blankIcon, noAroundIcon;
    const QIcon flagIcon, mineIcon, mineTriggeredIcon, appIcon;
    const std::vector<QIcon> surroundingMineIcons;
    const std::vector<std::string> numberColors;
    /**
     * map icons into integer
     * - note that opened mines with surrounding count
     *     - 0-8
     * - are all included
     */
    std::map<int, QIcon> iconMap;
    int getOffset() const;
    static void easy();
    static void medium();
    static void hard();
    static void custom();

private:
    static QIcon loadSvg(const QString& path);
    Ui::MainWindow* ui;
    int offset;
    void testIcons();
    void loadIcon(QIcon icon, int r, int c);
};


#endif // MAINWINDOW_H
