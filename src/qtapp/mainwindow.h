#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSettings>
#include <QFileDialog>
#include <QDir>

#include <defines.hpp>
#include <observer/PlantView.hpp>
#include <utils/ParametersReader.hpp>

#include <qtapp/tracemodel.h>
#include <qtapp/meteodatamodel.h>
#include <qtapp/parametersdatamodel.h>
#include <qtapp/view.h>

#include <QtCharts/QLineSeries>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayModels();

    void displayData(observer::PlantView * view, QString dirName,
                     double begin, double end);

    void addChart(int row, int col, QtCharts::QLineSeries * series, QtCharts::QLineSeries *refseries,
                  QGridLayout * lay, QString name);

     QtCharts::QLineSeries * getSeries(QString fileName, QDate endDate);

     void show_trace();
     void load_simulation(QString folderName);

 private slots:
     void on_lineEdit_returnPressed();
     void on_lineEdit_2_returnPressed();
     void on_lineEdit_3_returnPressed();
     void on_lineEdit_4_returnPressed();
     void on_actionSave_trace_triggered();
     void on_actionLoad_simulation_triggered();
     void on_actionLaunch_simulation_triggered();

     void on_checkBox_stateChanged(int arg1);

     void on_pushButton_clicked();

private:
     Ui::MainWindow *ui;

     VisibleTraceModel * trace_model;

private:
    QSettings * settings;
    ecomeristem::ModelParameters parameters;
    utils::ParametersReader reader;
    QGridLayout * lay;
    std::string simulation;
    QString folderName;
    QString refFolder;
    QDate currentDate;
    QDate startDate;
};

#endif // MAINWINDOW_H
