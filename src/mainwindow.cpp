#include "static.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QPainter>
#include <QTranslator>
#include <QApplication>
#include <QLocale>
#include <QAction>

#include "grid.h"
#include "LanguageChoiceAction.h"
using Difficulty = Field::GameMode;
std::unique_ptr<QSvgRenderer> loadRenderer(const std::string& resourcePrefix, const std::string& fileName);
std::vector<std::unique_ptr<QSvgRenderer>> loadNumberRenderers(const std::string& mineNumberPrefix);

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
      , resourcePrefix(":/group1/res/images/")
      , mineNumberPrefix(":/group1/res/numbers/")
      , ui(new Ui::MainWindow)
      , blankIconRenderer(loadRenderer(resourcePrefix, "blank.svg"))
      , noAroundIconRenderer(loadRenderer(resourcePrefix, "no_around.svg"))
      , flagIconRenderer(loadRenderer(resourcePrefix, "flag.svg"))
      , mineIconRenderer(loadRenderer(resourcePrefix, "mine.svg"))
      , mineTriggeredIconRenderer(loadRenderer(resourcePrefix, "exploded.svg"))

      , surroundingMineIconRenderers(loadNumberRenderers(mineNumberPrefix))
      , translator(new QTranslator(this))
{
    offset = static_cast<int>(surroundingMineIconRenderers.size());
    ui->setupUi(this);

    // 设置窗口自适应屏幕大小
    const QScreen* screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int screenWidth = screenGeometry.width();
    const int screenHeight = screenGeometry.height();
    this->resize(static_cast<int>(screenWidth * 0.8), static_cast<int>(screenHeight * 0.8));
    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);

    // Set window icon using the old method since we can't use renderIcon directly on MainWindow
    setWindowIcon(loadSvg(QString::fromStdString(resourcePrefix + "mine.svg")));

    connect(ui->bEasy, &QPushButton::clicked, this, &MainWindow::easy);
    connect(ui->bHard, &QPushButton::clicked, this, &MainWindow::hard);
    connect(ui->bCustom, &QPushButton::clicked, this, &MainWindow::custom);
    connect(ui->bMedium, &QPushButton::clicked, this, &MainWindow::medium);

    // 初始化语言支持
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    // 遍历系统语言，尝试加载对应的翻译文件
    for (const QString& locale : uiLanguages)
    {
        qWarning() << "found " << locale;
        const QString localeName = QLocale(locale).name();

        // 尝试加载该语言的翻译文件
        if (tryRegisterTranslation(localeName))
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

MainWindow::~MainWindow()
{
    for (const auto& field : fields)
    {
        delete field;
    }
    fields.clear();
    delete ui;
    mw.reset();
}

int MainWindow::getOffset() const
{
    return offset;
}

void MainWindow::easy()
{
    createGame(Difficulty::EASY);
}

void MainWindow::medium()
{
    createGame(Difficulty::MEDIUM);
}

void MainWindow::hard()
{
    createGame(Difficulty::HARD);
}

void MainWindow::custom()
{
    createGame(Difficulty::CUSTOM);
}


QString MainWindow::difficultyToStringStandard(Difficulty difficulty)
{
    switch (difficulty)
    {
    case Difficulty::EASY:
        return tr("EASY");
    case Difficulty::MEDIUM:
        return tr("MEDIUM");
    case Difficulty::HARD:
        return tr("HARD");
    case Difficulty::CUSTOM:
    default:
        return tr("CUSTOM");
    }
}

void MainWindow::createGame(const Difficulty difficulty)
{
    Field* field = nullptr;
    switch (difficulty)
    {
    case Difficulty::EASY:
        field = new Field(10, 10, 10, Field::GameMode::EASY);
        break;
    case Difficulty::MEDIUM:
        field = new Field(16, 16, 40, Field::GameMode::MEDIUM);
        break;
    case Difficulty::HARD:
        field = new Field(16, 30, 99, Field::GameMode::HARD);
        break;
    case Difficulty::CUSTOM:
    default:
        field = new Field(10, 10, 10, Field::GameMode::CUSTOM);
        break;
    }
    field->show();
}



// 动态创建语言菜单项
void MainWindow::createLanguageMenu()
{
    const std::set<QString>& locales = supportedLocales;
    // 获取语言菜单
    QMenu* languageMenu = this->ui->menuLanguage;
    if (!languageMenu)
    {

        return;
    }
    // 清除现有动作（如果有的话）
    languageMenu->clear();
    // 为每种支持的语言创建菜单项
    for (const QString& locale : locales)
    {
        auto* action = new LanguageChoiceAction(locale, languageMenu);
        languageMenu->addAction(action);
        // 连接信号槽
        connect(action, &LanguageChoiceAction::onChosen, this, &MainWindow::onLanguageActionTriggered);
    }
}

// Individual renderer methods
void MainWindow::setBlankIcon(QPushButton* button) const
{
    if (!button) return;

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    blankIconRenderer->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}

void MainWindow::setNoAroundIcon(QPushButton* button) const
{
    if (!button) return;

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    noAroundIconRenderer->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}

void MainWindow::setFlagIcon(QPushButton* button) const
{
    if (!button) return;

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    flagIconRenderer->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}

void MainWindow::setMineIcon(QPushButton* button) const
{
    if (!button) return;

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    mineIconRenderer->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}

void MainWindow::setMineTriggeredIcon(QPushButton* button) const
{
    if (!button) return;

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    mineTriggeredIconRenderer->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}

void MainWindow::setSurroundingMineIcon(int number, QPushButton* button) const
{
    if (!button || number < 1 || number > 8) return;

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    surroundingMineIconRenderers[number - 1]->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}

void MainWindow::renderIcon(Grid::State state, int surroundingMines, QPushButton* button) const
{
    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    renderIcon(state, surroundingMines, button, buttonSize);
}

void MainWindow::renderIcon(Grid::State state, const int surroundingMines, QPushButton* button, const QSize& size) const
{
    if (!button)
    {
        return;
    }

    QSvgRenderer* renderer = nullptr;

    switch (state)
    {
    case Grid::State::OPENED:
        qDebug() << QString("rendering icon for surrounding: %1").arg(surroundingMines);
        if (surroundingMines == 0)
        {
            renderer = noAroundIconRenderer.get();
        }
        else if (surroundingMines >= 1 && surroundingMines <= 8)
        {
            renderer = surroundingMineIconRenderers.at(surroundingMines - 1).get();
        }
        else
        {
            qDebug() << QString("not a valid surrounding count: %1").arg(surroundingMines);
            return;
        }
        break;
    case Grid::State::FLAGGED:
        renderer = flagIconRenderer.get();
        break;
    case Grid::State::TRIGGERED:
        renderer = mineIconRenderer.get();
        break;
    case Grid::State::UNOPENED:
        renderer = blankIconRenderer.get();
        break;
    }

    if (!renderer)
    {
        qDebug() << "No SVG renderer for state:" << static_cast<int>(state);
        return;
    }
    if (!renderer->isValid())
    {
        qDebug() << "Invalid SVG renderer for state:" << static_cast<int>(state);
        return;
    }

    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    renderer->render(&painter);
    button->setIcon(QIcon(pix));
    button->setIconSize(size);
}


QIcon MainWindow::loadSvg(const QString& path)
{
    qDebug() << path << " exists? " << QFile::exists(path);
    const QSize& size = QSize(32, 32);
    QSvgRenderer renderer(path);
    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    renderer.render(&painter);
    return QIcon(pix);
}
