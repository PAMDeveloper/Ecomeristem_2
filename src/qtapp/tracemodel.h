#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QAbstractTableModel>
#include <artis/utils/Trace.hpp>
#include <artis/utils/DoubleTime.hpp>
using namespace artis::utils;
class TraceModel : public QAbstractTableModel
{
public:
    TraceModel(QObject *parent = Q_NULLPTR);
    void setElements(const TraceElements<DoubleTime> & elements);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const {return 1;}


    TraceElements<DoubleTime> elements;
};

#endif // TRACEMODEL_H
