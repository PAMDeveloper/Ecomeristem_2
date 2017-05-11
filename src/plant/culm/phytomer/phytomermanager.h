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
}

// Variables nécessaires
/*
STOCK, NB_LEAF_PI, NBLEAVES, BOOL_CROSSED_PLASTO, NB_LEAF_MAX_AFTER_PI, COEFF_FLO_LAG,
FTSW, IC, TT_PI_To_Flo, TT_PI, TT, NB_LEAF_STEM_ELONG, NB_LEAF_PARAM2,
PHENOSTAGEAtPre_Flo, PHENOSTAGE_PRE_FLO_TO_FLO, PHENOSTAGE_TO_END_FILLING,
PHENOSTAGE_TO_MATURITY
*/
_old_phase = _phytomer_phase;
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
