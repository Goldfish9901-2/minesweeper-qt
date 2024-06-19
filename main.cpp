#include "static.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

MainWindow *mw=nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MineSweeper_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    mw=new MainWindow();

    mw->show();
    return a.exec();
}

Field* container(Grid* grid){
    for(Field* field: mw->fields){
        if(field->contains(grid)){
            return field;
        }
    }
    return nullptr;
}
