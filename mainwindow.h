#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

    QString directoryPath;

    QFileSystemModel* fileModel;
    QTableView* tableFileView;

    QChartView* chartView;

    QLabel* pathLabel;

    QPushButton* btnPrintChart;
    QPushButton* btnChangeDirectory;

    QCheckBox* chkbxBlackWhiteChart;

    QComboBox* comboboxChartType;

    QHBoxLayout* wrapperLayout;
    QVBoxLayout* fileExplorerLayout;
    QVBoxLayout* chartLayout;
    QHBoxLayout* chartWidgetLayout;

    QSplitter* fileSplitter;
    QSplitter* chartSplitter;
};
#endif // MAINWINDOW_H
