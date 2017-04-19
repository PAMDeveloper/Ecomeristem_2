/**
 * @file ecomeristem/internode/Model.hpp
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

#include <defines.hpp>
#include <plant/PlantState.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace model {

class InternodeModel : public CoupledModel < InternodeModel >
{
public:
    enum internode_phase {  INIT, VEGETATIVE, REALIZATION,
                            REALIZATION_NOGROWTH, MATURITY,
                            MATURITY_NOGROWTH };

    enum internals { INTERNODE_PHASE, INTERNODE_PREDIM, INTERNODE_LEN,
                     REDUCTION_INER, INER, EXP_TIME, INTER_DIAMETER,
                     VOLUME, BIOMASS, DEMAND, LAST_DEMAND, TIME_FROM_APP};
    enum externals { PLANT_PHASE, PLANT_STATE, LIG, LEAF_PREDIM, FTSW, P,
                     DD, DELTA_T};

    //    enum internals { BIOMASS, DEMAND, LAST_DEMAND, LEN };
    //    enum externals { DD, DELTA_T, FTSW, P, PHASE, STATE, PREDIM_LEAF,
    //                     LIG };

    InternodeModel(int index, bool is_on_mainstem) {
        Internal(INTERNODE_PHASE, &InternodeModel::_inter_phase);
        Internal(INTERNODE_PREDIM, &InternodeModel::_inter_predim);
        Internal(INTERNODE_LEN, &InternodeModel::_inter_len);
        Internal(REDUCTION_INER, &InternodeModel::_reduction_iner);
        Internal(INER, &InternodeModel::_iner);
        Internal(EXP_TIME, &InternodeModel::_exp_time);
        Internal(INTER_DIAMETER, &InternodeModel::_inter_diameter);
        Internal(VOLUME, &InternodeModel::_inter_volume);
        Internal(BIOMASS, &InternodeModel::_biomass);
        Internal(DEMAND, &InternodeModel::_demand);
        Internal(LAST_DEMAND, &InternodeModel::_last_demand);
        Internal(TIME_FROM_APP, &InternodeModel::_time_from_app);

        External(PLANT_PHASE, &InternodeModel::_plant_phase);
        External(PLANT_STATE, &InternodeModel::_plant_state);
        External(LIG, &InternodeModel::_lig);
        External(LEAF_PREDIM, &InternodeModel::_leaf_predim);
        External(FTSW, &InternodeModel::_ftsw);
        External(P, &InternodeModel::_p);
        External(DD, &InternodeModel::_dd);
        External(DELTA_T, &InternodeModel::_delta_t);
    }

    virtual ~InternodeModel()
    { }

    void compute(double t, bool /* update */){

        //InternodePredim
        if (_index - 1 - _nb_leaf_param2 < 0) {
            _LL_BL = _LL_BL_init;
        } else {
            _LL_BL = _LL_BL_init + _slope_LL_BL_at_PI * (_index - 1 -
                                                         _nb_leaf_param2);
        }
        _inter_predim = std::max(1e-4, _slope_length_IN *
                                 _leaf_predim - _leaf_length_to_IN_length);

        //ReductionINER
        if (_ftsw < _thresINER) {
            _reduction_iner = std::max(1e-4, (1. - (_thresINER - _ftsw) *
                                              _slopeINER) *
                                       (1. + (_p * _respINER)));
        } else {
            _reduction_iner = 1. + _p * _respINER;
        }

        //INER
        _iner = _inter_predim * _reduction_iner / (_plasto + _index *
                                                   (_ligulo - _plasto));

        //
        if (_inter_phase == VEGETATIVE) {

        } else {
            if (_inter_phase_1 == VEGETATIVE and
                _inter_phase == REALIZATION) {
                _exp_time = (_inter_predim - _inter_len) / _iner;
            } else {

            }
        }

        //InternodeLen & InternodeExpTime
        double inter_len_1 = _inter_len; //@TODO vérif sur Delphi l'ordre des calculs sur exptime
        if (_inter_phase == VEGETATIVE) {
            _inter_len = 0;
            _exp_time = 0;
        } else {
            if (_inter_phase_1 == VEGETATIVE and
                    _inter_phase == REALIZATION) {
                _inter_len = _iner * _dd;
                _exp_time = (_inter_predim - _inter_len) / _iner;
            } else {
                if (_inter_phase != REALIZATION_NOGROWTH and
                        _inter_phase != MATURITY_NOGROWTH) {
                    _inter_len = std::min(_inter_predim,
                                          _inter_len + _iner * std::min(_delta_t,
                                                                        _exp_time));
                    _exp_time = (_inter_predim - inter_len_1) / _iner;
                }
            }
        }

        //InternodeManager
        step_state(t);

        //DiameterPredim
        _inter_diameter = _IN_length_to_IN_diam * _inter_predim + _coef_lin_IN_diam;

        //Volume
        double radius = _inter_diameter / 2;
        _inter_volume = _inter_len * M_PI * radius * radius;

        //Biomass
        double biomass_1 = _biomass;
        _biomass = _inter_volume * _density;

        //InternodeDemand & InternodeLastDemand
        _last_demand = _demand; //@TODO check pourquoi le calcul est le même dans LastDemand et Demand
        if (_inter_phase == MATURITY or
            _inter_phase == MATURITY_NOGROWTH) {
            _demand = 0;
        } else {
            _demand = _biomass - biomass_1;
        }

        //InternodeTimeFromApp
        if (_first_day == t) {
            _time_from_app = _dd;
        } else {
            if (_inter_phase != REALIZATION_NOGROWTH) {
                _time_from_app = _time_from_app + _delta_t;
            }
        }
    }

    void step_state(double t) {
        _inter_phase_1 = _inter_phase;
        if (_inter_phase == INIT) {
            _inter_phase = VEGETATIVE;
        } else if (_inter_phase == VEGETATIVE and
                   _plant_state == PlantState::ELONG and _lig == t) {
            _inter_phase = REALIZATION;
        } else if (_inter_phase == REALIZATION and _inter_len >= _inter_predim) {
            _inter_phase = MATURITY;
        } else if (_inter_phase == REALIZATION and
                   (_plant_phase == PlantState::NOGROWTH or _plant_phase == PlantState::NOGROWTH3
                    or _plant_phase == PlantState::NOGROWTH4)) {
            _inter_phase = REALIZATION_NOGROWTH;
        } else if (_inter_phase == REALIZATION_NOGROWTH and
                   (_plant_phase == PlantState::GROWTH or
                    _plant_phase == PlantState::NEW_PHYTOMER3)) {
            _inter_phase = REALIZATION;
        } else if (_inter_phase == MATURITY and
                   (_plant_phase == PlantState::NOGROWTH or _plant_phase == PlantState::NOGROWTH3
                    or _plant_phase == PlantState::NOGROWTH4)) {
            _inter_phase = MATURITY_NOGROWTH;
        } else if (_inter_phase == MATURITY_NOGROWTH and
                   (_plant_phase == PlantState::GROWTH or
                    _plant_phase == PlantState::NEW_PHYTOMER3)) {
            _inter_phase = MATURITY;
        }
    }

    void init(double t,
              const ecomeristem::ModelParameters& parameters) {

        //parameters
        _LL_BL_init = parameters.get < double >("LL_BL_init");
        _slope_LL_BL_at_PI = parameters.get < double >("slope_LL_BL_at_PI");
        _nb_leaf_param2 = parameters.get < double >("nb_leaf_param2");
        _slope_length_IN = parameters.get < double >("slope_length_IN");
        _leaf_length_to_IN_length = parameters.get < double >("leaf_length_to_IN_length");
        _thresINER = parameters.get < double >("thresINER");
        _respINER = parameters.get < double >("resp_LER");
        _slopeINER = parameters.get < double >("slopeINER");
        _coef_ligulo = parameters.get < double >("coef_ligulo1");
        _plasto = parameters.get < double >("plasto_init");
        _ligulo = _plasto * _coef_ligulo;
        _IN_length_to_IN_diam =
            parameters.get < double >("IN_length_to_IN_diam");
        _coef_lin_IN_diam = parameters.get < double >("coef_lin_IN_diam");
        _density = parameters.get < double >("density_IN");

        //internals
        _inter_phase = INIT;
        _inter_phase_1 = INIT;
        _inter_len = 0;
        _inter_predim = 0;
        _LL_BL = 0;
        _reduction_iner = 0;
        _iner = 0;
        _exp_time = 0;
        _inter_diameter = 0;
        _inter_volume = 0;
        _biomass = 0;
        _demand = 0;
        _last_demand = 0;
        _first_day = t;
        _time_from_app = 0;
    }

