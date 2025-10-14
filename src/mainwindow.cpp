#include "static.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QPainter>
#include <QSizePolicy>

#include "grid.h"
using Difficulty = Field::GameMode;
QString joinRendererPath(const std::string& resourcePrefix, const std::string& fileName)
{
    QString path = QString::fromStdString(resourcePrefix + fileName);
    if (!QFile::exists(path))
    {
        qDebug() << "Icon not found: " << path;
        return QString::fromStdString(resourcePrefix + "maybe.svg");
    }
    return path;
    // return QFile::exists(path) ? path : QString::fromStdString(resourcePrefix + "maybe.svg");
}
// Lambda 初始化函数
std::unique_ptr<QSvgRenderer> loadRenderer(const std::string& resourcePrefix, const std::string& fileName)
{
    return std::make_unique<QSvgRenderer>(joinRendererPath(resourcePrefix, fileName));
}

std::vector<std::unique_ptr<QSvgRenderer>> loadNumberRenderers(const std::string& mineNumberPrefix)
{
    std::vector<std::unique_ptr<QSvgRenderer>> renderers;
    renderers.reserve(8);
    for (int i = 1; i <= 8; ++i)
    {
        renderers.push_back(loadRenderer(mineNumberPrefix, QString("%1mines.svg").arg(i).toStdString()));
    }
    return renderers;
}


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

void MainWindow::createGame(Difficulty difficulty)
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

void MainWindow::testIcons()
{
    int r = 0, c = 0;
    for (int i = 0; i < 8; ++i)
    {
        QPushButton* btn = new QPushButton(this);
        btn->setFixedSize(40, 40);
        renderIcon(Grid::State::OPENED, i + 1, btn);
        ui->iconDemoLayout->addWidget(btn, r, c++);
    }
    r = 1, c = 0;
    QPushButton* btn1 = new QPushButton(this);
    btn1->setFixedSize(40, 40);
    renderIcon(Grid::State::TRIGGERED, 0, btn1);
    ui->iconDemoLayout->addWidget(btn1, r, c++);

    QPushButton* btn2 = new QPushButton(this);
    btn2->setFixedSize(40, 40);
    renderIcon(Grid::State::FLAGGED, 0, btn2);
    ui->iconDemoLayout->addWidget(btn2, r, c++);

    QPushButton* btn3 = new QPushButton(this);
    btn3->setFixedSize(40, 40);
    renderIcon(Grid::State::UNOPENED, 0, btn3);
    ui->iconDemoLayout->addWidget(btn3, r, c++);

    QPushButton* btn4 = new QPushButton(this);
    btn4->setFixedSize(40, 40);
    renderIcon(Grid::State::OPENED, 0, btn4);
    ui->iconDemoLayout->addWidget(btn4, r, c++);
}

void MainWindow::loadTestIcon(const QString& iconName, int r, int c)
{
    // This method is no longer used with the new approach, but kept for compatibility
    qDebug() << QString("loading to location [ %1, %2 ]").arg(r).arg(c);
    QPushButton* btn = new QPushButton(this);
    btn->setFixedSize(40, 40);
    // We'll use the old method here for backward compatibility
    renderIcon(Grid::State::UNOPENED, 0, btn);
    ui->iconDemoLayout->addWidget(btn, r, c);
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
        qDebug()<<QString("rendering icon for surrounding: %1").arg(surroundingMines);
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
