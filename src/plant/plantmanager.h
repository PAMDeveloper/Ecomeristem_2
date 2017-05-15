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
                    ELONG,
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
    void step_state(double t) {
        phase_plant _plant_phase = INITIAL;
        state_plant _plant_state = NO_STATE;

        double  _stock, _phenostage, _nb_leaf_pi, _FTSW, _nb_leaf_stem_elong, _ic,
                _bool_crossed_plasto, _nb_leaf_max_after_pi,
                _phenostage_at_pre_flo, _phenostage_pre_flo_to_flo,
                _phenostage_to_end_filling, _phenostage_to_maturity;

        if (_FTSW <= 0 or _ic <= -1) {
            _plant_state = KILL;
            _plant_phase = DEAD;
            return;
        }

        //Globals states
        _plant_state >> NEW_PHYTOMER;
        if (_stock <= 0) {
            _plant_state << NOGROWTH;
            return;
        } else {
            _plant_state >> NOGROWTH;
        }

        switch (_plant_phase) {
        case INITIAL: {
            _plant_phase = VEGETATIVE;
            break;
        }
        case VEGETATIVE: {
            if ( _bool_crossed_plasto >= 0) {
                _plant_state << NEW_PHYTOMER;
                if ( _phenostage == _nb_leaf_stem_elong and _phenostage < _nb_leaf_pi - 1) {
                    _plant_phase = ELONG;
                } else if(_phenostage == _nb_leaf_pi - 1) {
                    _plant_phase = PI;
                }
            }
            break;
        }
        case ELONG: {
            if (_bool_crossed_plasto >= 0) {
                _plant_state << NEW_PHYTOMER;
                if (_phenostage == _nb_leaf_pi - 1) {
                    _plant_phase = PI;
                }
            }
            break;
        }
        case PI: {
            if (_bool_crossed_plasto >= 0) {
                if (_phenostage < _nb_leaf_pi + _nb_leaf_max_after_pi) {
                    _plant_state << NEW_PHYTOMER;
                } else if (_phenostage > _nb_leaf_pi + _nb_leaf_max_after_pi) {
                    _plant_phase = PRE_FLO;
                }
            }
            break;
        }
        case PRE_FLO: {
            if (_phenostage == _phenostage_at_pre_flo + _phenostage_pre_flo_to_flo) {
                _plant_phase = FLO;
            }
            break;
        }
        case FLO: {
            if (_phenostage == _phenostage_to_end_filling) {
                _plant_phase = END_FILLING;
            }
            break;
        }
        case END_FILLING: {
            if (_phenostage == _phenostage_to_maturity) {
                _plant_phase = MATURITY;
            }
            break;
        }}
    }
};

#endif
