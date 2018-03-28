#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QGridLayout>

#include <ModelParameters.hpp>
#include <observer/PlantView.hpp>
#include <utils/ParametersReader.hpp>
#include <artis/utils/DateTime.hpp>
#include <QSettings>
#include <qtapp/tracemodel.h>

#include <QMouseEvent>
#include <QDate>

QT_CHARTS_USE_NAMESPACE

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

    void addChart(int row, int col, QLineSeries * series, QLineSeries *refseries,
                  QGridLayout * lay, QString name);

     QLineSeries * getSeries(QString fileName, QDate endDate);

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
