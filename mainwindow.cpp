#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IOC.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(100, 100, 1000, 600);
    //Создаём виджеты
    btnPrintChart = new QPushButton("Print Chart");
    btnChangeDirectory = new QPushButton("Change Directory");
    comboboxChartType = new QComboBox();
    chkbxBlackWhiteChart = new QCheckBox("B/W");
    //Добавляем виды графиков(круговая и столбчатая)
    comboboxChartType->addItem("Pie");
    comboboxChartType->addItem("Bar");
    //определяем внешний вид
    wrapperLayout = new QHBoxLayout(this);//расположение внешнее
    fileExplorerLayout = new QVBoxLayout();//расположение для обзора
    chartLayout = new QVBoxLayout();//расположение для графиков
    chartWidgetLayout = new QHBoxLayout();//расположение для виджетов графика

    fileSplitter = new QSplitter();
    chartSplitter = new QSplitter();
    //добавляем расположения
    wrapperLayout->addLayout(chartLayout);
    wrapperLayout->addLayout(fileExplorerLayout);

    chartLayout->addLayout(chartWidgetLayout);

    fileExplorerLayout->addWidget(fileSplitter, 1);
    chartLayout->addWidget(chartSplitter);

    chartWidgetLayout->addWidget(btnPrintChart);
    chartWidgetLayout->addWidget(comboboxChartType);
    chartWidgetLayout->addWidget(chkbxBlackWhiteChart);

    // настройка просмотра файла
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    //устанавливаем текущую папку для просмотра
    directoryPath = QDir::currentPath();
    pathLabel = new QLabel();
    //устанавливаем в метку директорию
    pathLabel->setText(directoryPath);
    QModelIndex pathIndex = fileModel->setRootPath(directoryPath);

    tableFileView = new QTableView();
    tableFileView->setModel(fileModel);
    tableFileView->setRootIndex(pathIndex);
    //только для одного элемента выборка в таблице
    tableFileView->setSelectionMode(QAbstractItemView::SingleSelection);
    //настройка просмотра диаграммы
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    //расположение
    fileSplitter->addWidget(tableFileView);
    chartSplitter->addWidget(chartView);
    fileExplorerLayout->addWidget(btnChangeDirectory, 0, Qt::AlignBottom);
    fileExplorerLayout->addWidget(pathLabel, 0, Qt::AlignBottom);
    //подключаем слоты
    connect(btnChangeDirectory, SIGNAL(clicked(bool)), this, SLOT(changeDirectory()));//подключение изменения директории

    connect(
                tableFileView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),//подключение изменений выбранного файла
                this, SLOT(fileSelection(const QItemSelection, const QItemSelection))
                );
    connect(comboboxChartType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChartType()));//изменение типа графика
    connect(chkbxBlackWhiteChart, SIGNAL(toggled(bool)), this, SLOT(colorSwap()));//изменение цвета графика
    connect(btnPrintChart, SIGNAL(clicked()), this, SLOT(printChart()));//печать графика
}

void MainWindow::changeDirectory() {
    QFileDialog dialogWindow(this);//диалоговое окно
    dialogWindow.setFileMode(QFileDialog::Directory);//вид папок
    if (dialogWindow.exec()) {//если открылось, то
        directoryPath = dialogWindow.selectedFiles().first();//обновляем путь
        pathLabel->setText(directoryPath);
    }
    tableFileView->setRootIndex(fileModel->setRootPath(directoryPath));
}

void MainWindow::fileSelection(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);
//если индексов будет меньше одного, то вызываем ошибку
    QModelIndexList indexes = selected.indexes();
    if (indexes.count() < 1) {
        exceptionCall("Selection Error", "No items has been Selected");
        return;
    }
    filePath = fileModel->filePath(indexes.first());//получили выбранный путь до файла
    if (filePath.endsWith(".json")) { //если .json
        iocContainer.RegisterInstance<IDataStructure, JsonDataStructure>();
    }
    else if (filePath.endsWith(".sqlite")) { //если .sqlite
        iocContainer.RegisterInstance<IDataStructure, SqlDataStructure>();
    }
    else {//если формат не такой, то у нас появится ошибка
        exceptionCall("Wrong file format", "Select .json or .sqlite file");
        if (isChartActive) {//если график активный, то очищаем его
            auto chart = iocContainer.GetObject<IChart>();
            chart->getChart()->setTitle("");
            chart->clearChart();
            isChartActive = false;
        }
        return;
    }
    if (comboboxChartType->currentText() == "Pie") {//проверка типа графика
        iocContainer.RegisterInstance<IChart, PieChart>();
        isChartActive = true;
    }
    else if (comboboxChartType->currentText() == "Bar") {
        iocContainer.RegisterInstance<IChart, BarChart>();
        isChartActive = true;
    }
    if (isChartActive) {
        drawChart();//отрисовываем график
    }
}

void MainWindow::changeChartType() {
    if (comboboxChartType->currentText() == "Pie") {//создаем круговой график
        iocContainer.RegisterInstance<IChart, PieChart>();
    }
    else if (comboboxChartType->currentText() == "Bar") {//создаем столбчатую диаграмму
        iocContainer.RegisterInstance<IChart, BarChart>();
    }
    if (isChartActive) {
        drawChart();//отрисовываем
    }
}

void MainWindow::colorSwap() {
    if (isChartActive) drawChart();//если график активный, то рисуем его
}

void MainWindow::printChart() {
    if(isChartActive){//если график активен, то можем печатать график
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog-> setWindowTitle (tr ("Сохранить в ..."));//заголовок
        fileDialog->setDirectory(".");//путь по умолчанию
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->setViewMode(QFileDialog::Detail);//устанавливаем режим просмотра
        QStringList fileNames;
        if(fileDialog->exec())
        {
            fileNames = fileDialog->selectedFiles();
        }

        QPdfWriter pdfWriter(fileNames.first() + ".pdf");//название

        pdfWriter.setCreator("User");

        pdfWriter.setPageSize(QPageSize::A4);//размер страницы

        pdfWriter.setResolution (300);//разрешение
        QPainter painter(&pdfWriter);//добавление контента

        chartView->render(&painter);//рисуем
        painter.end();
    }
    else {
        exceptionCall("Pdf error", "No chart to print");//ошибка, если график не активен
    }
}

void MainWindow::drawChart() {
    auto chart = iocContainer.GetObject<IChart>();//используем ioc обращение к интерфейсам
    auto dataStructure = iocContainer.GetObject<IDataStructure>();//используем ioc обращение к интерфейсам
    QList<Data> items = dataStructure->getData(filePath);//получили данные
    chart->recreateChart(items, chkbxBlackWhiteChart->isChecked());//рисуем
    chartView->setChart(chart->getChart());//получаем представление графика
}

void MainWindow::exceptionCall(QString title, QString message) {
    QMessageBox *messageBox = new QMessageBox();
    messageBox->setWindowTitle(title);//устанавливаем заголовок в месседж боксе
    messageBox->setText(message);//устанавливаем сообщение
    messageBox->exec();//открываем
}

MainWindow::~MainWindow()
{
}
