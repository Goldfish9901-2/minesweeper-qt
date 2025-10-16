//
// Created by user on 2025/10/16.
//

#include "recordview.h"
#include "mainwindow.h"
#include <QTableWidgetItem>
#include <QDateTime>

RecordView::RecordView(QWidget* parent) : QWidget(parent), ui(new Ui::RecordView)
{
    ui->setupUi(this);

    // 设置窗口属性，使其成为独立窗口
    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose, false); // 不在关闭时删除对象

    // 初始化数据库
    if (!dbManager.init())
    {
        qDebug() << "Failed to initialize database in RecordView";
    }

    // 连接信号和槽
    connect(ui->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RecordView::onModeChanged);
    connect(ui->refreshButton, &QPushButton::clicked, this, &RecordView::onRefreshClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &RecordView::onBackClicked);

    refresh();
}

RecordView::~RecordView()
{
    delete ui;
}

void RecordView::refresh()
{
    loadRecords();
    displayRecords();
    retranslate();
}

void RecordView::onModeChanged(int index)
{
    retranslate();
}

void RecordView::onRefreshClicked()
{
    refresh();
}

void RecordView::onBackClicked()
{
    auto* mainWindow = qobject_cast<MainWindow*>(parent());
    if (mainWindow)
    {
        hide();
        mainWindow->returnToMainMenu();
    }
}

void RecordView::loadRecords()
{
    int modeIndex = ui->modeComboBox->currentIndex();

    switch (modeIndex)
    {
    case 0: // All
        records = dbManager.getAllRecords();
        break;
    case 1: // Easy
        records = dbManager.getRecords(Field::GameMode::EASY);
        break;
    case 2: // Medium
        records = dbManager.getRecords(Field::GameMode::MEDIUM);
        break;
    case 3: // Hard
        records = dbManager.getRecords(Field::GameMode::HARD);
        break;
    case 4: // Custom
        records = dbManager.getRecords(Field::GameMode::CUSTOM);
        break;
    default:
        records = dbManager.getAllRecords();
        break;
    }
}

void RecordView::displayRecords()
{
    retranslate();
    ui->recordTable->setRowCount(0); // 清空现有记录

    for (const auto& record : records)
    {
        int row = ui->recordTable->rowCount();
        ui->recordTable->insertRow(row);

        // Mode
        auto* modeItem = new QTableWidgetItem(modeToString(record.mode));
        modeItem->setTextAlignment(Qt::AlignCenter);
        ui->recordTable->setItem(row, 0, modeItem);

        // Width
        auto* widthItem = new QTableWidgetItem(QString::number(record.width));
        widthItem->setTextAlignment(Qt::AlignCenter);
        ui->recordTable->setItem(row, 1, widthItem);

        // Height
        auto* heightItem = new QTableWidgetItem(QString::number(record.height));
        heightItem->setTextAlignment(Qt::AlignCenter);
        ui->recordTable->setItem(row, 2, heightItem);

        // Mines
        auto* minesItem = new QTableWidgetItem(QString::number(record.mines));
        minesItem->setTextAlignment(Qt::AlignCenter);
        ui->recordTable->setItem(row, 3, minesItem);

        // Time
        auto* timeItem = new QTableWidgetItem(QString::number(record.secs));
        timeItem->setTextAlignment(Qt::AlignCenter);
        ui->recordTable->setItem(row, 4, timeItem);

        // Placeholder for date (as our current database schema doesn't store date)
        auto* dateItem = new QTableWidgetItem(tr("N/A"));
        dateItem->setTextAlignment(Qt::AlignCenter);
        ui->recordTable->setItem(row, 5, dateItem);
    }
}

QString RecordView::modeToString(Field::GameMode mode) const
{

    switch (mode)
    {
    case Field::GameMode::EASY:
        return tr("Easy");
    case Field::GameMode::MEDIUM:
        return tr("Medium");
    case Field::GameMode::HARD:
        return tr("Hard");
    case Field::GameMode::CUSTOM:
        return tr("Custom");
    default:
        return tr("Unknown");
    }
}
