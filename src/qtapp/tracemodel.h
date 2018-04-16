#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QDate>
#include <defines.hpp>
using namespace artis::utils;

enum customRole {
    DATE_ROLE = 20,
    MODEL_ROLE = 21,
    VAR_ROLE = 22,
    INT_VAR_ROLE = 23,
    PHASE_ROLE = 24,
    INT_MODEL_ROLE = 25,
    VALUE_MODEL_ROLE = 26
};

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


class VisibleTraceModel : public QSortFilterProxyModel {
public:
    int date_i;
    int model_i;
    int var_i;
    int phase;
    bool null_i;
    VisibleTraceModel(const TraceElements<DoubleTime> & elements, QObject *parent = 0);
    bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const;
    void setFilters(QString date, QString model, QString var, QString phase, bool nullOnly);
    bool save(QString path, QString sep);
};

#endif // TRACEMODEL_H
