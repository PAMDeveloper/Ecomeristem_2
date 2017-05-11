/**
 * @file ecomeristem/phytomer/Model.hpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2016 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2016 ULCO http://www.univ-littoral.fr
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
#include <plant/culm/phytomer/internode/InternodeModel.hpp>
#include <plant/culm/phytomer/leaf/LeafModel.hpp>

namespace model {

class PhytomerModel : public CoupledModel < PhytomerModel >
{
public:
    enum submodels { LEAF, INTERNODE };

    enum internals { LEAF_PREDIM,
                     LEAF_BIOMASS, LEAF_BLADE_AREA, LEAF_DEMAND,
                     INTERNODE_DEMAND, INTERNODE_LAST_DEMAND, INTERNODE_BIOMASS,
                     INTERNODE_LEN, LEAF_LAST_DEMAND, PLASTO_DELAY,
                     REALLOC_BIOMASS, SENESC_DW, SENESC_DW_SUM,
                     LEAF_CORRECTED_BIOMASS, LEAF_CORRECTED_BLADE_AREA,
                     LEAF_LEN };

    enum externals { DD, DELTA_T, FTSW, FCSTR, PREDIM_LEAF_ON_MAINSTEM,
                     PREDIM_PREVIOUS_LEAF, SLA, PLANT_PHASE, TEST_IC,
                     PLANT_STATE};

    enum phytomer_phase {
        INITIAL = 1,
        LEAFMORPHOGENESIS = 2,
        NOGROWTH = 3,
        PI = 4,
        FLO = 5,
        PRE_FLO = 6,
        NOGROWTH_PI = 7,
        NOGROWTH_FLO = 8,
        ELONG = 9,
        NOGROWTH_ELONG = 10,
        NOGROWTH_PRE_FLO = 11,
        ENDFILLING = 12,
        NOGROWTH_ENDFILLING = 13,
        MATURITY = 14,
        NOGROWTH_MATURITY = 15,
        DEAD = 1000
    };



    PhytomerModel(int index, bool is_on_mainstem) :
        _index(index),
        _is_first_phytomer(index == 1),
        _is_on_mainstem(is_on_mainstem),
        _internode_model(new InternodeModel(_index, _is_on_mainstem)),
        _leaf_model(new LeafModel(_index, _is_on_mainstem))
    {
        // submodels
        Submodels( ((LEAF, _leaf_model.get())) );
        Submodels( ((INTERNODE, _internode_model.get())) );

        // internals
        InternalS(LEAF_PREDIM,  _leaf_model.get(), LeafModel::LEAF_PREDIM);
        InternalS(LEAF_BIOMASS, _leaf_model.get(), LeafModel::BIOMASS);
        InternalS(LEAF_BLADE_AREA, _leaf_model.get(), LeafModel::BLADE_AREA);
        InternalS(LEAF_DEMAND, _leaf_model.get(), LeafModel::DEMAND);
        InternalS(LEAF_LAST_DEMAND, _leaf_model.get(), LeafModel::LAST_DEMAND);
        InternalS(PLASTO_DELAY, _leaf_model.get(), LeafModel::PLASTO_DELAY);
        InternalS(REALLOC_BIOMASS, _leaf_model.get(), LeafModel::REALLOC_BIOMASS);
        InternalS(SENESC_DW, _leaf_model.get(), LeafModel::SENESC_DW);
        InternalS(SENESC_DW_SUM, _leaf_model.get(), LeafModel::SENESC_DW_SUM);
        InternalS(LEAF_CORRECTED_BIOMASS, _leaf_model.get(), LeafModel::CORRECTED_BIOMASS);
        InternalS(LEAF_CORRECTED_BLADE_AREA, _leaf_model.get(), LeafModel::CORRECTED_BLADE_AREA);
        InternalS(LEAF_LEN, _leaf_model.get(), LeafModel::LEAF_LEN);
        InternalS(INTERNODE_LAST_DEMAND, _internode_model.get(), InternodeModel::LAST_DEMAND);
        InternalS(INTERNODE_DEMAND, _internode_model.get(), InternodeModel::DEMAND);
        InternalS(INTERNODE_BIOMASS, _internode_model.get(), InternodeModel::BIOMASS);
        InternalS(INTERNODE_LEN, _internode_model.get(), InternodeModel::INTERNODE_LEN);


        // externals
        External(PLANT_PHASE, &PhytomerModel::_plant_phase);
        External(PLANT_STATE, &PhytomerModel::_plant_state);
        External(TEST_IC, &PhytomerModel::_test_ic);
        External(FCSTR, &PhytomerModel::_fcstr);
        External(PREDIM_LEAF_ON_MAINSTEM, &PhytomerModel::_predim_leaf_on_mainstem);
        External(PREDIM_PREVIOUS_LEAF, &PhytomerModel::_predim_previous_leaf);
        External(FTSW, &PhytomerModel::_ftsw);
        External(DD, &PhytomerModel::_dd);
        External(DELTA_T, &PhytomerModel::_delta_t);
        External(SLA, &PhytomerModel::_sla);
    }

    virtual ~PhytomerModel()
    {
        //        if (internode_model) delete internode_model;
        //        if (leaf_model) delete leaf_model;
    }

//    STOCK, NB_LEAF_PI, NBLEAVES, BOOL_CROSSED_PLASTO, NB_LEAF_MAX_AFTER_PI, COEFF_FLO_LAG,
//    FTSW, IC, TT_PI_To_Flo, TT_PI, TT, NB_LEAF_STEM_ELONG, NB_LEAF_PARAM2,
//    PHENOSTAGEAtPre_Flo, PHENOSTAGE_PRE_FLO_TO_FLO, PHENOSTAGE_TO_END_FILLING,
//    PHENOSTAGE_TO_MATURITY

    /*
    void step_state() {
        switch (_phytomer_phase) {

        case PhytomerModel::INITIAL: {
            if (_stock >= 0 and _nb_leaves < _nb_leaf_pi) {
                _phytomer_phase = PhytomerModel::LEAFMORPHOGENESIS;
            } else {
                _phytomer_phase = PhytomerModel::NOGROWTH;
            }
            break;
        }

        case PhytomerModel::LEAFMORPHOGENESIS: {
            if (_FTSW <= 0 or _ic == -1) {
                _phytomer_phase = PhytomerModel::DEAD;
            } else {
                if (_stock == 0) {
                    _phytomer_phase = PhytomerModel::NOGROWTH;
                } else {
                    if (_bool_crossed_plasto < 0) {
                        _phytomer_phase = PhytomerModel::LEAFMORPHOGENESIS;
                    } else {
                        if (_nb_leaves == _nb_leaf_stem_elong and _nb_leaves < _nb_leaf_pi and _nb_leaf_stem_elong < _nb_leaf_pi) {
                            //Phytomer_Leaf_Creation
                            //Phytomer_Internode_Creation
                            //??Tiller_stock_individualisation??
                            //Start_Internode_Elong
                            //Tillers_Transition_To_Elong
                            //Root_Transition_To_Elong
                            _phytomer_phase = PhytomerModel::ELONG;
                        } else {
                            if(_nb_leaves == _nb_leaf_pi) {
                                //Phytomer_leaf_creation
                                //Phytomer_Internode_creation
                                //??Tiller_stock_individualisation??
                                //Start_Internode_Elong
                                //Phytomer_Panicle_Creation
                                //Phytomer_Peduncle_Creation
                                //Phytomer_PanicleActivation
                                //Tillers_Transition_To_PRE_PI
                                //StoreThermalTimeAtPi
                                //Root_Transition_To_PI
                                _phytomer_phase = PhytomerModel::PI;
                            } else {
                                _phytomer_phase = PhytomerModel::LEAFMORPHOGENESIS;
                            }
                        }
                    }
                }
            }
            break;
        }

        case PhytomerModel::NOGROWTH: {
            if (_FTSW <= 0 or _ic == -1) {
                _phytomer_phase = PhytomerModel::DEAD;
            } else {
                if (stock == 0) {
                    if (_nb_leaves < _nb_leaf_stem_elong and _nb_leaves < _nb_leaf_pi) {
                        if (_bool_crossed_plasto >= 0) {
                            //Phytomer_leaf_creation
                            //Phytomer_Internode_creation
                        }
                        _phytomer_phase = PhytomerModel::LEAFMORPHOGENESIS;
                    } else {
                        if (_nb_leaves == _nb_leaf_stem_elong and _nb_leaves < _nb_leaf_pi and _nb_leaf_stem_elong < _nb_leaf_pi) {
                            if (_bool_crossed_plasto) {
                                //Phytomer_leaf_creation
                                //Phytomer_Internode_creation
                                //?? Tiller_stock_individualisation ??
                                //Start_Internode_Elong
                                //Tillers_Transition_To_Elong
                                //Root_Transition_To_Elong
                                _phytomer_phase = PhytomerModel::ELONG;
                            }
                        } else {
                            if(_nb_leaves == _nb_leaf_pi) {
                                if (_bool_crossed_plasto) {
                                    //Phytomer_leaf_creation
                                    //Phytomer_Internode_creation
                                    //?? Tiller_stock_individualisation ??
                                    //Start_Internode_Elong
                                    //Phytomer_Panicle_Creation
                                    //Phytomer_Peduncle_Creation
                                    //Phytomer_PanicleActivation
                                    //Tillers_Transition_To_PRE_PI
                                    //StoreThermalTimeAtPi
                                    //Root_Transition_To_PI
                                    _phytomer_phase = PhytomerModel::PI;
                                }
                            }
                        }
                    }
                } else {
                    _phytomer_phase = PhytomerModel::NOGROWTH;
                }
            }
            break;
        }

        case PhytomerModel::PI: {
            if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_PI;
            } else if (stock > 0) {
                if (_bool_crossed_plasto >= 0) {
                    if (_nb_leaves <= _nb_leaf_pi + _nb_leaf_max_after_pi) {
                        //Phytomer_Leaf_Activation
                        //Start_Internode_Elong
                        _phytomer_phase = PhytomerModel::PI;
                    }
                    if (_nb_leaves == _nb_leaf_pi + _nb_leaf_max_after_pi + 1) {
                        //Start_Internode_Elong
                        //Phytomer_Peduncle_Activation
                        _phytomer_phase = PhytomerModel::PRE_FLO;
                        //Store_Phenostage_At_Preflo
                    }
                } else {
                    _phytomer_phase = PhytomerModel::PI;
                }
            }
            break;
        }

        case PhytomerModel::NOGROWTH_PI: {
            if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_PI
            } else if (_stock > 0) {
                if(_bool_crossed_plasto >= 0) {
                    if (_nb_leaves <= _nb_leaf_pi + _nb_leaf_max_after_pi) {
                        //Phytomer_Leaf_Activation
                        //Start_Internode_Elong
                        _phytomer_phase = PhytomerModel::PI;
                    }
                    if (_nb_leaves == _nb_leaf_pi + _nb_leaf_max_after_pi + 1) {
                        //Start_Internode_Elong
                        //Phytomer_Peduncle_Activation
                        _phytomer_phase = PhytomerModel::PRE_FLO;
                        //Store_Phenostage_At_Preflo
                    }
                } else {
                    _phytomer_phase = PhytomerModel::PI;
                }
            }
            break;
        }

        case PhytomerModel::PRE_FLO: {
            if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_PRE_FLO;
            } else if (_stock > 0) {
                if (_nb_leaves == _phenostage_at_pre_flo + _phenostage_pre_flo_to_flo) {
                    //Panicle_Transition_To_Flo
                    //Peduncle_Transition_To_Flo
                    _phytomer_phase = PhytomerModel::FLO;
                } else {
                    _phytomer_phase = PhytomerModel::PRE_FLO;
                }
            }
            break;
        }

        case PhytomerModel::NOGROWTH_PRE_FLO : {
            if (stock > 0) {
                if (_nb_leaves == _phenostage_at_pre_flo + _phenostage_pre_flo_to_flo) {
                    //Panicle_Transition_To_FLO
                    //Peduncle_Transition_To_FLO
                    _phytomer_phase = PhytomerModel::FLO;
                } else {
                    _phytomer_phase = PhytomerModel::PRE_FLO;
                }
            } else if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_PRE_FLO;
            }
            break;
        }


        case PhytomerModel::NOGROWTH_FLO: {
            if (_stock > 0) {
                if (_nb_leaves == _phenostage_to_end_filling) {
                    //Tillers_Transition_To_End_Filling
                    //Panicle_Transition_To_End_Filling
                    //Peduncle_Transition_To_End_Filling
                    _phytomer_phase = PhytomerModel::ENDFILLING;
                } else {
                    _phytomer_phase = PhytomerModel::FLO;
                }
            } else if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_FLO;
            }
            break;
        }

        case PhytomerModel::FLO: {
            if (_stock > 0) {
                if (_nb_leaves == _phenostage_to_end_filling) {
                    _phytomer_phase = PhytomerModel::ENDFILLING;
                    //Tillers_Transition_To_End_Filling
                    //Panicle_Transition_To_End_Filling
                    //Peduncle_Transition_To_End_Filling
                } else {
                    _phytomer_phase = PhytomerModel::FLO;
                }
            } else if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_FLO;
            }
            break;
        }

        case PhytomerModel::DEAD: {
            _phytomer_phase = PhytomerModel::DEAD;
            break;
        }

        case PhytomerModel::ELONG: {
            if (_FTSW <= 0 or _ic == -1) {
                _phytomer_phase = PhytomerModel::DEAD;
            } else {
                if (_stock == 0) {
                    _phytomer_phase = PhytomerModel::NOGROWTH_ELONG;
                } else if (_stock > 0) {
                    if (_bool_crossed_plasto >= 0) {
                        if (_nb_leaves >= _nb_leaf_stem_elong and _nb_leaves < _nb_leaf_pi) {
                            //Phytomer_Leaf_Creation
                            //Phytomer_Internode_Creation
                            //Start_Internode_Elong
                            _phytomer_phase = PhytomerModel::ELONG;
                        } else {
                            if (_nb_leaves == _nb_leaf_pi) {
                                //Phytomer_Leaf_Creation
                                //Phytomer_Internode_Creation
                                //Start_Internode_Elong
                                //Phytomer_Panicle_Creation
                                //Phytomer_Peduncle_Creation
                                //Phytomer_Panicle_Activation
                                //Tillers_Transition_To_PI
                                //Sotre_Thermal_Time_At_PI
                                //Tillers_Store_Phenostage_At_PI
                                //Root_Transition_To_PI
                                _phytomer_phase = PhytomerModel::PI;
                            }
                        }
                    } else {
                        _phytomer_phase = PhytomerModel::ELONG;
                    }
                }
            }
            break;
        }

        case PhytomerModel::NOGROWTH_ELONG: {
            if (_FTSW <= 0 or _ic == -1) {
                _phytomer_phase = PhytomerModel::DEAD;
            } else {
                if (_stock > 0) {
                    if (_nb_leaves >= _nb_leaf_stem_elong and _nb_leaves < _nb_leaf_pi) {
                        if (_bool_crossed_plasto >= 0) {
                            //Phytomer_Leaf_Creation
                            //Phytomer_Internode_Creation
                            //Start_Internode_Elong
                            _phytomer_phase = PhytomerModel::ELONG;
                        } else {
                            _phytomer_phase = PhytomerModel::ELONG;
                        }
                    } else {
                        if (_nb_leaves == _nb_leaf_pi) {
                            if (_bool_crossed_plasto >= 0) {
                                //Phytomer_Leaf_Creation
                                //Phytomer_Internode_Creation
                                //Start_Internode_Elong
                                //Phytomer_Panicle_Creation
                                //Phytomer_Peduncle_Creation
                                //Phytomer_Panicle_Activation
                                //Tillers_Transition_To_PI
                                //Sotre_Thermal_Time_At_PI
                                //Tillers_Store_Phenostage_At_PI
                                //Root_Transition_To_PI
                                _phytomer_phase = PhytomerModel::PI;
                            }
                        }
                    }
                } else if (_stock == 0) {
                    _phytomer_phase = PhytomerModel::NOGROWTH_ELONG;
                }
            }
            break;
        }

        case PhytomerModel::ENDFILLING: {
            if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_ENDFILLING;
            } else if (_stock > 0) {
                if (_nb_leaves == _phenostage_to_maturity) {
                    _phytomer_phase = PhytomerModel::MATURITY;
                    //Tillers_Transition_To_Maturity
                    //Panicle_Transition_To_Maturity
                    //Peduncle_Transition_To_Maturity
                } else {
                    _phytomer_phase = PhytomerModel::ENDFILLING;
                }
            }
            break;
        }

        case PhytomerModel::NOGROWTH_ENDFILLING: {
            if (_stock > 0) {
                if (_nb_leaves == _phenostage_to_maturity) {
                    _phytomer_phase = PhytomerModel::MATURITY;
                    //Tillers_Transition_To_Maturity
                    //Panicle_Transition_To_Maturity
                    //Peduncle_Transition_To_Maturity
                } else {
                    _phytomer_phase = PhytomerModel::ENDFILLING;
                }
            } else {
                _phytomer_phase = PhytomerModel::NOGROWTH_ENDFILLING;
            }
            break;
        }

        case PhytomerModel::MATURITY: {
            if (_stock == 0) {
                _phytomer_phase = PhytomerModel::NOGROWTH_MATURITY;
            } else {
                _phytomer_phase = PhytomerModel::MATURITY;
            }
            break;
        }

        case PhytomerModel::NOGROWTH_MATURITY: {
            if (_stock > 0) {
                _phytomer_phase = PhytomerModel::MATURITY;
            } else {
                _phytomer_phase = PhytomerModel::NOGROWTH_MATURITY;
            }
            break;
        }
        }
    }
    */

    void init(double t, const ecomeristem::ModelParameters& parameters)
    {
        // submodels
        _internode_model->init(t, parameters);
        _leaf_model->init(t, parameters);

        //internals
        _phytomer_phase = INITIAL;
    }

    void compute(double t, bool /* update */)
    {
        if (_leaf_model) {
            _leaf_model->put(t, LeafModel::DD, _dd);
            _leaf_model->put(t, LeafModel::DELTA_T, _delta_t);
            _leaf_model->put(t, LeafModel::FTSW, _ftsw);
            _leaf_model->put(t, LeafModel::FCSTR, _fcstr);
            _leaf_model->put(t, LeafModel::LEAF_PREDIM_ON_MAINSTEM, _predim_leaf_on_mainstem);
            _leaf_model->put(t, LeafModel::PREVIOUS_LEAF_PREDIM, _predim_previous_leaf);
            _leaf_model->put(t, LeafModel::SLA, _sla);
            _leaf_model->put < int >(t, LeafModel::PLANT_PHASE, _plant_phase); //@TODO set real value
            _leaf_model->put(t, LeafModel::TEST_IC, _test_ic);
            (*_leaf_model)(t);
        }

        _internode_model->put(t, InternodeModel::DD, _dd);
        _internode_model->put(t, InternodeModel::DELTA_T, _delta_t);
        _internode_model->put(t, InternodeModel::FTSW, _ftsw);
        _internode_model->put < int >(t, InternodeModel::PLANT_PHASE, _plant_phase);
        _internode_model->put < int >(t, InternodeModel::PLANT_STATE, _plant_state);
        _internode_model->put(t, InternodeModel::LIG, _leaf_model->get < double > (t, LeafModel::LIG_T));
        _internode_model->put(t, InternodeModel::LEAF_PREDIM, _leaf_model->get < double > (t, LeafModel::LEAF_PREDIM));
        _internode_model->put(t, InternodeModel::IS_LIG, _leaf_model->get < bool > (t, LeafModel::IS_LIG));
        (*_internode_model)(t);
    }

    void delete_leaf(double t)
    {

        //#ifdef WITH_TRACE
        //        utils::Trace::trace()
        //            << utils::TraceElement("CULM", t, artis::utils::COMPUTE)
        //            << "ADD LIG ; DELETE index = " << _index;
        //        utils::Trace::trace().flush();
        //#endif

        _leaf_model.release();
        //        change_internal(LEAF_BIOMASS, &PhytomerModel::_null);
        //        change_internal(LEAF_BLADE_AREA, &PhytomerModel::_null);
        //        change_internal(LEAF_DEMAND, &PhytomerModel::_null);
        //        change_internal(LEAF_LAST_DEMAND, &PhytomerModel::_null);
        //        change_internal(PREDIM, &PhytomerModel::_null);
        //        change_internal(PLASTO_DELAY, &PhytomerModel::_null);
        //        change_internal(REALLOC_BIOMASS, &PhytomerModel::_null);
        //        change_internal(SENESC_DW, &PhytomerModel::_null);
        //        change_internal(SENESC_DW_SUM, &PhytomerModel::_null);
        //        change_internal(LEAF_CORRECTED_BIOMASS, &PhytomerModel::_null);
        //        change_internal(LEAF_CORRECTED_BLADE_AREA, &PhytomerModel::_null);
        //        change_internal(LEAF_LEN, &PhytomerModel::_null);
    }

    //    double get_blade_area() const
    //    { return leaf_model->get_blade_area(); }

    LeafModel * leaf() const
    { return _leaf_model.get(); }

    InternodeModel * internode() const
    { return _internode_model.get(); }

    int get_index() const
    { return _index; }

    bool is_leaf_dead() const
    { return _leaf_model.get() == nullptr; }

    bool is_leaf_lig(double t) const {
        return !is_leaf_dead() &&
                _leaf_model->get < bool > (t, LeafModel::IS_LIG);
    }

private:
    //  attribute
    int _index;
    bool _is_first_phytomer;
    bool _is_on_mainstem;

    // submodels
    std::unique_ptr < InternodeModel > _internode_model;
    std::unique_ptr < LeafModel > _leaf_model;

    // internals
    phytomer_phase _phytomer_phase;
    // external variables
    double _ftsw;
    int _plant_phase;
    int _plant_state;
    double _fcstr;
    double _predim_leaf_on_mainstem;
    double _predim_previous_leaf;
    double _test_ic;
    double _dd;
    double _delta_t;
    double _sla;
};

} // namespace model
