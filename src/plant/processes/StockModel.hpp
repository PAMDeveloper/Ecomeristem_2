/**
 * @file ecomeristem/plant/stock/Model.hpp
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


#ifndef STOCK_MODEL_HPP
#define STOCK_MODEL_HPP

#include <defines.hpp>
#include <plant/PlantState.hpp>

namespace model {

class StockModel : public AtomicModel < StockModel >
{
public:
    enum internals { DAY_DEMAND, IC, TEST_IC, RESERVOIR_DISPO, SEED_RES, STOCK, SUPPLY, SURPLUS };

    enum externals { DEMAND_SUM, LEAF_LAST_DEMAND_SUM, INTERNODE_LAST_DEMAND_SUM,
                     PHASE, LEAF_BIOMASS_SUM, DELETED_LEAF_BIOMASS, REALLOC_BIOMASS_SUM, ASSIM };


    StockModel() {
        //    computed variables
        Internal(DAY_DEMAND, &StockModel::_day_demand);
        Internal(IC, &StockModel::_ic);
        Internal(TEST_IC, &StockModel::_test_ic);
        Internal(RESERVOIR_DISPO, &StockModel::_reservoir_dispo);
        Internal(SEED_RES, &StockModel::_seed_res);
        Internal(STOCK, &StockModel::_stock);
        Internal(SUPPLY, &StockModel::_supply);
        Internal(SURPLUS, &StockModel::_surplus);


        //    external variables
        External(DEMAND_SUM, &StockModel::_demand_sum);
        External(LEAF_LAST_DEMAND_SUM, &StockModel::_leaf_last_demand_sum);
        External(INTERNODE_LAST_DEMAND_SUM, &StockModel::_internode_last_demand_sum);
        External(PHASE, &StockModel::_phase);
        External(LEAF_BIOMASS_SUM, &StockModel::_leaf_biomass_sum);
        External(DELETED_LEAF_BIOMASS, &StockModel::_deleted_leaf_biomass);
        External(REALLOC_BIOMASS_SUM, &StockModel::_realloc_biomass_sum);
        External(ASSIM, &StockModel::_assim);
    }

    virtual ~StockModel()
    {}

    void compute(double t, bool /* update */) {
        //  day_demand
        if (_phase == PlantState::NOGROWTH or _phase == PlantState::NOGROWTH3 or
                _phase == PlantState::NOGROWTH4) {
            _day_demand = 0;
        } else {
            if (_demand_sum == 0) {
                _day_demand = _leaf_last_demand_sum + _internode_last_demand_sum;
            } else {
                _day_demand = _demand_sum + _leaf_last_demand_sum + _internode_last_demand_sum;
            }

        }
        //  index_competition
        if (t != _parameters.beginDate) {
            double resDiv, mean;
            double total = 0.;
            int n = 0;

            if (_day_demand_[0] != 0) {
                resDiv = (std::max(0., _seed_res_[0]) + _supply_[0]) / _day_demand_[0];
                total += resDiv;
                ++n;
            }

            if (_day_demand_[0] != 0) {
                resDiv = (std::max(0., _seed_res_[0]) + _supply_[0]) / _day_demand_[0];
                total += resDiv;
                ++n;
            }

            if (_day_demand_[1] != 0) {
                resDiv = (std::max(0., _seed_res_[1]) + _supply_[1]) / _day_demand_[1];
                total += resDiv;
                ++n;
            }

            if (_day_demand_[2] != 0) {
                resDiv = (std::max(0., _seed_res_[2]) + _supply_[2]) / _day_demand_[2];
                total += resDiv;
                ++n;
            }

            if (n != 0) {
                mean = total / n;
            } else {
                mean = _ic;
            }

            if (mean == 0 and _seed_res_[0] == 0 and _seed_res_[1] == 0 and _seed_res_[2] == 0) {
                _ic = 0.001;
                _test_ic = 0.001;
            } else {
                _ic = std::min(5.,mean);
                _test_ic = std::min(1., std::sqrt(_ic));
            }
        }

        //  reservoir_dispo
        _reservoir_dispo = _leaf_stock_max * _leaf_biomass_sum - _stock;

        //  seed_res
        if (t != _parameters.beginDate) {
            _seed_res = _gdw - _day_demand;
        } else {
            if (_seed_res > _day_demand) {
                _seed_res = _seed_res - _day_demand;
            } else {
                _seed_res = 0;
            }
        }

        //  stock
        //        if (_state == plant::ELONG) {
        //            _stock = _culm_stock;
        //            _deficit = _culm_deficit;
        //        } else {
        double stock = 0;

        if (_seed_res > 0) {
            if (_seed_res > _day_demand) {
                stock = _stock + std::min(_reservoir_dispo, _supply + _realloc_biomass_sum);
            } else {
                stock = _stock +
                        std::min(_reservoir_dispo, _supply - (_day_demand - _seed_res) +
                                 _realloc_biomass_sum);
            }
        } else {
            stock = _stock + std::min(_reservoir_dispo, _supply - _day_demand +
                                      _realloc_biomass_sum);
        }

        _stock = std::max(0., _deficit + stock);
        _deficit = std::min(0., _deficit + stock);
        //        }

        //  supply
        _supply = _assim;

        //  surplus
        //        if (_state == plant::ELONG) {
        //            _surplus = _culm_surplus_sum;
        //        } else {
        if (_seed_res > 0) {
            if (_seed_res > _day_demand) {
                _surplus = std::max(0., _supply - _reservoir_dispo + _realloc_biomass_sum);
            } else {
                _surplus = std::max(0., _supply - (_day_demand - _seed_res) -
                                    _reservoir_dispo + _realloc_biomass_sum);
            }
        } else {
            _surplus = std::max(0., _supply - _reservoir_dispo - _day_demand +
                                _realloc_biomass_sum);
        }
        //        }

        // Realloc biomass
        //        void realloc_biomass(double t, double value)
        //        {
        //            if (value > 0) {
        //                double qty = value * _realocationCoeff;
        //            }
        //        }

    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //    parameters variables
        _gdw = _parameters.get < double >("gdw");
        _leaf_stock_max = parameters.get < double >("leaf_stock_max");
//        _realocationCoeff = _parameters.get < double >("realocationCoeff");


        //    parameters variables (t)


        //    computed variables (internal)
        _day_demand = 0;
        _ic = 0;
        _test_ic = 0;
        _reservoir_dispo = 0;
        _seed_res = 0;
        _stock = 0;
        _deficit = 0;
        _supply = 0;
        _surplus = 0;

        //    external variables
        _supply_[2] = _supply_[1] = _supply_[0] = 0;
        _seed_res_[2] = _seed_res_[1] = _seed_res_[0] = 0;
        _day_demand_[2] = _day_demand_[1] = _day_demand_[0] = 0;
    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters
    double _gdw;
    double _leaf_stock_max;
//    double _realocationCoeff;

    //    parameters(t)

    //    internals - computed
    double _day_demand;
    double _ic;
    double _test_ic;
    double _seed_res;
    double _supply;
    double _reservoir_dispo;
    double _stock;
    double _deficit;
    double _surplus;

    //    externals
    double _demand_sum;
    double _leaf_last_demand_sum;
    double _internode_last_demand_sum;
    int _phase;
    double _day_demand_[3];
    double _seed_res_[3];
    double _supply_[3];
    double _leaf_biomass_sum;
    double _deleted_leaf_biomass;
    double _realloc_biomass_sum;
    //    int _state;
    //    double _culm_stock;
    //    double _culm_deficit;
    double _assim;

};

} // namespace model
#endif
