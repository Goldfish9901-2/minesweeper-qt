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
        // printf("%s\n",locale.toStdString().c_str());
        qDebug()<<"found "<<locale;
        const QString baseName = "MineSweeper_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }else{
            qDebug()<<locale<<" failed";
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
