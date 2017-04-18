/**
 * @file root/RootDemand.hpp
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

#ifndef ROOT_DEMAND_HPP
#define ROOT_DEMAND_HPP

#include <defines.hpp>
#include <plant/PlantState.hpp>

namespace model {

class RootDemandModel : public AtomicModel < RootDemandModel > {

public:
    enum internals { ROOT_DEMAND_COEF, ROOT_DEMAND, SURPLUS };

    enum externals { P, LEAF_DEMAND_SUM, LEAF_LAST_DEMAND_SUM, INTERNODE_DEMAND_SUM, INTERNODE_LAST_DEMAND_SUM,
                     PHASE, STATE };


    RootDemandModel() {
        //    computed variables
        Internal(ROOT_DEMAND_COEF, &RootDemandModel::_root_demand_coef);
        Internal(ROOT_DEMAND, &RootDemandModel::_root_demand);
        Internal(SURPLUS, &RootDemandModel::_surplus);

        //    external variables
        External(P, &RootDemandModel::_P);
        External(LEAF_DEMAND_SUM, &RootDemandModel::_leaf_demand_sum);
        External(LEAF_LAST_DEMAND_SUM, &RootDemandModel::_leaf_last_demand_sum);
        External(INTERNODE_DEMAND_SUM, &RootDemandModel::_internode_demand_sum);
        External(INTERNODE_LAST_DEMAND_SUM, &RootDemandModel::_internode_last_demand_sum);
        External(PHASE, &RootDemandModel::_phase);
        External(STATE, &RootDemandModel::_state);
    }

    virtual ~RootDemandModel()
    {}


    void compute(double t, bool /* update */) {
        // Root Demand Coef
        _root_demand_coef = _coeff1_R_d * std::exp(_coeff2_R_d * (t - _parameters.beginDate)) * (_P * _resp_R_d + 1);


        // Root Demand
        if (t == _parameters.beginDate) {
            _root_demand = (_leaf_demand_sum + _leaf_last_demand_sum +
                            _internode_demand_sum) * _root_demand_coef;
            _last_value = _root_demand;
            _root_biomass = _root_demand;
        } else {
            if (_phase == PlantState::NOGROWTH or _phase == PlantState::NOGROWTH4) {
                _root_demand = 0;
                _last_value = 0;
            } else {
                if (_state == PlantState::ELONG or _state == PlantState::PI) {
                    if (_leaf_demand_sum + _leaf_last_demand_sum + _internode_demand_sum
                            + _internode_last_demand_sum == 0) {
                        _root_demand = _last_value * _root_demand_coef;
                    } else {
                        _root_demand = (_leaf_demand_sum + _leaf_last_demand_sum + _internode_demand_sum +
                                        _internode_last_demand_sum) * _root_demand_coef;
                    }
                    if (_leaf_demand_sum + _leaf_last_demand_sum + _internode_demand_sum +
                            _internode_last_demand_sum != 0) {
                        _last_value = _leaf_demand_sum + _leaf_last_demand_sum
                                + _internode_demand_sum + _internode_last_demand_sum;
                    } else {
                        _last_value = 0;
                    }
                    _root_demand = std::min(_culm_surplus_sum, _root_demand);
                    _surplus = _culm_surplus_sum - _root_demand;

                } else {
                    if (_leaf_demand_sum + _leaf_last_demand_sum == 0) {
                        _root_demand = _last_value * _root_demand_coef;
                    } else {
                        _root_demand = (_leaf_demand_sum + _leaf_last_demand_sum) * _root_demand_coef;
                    }
                    if (_leaf_demand_sum + _internode_demand_sum != 0) {
                        _last_value = _leaf_demand_sum + _internode_demand_sum;
                    } else {
                        _last_value = 0;
                    }
                    _surplus = 0;
                }
            }
            _root_biomass = _root_biomass + _root_demand;
        }
    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //    parameters variables
        _coeff1_R_d = _parameters.get < double >("coeff1_R_d");
        _coeff2_R_d = _parameters.get < double >("coeff2_R_d");
        _resp_R_d = _parameters.get < double >("resp_R_d");

        //    parameters variables (t)

        //    computed variables (internal)
        _root_demand = 0;
        _root_demand_coef = 0;
        _last_value = 0;
        _root_biomass = 0;
        _surplus = 0;

        //    external variables

    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters
    double _coeff1_R_d;
    double _coeff2_R_d;
    double _resp_R_d;

    //    parameters(t)

    //    internals - computed
    double _root_demand_coef;
    double _root_demand;
    double _root_biomass;
    double _last_value;
    double _surplus;

    //    externals
    double _P;
    double _leaf_demand_sum;
    double _leaf_last_demand_sum;
    double _internode_demand_sum;
    double _internode_last_demand_sum;
    double _phase;
    double _state;
    double _culm_surplus_sum;
};

} // namespace model

#endif
