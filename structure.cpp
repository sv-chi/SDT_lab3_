#include "structure.h"

QList<Data> JsonDataStructure::getData(QString filePath) {
    QString val;
    QFile file;
    file.setFileName(filePath);
    QList<Data> result;
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
            Data temp{key, value};
            result.push_back(temp);
        }
    }}

QList<Data> SqlDataStructure::getData(QString filePath) {
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(filePath);
    QList<Data> result;
    if (dbase.open()) {
        QSqlQuery query ("SELECT * FROM " + dbase.tables().takeFirst() + " LIMIT 0,10");
        int i = 0;
               while (query.next() && i < 10) {
                   i++;
                   Data temp{query.value(0).toString(), query.value(1).toDouble()};
                   result.push_back(temp);   }
    }
}

