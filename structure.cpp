#include "structure.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>

void JsonDataStructure::getData(QString filePath) {
    QString val;
    QFile file;
    file.setFileName(filePath);
    if  (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        val = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject jsonObject = doc.object();

        QStringList keys (jsonObject.keys());
        int i = 0;
        QListIterator<QString> iterator(keys);
        while (iterator.hasNext() && i < 10) {
            QString key = iterator.next();
            double value = jsonObject.value(key).toDouble();
            qDebug() << key << value;
        }
    }
}

void SqlDataStructure::getData(QString filePath) {
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(filePath);

    if (dbase.open()) {
        QSqlQuery query ("SELECT * FROM " + dbase.tables().takeFirst() + " LIMIT 0,10");
        while (query.next()) {
            qDebug() << query.value(0).toString() << query.value(1).toDouble();
        }
    }
}

