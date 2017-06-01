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

    void displayData(observer::PlantView * view, QString dirName,
                     ecomeristem::ModelParameters * parameters,
                     QString begin, QString end);
    void addChart(int row, int col, QLineSeries * series, QLineSeries *refseries,
                  QGridLayout * lay, QString name);

     QLineSeries * getSeries(QString fileName, QDate endDate);

     void show_trace();

 private slots:
     void on_lineEdit_returnPressed();

     void on_lineEdit_2_returnPressed();

     void on_lineEdit_3_returnPressed();

     void on_lineEdit_4_returnPressed();

private:
     Ui::MainWindow *ui;

     VisibleTraceModel * trace_model;

private:
    ecomeristem::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string simulation;
    QString refFolder;
    QDate currentDate;
    QDate startDate;
};

#endif // MAINWINDOW_H
