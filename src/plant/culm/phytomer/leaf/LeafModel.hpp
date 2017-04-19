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
#include <plant/PlantState.hpp>

namespace model {

class LeafModel : public AtomicModel < LeafModel >
{
public:
    enum phase_t   { INIT, INITIAL, LIG, NOGROWTH };

    enum internals { LEAF_PHASE, LIFE_SPAN, REDUCTION_LER, LEN, LER,
                     EXP_TIME, PLASTO_DELAY, PREDIM, WIDTH,
                     TT_LIG, BLADE_AREA, CORRECTED_BLADE_AREA,
                     BIOMASS, DEMAND, LAST_DEMAND,
                     REALLOC_BIOMASS, SENESC_DW, SENESC_DW_SUM,
                     CORRECTED_BIOMASS, TIME_FROM_APP,
                     LIG_T, IS_LIG
                   };

    enum externals { DD, DELTA_T, FTSW, FCSTR, P, PHENO_STAGE,
                     PREDIM_LEAF_ON_MAINSTEM, PREDIM_PREVIOUS_LEAF,
                     SLA, GROW, PLANT_PHASE, STATE, STOP, TEST_IC };


    virtual ~LeafModel()
    { }

    LeafModel::LeafModel(int index, bool is_on_mainstem) :
        _index(index),
        _is_first_leaf(_index == 1),
        _is_on_mainstem(is_on_mainstem)
    {
        //internals
        Internal(LEAF_PHASE, &LeafModel::_leaf_phase);
        Internal(PREDIM, &LeafModel::_predim);
        Internal(LEN, &LeafModel::_len);
        Internal(LIFE_SPAN, &LeafModel::_life_span);
        Internal(REDUCTION_LER, &LeafModel::_reduction_ler);
        Internal(LER, &LeafModel::_ler);
        Internal(EXP_TIME, &LeafModel::_exp_time);
        Internal(LEN, &LeafModel::_len);
        Internal(PLASTO_DELAY, &LeafModel::_plasto_delay);
        Internal(WIDTH, &LeafModel::_width);
        Internal(TT_LIG, &LeafModel::_TT_Lig);
        Internal(BLADE_AREA, &LeafModel::_blade_area);
        Internal(CORRECTED_BLADE_AREA, &LeafModel::_corrected_blade_area);
        Internal(BIOMASS, &LeafModel::_biomass);
        Internal(REALLOC_BIOMASS, &LeafModel::_realloc_biomass);
        Internal(SENESC_DW, &LeafModel::_senesc_dw);
        Internal(SENESC_DW_SUM, &LeafModel::_senesc_dw_sum);
        Internal(CORRECTED_BIOMASS, &LeafModel::_corrected_biomass);
        Internal(DEMAND, &LeafModel::_demand);
        Internal(TIME_FROM_APP, &LeafModel::_time_from_app);
        Internal(LIG_T, &LeafModel::_lig_t);
        Internal(IS_LIG, &LeafModel::_is_lig);

        //externals
        External(PLANT_PHASE, &LeafModel::_plant_phase);
        External(TEST_IC, &LeafModel::_test_ic);
        External(FCSTR, &LeafModel::_fcstr);
        External(PREDIM_LEAF_ON_MAINSTEM, &LeafModel::_predim_leaf_on_mainstem);
        External(PREDIM_PREVIOUS_LEAF, &LeafModel::_predim_previous_leaf);
        External(FTSW, &LeafModel::_ftsw);
        External(P, &LeafModel::_p);
        External(DD, &LeafModel::_dd);
        External(DELTA_T, &LeafModel::_delta_t);
        External(GROW, &LeafModel::_grow);
        External(SLA, &LeafModel::_sla);
    }



