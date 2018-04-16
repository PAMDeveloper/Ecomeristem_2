#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QTextStream>

#include <chrono>
#include <ctime>
#include <qmath.h>

//using namespace artis::kernel;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
//    ui->tableView->horizontalHeader()->setStretchLastSection(true);
//    ui->tableView->horizontalHeader()->hide();
//    ui->tableView->verticalHeader()->hide();

    QScrollArea *scrollArea = new QScrollArea;
    QWidget *client = new QWidget();
    scrollArea->setWidget(client);
    scrollArea->setWidgetResizable(true);

    lay = new QGridLayout();
    client->setLayout(lay);
    lay->setSpacing(0);
    ui->tabWidget->addTab(scrollArea, "Variables");

    settings = new QSettings("CIRAD", "Ecomeristem");
    QString folder = settings->value("simulation_folder", "").toString();
//    if(!folder.isEmpty() && QDir(folder).exists()) {
//        folderName = folder;
//        ui->pathLabel->setText(folder);
//        load_simulation(folder);
//        on_actionLaunch_simulation_triggered();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_trace()
{
//    ui->tableView->setModel(NULL);
    trace_model->setFilters(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->lineEdit_4->text(), ui->checkBox->isChecked());
    ui->tableView->reset();
//    ui->tableView->setModel(trace_model);
}

void MainWindow::addChart(int row, int col,
                          QtCharts::QLineSeries *series, QtCharts::QLineSeries *refSeries,
                          QGridLayout * lay, QString name){
    QColor color = series->color();
    QtCharts::QChart *chart = new QtCharts::QChart();
    series->setColor(color.darker(200));
    if(refSeries != NULL){
        QPen pen;
        pen.setColor(color.lighter(100));
        pen.setWidth(3);
        refSeries->setPen(pen);
        name += " + ref";
        chart->addSeries(refSeries);
    }
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle(name);

    QtCharts::QDateTimeAxis *axisX = new QtCharts::QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("dd-MM");
    chart->addAxis(axisX, Qt::AlignBottom);
    if(refSeries != NULL)
        refSeries->attachAxis(axisX);
    series->attachAxis(axisX);


    bool refBigger = false;
    double maxVal = 0;
    double minVal = 999999999999999;
    for (int i = 0; i < series->count(); ++i) {
        if(series->at(i).y() > maxVal){
            maxVal = series->at(i).y();
            refBigger = false;
        }
        if(series->at(i).y() < minVal){
            minVal = series->at(i).y();
        }
        if(refSeries != NULL) {
            if(refSeries->at(i).y() > maxVal){
                maxVal = refSeries->at(i).y();
                refBigger = true;
            }
            if(refSeries->at(i).y() < minVal){
                minVal = refSeries->at(i).y();
            }
        }
    }

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis;
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setMax(maxVal*1.1 + 0.001);
    axisY->setMin(qMin<double>(-0.001,minVal - (maxVal-minVal)*0.1));
    if(refBigger) {
        if(refSeries != NULL)
            refSeries->attachAxis(axisY);
        series->attachAxis(axisY);
    } else {
        series->attachAxis(axisY);
        if(refSeries != NULL)
            refSeries->attachAxis(axisY);
    }


    ChartView *chartView = new ChartView(chart, series, refSeries, this);
    lay->addWidget(chartView, row, col);
}


QColor getColor(int i){
    double PHI = (1 + qSqrt(5))/2;
    double n = i * PHI - floor(i * PHI);
    int h = qFloor(n * 256);
    return QColor::fromHsv(h, 245, 245, 255);
}


QtCharts::QLineSeries * MainWindow::getSeries(QString fileName, QDate endDate){
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    QList<double> values;
    QFile inputFile(fileName);
    QDate lastDate;
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        in.readLine();
        in.readLine();
        in.readLine();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if(line.isEmpty())
                continue;
            QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            QDate date = QDate::fromString(list[1].split(",")[0], "dd/MM/yyyy");
            lastDate = date;
            if(date < startDate || date > endDate)
                continue;
            bool ok = false;
            double value = list[2].toDouble(&ok);
            QDateTime t = QDateTime(date);
            //          qDebug() << t.toMSecsSinceEpoch() << value;
            series->append(t.toMSecsSinceEpoch(), value);
        }
        inputFile.close();
    }
    if(lastDate < endDate){
        for (int i = 1; i < lastDate.daysTo(endDate); ++i) {
            series->append(QDateTime(lastDate.addDays(i)).toMSecsSinceEpoch(), 0);
        }
    }
    return series;
}

void MainWindow::displayModels(){

    if(ui->parametersTableView->model() != nullptr)
        delete ui->parametersTableView->model();

    if(ui->meteoTableView->model() != nullptr)
        delete ui->meteoTableView->model();

    ParametersDataModel * paramModel = new ParametersDataModel(&parameters);
    ui->parametersTableView->setModel(paramModel);

    MeteoDataModel * meteoModel = new MeteoDataModel(&parameters);
    ui->meteoTableView->setModel(meteoModel);
}

