/**
 * @file ecomeristem/root/Model.hpp
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

#ifndef ROOT_MODEL_HPP
#define ROOT_MODEL_HPP

#include "defines.hpp"
#include <model/kernel/AbstractCoupledModel.hpp>
#include <plant/root/processes/RootDemandModel.hpp>

namespace ecomeristem { namespace root {

class RootModel : public ecomeristem::AbstractCoupledModel < RootModel >
{
public:
    enum submodels { ROOT_DEMAND_MODEL };
    enum internals { ROOT_DEMAND_COEF, ROOT_DEMAND, ROOT_BIOMASS,
                     SURPLUS };
    enum externals { LEAF_DEMAND_SUM, LEAF_LAST_DEMAND_SUM,
                     INTERNODE_DEMAND_SUM, INTERNODE_LAST_DEMAND_SUM, P,
                     STOCK, PHASE, STATE, CULM_SURPLUS_SUM };

    RootModel():
        _root_demand_model(new model::RootDemandModel)
    {
        // submodels
        Submodels( ((ROOT_DEMAND, _root_demand_model.get())) );

        // internals

        // externals
;
    }

    virtual ~RootModel()
    { }

    void init(double t, const model::models::ModelParameters& parameters)
    {
        _root_demand_model->init(t, parameters);
    }

    void compute(double t, bool /* update */)
    {
        _root_demand_model(t);
    }

private:
    //external variables
    double _leaf_demand_sum;
    double _leaf_last_demand_sum;
    double _internode_demand_sum;
    double _internode_last_demand_sum;
    double _p;
    double _stock;
    double _grow;
    double _phase;
    double _state;
    double _culm_surplus_sum;

    // submodels
    std::unique_ptr < model::RootDemandModel > _root_demand_model;
};

} } // namespace ecomeristem root
