#ifndef HAPTIC_H
#define HAPTIC_H
#include <qglobal.h>
#pragma once

class Haptic
{
public:
    virtual void vibrate(int ms = 50);
};

#endif // HAPTIC_H
