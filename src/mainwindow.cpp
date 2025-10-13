#include "static.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QPainter>
#include <QSizePolicy>

#include "grid.h"

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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
      , resourcePrefix(":/group1/res/images/")
      , mineNumberPrefix(":/group1/res/numbers/")
      , blankIcon(loadSvg(QString::fromStdString(resourcePrefix + "blank.svg")))
      , noAroundIcon(loadSvg(QString::fromStdString(resourcePrefix + "no_around.svg")))
      , flagIcon(loadSvg(QString::fromStdString(resourcePrefix + "flag.svg")))
      , mineIcon(loadSvg(QString::fromStdString(resourcePrefix + "mine.svg")))
      , mineTriggeredIcon(loadSvg(QString::fromStdString(resourcePrefix + "exploded.svg")))
      , appIcon(mineIcon)
      , surroundingMineIcons(
          [this]
          {
              std::vector<QIcon> icons;
              for (int i = 0; i < 8; ++i)
              {
                  icons.push_back(
                      loadSvg(
                          QString::fromStdString(mineNumberPrefix) +
                          QString("%1mines.svg").arg(i + 1)
                      )
                  );
              }
              return icons;
          }()
      )
      , iconMap(
          [this]
          {
              std::map<int, QIcon> tempIconMap;
              tempIconMap.insert(std::pair<int, QIcon>(0, noAroundIcon));
              int i = 1;
              for (; i <= 8; ++i)
              {
                  tempIconMap.insert(std::pair(i, surroundingMineIcons[i - 1]));
              }
              offset = i;
              tempIconMap.insert(std::pair(i + static_cast<int>(Grid::State::TRIGGERED), mineTriggeredIcon));
              tempIconMap.insert(std::pair(i + static_cast<int>(Grid::State::FLAGGED), flagIcon));
              tempIconMap.insert(std::pair(i + static_cast<int>(Grid::State::UNOPENED), blankIcon));
              return tempIconMap;
          }())
      , numberColors(std::vector<std::string>{
          "#0000FF", "#008000", "#FF0000", "#000080",
          "#800000", "#008080", "#000000", "#808080"
      })

      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口自适应屏幕大小
    const QScreen* screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int screenWidth = screenGeometry.width();
    const int screenHeight = screenGeometry.height();
    this->resize(static_cast<int>(screenWidth * 0.8), static_cast<int>(screenHeight * 0.8));
    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);


    setWindowIcon(mineIcon);

    testIcons();

    connect(ui->bEasy, &QPushButton::clicked, this, &MainWindow::easy);
    connect(ui->bHard, &QPushButton::clicked, this, &MainWindow::hard);
    connect(ui->bCustom, &QPushButton::clicked, this, &MainWindow::custom);
    connect(ui->bMedium, &QPushButton::clicked, this, &MainWindow::medium);
}

MainWindow::~MainWindow()
{
    // instance = nullptr;
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
    auto* field = new Field(10, 10, 10, "easy");
    field->show();
}

void MainWindow::medium()
{
    auto* field = new Field(16, 16, 40, "medium");
    // fields.append(field);
    field->show();
}

void MainWindow::hard()
{
    auto* field = new Field(16, 30, 99, "hard");
    // fields.append(field);
    field->show();
}

void MainWindow::custom()
{
    auto* field = new Field(10, 10, 10, "custom");
    // fields.append(field);
    field->show();
}

void MainWindow::testIcons()
{
    int r = 0, c = 0;
    for (auto& icon : this->surroundingMineIcons)
    {
        loadIcon(icon, r, c++);
    }
    r = 1, c = 0;
    loadIcon(mineIcon, r, c++);
    loadIcon(mineTriggeredIcon, r, c++);
    loadIcon(blankIcon, r, c++);
    loadIcon(noAroundIcon, r, c++);
    loadIcon(flagIcon, r, c++);
}

void MainWindow::loadIcon(QIcon icon, int r, int c)
{
    qDebug() << QString("loading to location [ %1, %2 ]").arg(r).arg(c);
    QPushButton* btn = new QPushButton(this);
    btn->setWindowIcon(icon);
    btn->setIconSize(QSize(40, 40));
    ui->iconDemoLayout->addWidget(btn, r, c);
}

