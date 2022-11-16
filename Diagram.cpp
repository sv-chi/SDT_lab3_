#include "Diagram.h"

void BarChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    Data data;
    QChart* chart = getChart();
    chart->setTitle("BarChart");
    QBarSeries *series = new QBarSeries(chart);
    int length = list.count();
    for (int i = 0; i < length; i++) {
        QBarSet* set = new QBarSet(list.at(i).key);
        * set << list.at(i).value;
        if (isBWEnabled)
        {int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            set->setBrush(QColor(bw, bw, bw));
        }
        series->append(set);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

void PieChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    Data data;
    QChart* chart = getChart();
    chart->setTitle("PieChart");
    QPieSeries * series = new QPieSeries(chart);
    int length = list.count();
    for (int i = 0; i < length; i++) {
        QPieSlice* slice = series->append(list.at(i).key, list.at(i).value);
        if (isBWEnabled)
        {int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            slice->setBrush(QColor(bw, bw, bw));
        }else{
            int c = (rand() % 255);

            int c2 = (rand() % 255);

            int c3 = (rand() % 255);

            slice->setBrush(QColor(c, c2, c3));
        }
    }
    chart->removeAllSeries();
    chart->addSeries(series);
}
