//
// Created by jerry on 2025/10/15.
//
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QPainter>
#include <QTranslator>
#include <QApplication>

#include <QMenu>

#include "grid.h"

using Difficulty = Field::GameMode;

QString joinRendererPath(const std::string& resourcePrefix, const std::string& fileName)
{
    QString path = QString::fromStdString(resourcePrefix + fileName);
    if (!QFile::exists(path))
    {
        qFatal() << "Icon not found: " << path;
    }
    return path;
    // return QFile::exists(path) ? path : QString::fromStdString(resourcePrefix + "maybe.svg");
}

// Lambda ��ʼ������
std::unique_ptr<QSvgRenderer> loadRenderer(const std::string& resourcePrefix, const std::string& fileName)
{
    return std::make_unique<QSvgRenderer>(joinRendererPath(resourcePrefix, fileName));
}

std::vector<std::unique_ptr<QSvgRenderer>> loadNumberRenderers(const std::string& mineNumberPrefix)
{
    std::vector<std::unique_ptr<QSvgRenderer>> renderers;
    renderers.reserve(8);
    for (int i = 1; i <= 8; ++i)
    {
        renderers.push_back(loadRenderer(mineNumberPrefix, QString("%1mines.svg").arg(i).toStdString()));
    }
    return renderers;
}

void MainWindow::renderIcon(const Grid::State state, const int surroundingMines, QPushButton* button) const
{
    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    renderIcon(state, surroundingMines, button, buttonSize);
}

void MainWindow::renderIcon(Grid::State state, const int surroundingMines, QPushButton* button, const QSize& size) const
{
    if (!button)
    {
        return;
    }

    QSvgRenderer* renderer = nullptr;

    switch (state)
    {
    case Grid::State::OPENED:
        qDebug() << QString("rendering icon for surrounding: %1").arg(surroundingMines);
        if (surroundingMines == 0)
        {
            renderer = noAroundIconRenderer.get();
        }
        else if (surroundingMines >= 1 && surroundingMines <= 8)
        {
            renderer = surroundingMineIconRenderers.at(surroundingMines - 1).get();
        }
        else
        {
            qDebug() << QString("not a valid surrounding count: %1").arg(surroundingMines);
            return;
        }
        break;
    case Grid::State::FLAGGED:
        renderer = flagIconRenderer.get();
        break;
    case Grid::State::TRIGGERED:
        renderer = mineIconRenderer.get();
        break;
    case Grid::State::UNOPENED:
        renderer = blankIconRenderer.get();
        break;
    }

    if (!renderer)
    {
        qDebug() << "No SVG renderer for state:" << static_cast<int>(state);
        return;
    }
    if (!renderer->isValid())
    {
        qDebug() << "Invalid SVG renderer for state:" << static_cast<int>(state);
        return;
    }

    applyRenderer(renderer, button);
}

QIcon MainWindow::loadSvg(const QString& path)
{
    qDebug() << path << " exists? " << QFile::exists(path);
    const QSize& size = QSize(32, 32);
    QSvgRenderer renderer(path);
    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    renderer.render(&painter);
    return QIcon(pix);
}

void MainWindow::initAutoAdjust()
{
    // ���ô�������Ӧ��Ļ��С
    const QScreen* screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int screenWidth = screenGeometry.width();
    const int screenHeight = screenGeometry.height();
    this->resize(static_cast<int>(screenWidth * 0.8), static_cast<int>(screenHeight * 0.8));
    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);
}
