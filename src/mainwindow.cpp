#include "mainwindow.h"
#include "field.h"
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
#include <QMessageBox>

#include "grid.h"
#include "LanguageChoiceAction.h"
#include "recordview.h"
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
      , field(nullptr)
      , recordView(nullptr)
{
    offset = static_cast<int>(surroundingMineIconRenderers.size());
    ui->setupUi(this);

    initAutoAdjust();

    // Set window icon using the old method since we can't use renderIcon directly on MainWindow
    setWindowIcon(loadSvg(QString::fromStdString(resourcePrefix + "mine.svg")));

    connect(ui->bEasy, &QPushButton::clicked, this, &MainWindow::easy);
    connect(ui->bHard, &QPushButton::clicked, this, &MainWindow::hard);
    connect(ui->bCustom, &QPushButton::clicked, this, &MainWindow::custom);
    connect(ui->bMedium, &QPushButton::clicked, this, &MainWindow::medium);

    connect(ui->actionMain, &QAction::triggered, this, [this](bool)
    {
        this->returnToMainMenu();
    });
    connect(ui->actionGame, &QAction::triggered, this, [this](bool)
    {
        if (!field)
        {
            QMessageBox::warning(
                this,
                tr("invalid option"),
                tr("You must start a game first")
            );
            emit ui->actionMain->triggered();
            return;
        }
        hideAll();
        field->show();
    });

    // 连接记录视图动作
    connect(ui->actionRecord, &QAction::triggered, this, &MainWindow::showRecordView);

    initInternational();
}

MainWindow::~MainWindow()
{
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


QString MainWindow::difficultyToStringStandard(const Difficulty difficulty)
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

void MainWindow::returnToMainMenu(const bool destroyGame)
{
    hideAll();

    if (destroyGame)
    {
        delete field;
        field = nullptr;
    }

    ui->centralwidget->show();
}

void MainWindow::createGame(const Difficulty difficulty)
{
    ui->centralwidget->hide();
    if (field)
    {
        if (field->isGaming())
        {
            const auto result = QMessageBox::question(
                this,
                tr("You have already started a game"),
                tr("Do you wish to abort and start a new one? "),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
            );
            if (result == QMessageBox::No)
            {
                return;
            }
        }
        field->hide();
        delete field;
    }
    switch (difficulty)
    {
    case Difficulty::EASY:
        field = new Field(10, 10, 10, Field::GameMode::EASY, this);
        break;
    case Difficulty::MEDIUM:
        field = new Field(16, 16, 40, Field::GameMode::MEDIUM, this);
        break;
    case Difficulty::HARD:
        field = new Field(16, 30, 99, Field::GameMode::HARD, this);
        break;
    case Difficulty::CUSTOM:
    default:
        field = new Field(10, 10, 10, Field::GameMode::CUSTOM, this);
        break;
    }
    field->show();
}

void MainWindow::showRecordView()
{
    if (!recordView) {
        recordView = new RecordView(this);
    }
    
    // 隐藏其他界面元素
    // hideAll();
    
    // 显示记录视图窗口
    recordView->refresh();
    recordView->show();
    recordView->raise();  // 确保窗口在最前面
    recordView->activateWindow();  // 激活窗口
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

void MainWindow::applyRenderer(QSvgRenderer* renderer, QPushButton* button)
{

    if (!button)
    {
        qFatal()<<"Button is nullptr";
        return;
    }
    if (!renderer)
    {
        qFatal()<<"Renderer is nullptr";
        return;
    }
    if (!renderer->isValid())
    {
        qFatal()<<"Renderer is invalid";
    }

    QSize buttonSize = button->size();
    if (buttonSize.width() <= 0 || buttonSize.height() <= 0)
    {
        buttonSize = QSize(32, 32); // Default size
    }

    QPixmap pix(buttonSize);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    renderer->render(&painter, QRectF(0, 0, pix.width(), pix.height())); // 指定绘制区域

    button->setIcon(QIcon(pix));
    button->setIconSize(buttonSize);
}


void MainWindow::hideAll() const
{
    ui->centralwidget->hide();
    if (field) field->hide();
    if (recordView) recordView->hide();
}



