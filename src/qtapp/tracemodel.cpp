#include "tracemodel.h"

#include <QDebug>
#include <QFile>
#include <utils/juliancalculator.h>

VisibleTraceModel::VisibleTraceModel(const TraceElements<DoubleTime> & elements, QObject *parent)
: QSortFilterProxyModel(parent)
{
    setSourceModel(new TraceModel(elements));
}

bool isNullValue(double value){
    if (value != value){
        return true;
    }
    else if (value >= 2.5e6){
        return true;
    }
    else if (value <= -2.5e6){
        return true;
    }
    else
        return false;
}

bool VisibleTraceModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if(date_i + model_i + var_i + phase == -4 && !null_i)
        return true;
    bool accepted = true;
    if(null_i) accepted &= isNullValue(index.data(VALUE_MODEL_ROLE).toDouble());
    if(date_i != -1) accepted &= (date_i == index.data(DATE_ROLE).toInt());
    if(accepted == false) return accepted;
    if(model_i != -1) accepted &= (model_i == index.data(MODEL_ROLE).toInt() || model_i == index.data(INT_MODEL_ROLE).toInt());
    if(accepted == false) return accepted;
    if(var_i != -1) accepted &= (var_i == index.data(VAR_ROLE).toInt() || var_i == index.data(INT_VAR_ROLE).toInt());
    if(accepted == false) return accepted;
    if(phase != -1) accepted &= (phase == index.data(PHASE_ROLE).toInt());
    return accepted;
}

void  VisibleTraceModel::setFilters(QString date, QString model, QString var, QString phase, bool nullOnly) {
    if(phase.isEmpty()) {
        this->phase = -1;
    } else if(phase.contains(QRegExp("[a-zA-Z]"))) {
        this->phase = -1;
        for (int i = 0; i < TraceTypesStr.size(); ++i) {
            if(TraceTypesStr[i] == phase.toStdString()) {
                this->phase = i;
                break;
            }
        }
    } else {
          this->phase = phase.toInt();
    }
    date_i = date.isEmpty() ? -1 : QDate::fromString(date, "yyyy-MM-dd").toJulianDay();
    model_i = model.isEmpty() ? -1 : KernelInfo::term(model.toStdString());
    var_i = var.isEmpty() ? -1 : KernelInfo::term(var.toStdString());
    null_i = nullOnly;
    invalidate();
}

TraceModel::TraceModel(const TraceElements<DoubleTime> & elements, QObject *parent)
    :QAbstractTableModel(parent) {
    this->elements = elements;
}


QVariant TraceModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        if(index.column() == 0) {
            double time = elements[index.row()].get_time();
//            return time;
            if(time != DoubleTime::null)
                return QString::fromStdString(
                            JulianCalculator::toStringDate(time, JulianCalculator::YMD, '/')
                        );
            return "";
        } else if(index.column() == 1) {
            return QString::fromStdString(
                        TraceTypesStr[elements[index.row()].get_type()]
                    );
        } else if(index.column() == 2) {
            return QString::fromStdString(
                        elements[index.row()].get_model_name()
                    );
        } else if(index.column() == 3) {
            return QString::fromStdString(
                        elements[index.row()].get_kernel_info().to_string()
                    ).remove(0,1);
        }
        return QString::fromStdString(elements[index.row()].to_string(artis::utils::DATE_FORMAT_YMD));
    }
    else if(role == DATE_ROLE) {
        return elements[index.row()].get_time();
    }else if(role == MODEL_ROLE) {
        return elements[index.row()].get_model_name_idx();
    }else if(role == INT_MODEL_ROLE) {
        return elements[index.row()].get_kernel_info().tgt_model_idx();
    }else if(role == VAR_ROLE) {
        return elements[index.row()].get_kernel_info().var_idx();
    }else if(role == INT_VAR_ROLE) {
        return elements[index.row()].get_kernel_info().tgt_internal_var_idx();
    } else if(role == PHASE_ROLE) {
        return (int)elements[index.row()].get_type();
    } else if(role == VALUE_MODEL_ROLE) {
        QStringList l = this->index(index.row(),3).data().toString().split("=");
        double r;
        bool ok;
        if(l.size() < 2) r = 0;
        else {
            r = l.at(1).toDouble(&ok);
            if( (!ok || l.at(1) == "NA") && (l.at(1)!="false") && l.at(1)!="true")
                r = std::numeric_limits<qreal>::max();
        }
        return r;
    }
    return QVariant();
}

QVariant TraceModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "Date";
            break;
        case 1:
            return "Op.";
            break;
        case 2:
            return "Model";
            break;
        case 3:
            return "Var.";
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

int TraceModel::rowCount(const QModelIndex &) const {
    return elements.size();
}

bool VisibleTraceModel::save(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < rowCount(); row++) {
            for (int col = 0; col < columnCount(); col++) {
                out << this->index(row, col).data().toString() << sep;
            }
            out << "\n";
        }
        file.close();
        return true;
    }
    return false;
}