private:

    // attributes
    int _index;
    bool _is_first_internode;
    bool _is_on_mainstem;

    // parameters
    double _LL_BL_init;
    double _slope_LL_BL_at_PI;
    double _nb_leaf_param2;
    double _slope_length_IN;
    double _leaf_length_to_IN_length;
    double _thresINER;
    double _respINER;
    double _slopeINER;
    double _coef_ligulo;
    double _ligulo;
    double _plasto;
    double _IN_length_to_IN_diam;
    double _coef_lin_IN_diam;
    double _density;

    // internals
    double _inter_phase;
    double _inter_phase_1;
    double _inter_len;
    double _inter_predim;
    double _LL_BL;
    double _reduction_iner;
    double _iner;
    double _exp_time;
    double _inter_diameter;
    double _inter_volume;
    double _biomass;
    double _demand;
    double _last_demand;
    double _time_from_app;
    double _first_day; //@TODO unused

    // externals
    double _plant_phase;
    double _plant_state;
    double _leaf_predim;
    double _lig;
    double _ftsw;
    double _p;
    double _dd;
    double _delta_t;


    //// external variables
    //    double _dd;
    //    double _delta_t;
    //    double _ftsw;
    //    double _p;
    //    double _phase;
    //    double _state;
    //    double _test_ic;
    //    double _predim_leaf;
    //    double _lig;
};

} // namespace model
