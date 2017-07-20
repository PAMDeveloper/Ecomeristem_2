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
#include <plant/RootModel.hpp>

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
                     PlantModel::LEAF_BIOM_STRUCT });
        selector("BIOMIN", artis::kernel::DOUBLE, {
                     PlantModel::INTERNODE_BIOM_STRUCT });
        selector("SENESC_DW", artis::kernel::DOUBLE, {
                     PlantModel::SENESC_DW_SUM });
        selector("PHT", artis::kernel::DOUBLE, {
                     PlantModel::HEIGHT });
        selector("LIG", artis::kernel::DOUBLE, {
                     PlantModel::LIG });
        selector("TT_LIG", artis::kernel::DOUBLE, {
                     PlantModel::TT_LIG });
        selector("TT", artis::kernel::DOUBLE, {
                     PlantModel::TT});
        selector("DD", artis::kernel::DOUBLE, {
                     PlantModel::DD});
        selector("EDD", artis::kernel::DOUBLE, {
                     PlantModel::EDD});
        selector("SLA", artis::kernel::DOUBLE, {
                     PlantModel::SLA});

        //AssimilationModel
        selector("ASSIM", artis::kernel::DOUBLE, {
                     PlantModel::ASSIMILATION,
                     AssimilationModel::ASSIM}); 
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
