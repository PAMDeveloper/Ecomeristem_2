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

#define _USE_MATH_DEFINES
#include <math.h>

namespace model {

class InternodeModel : public AtomicModel < InternodeModel >
{
public:
    enum internode_phase {  INITIAL, VEGETATIVE, REALIZATION,
                            REALIZATION_NOGROWTH, MATURITY,
                            MATURITY_NOGROWTH };

    enum internals { INTERNODE_PHASE, INTERNODE_PHASE_1, INTERNODE_PREDIM, INTERNODE_LEN,
                     REDUCTION_INER, INER, EXP_TIME, INTER_DIAMETER,
                     VOLUME, BIOMASS, DEMAND, LAST_DEMAND, TIME_FROM_APP};

    enum externals { PLANT_PHASE, PLANT_STATE, CULM_PHASE, LIG, IS_LIG, LEAF_PREDIM, FTSW,
                     DD, DELTA_T, PLASTO, LIGULO, NB_LIG };

    //    enum internals { BIOMASS, DEMAND, LAST_DEMAND, LEN };
    //    enum externals { DD, DELTA_T, FTSW, P, PHASE, STATE, PREDIM_LEAF,
    //                     LIG };

    InternodeModel(int index, bool is_on_mainstem):
        _index(index),
        _is_on_mainstem(is_on_mainstem)
    {
        Internal(INTERNODE_PHASE, &InternodeModel::_inter_phase);
        Internal(INTERNODE_PHASE_1, &InternodeModel::_inter_phase_1);
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

        External(PLANT_STATE, &InternodeModel::_plant_state);
        External(PLANT_PHASE, &InternodeModel::_plant_phase);
        External(CULM_PHASE, &InternodeModel::_culm_phase);
        External(LIG, &InternodeModel::_lig);
        External(IS_LIG, &InternodeModel::_is_lig);
        External(LEAF_PREDIM, &InternodeModel::_leaf_predim);
        External(FTSW, &InternodeModel::_ftsw);
        External(DD, &InternodeModel::_dd);
        External(DELTA_T, &InternodeModel::_delta_t);
        External(PLASTO, &InternodeModel::_plasto);
        External(LIGULO, &InternodeModel::_ligulo);
        External(NB_LIG, &InternodeModel::_nb_lig);
    }

    virtual ~InternodeModel()
    { }

    void compute(double t, bool /* update */){
        _p = _parameters.get(t).P;

        if ((_culm_phase == culm::ELONG or _culm_phase == culm::PI or _culm_phase == culm::PRE_FLO) and _lig == t and _plant_phase != plant::FLO and _nb_lig > 0 ){
            _cste_ligulo = _ligulo;
            _cste_plasto = _plasto;
        }

        //InternodePredim
        if(_index < _nb_leaf_param2) {
            _LL_BL = _LL_BL_init;
        } else {
            _LL_BL = _LL_BL_init + _slope_LL_BL_at_PI * (_index + 1. - _nb_leaf_param2);
        }
        _inter_predim = std::max(1e-4, _slope_length_IN * (1. - (_coeff_species*(1/_LL_BL))) * _leaf_predim - _leaf_length_to_IN_length);

        //ReductionINER
        if (_ftsw < _thresINER) {
            _reduction_iner = std::max(1e-4, ((1./_thresINER) * _ftsw)  * //@TODO vérifier l'équation
                                       (1. + (_p * _respINER)));
        } else {
            _reduction_iner = 1. + _p * _respINER;
        }

        //INER
        if (_inter_phase == REALIZATION) {
            _iner = _inter_predim * _reduction_iner / (_cste_plasto + _index * (_cste_ligulo - _cste_plasto));
        } else {
            _iner = _inter_predim * _reduction_iner / (_cste_plasto + _index * (_ligulo - _plasto));
        }
        //InternodeManager
        step_state(t);

        //InternodeLen & InternodeExpTime
        if (_inter_phase == VEGETATIVE) {
            _inter_len = 0;
            _exp_time = 0;
        } else {
            if (_inter_phase_1 == VEGETATIVE and _inter_phase == REALIZATION) {
                _inter_len = _iner * _dd;
                _exp_time = (_inter_predim - _inter_len) / _iner;
            } else {
                if (!(_plant_state & plant::NOGROWTH) and (_plant_phase == plant::ELONG or _plant_phase == plant::PI or _plant_phase == plant::PRE_FLO or _plant_phase == plant::FLO)) {
                    _exp_time = (_inter_predim - _inter_len) / _iner;
                    _inter_len = std::min(_inter_predim, _inter_len + _iner * std::min(_delta_t, _exp_time));
                }
            }
        }

        //DiameterPredim
        _inter_diameter = _IN_length_to_IN_diam * _inter_predim + _coef_lin_IN_diam;

        //Volume
        double radius = _inter_diameter / 2;
        _inter_volume = _inter_len * M_PI * radius * radius;

        //Biomass
        double biomass_1 = _biomass;
        _biomass = _inter_volume * _density;

        //InternodeDemand & InternodeLastDemand
        _last_demand = 0;
        if (_inter_len >= _inter_predim) {
            _demand = 0;
            if (! _is_mature) {
                _last_demand = _biomass - biomass_1;
                _is_mature = true;
            }
        } else {
            _demand = _biomass - biomass_1;
        }

        //InternodeTimeFromApp
        if (_first_day == t) {
            _time_from_app = _dd;
        } else {
            if (!(_plant_state & plant::NOGROWTH)) {
                _time_from_app = _time_from_app + _delta_t;
            }
        }
    }

