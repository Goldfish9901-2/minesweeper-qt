#include "static.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QScreen>
#include <QGuiApplication>

QIcon *flagIcon,*mineIcon,*mineTriggeredIcon,*appIcon;
std::string resourcePrefix;
QList<std::string> numberColors;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口自适应屏幕大小
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->resize(screenWidth * 0.8, screenHeight * 0.8);
    this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);

    resourcePrefix = ":/group1/images/";

    flagIcon=new QIcon(QString::fromStdString(resourcePrefix+"flag.ico"));
    mineIcon=new QIcon(QString::fromStdString(resourcePrefix+"mine2.ico"));
    mineTriggeredIcon=new QIcon(QString::fromStdString(resourcePrefix+"mine3.ico"));
    if(!mineIcon->isNull())
        setWindowIcon(*mineIcon);
    else QMessageBox::warning(this,"Error","No icon found for this application");
    appIcon=mineIcon;

    numberColors=QList<std::string>{"#0000FF","#008000","#FF0000","#000080","#800000","#008080","#000000","#808080"};

    connect(ui->bEasy, &QPushButton::clicked, this, &MainWindow::easy);
    connect(ui->bHard, &QPushButton::clicked, this, &MainWindow::hard);
    connect(ui->bCustom, &QPushButton::clicked, this, &MainWindow::custom);
    connect(ui->bMedium, &QPushButton::clicked, this, &MainWindow::medium);

}

MainWindow::~MainWindow()
{
    // instance = nullptr;
    for (auto& field : fields)
    {
        delete field;
    }
    delete ui;
}
void MainWindow::easy()
{

    Field *field = new Field(10, 10, 10, "easy");
    field->show();
}
void MainWindow::medium()
{
    Field *field = new Field(16, 16, 40, "medium");
    // fields.append(field);
    field->show();
}

void MainWindow::hard()
{
    Field *field = new Field(16, 30, 99, "hard");
    // fields.append(field);
    field->show();
}
void MainWindow::custom()
{
    Field *field = new Field(10, 10, 10, "custom");
    // fields.append(field);
    field->show();
}
