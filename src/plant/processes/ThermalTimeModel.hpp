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

namespace model {

class ThermalTimeModel : public AtomicModel < ThermalTimeModel >
{
public:
    enum tt_state { INIT, DEAD, STOCK_AVAILABLE, NO_STOCK };

    enum internals { STATE, DELTA_T, TT, BOOL_CROSSED_PLASTO,
                     PLASTO_VISU, LIGULO_VISU, PHENO_STAGE, SLA, DD, EDD };

    enum externals {  PLANT_STATE, PLASTO_DELAY, PLASTO };


    ThermalTimeModel() {
        //    computed variables
        Internal(STATE, &ThermalTimeModel::_tt_state);
        Internal(DELTA_T, &ThermalTimeModel::_deltaT);
        Internal(TT, &ThermalTimeModel::_TT);
        Internal(BOOL_CROSSED_PLASTO, &ThermalTimeModel::_boolCrossedPlasto);
        Internal(PLASTO_VISU, &ThermalTimeModel::_plastoVisu);
        Internal(LIGULO_VISU, &ThermalTimeModel::_liguloVisu);
        Internal(PHENO_STAGE, &ThermalTimeModel::_phenoStage);
        Internal(SLA, &ThermalTimeModel::_sla);
        Internal(DD, &ThermalTimeModel::_DD);
        Internal(EDD, &ThermalTimeModel::_EDD);

        //    external variables
        External(PLASTO_DELAY, &ThermalTimeModel::_plasto_delay);
        External(PLANT_STATE, &ThermalTimeModel::_plant_state);
        External(PLASTO, &ThermalTimeModel::_plasto);

    }

    virtual ~ThermalTimeModel()
    {}


    void step_state() {
        switch (_tt_state) {
        case INIT: {
            _tt_state = STOCK_AVAILABLE;
            break;
        }
        case DEAD: {
            break;
        }
        case STOCK_AVAILABLE: {
            if (_plant_state & plant::NOGROWTH) {
                _tt_state = NO_STOCK;
            }
            break;
        }
        case NO_STOCK: {
            if (!(_plant_state & plant::NOGROWTH) or (_plant_state & plant::NEW_PHYTOMER)){
                _tt_state = STOCK_AVAILABLE;
            }
            break;
        }};
    }


    void compute(double t, bool /* update */) {
        // parameters
        _Ta = _parameters.get(t).Temperature;

        //ThermalTimeManager
        step_state();

        //DeltaT
        _deltaT = _Ta - _Tb;

        //TT
        _TT = _TT + _deltaT;

        //DD
        if (_tt_state == STOCK_AVAILABLE) {
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

        //PhenoStage
        if (_tt_state == STOCK_AVAILABLE) {
            if (_boolCrossedPlasto >= 0) {
                _phenoStage = _phenoStage + 1;
            }
        }

        // SLA
        _sla = _FSLA - _SLAp * std::log(_phenoStage);

        //PlastoVisu
        if (_tt_state == STOCK_AVAILABLE) {
            _plastoVisu = _plastoVisu - _plasto_delay;
        } else {
            _plastoVisu = _plastoVisu + _EDD;
        }

        //LiguloVisu
        if (_tt_state == STOCK_AVAILABLE) {
            _liguloVisu = _liguloVisu - _plasto_delay;
        } else {
            _liguloVisu = _liguloVisu + _EDD;
        }
    }



    void init(double /*t*/, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;
        //    paramaters variables
        _Tb = _parameters.get < double >("Tb");
        _coef_ligulo = _parameters.get < double >("coef_ligulo1");
        _FSLA = _parameters.get < double >("FSLA");
        _SLAp = _parameters.get < double >("SLAp");
        _plasto_init = _parameters.get < double >("plasto_init");

        //    computed variables
        _tt_state = INIT;
        _deltaT = 0;
        _TT = 0;
        _boolCrossedPlasto = 0;
        _plastoVisu = _plasto_init;
        _liguloVisu = _plasto_init * _coef_ligulo;
        _phenoStage = 1;
        _sla = _FSLA;
        _DD = 0;
        _EDD = 0;
    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters
    double _Tb;
    double _coef_ligulo;
    double _FSLA;
    double _SLAp;
    double _plasto_init;

    //    parameters(t)
    double _Ta;

    //    internals
    int _tt_state;
    double _deltaT;
    double _TT;
    double _boolCrossedPlasto;
    double _plastoVisu;
    double _liguloVisu;
    int _phenoStage;
    double _sla;
    double _DD;
    double _EDD;

    //    externals
    double _plasto;
    double _plasto_delay;
    plant::plant_state _plant_state;
};

} // namespace model
#endif
