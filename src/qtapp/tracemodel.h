#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QAbstractTableModel>
#include <artis/utils/Trace.hpp>
#include <artis/utils/DoubleTime.hpp>
enum customRole {
    DATE_ROLE = 101,
    MODEL_ROLE = 102,
    VAR_ROLE = 103,
    INT_VAR_ROLE = 104,
    PHASE_ROLE = 105
};

using namespace artis::utils;
class TraceModel : public QAbstractTableModel
{
public:
    TraceModel(const TraceElements<DoubleTime> & elements, QObject *parent = Q_NULLPTR);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const {return 1;}

    TraceElements<DoubleTime> elements;
};

#include <QSortFilterProxyModel>
#include <QDate>

class VisibleTraceModel : public QSortFilterProxyModel {
public:
    QString date; QString model; QString var; int phase;
    int date_i; int model_i; int var_i;
    VisibleTraceModel(const TraceElements<DoubleTime> & elements, QObject *parent = 0);
    bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const;
    void setFilters(QString date, QString model, QString var, QString phase) {
        this->date = date; this->model = model; this->var = var; this->phase = phase.isEmpty() ? -1 :phase.toInt();
        date_i = QDate::fromString(date, "yyyy-MM-dd").toJulianDay();
        model_i = KernelInfo::term(model.toStdString());
        var_i = KernelInfo::term(var.toStdString());
        invalidateFilter();
    }
};

#endif // TRACEMODEL_H
