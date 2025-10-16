//
// Created by jerry on 2025/10/14.
//
#include <QMessageBox>
#include <ui_mainwindow.h>

#include "mainwindow.h"

void MainWindow::initInternational()
{
    // 初始化语言支持
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    // 遍历系统语言，尝试加载对应的翻译文件
    for (const QString& locale : uiLanguages)
    {
        qWarning() << "found " << locale;

        // 尝试加载该语言的翻译文件
        if (const QString localeName = QLocale(locale).name();
            tryRegisterTranslation(localeName))
        {
            qDebug() << "Successfully loaded translation for" << localeName;
        }
        else
        {
            qWarning() << locale << " failed";
        }
    }


    // 创建语言菜单
    createLanguageMenu();
}

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
        ui->retranslateUi(this); // 刷新 UI 文本
        if (field != nullptr  && field->isGaming())
            field->retranslateUi();
        if (recordView != nullptr)
            recordView->retranslate();
        return;
    }
    QMessageBox::warning(
        nullptr,
        tr("invalid language"),
        language);
}