    void LeafModel::compute(double t, bool /* update */)
    {
        //LeafManager
        step_state();

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
        if (_is_first_leaf and _is_on_mainstem) {
            _exp_time = _predim / _ler;
            _is_first_leaf = false;
        } else {
            _exp_time = (_predim - _len) / _ler;
        }

        //LeafLen
        if (_first_day == t) {
            _len = _ler * _dd;
        } else {

            if (not (_plant_phase == PlantState::NOGROWTH or _plant_phase == PlantState::NOGROWTH3
                     or _plant_phase == PlantState::NOGROWTH4)) {
                _len = std::min(_predim,
                                _len + _ler * std::min(_delta_t, _exp_time));
            }
        }

        //PlastoDelay
        _plasto_delay = std::min(((_delta_t > _exp_time) ? _exp_time :
                                                           _delta_t) * (-1. + _reduction_ler), 0.);

        //Width
        _width = _len * _WLR / _LL_BL;

        //ThermalTimeSinceLigulation
        if (not _is_lig) {
            if(_leaf_phase == LeafModel::LIG) {
                _is_lig = true;
                if(_lig_t == 0) {
                    _lig_t = t;
                }
            }
        } else {
            _TT_Lig += _delta_t; //@TODO vérifier si c'est calculé qu'une seule fois
        }

        //BladeArea
        _blade_area = _len * _width * _allo_area / _LL_BL;
        if (not _is_lig) {
            _corrected_blade_area = 0;
        } else {
            if (_blade_area < 0) {
                _blade_area = 0;
            }
            _corrected_blade_area = _blade_area * (1 - _TT_Lig / _life_span);
        }

        //Biomass
        _old_biomass = 0;
        if (_first_day == t) {
            _biomass = (1. / _G_L) * _blade_area / _sla;
            _corrected_biomass = 0;
            _realloc_biomass = 0;
            _sla_cste = _sla;
            _old_biomass = _biomass;
        } else {
            if (_leaf_phase != LeafModel::NOGROWTH) {
                if (not _is_lig) {
                    _biomass = (1. / _G_L) * _blade_area / _sla_cste;
                    _corrected_biomass = 0;
                    _realloc_biomass = 0;
                    _old_biomass = _biomass;
                } else {
                    if (_corrected_biomass > 0) {
                        _old_biomass = _corrected_biomass;
                    } else {
                        _old_biomass = _biomass;
                    }
                    _corrected_biomass = _biomass * (1. - _TT_Lig / _life_span);
                    _realloc_biomass = (_old_biomass - _corrected_biomass) *
                            _realocationCoeff;
                    _senesc_dw = (_old_biomass - _corrected_biomass) *
                            (1 - _realocationCoeff);
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
                   (_plant_phase == PlantState::NOGROWTH or _plant_phase == PlantState::NOGROWTH3
                    or _plant_phase == PlantState::NOGROWTH4)) {
            _leaf_phase = LeafModel::NOGROWTH;
        } else if (_leaf_phase == LeafModel::NOGROWTH and
                   (_plant_phase == PlantState::GROWTH or
                    _plant_phase == PlantState::NEW_PHYTOMER3)) {
            _leaf_phase = LeafModel::INITIAL;
        }
    }

    void LeafModel::init(double t,
                         const ecomeristem::ModelParameters& parameters)
    {
        //parameters
        _coeffLifespan = parameters.get < double >("coeff_lifespan");
        _mu = parameters.get < double >("mu");
        _Lef1 = parameters.get < double >("Lef1");
        _MGR = parameters.get < double >("MGR_init");
        _thresLER = parameters.get < double >("thresLER");
        _respLER = parameters.get < double >("resp_LER");
        _coef_ligulo = parameters.get < double >("coef_ligulo1");
        _plasto = parameters.get < double >("plasto_init");
        _ligulo = _plasto * _coef_ligulo;
        _WLR = parameters.get < double >("WLR");
        _LL_BL = parameters.get < double >("LL_BL_init");
        _allo_area = parameters.get < double >("allo_area");
        _G_L = parameters.get < double >("G_L");
        _realocationCoeff = parameters.get < double >("realocationCoeff");

        //internals
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
        _corrected_blade_area = 0;
        _blade_area = 0;
        _biomass = 0;
        _corrected_biomass = 0;
        _senesc_dw = 0;
        _senesc_dw_sum = 0;
        _demand = 0;
        _time_from_app = 0;
        _lig_t = 0;
    }

    //    double get_blade_area() const
    //    { return blade_area_model.get_blade_area(); }

private:
    // parameters
    double _coeffLifespan;
    double _mu;
    double _Lef1;
    double _MGR;
    double _thresLER;
    double _respLER;
    double _coef_ligulo;
    double _ligulo;
    double _plasto;
    double _WLR;
    double _LL_BL;
    double _allo_area;
    double _G_L;
    double _realocationCoeff;

    // attributes
    int _index;
    bool _is_first_leaf;
    bool _is_on_mainstem;

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
    double _blade_area;
    double _corrected_blade_area;
    double _biomass;
    double _corrected_biomass;
    double _realloc_biomass;
    double _old_biomass;
    double _senesc_dw;
    double _senesc_dw_sum;
    double _demand;
    double _time_from_app;
    double _sla_cste;
    double _lig_t;

    // external variables
    double _ftsw;
    double _p;
    double _plant_phase;
    double _fcstr;
    double _predim_leaf_on_mainstem;
    double _predim_previous_leaf;
    double _test_ic;
    double _dd;
    double _delta_t;
    double _grow;
    double _sla;

};

} // namespace model
