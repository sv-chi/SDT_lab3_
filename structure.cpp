#include "structure.h"

QList<Data> JsonDataStructure::getData(QString filePath) {
    QString val;
    QFile file;
    file.setFileName(filePath);
    QList<Data> result;
    if  (file.open(QIODevice::ReadOnly | QIODevice::Text)) {//открываем файл, если открылся, то

        val = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());//открываем как .json
        QJsonObject jsonObject = doc.object();//как объект
        QStringList keys (jsonObject.keys());//итерация по ключам
        int i = 0;
        QListIterator<QString> iterator(keys);
        while (iterator.hasNext() && i < 10) {//идем либо по всем ключам, либо получаем 10 ключей
            QString key = iterator.next();
            double value = jsonObject.value(key).toDouble();
            Data temp{key, value};
            result.push_back(temp);
            i++;
        }
    }
    else {
        qDebug() << "Json was not opened";
    }
    return result;
}

QList<Data> SqlDataStructure::getData(QString filePath) {
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создаем соединение по умолчанию с драйвером "QSQLITE"
    dbase.setDatabaseName(filePath); // устанавливаем связь
    QList<Data> result;//результат
    if (dbase.open()) {//если открылась база, то
        QSqlQuery query ("SELECT * FROM " + dbase.tables().takeFirst());//делаем запрос
        int i = 0;//запускаем счетчик
        while (query.next() && i < 10) {//который увеличивается до 10, чтобы получить либо все данные, либо 10 штук
            i++;
            Data temp{query.value(0).toString(), query.value(1).toDouble()};//создаем структуру
            result.push_back(temp);//отправляем список
        }
    }
    else {
        qDebug() << " Sqlite was not opened";
    }
    return result;
}

