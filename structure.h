#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QList>
#include <QFile>
//структура, в которой хранятся данные
struct Data {
public:
    QString key;
    double value;
};
//интерфейс получения данных
class IDataStructure {
public:
    virtual QList<Data> getData(QString) = 0;//функция получения данных
    virtual ~IDataStructure() = default;

};
//получение данных от .sqlite
class SqlDataStructure: public IDataStructure {
public:
    QList<Data> getData(QString);
    ~SqlDataStructure() = default;
};
//получение данных от .json
class JsonDataStructure: public IDataStructure {
public:
    QList<Data> getData(QString);
    ~JsonDataStructure() = default;
};

#endif // STRUCTURE_H
