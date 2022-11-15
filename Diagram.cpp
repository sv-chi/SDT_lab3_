#include "Diagram.h"

void BarChart::recreateChart(QList<Data> list) {
    Data data;
    QBarSeries *series = new QBarSeries(chart);
    for (int i = 0; i < list.count(); i++) {
        QBarSet* set = new QBarSet(list.at(i).key);
        * set << list.at(i).value;
        series->append(set);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}
void PieChart::recreateChart(QList<Data> list) {
    Data data;
    QPieSeries * series = new QPieSeries(chart);
    for (int i = 0; i < list.count(); i++) {
        series->append(list.at(i).key, list.at(i).value);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
}
