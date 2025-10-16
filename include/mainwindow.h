#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grid.h"
#include "field.h"
#include "recordview.h"

#include <QMainWindow>
#include <QSvgRenderer>
#include <QLocale>
#include <QTranslator>
#include <functional>
#include <QAction>
#include <set>

#include "Localeable.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow,public Localeable
{
    Q_OBJECT

public:
    std::vector<Field*> fields;
    void initInternational();
    void initAutoAdjust();
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    const std::string resourcePrefix, mineNumberPrefix;
    // 存储实际支持的语言列表（基于成功加载的翻译文件）
    static std::set<QString> supportedLocales;
    [[nodiscard]] bool tryRegisterTranslation(const QString& langID) const;


    // Public interface for rendering icons
    void renderIcon(Grid::State state, int surroundingMines, QPushButton* button) const;
    void renderIcon(Grid::State state, int surroundingMines, QPushButton* button, const QSize& size) const;

    [[nodiscard]] int getOffset() const;
    void easy();
    void medium();
    void hard();
    void custom();
    // 将Difficulty枚举转换为std::string
    static QString difficultyToStringStandard(Field::GameMode difficulty);


    void returnToMainMenu(bool destroyGame);

public slots:
    void returnToMainMenu() { returnToMainMenu(false); }

private:
    static QIcon loadSvg(const QString& path);
    Ui::MainWindow* ui;
    int offset;

    // SVG renderers stored as private const members
    const std::unique_ptr<QSvgRenderer> blankIconRenderer;
    const std::unique_ptr<QSvgRenderer> noAroundIconRenderer;
    const std::unique_ptr<QSvgRenderer> flagIconRenderer;
    const std::unique_ptr<QSvgRenderer> mineIconRenderer;
    const std::unique_ptr<QSvgRenderer> mineTriggeredIconRenderer;
    const std::vector<std::unique_ptr<QSvgRenderer>> surroundingMineIconRenderers; // 1-8数字图标渲染器

    // 语言翻译器
    QTranslator* translator;

    // 动态创建语言菜单项
    void createLanguageMenu();
    void testIcons();
    void loadTestIcon(const QString& iconName, int r, int c);

    // 根据难度创建游戏
    void createGame(Field::GameMode difficulty);
    Field* field;
    
    // 记录视图
    RecordView* recordView;

    // Individual renderer methods - 修改为直接设置图标到按钮
    void setBlankIcon(QPushButton* button) const;
    void setNoAroundIcon(QPushButton* button) const;
    void setFlagIcon(QPushButton* button) const;
    void setMineIcon(QPushButton* button) const;
    void setMineTriggeredIcon(QPushButton* button) const;
    void setSurroundingMineIcon(int number, QPushButton* button) const;
    void hideAll() const;

private slots:
    // 动态语言切换槽函数
    void onLanguageActionTriggered(const QString& language);
    void showRecordView();
    void retranslate() override{ui->retranslateUi(this);}
};

#endif // MAINWINDOW_H