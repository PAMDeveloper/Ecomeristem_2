/**
 * @file ecomeristem/plant/thermal-time/Model.hpp
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


#ifndef SAMPLE_ATOMIC_MODEL_HPP
#define SAMPLE_ATOMIC_MODEL_HPP

#include <defines.hpp>
#include <plant/culm/processes/CulmStockModel.hpp>

namespace model {

class CulmModel : public CoupledModel < CulmModel >
{
public:
    enum submodels { CULM_STOCK };

    enum internals { };

    enum externals { STATE };


    CulmModel():
        _culm_stock_model(new CulmStockModel)
    {
        // submodels
        Submodels( ((CULM_STOCK, _culm_stock_model.get())) );

        //    computed variables

        //    external variables
        External(STATE, &CulmModel::_state);

    }

    virtual ~CulmModel()
    {}


    //@TODO gérer le deleteLeaf et le reallocBiomassSum var
    void compute(double t, bool /* update */) {
        //Phytomers
        compute_phytomers(t);

        //Sum

        //StockModel
        if (_state == PlantState::ELONG) {
            compute_stock(t);
        }
    }

    void compute_stock(double t) {
        _culm_stock_model->put(t, CulmStockModel::PLANT_DEFICIT, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_STOCK, 0);
        _culm_stock_model->put(t, CulmStockModel::LEAF_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_LEAF_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::ASSIM, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::LEAF_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::LEAF_LAST_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_LAST_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::REALLOC_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_STATE, PlantState::VEGETATIVE);
        (*_culm_stock_model)(t);
    }

    void compute_phytomers(double t) {


    }



    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //    parameters variables

        //    parameters variables (t)

        //    computed variables (internal)

    }

private:
    ecomeristem::ModelParameters _parameters;

    //  submodels
    std::unique_ptr < model::CulmStockModel > _culm_stock_model;

    //    parameters

    //    parameters(t)

    //    internals - computed

    //    externals
    int _state;
};

} // namespace model
#endif
