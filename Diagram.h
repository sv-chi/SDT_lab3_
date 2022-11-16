#ifndef DIAGRAM_H
#define DIAGRAM_H
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <structure.h>

class IChart {
    QChart* chart = new QChart();
public:
    QChart* getChart() {return chart;}
    virtual void recreateChart(QList<Data>, bool) = 0;
    virtual ~IChart() = default;
    void clearChart() {chart->removeAllSeries();}
};

class BarChart: public IChart {
public:
    void recreateChart(QList<Data>, bool);
    ~BarChart() = default;
};

class PieChart: public IChart {
public:
    void recreateChart(QList<Data>, bool);
    ~PieChart() = default;
};

#endif // DIAGRAM_H
