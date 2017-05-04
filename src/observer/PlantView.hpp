/**
 * @file model/observer/PlantView.hpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS file
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

#ifndef MODEL_OBSERVER_PLANT_VIEW_HPP
#define MODEL_OBSERVER_PLANT_VIEW_HPP

#include <artis/observer/View.hpp>
#include <artis/utils/DateTime.hpp>

#include <ModelParameters.hpp>

#include <plant/PlantModel.hpp>
#include <plant/processes/ThermalTimeModel.hpp>
#include <plant/processes/WaterBalanceModel.hpp>
#include <plant/processes/AssimilationModel.hpp>
#include <plant/processes/PlantStockModel.hpp>
#include <plant/processes/TilleringModel.hpp>
#include <plant/root/RootModel.hpp>

using namespace model;
namespace observer {

class PlantView : public artis::observer::View <
        artis::utils::DoubleTime, ecomeristem::ModelParameters >
{
public:
    PlantView()
    {
        //PlantModel
        selector("BIOMLEAF", artis::kernel::DOUBLE, {
                     PlantModel::LEAF_BIOMASS_SUM });     
        selector("BIOMIN", artis::kernel::DOUBLE, {
                     PlantModel::INTERNODE_BIOMASS_SUM });
        selector("SENESC_DW", artis::kernel::DOUBLE, {
                     PlantModel::SENESC_DW_SUM });
        selector("PHT", artis::kernel::DOUBLE, {
                     PlantModel::HEIGHT });
        selector("P_PHASE", artis::kernel::INT, {
                     PlantModel::PLANT_PHASE });
        selector("P_STATE", artis::kernel::INT, {
                     PlantModel::PLANT_STATE });
        selector("PAI", artis::kernel::DOUBLE, {
                     PlantModel::PAI });
        selector("LIG", artis::kernel::DOUBLE, {
                     PlantModel::LIG });
        selector("TT_LIG", artis::kernel::DOUBLE, {
                     PlantModel::TT_LIG });
        selector("IH", artis::kernel::DOUBLE, {
                     PlantModel::IH });

        //ThermalTimeModel
        selector("TT", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::TT});
        selector("BOOL_CROSSED_PLASTO", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::BOOL_CROSSED_PLASTO});
        selector("PHENOSTAGE", artis::kernel::INT, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::PHENO_STAGE});
        selector("DD", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::DD});
        selector("EDD", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::EDD});
        selector("SLA", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::SLA});

        //AssimilationModel
        selector("ASSIM_NET", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::ASSIM});
        selector("ASSIM", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::ASSIM_NET_COR});
        selector("INTERC", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::INTERC});
        selector("LAI", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::LAI});

//        //StockModel
        selector("DAYDEMAND", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::DAY_DEMAND});
        selector("IC", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::IC});
        selector("TESTIC", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::TEST_IC});
        selector("RESERVOIRDISPO", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::RESERVOIR_DISPO});
        selector("SEEDRES", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::SEED_RES});
        selector("STOCK", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::STOCK});
        selector("SUPPLY", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::SUPPLY});
        selector("SURPLUS", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::SURPLUS});
        selector("DEFICIT", artis::kernel::DOUBLE, {
                     PlantModel::STOCK,
                     PlantStockModel::DEFICIT});

        //TilleringModel
        selector("CREATE_TILLER", artis::kernel::DOUBLE, {
                     PlantModel::TILLERING,
                     TilleringModel::CREATE});

        //RootModel
        selector("R_D", artis::kernel::DOUBLE, {
                     PlantModel::ROOT,
                     RootModel::ROOT_DEMAND_COEF});

        //WaterBalanceModel
        //CulmStockModel
        //PhytomerModel
        //LeafModel
        //InternodeModel

    }

    virtual ~PlantView()
    { }
};

}

#endif
