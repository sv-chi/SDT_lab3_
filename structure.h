#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <QList>
#include <QFile>


class IDataStructure {
public:
    virtual void getData(QString);
};

class SqlDataStructure: IDataStructure {
public:
    void getData(QString);
};

class JsonDataStructure: IDataStructure {
public:
    void getData(QString);
};

#endif // STRUCTURE_H
