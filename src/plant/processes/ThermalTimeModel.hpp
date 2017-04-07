/**
 * @file ecomeristem/plant/thermal-time/Model.hpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef THERMAL_TIME_MODEL_HPP
#define THERMAL_TIME_MODEL_HPP

#include <defines.hpp>
#include <plant/PlantState.hpp>
#include <QDebug>
namespace model {

class ThermalTimeModel : public AtomicModel < ThermalTimeModel >
{
public:
    enum states { INIT, DEAD, STOCK_AVAILABLE, NO_STOCK };

    enum internals { STATE,
                     DELTA_T,
                     TT,
                     BOOL_CROSSED_PLASTO,
                     TT_LIG,
                     PLASTO_VISU,
                     LIGULO_VISU,
                     PHENOSTAGE,
                     DD,
                     EDD,
                     IH
                   };

    enum externals { PHASE, LIG, PLASTO_DELAY };


    ThermalTimeModel() {
        //    computed variables
        Internal(STATE, &ThermalTimeModel::_state);
        Internal(DELTA_T, &ThermalTimeModel::_deltaT);
        Internal(TT, &ThermalTimeModel::_TT);
        Internal(BOOL_CROSSED_PLASTO, &ThermalTimeModel::_boolCrossedPlasto);
        Internal(TT_LIG, &ThermalTimeModel::_TT_lig);
        Internal(PLASTO_VISU, &ThermalTimeModel::_plastoVisu);
        Internal(LIGULO_VISU, &ThermalTimeModel::_liguloVisu);
        Internal(PHENOSTAGE, &ThermalTimeModel::_phenoStage);
        Internal(DD, &ThermalTimeModel::_DD);
        Internal(EDD, &ThermalTimeModel::_EDD);
        Internal(IH, &ThermalTimeModel::_IH);

        //    external variables
        External(PLASTO_DELAY, &ThermalTimeModel::_plasto_delay);
        External(PHASE, &ThermalTimeModel::_phase);
        External(LIG, &ThermalTimeModel::_lig);
    }

    virtual ~ThermalTimeModel()
    {}


    void step_state() {
        switch (_state) {
        case INIT: {
            _state = STOCK_AVAILABLE;
            break;
        }
        case DEAD: {
            break;
        }
        case STOCK_AVAILABLE: {
            if (_phase == PlantState::NOGROWTH or _phase == PlantState::NOGROWTH3 or
                    _phase == PlantState::NOGROWTH4) {
                _state = NO_STOCK;
            }
            break;
        }
        case NO_STOCK: {
            if (_phase == PlantState::GROWTH or _phase == PlantState::NEW_PHYTOMER3){
                _state = STOCK_AVAILABLE;
            }
            break;
        }};
    }


    void compute(double t, bool /* update */) {
        qDebug() << "**************";
        //ThermalTimeManager
        step_state();

        //DeltaT
        _Ta = _parameters.get(t).Temperature;
//        trace_element(t, artis::utils::COMPUTE, boost::lexical_cast<std::string>(_Ta));
        Trace::trace() << TraceElement( path(this), t, artis::utils::COMPUTE)
                << artis::utils::KernelInfo("_Ta", true, boost::lexical_cast<std::string>(_Ta));
        Trace::trace().flush();

//        std::cout << boost::lexical_cast<std::string>(_Ta) << std::endl;
        _deltaT = _Ta - _Tb;

        //TT
        _TT = _TT + _deltaT;

        //DD
        if (_state == STOCK_AVAILABLE) {
            double tempDD = _DD + _deltaT + _plasto_delay;

            _boolCrossedPlasto = tempDD - _plasto;

            if (_boolCrossedPlasto <= 0) {
                _EDD = _deltaT + _plasto_delay;
            } else {
                _EDD = _plasto - _DD;
            }

            if (_boolCrossedPlasto >= 0) {
                _DD = tempDD - _plasto;
            } else {
                _DD = tempDD;
            }
        }

        //TT_Lig
        if (t != _parameters.beginDate) {
            if (_lig_1 == _lig) {
                if (_state == STOCK_AVAILABLE) {
                    _TT_lig = _TT_lig + _EDD;
                }
            } else {
                _TT_lig = 0;
            }
        }

        //PhenoStage
        if (_state == STOCK_AVAILABLE) {
            if (_boolCrossedPlasto >= 0) {
                _phenoStage = _phenoStage + 1;
            }
        }

        //PlastoVisu
        if (_state == STOCK_AVAILABLE) {
            _plastoVisu = _plastoVisu - _plasto_delay;
        } else {
            _plastoVisu = _plastoVisu + _EDD;
        }

        //LiguloVisu
        if (_state == STOCK_AVAILABLE) {
            _liguloVisu = _liguloVisu - _plasto_delay;
        } else {
            _liguloVisu = _liguloVisu + _EDD;
        }

        //IH
        if (_state == STOCK_AVAILABLE) {
            _IH = _lig + std::min(1., _TT_lig / _liguloVisu);
        }

        //Day step
        _lig_1 = _lig;
    }



    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //    paramaters variables
        _Tb = _parameters.get < double >("Tb");
        _coef_ligulo = _parameters.get < double >("coef_ligulo1");
        _plasto = _parameters.get < double >("plasto_init");
        //_TA

        //    computed variables
        _state = INIT;
        _deltaT = 0;
        _TT = 0;
        _boolCrossedPlasto = 0;
        _TT_lig = 0;
        _lig_1 = 0;
        _plastoVisu = _plasto;
        _liguloVisu = _plasto * _coef_ligulo;
        _phenoStage = 1;
        _DD = 0;
        _EDD = 0;
        _IH = 0;

        //    external variables
        _plasto_delay = 0;
        _phase = PlantState::INIT;
        _lig = 0;
    }

private:
    //    parameters variables
    ecomeristem::ModelParameters _parameters;
    double _Tb;
    double _plasto;
    double _coef_ligulo;
    //    temporal parameters variables
    double _Ta;

    //    computed variables
    int _state;
    double _deltaT;
    double _TT;
    double _boolCrossedPlasto;
    double _TT_lig;
    double _lig_1;
    double _plastoVisu;
    double _liguloVisu;
    int _phenoStage;
    double _DD;
    double _EDD;
    double _IH;

    //    external variables
    double _plasto_delay;
    int _phase;
    double _lig;
};

} // namespace model
#endif
