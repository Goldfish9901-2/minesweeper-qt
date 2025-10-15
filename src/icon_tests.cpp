//
// Created by jerry on 2025/10/15.
//
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QPainter>
#include <QTranslator>
#include <QApplication>

#include "grid.h"


void MainWindow::testIcons()
{
    int r = 0, c = 0;
    for (int i = 0; i < 8; ++i)
    {
        auto* btn = new QPushButton(this);
        btn->setFixedSize(40, 40);
        renderIcon(Grid::State::OPENED, i + 1, btn);
        ui->iconDemoLayout->addWidget(btn, r, c++);
    }
    r = 1, c = 0;
    auto* btn1 = new QPushButton(this);
    btn1->setFixedSize(40, 40);
    renderIcon(Grid::State::TRIGGERED, 0, btn1);
    ui->iconDemoLayout->addWidget(btn1, r, c++);

    auto* btn2 = new QPushButton(this);
    btn2->setFixedSize(40, 40);
    renderIcon(Grid::State::FLAGGED, 0, btn2);
    ui->iconDemoLayout->addWidget(btn2, r, c++);

    auto* btn3 = new QPushButton(this);
    btn3->setFixedSize(40, 40);
    renderIcon(Grid::State::UNOPENED, 0, btn3);
    ui->iconDemoLayout->addWidget(btn3, r, c++);

    auto* btn4 = new QPushButton(this);
    btn4->setFixedSize(40, 40);
    renderIcon(Grid::State::OPENED, 0, btn4);
    ui->iconDemoLayout->addWidget(btn4, r, c++);
}

void MainWindow::loadTestIcon(const QString& iconName, int r, int c)
{
    // This method is no longer used with the new approach, but kept for compatibility
    qDebug() << QString("loading to location [ %1, %2 ]").arg(r).arg(c);
    auto* btn = new QPushButton(this);
    btn->setFixedSize(40, 40);
    // We'll use the old method here for backward compatibility
    renderIcon(Grid::State::UNOPENED, 0, btn);
    ui->iconDemoLayout->addWidget(btn, r, c);
}
