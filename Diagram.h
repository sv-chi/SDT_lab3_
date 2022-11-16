#ifndef DIAGRAM_H
#define DIAGRAM_H
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <structure.h>

class IChart {//интерфейс
    QChart* chart = new QChart();//диаграмма
public:
    QChart* getChart() {return chart;}
    virtual void recreateChart(QList<Data>, bool) = 0;//построение диаграммы
    virtual ~IChart() = default;
    void clearChart() {chart->removeAllSeries();}//очистка
};

class BarChart: public IChart {//диаграмма столбчатая
public:
    void recreateChart(QList<Data>, bool);
    ~BarChart() = default;
};

class PieChart: public IChart {//круговая диаграмма
public:
    void recreateChart(QList<Data>, bool);
    ~PieChart() = default;
};

#endif // DIAGRAM_H
