//
// Created by jerry on 2025/10/15.
//
#include "static.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QPainter>
#include <QTranslator>
#include <QApplication>
#include <QLocale>
#include <QMap>
#include <QAction>
#include <QMenu>

#include "grid.h"

using Difficulty = Field::GameMode;
QString joinRendererPath(const std::string& resourcePrefix, const std::string& fileName)
{
    QString path = QString::fromStdString(resourcePrefix + fileName);
    if (!QFile::exists(path))
    {
        qDebug() << "Icon not found: " << path;
        return QString::fromStdString(resourcePrefix + "maybe.svg");
    }
    return path;
    // return QFile::exists(path) ? path : QString::fromStdString(resourcePrefix + "maybe.svg");
}
// Lambda ³õÊ¼»¯º¯Êý
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