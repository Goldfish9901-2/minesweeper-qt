//
// Created by user on 2025/10/16.
//

#ifndef MINESWEEPER_RECORDVIEW_H
#define MINESWEEPER_RECORDVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include "database.h"
#include "ui_record.h"

class RecordView : public QWidget
{
Q_OBJECT

public:
    explicit RecordView(QWidget* parent = nullptr);
    ~RecordView() override;

    void refresh();

private slots:
    void onModeChanged(int index);
    void onRefreshClicked();
    void onBackClicked();

private:
    Ui::RecordView* ui;
    DatabaseManager dbManager;
    QList<Record> records;

    void loadRecords();
    void displayRecords();
    QString modeToString(Field::GameMode mode) const;
};


#endif //MINESWEEPER_RECORDVIEW_H