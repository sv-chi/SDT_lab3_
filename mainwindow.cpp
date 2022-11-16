#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IOC.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(100, 100, 1000, 600);
    btnPrintChart = new QPushButton("Print Chart");
    btnChangeDirectory = new QPushButton("Change Directory");
    comboboxChartType = new QComboBox();
    chkbxBlackWhiteChart = new QCheckBox("B/w Chart");
    comboboxChartType->addItem("Pie");
    comboboxChartType->addItem("Bar");

    wrapperLayout = new QHBoxLayout(this);
    fileExplorerLayout = new QVBoxLayout();
    chartLayout = new QVBoxLayout();
    chartWidgetLayout = new QHBoxLayout();

    fileSplitter = new QSplitter();
    chartSplitter = new QSplitter();


    wrapperLayout->addLayout(chartLayout);
    wrapperLayout->addLayout(fileExplorerLayout);

    chartLayout->addLayout(chartWidgetLayout);

    fileExplorerLayout->addWidget(fileSplitter, 1);
    chartLayout->addWidget(chartSplitter);

    chartWidgetLayout->addWidget(btnPrintChart);
    chartWidgetLayout->addWidget(comboboxChartType);
    chartWidgetLayout->addWidget(chkbxBlackWhiteChart);

    // file view setup

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
                tableFileView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
                this, SLOT(fileSelection(const QItemSelection, const QItemSelection))
                );
    connect(comboboxChartType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChartType()));
    connect(chkbxBlackWhiteChart, SIGNAL(toggled(bool)), this, SLOT(colorSwap()));
    connect(btnPrintChart, SIGNAL(clicked()), this, SLOT(printChart()));
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
        exceptionCall("Selection Error", "No items has been Selected");
        return;
    }
    filePath = fileModel->filePath(indexes.first());
    if (filePath.endsWith(".json")) {
        iocContainer.RegisterInstance<IDataStructure, JsonDataStructure>();
    }
    else if (filePath.endsWith(".sqlite")) {
        iocContainer.RegisterInstance<IDataStructure, SqlDataStructure>();
    }
    else {
        exceptionCall("Wrong file format", "Select .json or .sqlite file");
        if (isChartActive) {
            auto chart = iocContainer.GetObject<IChart>();
            chart->getChart()->setTitle("");
            chart->clearChart();
            isChartActive = false;
        }
        return;
    }
    if (comboboxChartType->currentText() == "Pie") {
        iocContainer.RegisterInstance<IChart, PieChart>();
        isChartActive = true;
    }
    else if (comboboxChartType->currentText() == "Bar") {
        iocContainer.RegisterInstance<IChart, BarChart>();
        isChartActive = true;
    }
    if (isChartActive) {
        drawChart();
    }
}

void MainWindow::changeChartType() {
    if (comboboxChartType->currentText() == "Pie") {
        iocContainer.RegisterInstance<IChart, PieChart>();
    }
    else if (comboboxChartType->currentText() == "Bar") {
        iocContainer.RegisterInstance<IChart, BarChart>();
    }
    if (isChartActive) {
        drawChart();
    }
}

void MainWindow::colorSwap() {
    if (isChartActive) drawChart();
}

void MainWindow::printChart() {
    if(isChartActive){
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog-> setWindowTitle (tr ("Сохранить в ..."));
        fileDialog->setDirectory(".");
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->setViewMode(QFileDialog::Detail);
        QStringList fileNames;
        if(fileDialog->exec())
        {
            fileNames = fileDialog->selectedFiles();
        }

        QPdfWriter pdfWriter(fileNames.first() + ".pdf");

        pdfWriter.setCreator("User");

        pdfWriter.setPageSize(QPageSize::A4);

        pdfWriter.setResolution (300);
        QPainter painter(&pdfWriter);

        chartView->render(&painter);
        painter.end();
    }
    else {
        exceptionCall("Pdf error", "No chart to print");
    }
}

void MainWindow::drawChart() {
    auto chart = iocContainer.GetObject<IChart>();
    auto dataStructure = iocContainer.GetObject<IDataStructure>();
    QList<Data> items = dataStructure->getData(filePath);
    chart->recreateChart(items, chkbxBlackWhiteChart->isChecked());
    chartView->setChart(chart->getChart());
}

void MainWindow::exceptionCall(QString title, QString message) {
    QMessageBox *messageBox = new QMessageBox();
    messageBox->setWindowTitle(title);
    messageBox->setText(message);
    messageBox->exec();
}

MainWindow::~MainWindow()
{
}
