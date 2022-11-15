#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
     : QWidget(parent)
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

       tableFileView->setSelectionMode(QAbstractItemView::SingleSelection);

       chartView = new QChartView();
       chartView->setRenderHint(QPainter::Antialiasing);


       fileSplitter->addWidget(tableFileView);
       chartSplitter->addWidget(chartView);
       fileExplorerLayout->addWidget(btnChangeDirectory, 0, Qt::AlignBottom);
       fileExplorerLayout->addWidget(pathLabel, 0, Qt::AlignBottom);

       connect(btnChangeDirectory, SIGNAL(clicked(bool)), this, SLOT(changeDirectory()));
           connect(
                       tableFileView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                       this, SLOT(fileSelection(const QItemSelection &, const QItemSelection &))
                       );
       }

       void MainWindow::changeDirectory() {
           QFileDialog dialogWindow(this);
           dialogWindow.setFileMode(QFileDialog::Directory);
           if (dialogWindow.exec()) {
               directoryPath = dialogWindow.selectedFiles().first();
               pathLabel->setText(directoryPath);
           }
           tableFileView->setRootIndex(fileModel->setRootPath(directoryPath));
       }

       void MainWindow::fileSelection(const QItemSelection &selected, const QItemSelection &deselected) {
           Q_UNUSED(deselected);

           QModelIndexList indexes = selected.indexes();
           if (indexes.count() < 1) {
               exceptionCall("Error", "Select item");
               return;
           }
           QString filePath = fileModel->filePath(indexes.first());
           if (filePath.endsWith(".json")) {
               auto* json = new JsonDataStructure();
               json->getData(filePath);
           }
           else if (filePath.endsWith(".sqlite")) {
               auto* sql = new SqlDataStructure();
               sql->getData(filePath);
           }
           else {
               exceptionCall("Wrong file format", "Select .json or .sqlite file");
               return;
           }
       }

       void MainWindow::exceptionCall(QString title, QString message) {
           QMessageBox *messageBox = new QMessageBox();
           messageBox->setWindowTitle(title);
           messageBox->setText(message);
           messageBox->exec();
   }

   MainWindow::~MainWindow() {}
