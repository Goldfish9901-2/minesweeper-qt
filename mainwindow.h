#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grid.h"
#include "field.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<Field*> fields;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void easy();
    void medium();
    void hard();
    void custom();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
