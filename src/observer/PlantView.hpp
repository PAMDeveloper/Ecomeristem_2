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
//        selector("LEAF_BIOMASS_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::LEAF_BIOMASS_SUM });
//        selector("LEAF_DEMAND_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::LEAF_DEMAND_SUM });
//        selector("LEAF_LAST_DEMAND_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::LEAF_LAST_DEMAND_SUM });
//        selector("INTERNODE_BIOMASS_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::INTERNODE_BIOMASS_SUM });
//        selector("INTERNODE_DEMAND_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::INTERNODE_DEMAND_SUM });
//        selector("INTERNODE_LAST_DEMAND_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::INTERNODE_LAST_DEMAND_SUM });
//        selector("SENESC_DW_SUM", artis::kernel::DOUBLE, {
//                     PlantModel::SENESC_DW_SUM });
//        selector("PHT", artis::kernel::DOUBLE, {
//                     PlantModel::PLANT_HEIGHT });
        selector("PP_PHASE", artis::kernel::INT, {
                     PlantModel::PLANT_PHASE });
        selector("P_STATE", artis::kernel::INT, {
                     PlantModel::PLANT_STATE });
        selector("PAI", artis::kernel::DOUBLE, {
                     PlantModel::PAI });
        selector("LIG", artis::kernel::DOUBLE, {
                     PlantModel::LIG });


        //ThermalTimeModel
        selector("STATE", artis::kernel::INT, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::STATE});
//        selector("DELTA_T", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::DELTA_T});
//        selector("TT", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::TT});
//        selector("BOOL_CROSSED_PLASTO", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::BOOL_CROSSED_PLASTO});
//        selector("TT_LIG", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::TT_LIG});
//        selector("PLASTO_VISU", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::PLASTO_VISU});
//        selector("LIGULO_VISU", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::LIGULO_VISU});
//        selector("PHENOSTAGE", artis::kernel::INT, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::PHENO_STAGE});
        selector("DD", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::DD});
        selector("EDD", artis::kernel::DOUBLE, {
                     PlantModel::THERMAL_TIME,
                     ThermalTimeModel::EDD});
//        selector("IH", artis::kernel::DOUBLE, {
//                     PlantModel::THERMAL_TIME,
//                     ThermalTimeModel::IH});

        //WaterBalanceModel
//        selector("CSTR", artis::kernel::DOUBLE, {
//                     PlantModel::WATER_BALANCE,
//                     WaterBalanceModel::CSTR});
//        selector("FCSTR", artis::kernel::DOUBLE, {
//                     PlantModel::WATER_BALANCE,
//                     WaterBalanceModel::FCSTR});
//        selector("FTSW", artis::kernel::DOUBLE, {
//                     PlantModel::WATER_BALANCE,
//                     WaterBalanceModel::FTSW});
//        selector("TRANSPIRATION", artis::kernel::DOUBLE, {
//                     PlantModel::WATER_BALANCE,
//                     WaterBalanceModel::TRANSPIRATION});
//        selector("SWC", artis::kernel::DOUBLE, {
//                     PlantModel::WATER_BALANCE,
//                     WaterBalanceModel::SWC});

        //AssimilationModel
        selector("ASSIM", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::ASSIM});
//        selector("ASSIM_NET", artis::kernel::DOUBLE, {
//                     PlantModel::ASSIMILATION,
//                     AssimilationModel::ASSIM_NET_COR});
//        selector("ASSIM_POT", artis::kernel::DOUBLE, {
//                     PlantModel::ASSIMILATION,
//                     AssimilationModel::ASSIM_POT});
//        selector("INTERC", artis::kernel::DOUBLE, {
//                     PlantModel::ASSIMILATION,
//                     AssimilationModel::INTERC});
        selector("LAI", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::LAI});
//        selector("RESP_MAINT", artis::kernel::DOUBLE, {
//                     PlantModel::ASSIMILATION,
//                     AssimilationModel::RESP_MAINT});

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
        selector("RESERVOIR_DISPO", artis::kernel::DOUBLE, {
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


        //TilleringModel
//        selector("NBTILLER", artis::kernel::DOUBLE, {
//                     PlantModel::TILLERING,
//                     TilleringModel::NB_TILLERS});
//        selector("CREATE_TILLER", artis::kernel::DOUBLE, {
//                     PlantModel::TILLERING,
//                     TilleringModel::CREATE});

        //RootModel
//        selector("ROOT_DEMAND_COEF", artis::kernel::DOUBLE, {
//                     PlantModel::ROOT,
//                     RootModel::ROOT_DEMAND_COEF});
//        selector("ROOT_DEMAND", artis::kernel::DOUBLE, {
//                     PlantModel::ROOT,
//                     RootModel::ROOT_DEMAND});
//        selector("SURPLUS", artis::kernel::DOUBLE, {
//                     PlantModel::ROOT,
//                     RootModel::SURPLUS});

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
