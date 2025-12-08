//
// Created by user on 2025/10/16.
//

#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#ifdef Q_OS_WASM
#include <emscripten.h>
#endif

DatabaseManager::DatabaseManager()
{
    qDebug()<<"init sqlite..";
#ifdef Q_OS_WASM
    // 挂载 IDBFS
    EM_ASM(
        try { FS.mkdir('/data'); } catch(e) {}
        FS.mount(IDBFS, {}, '/data');
        );
    dbPath = "/data/records.db";
#else
    const QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataPath))
        dir.mkpath(dataPath);
    dbPath = dataPath + "/records.db";
#endif
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool DatabaseManager::init()
{
    if (!db.open())
    {
        qDebug() << "Cannot open database:" << db.lastError().text();
        return false;
    }
#ifdef Q_OS_WASM
    // 调用 JS 同步 IndexedDB
    EM_ASM({
        FS.syncfs(function(err) {
            if(err) {
                console.error('DB sync failed', err);
            } else {
                console.log('DB synced to IndexedDB');
            }
        });
    });
#endif
    return createTables();
}

bool DatabaseManager::createTables() const
{
    QSqlQuery query(db);

    // 创建记录表
    const QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            mode INTEGER NOT NULL,
            width INTEGER NOT NULL,
            height INTEGER NOT NULL,
            mines INTEGER NOT NULL,
            secs INTEGER NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createTableQuery))
    {
        qDebug() << "Cannot create table:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::saveRecord(const Record& record) const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO records (mode, width, height, mines, secs, timestamp) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(static_cast<int>(record.mode));
    query.addBindValue(record.width);
    query.addBindValue(record.height);
    query.addBindValue(record.mines);
    query.addBindValue(record.secs);
    query.addBindValue(record.timestamp);

    if (!query.exec())
    {
        qDebug() << "Cannot insert record:" << query.lastError().text();
        return false;
    }

    return true;
}

QList<Record> DatabaseManager::getRecords(Field::GameMode mode) const
{
    QList<Record> records;
    QSqlQuery query(db);
    query.prepare("SELECT mode, width, height, mines, secs, timestamp FROM records WHERE mode = ? ORDER BY secs ASC");
    query.addBindValue(static_cast<int>(mode));

    if (!query.exec())
    {
        qDebug() << "Cannot fetch records:" << query.lastError().text();
        return records;
    }

    while (query.next())
    {
        Field::GameMode mode = static_cast<Field::GameMode>(query.value(0).toInt());
        unsigned short width = static_cast<unsigned short>(query.value(1).toInt());
        unsigned short height = static_cast<unsigned short>(query.value(2).toInt());
        unsigned short mines = static_cast<unsigned short>(query.value(3).toInt());
        unsigned int secs = query.value(4).toUInt();
        QDateTime timestamp = query.value(5).toDateTime();

        records.append(Record(mode, width, height, mines, secs, timestamp));
    }

    return records;
}

QList<Record> DatabaseManager::getAllRecords() const
{
    QList<Record> records;
    QSqlQuery query(db);
    query.prepare("SELECT mode, width, height, mines, secs, timestamp FROM records ORDER BY secs ASC");

    if (!query.exec())
    {
        qDebug() << "Cannot fetch records:" << query.lastError().text();
        return records;
    }

    while (query.next())
    {
        Field::GameMode mode = static_cast<Field::GameMode>(query.value(0).toInt());
        unsigned short width = static_cast<unsigned short>(query.value(1).toInt());
        unsigned short height = static_cast<unsigned short>(query.value(2).toInt());
        unsigned short mines = static_cast<unsigned short>(query.value(3).toInt());
        unsigned int secs = query.value(4).toUInt();
        QDateTime timestamp = query.value(5).toDateTime();

        records.append(Record(mode, width, height, mines, secs, timestamp));
    }

    return records;
}
void DatabaseManager::syncIfWasm() const
{
#ifdef Q_OS_WASM
    EM_ASM(
        FS.syncfs(function(err){
            if(err) console.log('DB sync failed', err);
            else console.log('DB synced to IndexedDB');
        });
        );
#endif
}
