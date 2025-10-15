//
// Created by jerry on 2025/10/16.
//

#ifndef MINESWEEPER_RECORD_H
#define MINESWEEPER_RECORD_H
#include "field.h"
class Field;

class Record
{
public:
     const Field::GameMode mode;
     const unsigned short width, height, mines;
     const unsigned _int32 secs;
     Record(Field::GameMode mode, unsigned short width, unsigned short height, unsigned short mines, unsigned _int32 secs)
         : mode(mode), width(width), height(height), mines(mines), secs(secs)
     {
     }
};
#endif //MINESWEEPER_RECORD_H
