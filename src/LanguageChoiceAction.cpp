//
// Created by jerry on 2025/10/15.
//

#include "LanguageChoiceAction.h"

#include <QLocale>
LanguageChoiceAction::LanguageChoiceAction(const QString& language, QObject* parent)
    : QAction(parent)
    , language(language)
{
    // 尝试用 QLocale 解析这个语言标识符（如 "zh_CN"）
    const QLocale locale(language);

    QString displayName;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    // Qt 6.5+ 有 nativeLanguageName()
    displayName = locale.nativeLanguageName();
#else
    // 否则使用英文名称（兼容 Qt 5）
    QString langName = QLocale::languageToString(locale.language());
    QString country  = QLocale::territoryToString(locale.territory());
    if (!country.isEmpty())
        displayName = QString("%1 (%2)").arg(langName, country);
    else
        displayName = langName;
#endif

    // 设置菜单显示文字
    setText(displayName);

    // 你自己的语言代码（如 "zh_CN"）
    setData(language);

    // 信号连接
    connect(this, &QAction::triggered, this, &LanguageChoiceAction::chosen);
}


QString LanguageChoiceAction::tryLocal(const QString& languageID)
{
    QString languageName = QLocale(languageID).nativeLanguageName();
    return languageName.isEmpty() ? languageID : languageName;
}


void LanguageChoiceAction::chosen()
{
    emit onChosen(language);
}
