#include "tracemodel.h"

TraceModel::TraceModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}


void TraceModel::setElements(const TraceElements<DoubleTime> & elements) {
    beginResetModel();
    this->elements = elements;
    endResetModel();
}

QVariant TraceModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        QString result;
        for (int i = 0; i < 10; ++i) {
            if(index.row()*10 + i < elements.size()) {
                result += QString::fromStdString(elements[index.row()*10 + i].to_string(artis::utils::DATE_FORMAT_YMD));
                result += "\n";
            }
        }
        return result;

    }
    return QVariant();
}

int TraceModel::rowCount(const QModelIndex &parent) const {
    return (int)(elements.size() / 10);
}
