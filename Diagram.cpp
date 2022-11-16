#include "Diagram.h"
//столбчатая диаграмма
void BarChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    Data data;//данные из списка
    QChart* chart = getChart();//сама диаграмма
    chart->setTitle("BarChart");//заголовок
    QBarSeries *series = new QBarSeries(chart);
    int length = list.count();//кол-во данных
    for (int i = 0; i < length; i++) {
        QBarSet* set = new QBarSet(list.at(i).key);
        * set << list.at(i).value;
        if (isBWEnabled)//если выбрали чб, то
        {int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;//черно-белой делаем диаграмму
            set->setBrush(QColor(bw, bw, bw));//присваем это диаграмме
        }
        series->append(set);
    }
    chart->removeAllSeries();//очистили диаграммы
    chart->addSeries(series);//создали новые
    chart->createDefaultAxes();//создали новые
}

void PieChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    Data data;//данные из списка
    QChart* chart = getChart();//сама диаграмма
    chart->setTitle("PieChart");//заголовок
    QPieSeries * series = new QPieSeries(chart);
    int length = list.count();//кол-во данных
    for (int i = 0; i < length; i++) {
        QPieSlice* slice = series->append(list.at(i).key, list.at(i).value);
        if (isBWEnabled)//тут мы снова проверяем чб
        {int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            slice->setBrush(QColor(bw, bw, bw));
        }else{//а если он не нужен, то делаем разные цвета диаграммы(разноцветные)
            int c = (rand() % 255);

            int c2 = (rand() % 255);

            int c3 = (rand() % 255);

            slice->setBrush(QColor(c, c2, c3));//присвоили цвет
        }
    }
    chart->removeAllSeries();//очистили диаграммы
    chart->addSeries(series);//создали новые
}
