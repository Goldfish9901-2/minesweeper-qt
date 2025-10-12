#include <QWidget>
#include <QTimer>
#include <QRandomGenerator>
#include <QMessageBox>

#ifndef FIELD_H
#define FIELD_H

#include "grid.h"
#include "ui_field.h"
class MainWindow;

class Field : public QWidget
{
    Q_OBJECT

public:

    explicit Field(QWidget* parent = nullptr);
    Field(int rows, int cols, int mines,
          std::string mode = "custom", QWidget* parent = nullptr);
    ~Field() override;

    void generateMines(Grid* start);
    bool isStarted();
    bool contains(Grid* grid);

    void updateFlags();


private slots:
    void check();
    void updateTime();

private:
    friend class Grid;
    Ui_Field* ui;
    int rows, cols, mines,secs;
    bool started;
    QList<Grid*> grids;
    QRandomGenerator* random;
    QTimer* timer;
    QString mode;
};


#endif // FIELD_H
