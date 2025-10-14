//
// Created by jerry on 2025/10/14.
//
#include <QMessageBox>
#include <ui_mainwindow.h>

#include "static.h"
// mainwindow.cpp
std::set<QString> MainWindow::supportedLocales;

// 尝试加载翻译文件，成功则添加到支持列表
bool MainWindow::tryRegisterTranslation(const QString& langID) const
{
    if (const QString baseName = "MineSweeper_" + QLocale(langID).name();
        !translator->load(":/i18n/" + baseName))
        return false;
    supportedLocales.insert(langID);

    return true;
}

void MainWindow::onLanguageActionTriggered(const QString& language)
{
    if (tryRegisterTranslation(language))
    {
        QApplication::installTranslator(translator);
        ui->retranslateUi(this);  // 刷新 UI 文本
        return;
    }
    QMessageBox::warning(
        nullptr,
        tr("invalid language"),
        language);
}

