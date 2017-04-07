/**
 * @file ecomeristem/plant/Model.hpp
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

#ifndef PLANT_MODEL_HPP
#define PLANT_MODEL_HPP

#include "defines.hpp"
#include <plant/processes/ThermalTimeModel.hpp>



//#include <model/models/ecomeristem/culm/CulmModel.hpp>

//#include <model/models/ecomeristem/plant/Lig.hpp>
//#include <model/models/ecomeristem/plant/PlantManager.hpp>
//#include <model/models/ecomeristem/plant/TillerManager.hpp>
//#include <model/models/ecomeristem/plant/assimilation/PlantAssimilationModel.hpp>
//#include <model/models/ecomeristem/root/RootModel.hpp>
//#include <model/models/ecomeristem/plant/SLA.hpp>
//#include <model/models/ecomeristem/plant/stock/PlantStockModel.hpp>
//#include <model/models/ecomeristem/plant/thermal-time/ThermalTimeModel.hpp>
//#include <model/models/ecomeristem/plant/water-balance/WaterBalanceModel.hpp>
//#include <model/kernel/AbstractModel.hpp>
//#include <model/kernel/AbstractCoupledModel.hpp>

class PlantModel : public CoupledModel < PlantModel >
{
public:
    enum submodels { ASSIMILATION, ROOT, STOCK, THERMAL_TIME, WATER_BALANCE,
                     MANAGER, TILLER_MANAGER, SLA, CULMS };

    enum internals { LAI, DELTA_T, DD, EDD, IH, LIGULO_VISU, PHENO_STAGE,
                     PLASTO_VISU, TT, TT_LIG, BOOL_CROSSED_PLASTO,
                     ASSIM, CSTR, ROOT_DEMAND_COEF, ROOT_DEMAND,
                     ROOT_BIOMASS, /*STOCK,*/ GROW, SUPPLY, DEFICIT, IC,
                     SURPLUS, TEST_IC, DAY_DEMAND, RESERVOIR_DISPO,
                     SEED_RES, LEAF_BIOMASS_SUM, INTERNODE_BIOMASS_SUM,
                     SENESC_DW_SUM, LEAF_LAST_DEMAND_SUM,
                     INTERNODE_LAST_DEMAND_SUM, LEAF_DEMAND_SUM,
                     INTERNODE_DEMAND_SUM };

    PlantModel():
    _thermal_time_model(new model::ThermalTimeModel)
    {
        // submodels
        Submodels( ((THERMAL_TIME, _thermal_time_model.get())) );

        // local internals
        Internal( LEAF_BIOMASS_SUM, &PlantModel::_leaf_biomass_sum );
        Internal( LEAF_DEMAND_SUM, &PlantModel::_leaf_demand_sum );
        Internal( LEAF_LAST_DEMAND_SUM, &PlantModel::_leaf_last_demand_sum );
        Internal( INTERNODE_BIOMASS_SUM, &PlantModel::_internode_biomass_sum );
        Internal( INTERNODE_DEMAND_SUM, &PlantModel::_internode_demand_sum );
        Internal( INTERNODE_LAST_DEMAND_SUM, &PlantModel::_internode_last_demand_sum );
        Internal( SENESC_DW_SUM, &PlantModel::_senesc_dw_sum );

    }

    virtual ~PlantModel()
    {

//        std::vector < culm::CulmModel* >::const_iterator it = culm_models.begin();

//        while (it != culm_models.end()) {
//            delete *it;
//            ++it;
//        }
    }

    void compute(double t, bool /* update */) {
        _thermal_time_model->put < double >(t, model::ThermalTimeModel::PLASTO_DELAY, 0);
        _thermal_time_model->put < int >(t, model::ThermalTimeModel::PHASE, PlantState::INIT);
        _thermal_time_model->put < double >(t, model::ThermalTimeModel::LIG, 0);
        (*_thermal_time_model)(t);
    }

    void init(double t, const ecomeristem::ModelParameters& parameters)
    {
        //internal variables (local)
        _leaf_biomass_sum = 0;
        _leaf_demand_sum = 0;
        _leaf_last_demand_sum = 0;
        _internode_demand_sum = 0;
        _internode_last_demand_sum = 0;
        _internode_biomass_sum = 0;
        _senesc_dw_sum = 0;

        //submodels
        _thermal_time_model->init(t, parameters);
    }
        bool is_dead() const
        { /*return not culm_models.empty() and culm_models[0]->is_dead();*/ }

private:

    // submodels
    std::unique_ptr < model::ThermalTimeModel > _thermal_time_model;


//    void compute_assimilation(double t);
//    void compute_culms(double t);
//    void compute_height(double t);
//    void compute_lig(double t);
//    void compute_manager(double t);
//    void compute_root(double t);
//    void compute_sla(double t);
//    void compute_stock(double t);
//    void compute_thermal_time(double t);
//    void compute_tiller(double t);
//    void compute_water_balance(double t);
//    void create_culm(double t, int n);
//    void create_phytomer(double t);
//    bool culms_is_stable(double t);
//    void delete_leaf(double t);
//    double get_deficit(double t) const;
//    double get_phase(double t) const;
//    double get_root_demand(double t) const;
//    double get_state(double t) const;
//    double get_stock(double t) const;
//    void search_deleted_leaf(double t);

//// parameters
//    double _nbleaf_enabling_tillering;
//    double _realocationCoeff;
//    double _LL_BL;

//    const model::models::ModelParameters* _parameters;

// internal variables (local)
    double _leaf_biomass_sum;
    double _leaf_demand_sum;
    double _leaf_last_demand_sum;
    double _internode_biomass_sum;
    double _internode_demand_sum;
    double _internode_last_demand_sum;
    double _senesc_dw_sum;

//    double _leaf_blade_area_sum;
//    double _demand_sum;
//    bool _culm_is_computed;
//    double _realloc_biomass_sum;
//    double _lig;
//    double _deleted_leaf_biomass;
//    double _deleted_leaf_blade_area;
//    double _height;

//// external variables
//    double _etp;
//    double _p;
//    double _radiation;
//    double _ta;
//    double _water_supply;
//    int _culm_index;
//    int _leaf_index;

//    ecomeristem::plant::assimilation::PlantAssimilationModel assimilation_model;
//    ecomeristem::root::RootModel root_model;
//    ecomeristem::plant::stock::PlantStockModel stock_model;
//    ecomeristem::plant::water_balance::WaterBalanceModel water_balance_model;
//    ecomeristem::plant::PlantManager manager_model;
//    ecomeristem::plant::TillerManager tiller_manager_model;
////    ecomeristem::plant::Lig lig_model;
//    ecomeristem::plant::Sla sla_model;

//    std::vector < culm::CulmModel* > culm_models;
//    double _begin;
};

#endif //PLANT_MODEL_HPP
