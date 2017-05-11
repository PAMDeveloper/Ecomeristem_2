/**
 * @file ecomeristem/leaf/Model.hpp
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

namespace model {

class LeafModel : public AtomicModel < LeafModel >
{
public:
    enum phase_t   { INIT, INITIAL, LIG, NOGROWTH };

    enum internals { LEAF_PHASE, LIFE_SPAN, REDUCTION_LER, LEAF_LEN, LER,
                     EXP_TIME, PLASTO_DELAY, LEAF_PREDIM, WIDTH,
                     TT_LIG, BLADE_AREA, BIOMASS, DEMAND, LAST_DEMAND,
                     REALLOC_BIOMASS, SENESC_DW, SENESC_DW_SUM,
                     TIME_FROM_APP, LIG_T, IS_LIG, IS_LIG_T, OLD_BIOMASS,
                     LAST_LEAF_BIOMASS, SLA_CSTE
                   };

    enum externals { DD, DELTA_T, FTSW, FCSTR,
                     LEAF_PREDIM_ON_MAINSTEM, PREVIOUS_LEAF_PREDIM,
                     SLA, PLANT_PHASE, TEST_IC, MGR };


    virtual ~LeafModel()
    { }

    LeafModel(int index, bool is_on_mainstem, double plasto, double ligulo, double LL_BL) :
        _index(index),
        _is_first_leaf(_index == 1),
        _is_on_mainstem(is_on_mainstem),
        _plasto(plasto),
        _ligulo(ligulo),
        _LL_BL(LL_BL)
    {
        //internals
        Internal(LEAF_PHASE, &LeafModel::_leaf_phase);
        Internal(LEAF_PREDIM, &LeafModel::_predim);
        Internal(LEAF_LEN, &LeafModel::_len);
        Internal(LIFE_SPAN, &LeafModel::_life_span);
        Internal(REDUCTION_LER, &LeafModel::_reduction_ler);
        Internal(LER, &LeafModel::_ler);
        Internal(EXP_TIME, &LeafModel::_exp_time);
        Internal(LEAF_LEN, &LeafModel::_len);
        Internal(PLASTO_DELAY, &LeafModel::_plasto_delay);
        Internal(WIDTH, &LeafModel::_width);
        Internal(TT_LIG, &LeafModel::_TT_Lig);
        Internal(BLADE_AREA, &LeafModel::_blade_area);
        Internal(BIOMASS, &LeafModel::_biomass);
        Internal(REALLOC_BIOMASS, &LeafModel::_realloc_biomass);
        Internal(SENESC_DW, &LeafModel::_senesc_dw);
        Internal(SENESC_DW_SUM, &LeafModel::_senesc_dw_sum);
        Internal(DEMAND, &LeafModel::_demand);
        Internal(LAST_DEMAND, &LeafModel::_last_demand);
        Internal(TIME_FROM_APP, &LeafModel::_time_from_app);
        Internal(LIG_T, &LeafModel::_lig_t);
        Internal(IS_LIG, &LeafModel::_is_lig);
        Internal(IS_LIG_T, &LeafModel::_is_lig_t);
        Internal(OLD_BIOMASS, &LeafModel::_old_biomass);
        Internal(LAST_LEAF_BIOMASS, &LeafModel::_last_leaf_biomass);
        Internal(SLA_CSTE, &LeafModel::_sla_cste);


        //externals
        External(PLANT_PHASE, &LeafModel::_plant_phase);
        External(TEST_IC, &LeafModel::_test_ic);
        External(FCSTR, &LeafModel::_fcstr);
        External(LEAF_PREDIM_ON_MAINSTEM, &LeafModel::_predim_leaf_on_mainstem);
        External(PREVIOUS_LEAF_PREDIM, &LeafModel::_predim_previous_leaf);
        External(FTSW, &LeafModel::_ftsw);
        External(DD, &LeafModel::_dd);
        External(DELTA_T, &LeafModel::_delta_t);
        External(SLA, &LeafModel::_sla);
        External(MGR, &LeafModel::_MGR);
    }



    void compute(double t, bool /* update */)
    {
        _p = _parameters.get(t).P;


        //LifeSpan
        if (t == _first_day) {
            _life_span = _coeffLifespan * std::exp(_mu * _index);
        }

        //LeafPredim
        if (t == _first_day) {
            if (_is_first_leaf and _is_on_mainstem) {
                _predim = _Lef1;
            } else if (not _is_first_leaf and _is_on_mainstem) {
                _predim =  _predim_leaf_on_mainstem + _MGR * _test_ic * _fcstr;
            } else if (_is_first_leaf and not _is_on_mainstem) {
                _predim = 0.5 * (_predim_leaf_on_mainstem + _Lef1) *
                        _test_ic * _fcstr;
            } else {
                _predim = 0.5 * (_predim_leaf_on_mainstem +
                                 _predim_previous_leaf) +
                        _MGR * _test_ic * _fcstr;
            }
        }


        //@TODO Ajouter le calcul avec TestIC dans l'ancienne Génération et pas seulement dans NG
        //ReductionLER
        if (_ftsw < _thresLER) {
            _reduction_ler = std::max(1e-4, ((1. / _thresLER) * _ftsw) *
                                      (1. + (_p * _respLER)));
        } else {
            _reduction_ler = 1. + _p * _respLER;
        }

        //LER
        _ler = _predim * _reduction_ler / (_plasto + _index *
                                           (_ligulo - _plasto));

        //LeafExpTime
        _exp_time = (_predim - _len) / _ler;

        //LeafLen
        if (_first_day == t) {
            _len = _ler * _dd;
        } else {
            if (not (_plant_phase == plant::NOGROWTH or _plant_phase == plant::NOGROWTH3
                     or _plant_phase == plant::NOGROWTH4)) {
                _len = std::min(_predim,
                                _len + _ler * std::min(_delta_t, _exp_time));
            }
        }

        //LeafManager
        step_state();

        //PlastoDelay
        _plasto_delay = std::min(((_delta_t > _exp_time) ? _exp_time :_delta_t)
                                 * (-1. + _reduction_ler), 0.);

        //Width
        _width = _len * _WLR / _LL_BL;

        //ThermalTimeSinceLigulation
        _is_lig_t = false;
        if (not _is_lig) {
            if(_leaf_phase == LeafModel::LIG) {
                _is_lig = true;
                _is_lig_t = true;
                if(_lig_t == 0) {
                    _lig_t = t;
                }
            }
        } else {
            _TT_Lig += _delta_t; //@TODO vérifier si c'est calculé qu'une seule fois
        }

        //BladeArea
        if (not _is_lig || _is_lig_t) {
            _blade_area = _len * _width * _allo_area / _LL_BL;
            if (_is_lig_t) {
                _last_blade_area = _blade_area;
            }
        } else {
            _blade_area = _last_blade_area * (1 - _TT_Lig / _life_span);
        }

        if(t >= _parameters.beginDate + 35){

        }

        //Biomass
        _old_biomass = 0;
        if (_first_day == t) {
            _biomass = (1. / _G_L) * _blade_area / _sla;
            _realloc_biomass = 0;
            _sla_cste = _sla;
            _old_biomass = _biomass;
        } else {
            if (_leaf_phase != LeafModel::NOGROWTH) {
                if (not _is_lig || _is_lig_t) {
                    _old_biomass = _biomass;
                    _biomass = (1. / _G_L) * _blade_area / _sla_cste;
                    _realloc_biomass = 0;
                    if (_is_lig_t) {
                        _last_leaf_biomass = _biomass;
                    }
                } else {
                    _old_biomass = _biomass;
                    _biomass = _last_leaf_biomass * (1. - _TT_Lig / _life_span);
                    double delta_biomass = _old_biomass - _biomass;
                    _realloc_biomass = delta_biomass * _realocationCoeff;
                    _senesc_dw = delta_biomass * (1 - _realocationCoeff);
                    _senesc_dw_sum = _senesc_dw_sum + _senesc_dw;
                }
            }
        }

        //LeafDemand
        if (_first_day == t) {
            _demand = _biomass;
        } else {
            if (not _is_lig) {
                _demand = _biomass - _old_biomass;
            } else {
                _demand = 0;
            }
        }

        // LeafLastDemand @TODO calcul doit être placé avant la mise à jour de _is_lig, solution par _is_lig_t
        if (_is_lig_t) {
            _last_demand = _biomass - _old_biomass;
        } else {
            _last_demand = 0;
        }

        //LeafTimeFromApp
        if (_first_day == t) {
            _time_from_app = _dd;
        } else {
            if (_leaf_phase != LeafModel::NOGROWTH) {
                _time_from_app = _time_from_app + _delta_t;
            }
        }
    }

    //@TODO modifier LIG en flag pour signifier le bool _lig
    void step_state() {
        if (_leaf_phase == LeafModel::INIT) {
            _leaf_phase = LeafModel::INITIAL;
        } else if (_leaf_phase == LeafModel::INITIAL and _len >= _predim) {
            _leaf_phase = LeafModel::LIG;
        } else if (_leaf_phase == LeafModel::LIG and _len < _predim) {
            _leaf_phase = LeafModel::INITIAL;
        } else if (_leaf_phase == LeafModel::INITIAL and
                   (_plant_phase == plant::NOGROWTH or _plant_phase == plant::NOGROWTH3
                    or _plant_phase == plant::NOGROWTH4)) {
            _leaf_phase = LeafModel::NOGROWTH;
        } else if (_leaf_phase == LeafModel::NOGROWTH and
                   (_plant_phase == plant::GROWTH or
                    _plant_phase == plant::NEW_PHYTOMER3)) {
            _leaf_phase = LeafModel::INITIAL;
        }
    }

    void init(double t,
              const ecomeristem::ModelParameters& parameters)
    {
        _parameters = parameters;

        //parameters
        _coeffLifespan = parameters.get < double >("coeff_lifespan");
        _mu = parameters.get < double >("mu");
        _Lef1 = parameters.get < double >("Lef1");
        _thresLER = parameters.get < double >("thresLER");
        _respLER = parameters.get < double >("resp_LER");
        _WLR = parameters.get < double >("WLR");
        _allo_area = parameters.get < double >("allo_area");
        _G_L = parameters.get < double >("G_L");
        _realocationCoeff = parameters.get < double >("realocationCoeff");

        //internals
        _realloc_biomass = 0;
        _first_day = t;
        _life_span = 0;
        _leaf_phase = LeafModel::INIT;
        _predim = 0;
        _reduction_ler = 0;
        _ler = 0;
        _exp_time = 0;
        _len = 0;
        _plasto_delay = 0;
        _width = 0;
        _TT_Lig = 0;
        _is_lig = false;
        _is_lig_t = false;
        _blade_area = 0;
        _biomass = 0;
        _old_biomass = 0;
        _senesc_dw = 0;
        _senesc_dw_sum = 0;
        _demand = 0;
        _last_demand = 0;
        _time_from_app = 0;
        _lig_t = 0;
        _last_blade_area = 0;
        _last_leaf_biomass = 0;
        _sla_cste = 0;
    }

    //    double get_blade_area() const
    //    { return blade_area_model.get_blade_area(); }

