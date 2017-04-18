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

namespace model {

class ThermalTimeModel : public AtomicModel < ThermalTimeModel >
{
public:
    enum states { INIT, DEAD, STOCK_AVAILABLE, NO_STOCK };

    enum internals { LIG, STATE, DELTA_T, TT, BOOL_CROSSED_PLASTO, TT_LIG,
                     PLASTO_VISU, LIGULO_VISU, PHENOSTAGE, SLA, DD, EDD, IH };

    enum externals {  PHASE, PLASTO_DELAY, LEAF_LEN, LEAF_PREDIM };


    ThermalTimeModel() {
        //    computed variables
        Internal(LIG, &ThermalTimeModel::_lig);
        Internal(STATE, &ThermalTimeModel::_state);
        Internal(DELTA_T, &ThermalTimeModel::_deltaT);
        Internal(TT, &ThermalTimeModel::_TT);
        Internal(BOOL_CROSSED_PLASTO, &ThermalTimeModel::_boolCrossedPlasto);
        Internal(TT_LIG, &ThermalTimeModel::_TT_lig);
        Internal(PLASTO_VISU, &ThermalTimeModel::_plastoVisu);
        Internal(LIGULO_VISU, &ThermalTimeModel::_liguloVisu);
        Internal(PHENOSTAGE, &ThermalTimeModel::_phenoStage);
        Internal(SLA, &ThermalTimeModel::_sla);
        Internal(DD, &ThermalTimeModel::_DD);
        Internal(EDD, &ThermalTimeModel::_EDD);
        Internal(IH, &ThermalTimeModel::_IH);

        //    external variables
        External(PLASTO_DELAY, &ThermalTimeModel::_plasto_delay);
        External(PHASE, &ThermalTimeModel::_phase);
        External(LEAF_LEN, &ThermalTimeModel::_leafLen);
        External(LEAF_PREDIM, &ThermalTimeModel::_leafPredim);

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
        //ThermalTimeManager
        step_state();

        // lig @TODO
        if (_leafLen >= _leafPredim) {
            _lig = _lig + 1;
        }

        //DeltaT
        _Ta = _parameters.get(t).Temperature;
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

        // SLA
        _sla = _FSLA - _SLAp * std::log(_phenoStage);

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
        _FSLA = _parameters.get < double >("FSLA");
        _SLAp = _parameters.get < double >("SLAp");

        //    computed variables
        _state = INIT;
        _deltaT = 0;
        _TT = 0;
        _boolCrossedPlasto = 0;
        _TT_lig = 0;
        _lig = 0;
        _lig_1 = 0;
        _plastoVisu = _plasto;
        _liguloVisu = _plasto * _coef_ligulo;
        _phenoStage = 1;
        _sla = 0;
        _DD = 0;
        _EDD = 0;
        _IH = 0;
    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters
    double _Tb;
    double _plasto;
    double _coef_ligulo;
    double _FSLA;
    double _SLAp;

    //    parameters(t)
    double _Ta;

    //    internals - computed
    int _state;
    double _deltaT;
    double _TT;
    double _boolCrossedPlasto;
    double _TT_lig;
    double _lig;
    double _lig_1;
    double _plastoVisu;
    double _liguloVisu;
    int _phenoStage;
    double _sla;
    double _DD;
    double _EDD;
    double _IH;

    //    externals
    double _plasto_delay;
    int _phase;
    double _leafLen;
    double _leafPredim;
};

} // namespace model
#endif
