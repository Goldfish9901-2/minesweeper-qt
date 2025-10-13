#include "static.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

std::shared_ptr<MainWindow> mw;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    qDebug() << " application started. loading translations...";
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages)
    {
        // printf("%s\n",locale.toStdString().c_str());
        qWarning() << "found " << locale;
        const QString baseName = "MineSweeper_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            QApplication::installTranslator(&translator);

            break;
        }
        qWarning() << locale << " failed";
    }
    mw = std::make_shared<MainWindow>(new MainWindow());

    mw->show();
    return QApplication::exec();
}

Field* container(Grid* grid)
{
    for (const auto& field : mw->fields)
    {
        if (field->contains(grid))
        {
            return field;
        }
    }
    return nullptr;
}
