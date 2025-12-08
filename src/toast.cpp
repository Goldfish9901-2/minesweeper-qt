#include "toast.h"
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QPoint>
#include <QRect>

class ToastWidget : public QWidget
{
public:
    explicit ToastWidget(const QString& msg, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_AlwaysStackOnTop);
        setAttribute(Qt::WA_DeleteOnClose);
        
        auto* label = new QLabel(msg, this);
        label->setStyleSheet("background:#000000AA;color:white;padding:8px;border-radius:6px;");
        label->setWordWrap(true);
        
        auto* layout = new QVBoxLayout(this);
        layout->addWidget(label);
        layout->setContentsMargins(0, 0, 0, 0);
        
        setLayout(layout);
        adjustSize();
        
        // 居中显示在父窗口或屏幕中央
        if (parent) {
            const QPoint globalPos = parent->mapToGlobal(
                QPoint((parent->width() - width()) / 2, (parent->height() - height()) / 2));
            move(globalPos);
        } else {
            const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
            move((screenGeometry.width() - width()) / 2, screenGeometry.height() / 2);
        }
    }
};

void Toast::display(const QString& msg, QWidget* parent)
{
    auto* toast = new ToastWidget(msg, parent);
    toast->show();
    QTimer::singleShot(3000, toast, [toast] { 
        toast->close(); 
        toast->deleteLater();
    });
}
