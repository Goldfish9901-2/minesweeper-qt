//
// Created by jerry on 2025/10/15.
//

#ifndef MINESWEEPER_LANGUAGE_CHOICE_ACTION_H
#define MINESWEEPER_LANGUAGE_CHOICE_ACTION_H
#include <QAction>


class LanguageChoiceAction final : public QAction
{
    Q_OBJECT

public:
    LanguageChoiceAction(const QString& language, QObject* parent);
    static QString tryLocal(const QString& languageID);
signals:
    void onChosen(QString& string);

public slots:
    void chosen();

private:
    QString language;
};


#endif //MINESWEEPER_LANGUAGE_CHOICE_ACTION_H
