#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QList>
#include <QFile>

struct Data {
public:
    QString key;
    double value;
};

class IDataStructure {
public:
    virtual QList<Data> getData(QString) = 0;
    virtual ~IDataStructure() = default;

};

class SqlDataStructure: public IDataStructure {
public:
    QList<Data> getData(QString);
    ~SqlDataStructure() = default;
};

class JsonDataStructure: public IDataStructure {
public:
    QList<Data> getData(QString);
    ~JsonDataStructure() = default;
};

#endif // STRUCTURE_H
