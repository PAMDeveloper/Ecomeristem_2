#ifndef PLANT_MANAGER_H
#define PLANT_MANAGER_H

using namespace std;


enum state_plant { NO_STATE = 0,
                   NOGROWTH = 1,
                   NEW_PHYTOMER = 2,
                   LIG = 4,
                   KILL = 8 };

enum phase_plant {  INITIAL = 0,
                    VEGETATIVE,
                    PRE_ELONG,
                    ELONG,
                    PRE_PI,
                    PI,
                    PRE_FLO,
                    FLO,
                    END_FILLING,
                    MATURITY,
                    DEAD };

inline bool operator&(state_plant a, state_plant b)
{return (static_cast<int>(a) & static_cast<int>(b)) != 0;}
inline void operator<<(state_plant& a, state_plant b)
{a = static_cast<state_plant>(static_cast<int>(a) | static_cast<int>(b));}
inline void operator>>(state_plant& a, state_plant b)
{a = static_cast<state_plant>(static_cast<int>(a) & !static_cast<int>(b));}

class test {
    bool isGrowing(state_plant _plant_state) {return !(_plant_state & NOGROWTH);}

    void step_state2(double t) {
        phase_plant _plant_phase = INITIAL;
        state_plant _plant_state = NO_STATE;

        double _stock, _nb_leaves, _nb_leaf_pi, _FTSW, _nb_leaf_stem_elong, _ic,
                _bool_crossed_plasto, stock, _nb_leaf_max_after_pi,
                _phenostage_at_pre_flo, _phenostage_pre_flo_to_flo,
                _phenostage_to_end_filling, _phenostage_to_maturity;

        //Globals states
        _plant_state >> NEW_PHYTOMER;
        if (_stock <= 0) {
            _plant_state << NOGROWTH;
        } else {
            _plant_state >> NOGROWTH;
        }

        switch (_plant_phase) {
        case INITIAL: {
            if (isGrowing(_plant_state) and _nb_leaves < _nb_leaf_pi) {
                _plant_state >> NOGROWTH;
                _plant_phase = VEGETATIVE;
            } else {
                _plant_state = KILL;
                _plant_phase = DEAD;
            }
            break;
        }

        case VEGETATIVE: {

            if (_FTSW <= 0 or _ic <= -1) {
                _plant_state = KILL;
                _plant_phase = DEAD;
            } else {
                if (isGrowing(_plant_state)) {
                    if ( _bool_crossed_plasto >= 0) {
                        _plant_state << NEW_PHYTOMER;
                        if ( _nb_leaves == _nb_leaf_stem_elong and
                             _nb_leaves < _nb_leaf_pi) {
                            _plant_phase = ELONG;
                            //Phytomer_Leaf_Creation
                            //Phytomer_Internode_Creation
                            //??Tiller_stock_individualisation?? -- //@TODO culmManager
                            //Start_Internode_Elong
                            //Tillers_Transition_To_Elong
                            //Root_Transition_To_Elong
                        } else {
                            if(_nb_leaves == _nb_leaf_pi) {
                                _plant_phase = PI;
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
                            }
                        }
                    }
                }
            }
            break;
        }

        case ELONG: {
            if (_FTSW <= 0 or _ic <= -1) {
                _plant_state = KILL;
                _plant_phase = DEAD;
            } else {
                if (isGrowing(_plant_state)) {
                    if (_bool_crossed_plasto >= 0) {
                        _plant_state << NEW_PHYTOMER;
                        if (_nb_leaves == _nb_leaf_pi) {
                            _plant_phase = PI;
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
                        }
                    }
                }
            }
            break;
        }

        case PI: {
            if (isGrowing(_plant_state)) {
                if (_bool_crossed_plasto >= 0) {
                    if (_nb_leaves == _nb_leaf_pi + _nb_leaf_max_after_pi + 1) {
                        //Start_Internode_Elong
                        //Phytomer_Peduncle_Activation
                        _plant_phase = PRE_FLO;
                        //Store_Phenostage_At_Preflo
                    }
                }
            }
            break;
        }

        case PRE_FLO: {
            if (isGrowing(_plant_state)) {
                if (_nb_leaves == _phenostage_at_pre_flo + _phenostage_pre_flo_to_flo) {
                    //Panicle_Transition_To_Flo
                    //Peduncle_Transition_To_Flo
                    _plant_phase = FLO;
                }
            }
            break;
        }

        case FLO: {
            if (isGrowing(_plant_state)) {
                if (_nb_leaves == _phenostage_to_end_filling) {
                    _plant_phase = END_FILLING;
                    //Tillers_Transition_To_End_Filling
                    //Panicle_Transition_To_End_Filling
                    //Peduncle_Transition_To_End_Filling
                }
            }
            break;
        }

        case END_FILLING: {
            if (isGrowing(_plant_state)) {
                if (_nb_leaves == _phenostage_to_maturity) {
                    _plant_phase = MATURITY;
                    //Tillers_Transition_To_Maturity
                    //Panicle_Transition_To_Maturity
                    //Peduncle_Transition_To_Maturity
                }
            }
            break;
        }


        }
    }
};

#endif
