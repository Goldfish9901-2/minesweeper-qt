//
// Created by jerry on 2025/10/16.
//

#ifndef MINESWEEPER_RECORD_H
#define MINESWEEPER_RECORD_H
#include "field.h"
#include <QDateTime>

class Field;

class Record
{
public:
     Field::GameMode mode;
     unsigned short width, height, mines;
     unsigned int secs;
     QDateTime timestamp;

     Record() : mode(Field::GameMode::EASY), width(0), height(0), mines(0), secs(0), timestamp(QDateTime::currentDateTime())
     {
     }

     Record(const Field::GameMode mode,
         const unsigned short width,
         const unsigned short height,
         const unsigned short mines,
         const unsigned int secs,
         const QDateTime& timestamp)
         : mode(mode), width(width), height(height), mines(mines), secs(secs), timestamp(timestamp)
     {
     }
     
     Record(const Field::GameMode mode,
         const unsigned short width,
         const unsigned short height,
         const unsigned short mines,
         const unsigned int secs)
         : mode(mode), width(width), height(height), mines(mines), secs(secs), timestamp(QDateTime::currentDateTime())
     {
     }
};
#endif //MINESWEEPER_RECORD_H