private:
    ecomeristem::ModelParameters _parameters;
    // parameters
    double _coeffLifespan;
    double _mu;
    double _Lef1;
    double _thresLER;
    double _respLER;
    double _WLR;
    double _allo_area;
    double _G_L;
    double _realocationCoeff;

    // attributes
    int _index;
    bool _is_first_leaf;
    bool _is_on_mainstem;
    double _plasto;
    double _ligulo;
    double _LL_BL;

    // internal variable
    double _width;
    double _leaf_phase;
    double _predim;
    double _first_day;
    double _life_span;
    double _reduction_ler;
    double _ler;
    double _exp_time;
    double _len;
    double _plasto_delay;
    double _TT_Lig;
    bool   _is_lig;
    bool   _is_lig_t;
    double _blade_area;
    double _biomass;
    double _realloc_biomass;
    double _old_biomass;
    double _senesc_dw;
    double _senesc_dw_sum;
    double _demand;
    double _last_demand;
    double _time_from_app;
    double _sla_cste;
    double _lig_t;
    double _last_blade_area;
    double _last_leaf_biomass;

    // external variables
    double _MGR;
    double _ftsw;
    double _p;
    int _plant_phase;
    double _fcstr;
    double _predim_leaf_on_mainstem;
    double _predim_previous_leaf;
    double _test_ic;
    double _dd;
    double _delta_t;
    double _sla;

};

} // namespace model
