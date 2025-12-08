#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QString>

class Toast
{
public:
    static void display(const QString& msg, QWidget* parent = nullptr);

private:
    explicit Toast() = default;
};

#endif // TOAST_H