void MainWindow::displayData(observer::PlantView * view,
                             QString dirName,
                             double begin, double end){

//    ui->tabWidget->setCurrentWidget(scrollArea);
    QLayoutItem *item;
    while((item = lay->takeAt(0))) {
        if (item->layout()) {
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }


    startDate = QDate::fromJulianDay(begin);
    QDate endDate = QDate::fromJulianDay(end);
    currentDate = startDate;
    const int numCol = 2;

    QStringList outRefs;
    if(refFolder.isEmpty() && QDir(dirName+"/ref").exists())
        refFolder = dirName+"/ref";

    if(!refFolder.isEmpty()){
        QDir dir(refFolder);
        dir.setFilter(QDir::Files);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            outRefs << fileInfo.fileName().toLower();
        }
    }

    std::map<std::string, std::vector < std::pair < double, std::string > > > m = view->values();
    int j = 0;
    for(auto it = m.begin(); it != m.end(); ++it) {
        QString param = QString::fromStdString(it->first);
        QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
        series->setColor(getColor(j));
        QtCharts::QLineSeries * refSeries;
        QString pCpy = param;
        refSeries = NULL;
        QString refName = pCpy.replace("Plant:","").toLower() +"_out.txt";
        if(outRefs.contains(refName)) {
            refSeries = getSeries(refFolder + "/" + pCpy.replace("Plant:","").toLower() +"_out.txt", endDate);
            outRefs.removeAll(refName);
        }

        for (int i = 0; i < startDate.daysTo(endDate); ++i) {
            double value = view->get(startDate.addDays(i).toJulianDay(),
                                     param.toLocal8Bit().constData());
            QDateTime momentInTime;
            momentInTime.setDate(startDate.addDays(i));
            series->append(momentInTime.toMSecsSinceEpoch(), value);
        }

        addChart(j/numCol,j%numCol,series, refSeries, lay, param);
        j++;
    }
}




void MainWindow::on_lineEdit_returnPressed()
{
    show_trace();
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    show_trace();
}

void MainWindow::on_lineEdit_3_returnPressed()
{
    show_trace();
}

void MainWindow::on_lineEdit_4_returnPressed()
{
    show_trace();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    show_trace();
}

void MainWindow::on_actionSave_trace_triggered()
{
    QString fileName =  QFileDialog::getSaveFileName(this,
                                                     tr("Save trace"), "",
                                                     tr("txt (*.txt)"));
    std::ofstream out(fileName.toStdString());
    out << std::fixed << ::Trace::trace().elements().to_string();
    out.close();

}

void MainWindow::load_simulation(QString folderName) {
    ui->pathLabel->setText(folderName);
    std::string dirName = folderName.toStdString();
    utils::ParametersReader reader;
    reader.loadParametersFromFiles(dirName, parameters);
    displayModels();
}

void MainWindow::on_actionLoad_simulation_triggered()
{
    folderName = QFileDialog::getExistingDirectory(this, "Simulation folder", settings->value("simulation_folder", "").toString());
    if(folderName.isEmpty())
        return;
    settings->setValue("simulation_folder", folderName);
    load_simulation(folderName);
//    auto vobsMap = reader.loadVObsFromFile("D:/PAMStudio_dev/data/ecomeristem/ng/vobs_G1_C_BFF2015.txt");
    //for (int i = 0; i < 200000; ++i) {

}

void MainWindow::on_actionLaunch_simulation_triggered()
{
    load_simulation(settings->value("simulation_folder", "").toString());

    ::Trace::trace().clear();
    GlobalParameters globalParameters;
    EcomeristemContext context(parameters.get("BeginDate"), parameters.get("EndDate"));
    PlantModel * m = new PlantModel;
    EcomeristemSimulator simulator(m, globalParameters);
    observer::PlantView *view = new observer::PlantView();
    simulator.attachView("plant", view);
    simulator.init(parameters.get("BeginDate"), parameters);
    simulator.run(context);

//    ResultParser * parser = new ResultParser();
//    std::map <std::string, std::vector<double> > resultMap = parser->resultsToMap(&simulator);
//    std::vector<double> tillers = resultMap["tillernb_1"];
//    for(auto t: tillers)
//        std::cout << t;

/* TEST */
//    QString folderName = settings->value("simulation_folder", "").toString();
//    std::string dirName = folderName.toStdString();
//    utils::ParametersReader reader;
//    reader.loadParametersFromFiles(dirName, parameters);

//    ::Trace::trace().clear();
//    GlobalParameters globalParameters2;
//    EcomeristemContext context2(parameters.get("BeginDate"), parameters.get("EndDate"));
//    PlantModel * m2 = new PlantModel;
//    EcomeristemSimulator simulator2(m2, globalParameters2);
//    observer::PlantView *view2 = new observer::PlantView();
//    simulator.attachView("plant2", view2);
//    simulator.init(parameters.get("BeginDate"), parameters);
//    simulator.run(context2);

//    ResultParser * parser2 = new ResultParser();
//    std::map <std::string, std::vector<double> > resultMap2 = parser2->resultsToMap(&simulator);
//    std::vector<double> tillers_2 = resultMap2["tillernb_1"];
//    qDebug() << std::equal(tillers.begin(), tillers.end(), tillers_2.begin());
//    for(auto t: tillers_2)
//        std::cout << t;
/*     */

    if(ui->tableView->model() != nullptr)
        delete ui->tableView->model();

    trace_model = new VisibleTraceModel(::Trace::trace().elements());
    ui->tableView->setModel(trace_model);
    show_trace();
    displayData(view, folderName,
                parameters.get("BeginDate"),
                parameters.get("EndDate"));

//    QMessageBox::about(this, "Simulation finished", folderName + " simulation done.");
}


void MainWindow::on_pushButton_clicked()
{
    QString selectedFilter;
    QString filePath = QFileDialog::getSaveFileName(
                this, "Save trace as csv", "" , "csv tab separated (*.csv);;csv semicolon separated (*.csv)",&selectedFilter);
    if(filePath.isEmpty()) return;
    QString sep = (selectedFilter == "csv tab separated (*.csv)" ? "\t" : ";");
    trace_model->save(filePath, sep);
}