    void step_state(double t) {
        _inter_phase_1 = _inter_phase;

        switch (_inter_phase) {
        case INITIAL:
            _inter_phase = VEGETATIVE;
            break;
        case VEGETATIVE:
            //@TODO : virer la condition nb_lig, erreur dans delphi
            if((_culm_phase == culm::ELONG or _culm_phase == culm::PI or _culm_phase == culm::PRE_FLO) and _lig == t and _plant_phase != plant::FLO and _nb_lig > 0) {
                _inter_phase = REALIZATION;
            }
            break;
        case REALIZATION:
            if(_inter_len >= _inter_predim) {
                _inter_phase = MATURITY;
            }
            break;
        }
    }

    void init(double t,
              const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;
        //parameters
        _slope_length_IN = parameters.get < double >("slope_length_IN");
        _leaf_length_to_IN_length = parameters.get < double >("leaf_length_to_IN_length");
        _LL_BL_init = parameters.get < double >("LL_BL_init");
        _nb_leaf_param2 = parameters.get < double >("nb_leaf_param2");
        _slope_LL_BL_at_PI = parameters.get < double >("slope_LL_BL_at_PI");
        _thresINER = parameters.get < double >("thresINER");
        _respINER = parameters.get < double >("resp_LER");
        _slopeINER = parameters.get < double >("slopeINER");
        _IN_length_to_IN_diam =
                parameters.get < double >("IN_length_to_IN_diam");
        _coef_lin_IN_diam = parameters.get < double >("coef_lin_IN_diam");
        _density = parameters.get < double >("density_IN");
        _coeff_species = parameters.get <double> ("coeff_species");

        //internals
        _inter_phase = INITIAL;
        _inter_phase_1 = INITIAL;
        _inter_len = 0;
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
        _inter_predim = 0;
        _is_mature = false;
        _cste_ligulo = 0;
        _cste_plasto = 0;
    }

private:
    ecomeristem::ModelParameters _parameters;
    // attributes
    int _index;
    bool _is_first_internode;
    bool _is_on_mainstem;

    // parameters
    double _LL_BL_init;
    double _nb_leaf_param2;
    double _slope_LL_BL_at_PI;
    double _slope_length_IN;
    double _leaf_length_to_IN_length;
    double _thresINER;
    double _respINER;
    double _slopeINER;
    double _IN_length_to_IN_diam;
    double _coef_lin_IN_diam;
    double _density;
    double _coeff_species;

    // internals
    double _LL_BL;
    internode_phase _inter_phase;
    internode_phase _inter_phase_1;
    double _inter_len;
    double _inter_predim;
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
    bool _is_mature;
    double _cste_ligulo;
    double _cste_plasto;

    // externals
    plant::plant_state _plant_state;
    plant::plant_phase _plant_phase;
    culm::culm_phase _culm_phase;
    double _leaf_predim;
    double _lig;
    bool _is_lig;
    double _ftsw;
    double _p;
    double _dd;
    double _delta_t;
    double _ligulo;
    double _plasto;
    double _nb_lig;

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
