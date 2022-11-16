#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Diagram.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
class MainWindow : public QWidget
{
    Q_OBJECT

private slots://слоты
    void changeDirectory();//смена директории
    void fileSelection(const QItemSelection &selected, const QItemSelection &deselected);//изменение файла выбранного
    void changeChartType();//изменение типа графика
    void printChart();//печать диаграммы
    void colorSwap();//изменение цвета
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString directoryPath;//строка хранит директорию
    QString filePath;//строкахранит путь

    QFileSystemModel* fileModel;
    QTableView* tableFileView;

    QChartView* chartView;

    QLabel* pathLabel;//метка, показывающая путь

    QPushButton* btnPrintChart;//кнопка печати
    QPushButton* btnChangeDirectory;//кнопка изменения директории в таблице

    QCheckBox* chkbxBlackWhiteChart;//изменение цвета

    QComboBox* comboboxChartType;//изменение типа

    QHBoxLayout* wrapperLayout;//внешний вид экрана
    QVBoxLayout* fileExplorerLayout;//верстка файлового проводника и его виджетов
    QVBoxLayout* chartLayout;//разметка для диаграммы и виджетов
    QHBoxLayout* chartWidgetLayout;//разметка виджетов диаграммы

    QSplitter* fileSplitter;
    QSplitter* chartSplitter;

    bool isChartActive = false;//переменная, которая хранит активность графика

    void exceptionCall(QString title, QString message);//вызов месседжбокса
    void drawChart();//рисовка графика
};
#endif // MAINWINDOW_H
