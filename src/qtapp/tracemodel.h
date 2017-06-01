#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QAbstractTableModel>
#include <artis/utils/Trace.hpp>
#include <artis/utils/DoubleTime.hpp>
enum customRole {
    DATE_ROLE = 20,
    MODEL_ROLE = 21,
    VAR_ROLE = 22,
    INT_VAR_ROLE = 23,
    PHASE_ROLE = 24,
    INT_MODEL_ROLE = 25
};

using namespace artis::utils;
class TraceModel : public QAbstractTableModel
{
public:
    TraceModel(const TraceElements<DoubleTime> & elements, QObject *parent = Q_NULLPTR);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const {return 4;}
    TraceElements<DoubleTime> elements;
};

#include <QSortFilterProxyModel>
#include <QDate>

class VisibleTraceModel : public QSortFilterProxyModel {
public:
    int date_i; int model_i; int var_i;  int phase;
    VisibleTraceModel(const TraceElements<DoubleTime> & elements, QObject *parent = 0);
    bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const;
    void setFilters(QString date, QString model, QString var, QString phase);
};

#endif // TRACEMODEL_H
