#include "field.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <vector>
#include <set>




int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    qDebug() << " application started. loading translations...";
    MainWindow mw;
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    // 遍历系统语言，尝试加载对应的翻译文件
    for (const QString& locale : uiLanguages)
    {
        qWarning() << "found " << locale;
        const QString localeName = QLocale(locale).name();


        if (mw.tryRegisterTranslation(localeName))
        {
            QApplication::installTranslator(&translator);
        }
        qWarning() << locale << " failed";
    }


    mw.show();
    return QApplication::exec();
}
