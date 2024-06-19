#ifndef STATIC_H
#define STATIC_H

#include "mainwindow.h"
#include <QIcon>

extern MainWindow* mw;

Field* container(Grid* grid);

extern QList<QIcon*> mineIcons;

extern QIcon *flagIcon, *mineIcon,*mineTriggeredIcon, *appIcon;
extern std::string resourcePrefix;

extern QList<std::string> numberColors;

#endif // STATIC_H
