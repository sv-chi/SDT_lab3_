#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
     : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    this->setGeometry(100, 100, 1000, 600);

       btnChangeDirectory = new QPushButton("Change Directory");
       btnPrintChart = new QPushButton("Print Chart");
       chkbxBlackWhiteChart = new QCheckBox("B/w Chart");
       comboboxChartType = new QComboBox();
       comboboxChartType->addItem("Bar");
       comboboxChartType->addItem("Pie");


       wrapperLayout = new QHBoxLayout(this);
       fileExplorerLayout = new QVBoxLayout();
       chartLayout = new QVBoxLayout();
       chartWidgetLayout = new QHBoxLayout();

       fileSplitter = new QSplitter();
       chartSplitter = new QSplitter();

       wrapperLayout->addLayout(fileExplorerLayout);
       wrapperLayout->addLayout(chartLayout);

       chartLayout->addLayout(chartWidgetLayout);

       fileExplorerLayout->addWidget(fileSplitter, 1);
       chartLayout->addWidget(chartSplitter);

       chartWidgetLayout->addWidget(comboboxChartType);
       chartWidgetLayout->addWidget(chkbxBlackWhiteChart);
       chartWidgetLayout->addWidget(btnPrintChart);

       fileModel = new QFileSystemModel(this);
       fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

       directoryPath = QDir::currentPath();
       pathLabel = new QLabel();
       pathLabel->setText(directoryPath);
       QModelIndex pathIndex = fileModel->setRootPath(directoryPath);

       tableFileView = new QTableView();
       tableFileView->setModel(fileModel);
       tableFileView->setRootIndex(pathIndex);

       chartView = new QChartView();
       chartView->setRenderHint(QPainter::Antialiasing);


       fileSplitter->addWidget(tableFileView);
       chartSplitter->addWidget(chartView);
       fileExplorerLayout->addWidget(btnChangeDirectory, 0, Qt::AlignBottom);
       fileExplorerLayout->addWidget(pathLabel, 0, Qt::AlignBottom);
   }

   MainWindow::~MainWindow()
   {
       delete ui;
   }
