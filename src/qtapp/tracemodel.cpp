#include "tracemodel.h"


VisibleTraceModel::VisibleTraceModel(const TraceElements<DoubleTime> & elements, QObject *parent)
: QSortFilterProxyModel(parent)
{
    setSourceModel(new TraceModel(elements));
}


bool VisibleTraceModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    bool accepted = true;
    if(!date.isEmpty()) accepted &= (date_i == index.data(DATE_ROLE).toInt());
    if(!model.isEmpty()) accepted &= (model_i == index.data(MODEL_ROLE).toInt());
    if(!var.isEmpty()) accepted &= (var_i == index.data(VAR_ROLE).toInt() || var_i == index.data(INT_VAR_ROLE).toInt());
    if(phase != -1) accepted &= (phase == index.data(PHASE_ROLE).toInt());
    return accepted;
}

TraceModel::TraceModel(const TraceElements<DoubleTime> & elements, QObject *parent)
    :QAbstractTableModel(parent) {
    this->elements = elements;
}


QVariant TraceModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        return QString::fromStdString(elements[index.row()].to_string(artis::utils::DATE_FORMAT_YMD));
    }
    else if(role == DATE_ROLE) {
        return elements[index.row()].get_time();
    }else if(role == MODEL_ROLE) {
        return elements[index.row()].get_kernel_info().tgt_model_idx();
    }else if(role == VAR_ROLE) {
        return elements[index.row()].get_kernel_info().var_idx();
    }else if(role == INT_VAR_ROLE) {
        return elements[index.row()].get_kernel_info().tgt_internal_var_idx();
    } else if(role == PHASE_ROLE) {
        return (int)elements[index.row()].get_type();
    }
    return QVariant();
}

int TraceModel::rowCount(const QModelIndex &parent) const {
    return elements.size();
}
