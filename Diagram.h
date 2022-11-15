#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <structure.h>

class IChart {
public:
    virtual QChart* getChart() = 0;
    virtual void recreateChart(QList<Data>) = 0;
    virtual ~IChart() = default;
};

class BarChart: public IChart {
    QChart* chart = new QChart();
public:
    QChart* getChart() {return chart;}
    void recreateChart(QList<Data>);
    ~BarChart() {delete chart;}
};

class PieChart: public IChart {
    QChart* chart = new QChart();
public:
    QChart* getChart() {return chart;}
    void recreateChart(QList<Data>);
    ~PieChart() {delete chart;}
};

#endif // DIAGRAM_H
