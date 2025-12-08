//
// Created by user on 2025/10/16.
//

#ifndef MINESWEEPER_DATABASE_H
#define MINESWEEPER_DATABASE_H

#include <QtSql>
#include "record.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool init();
    bool saveRecord(const Record& record) const;
    QList<Record> getRecords(Field::GameMode mode) const;
    QList<Record> getAllRecords() const;

private:
    QSqlDatabase db;
    QString dbPath;

private:
    bool createTables() const;
    void syncIfWasm() const;

};

#endif //MINESWEEPER_DATABASE_H
