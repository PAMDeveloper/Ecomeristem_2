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

namespace model {

class PlantStockModel : public AtomicModel < PlantStockModel >
{
public:
    enum internals { DAY_DEMAND, IC, TEST_IC, RESERVOIR_DISPO, SEED_RES, STOCK,
                     SUPPLY, SURPLUS, DEFICIT };

    enum externals { DEMAND_SUM, LEAF_LAST_DEMAND_SUM,
                     INTERNODE_LAST_DEMAND_SUM, PLANT_PHASE, LEAF_BIOMASS_SUM,
                     DELETED_LEAF_BIOMASS, REALLOC_BIOMASS_SUM, ASSIM,
                     PLANT_STATE, CULM_STOCK, CULM_DEFICIT, CULM_SURPLUS_SUM };


    PlantStockModel() {
        //    computed variables
        Internal(DAY_DEMAND, &PlantStockModel::_day_demand);
        Internal(IC, &PlantStockModel::_ic);
        Internal(TEST_IC, &PlantStockModel::_test_ic);
        Internal(RESERVOIR_DISPO, &PlantStockModel::_reservoir_dispo);
        Internal(SEED_RES, &PlantStockModel::_seed_res);
        Internal(STOCK, &PlantStockModel::_stock);
        Internal(SUPPLY, &PlantStockModel::_supply);
        Internal(SURPLUS, &PlantStockModel::_surplus);
        Internal(DEFICIT, &PlantStockModel::_deficit);


        //    external variables
        External(DEMAND_SUM, &PlantStockModel::_demand_sum);
        External(LEAF_LAST_DEMAND_SUM, &PlantStockModel::_leaf_last_demand_sum);
        External(INTERNODE_LAST_DEMAND_SUM, &PlantStockModel::_internode_last_demand_sum);
        External(PLANT_PHASE, &PlantStockModel::_plant_phase);
        External(LEAF_BIOMASS_SUM, &PlantStockModel::_leaf_biomass_sum);
        External(DELETED_LEAF_BIOMASS, &PlantStockModel::_deleted_leaf_biomass);
        External(REALLOC_BIOMASS_SUM, &PlantStockModel::_realloc_biomass_sum);
        External(ASSIM, &PlantStockModel::_assim);
        External(PLANT_STATE, &PlantStockModel::_state);
        External(CULM_STOCK, &PlantStockModel::_culm_stock);
        External(CULM_DEFICIT, &PlantStockModel::_culm_deficit);
        External(CULM_SURPLUS_SUM, &PlantStockModel::_culm_surplus_sum);
    }

    virtual ~PlantStockModel()
    {}

    void compute_IC(double t)
    {
        if (t != _parameters.beginDate) {
            double resDiv, mean;
            double total = 0.;
            int n = 0;

            if (_day_demand_[0] != 0) {
                resDiv = (std::max(0., _seed_res_[0]) + _supply_[0]) /
                        _day_demand_[0];
                total += 2*resDiv;
                n += 2;
            }

            if (_day_demand_[1] != 0) {
                resDiv = (std::max(0., _seed_res_[1]) + _supply_[1]) /
                        _day_demand_[1];
                total += resDiv;
                ++n;
            }

            if (_day_demand_[2] != 0) {
                resDiv = (std::max(0., _seed_res_[2]) + _supply_[2]) /
                        _day_demand_[2];
                total += resDiv;
                ++n;
            }

            if (n != 0) {
                mean = total / n;
            } else {
                mean = _ic_1;
            }

            double tmp = std::min(5., mean);

            _ic_1 = _ic;
            if (tmp == 0 and _seed_res_[0] == 0 and _seed_res_[1] == 0 and
                    _seed_res_[2] == 0) {
                _ic = 0.001;
                _test_ic = 0.001;
            } else {
                _ic = tmp;
                _test_ic = std::min(1., std::sqrt(tmp));
            }
        }

        //Algo florian pour le calcul de l'IC
        //        if (t != _parameters.beginDate) {
        //            double mean;
        //            unsigned int n = 2;
        //            if (_day_demand != 0) {
        //                _ic_[0] = std::max(0., _seed_res + _supply / _day_demand); //@TODO vérifier pourquoi négatif
        //            } else {
        //                _ic_[0] = _ic;
        //            }

        //            mean = 2. * _ic_[0];

        //            for (unsigned int i = 1; i < 3; i++) {
        //                if (_ic_[i] != 0) {
        //                    mean = mean + _ic_[i];
        //                    n = n + 1;
        //                }
        //            }
        //            mean = mean / n;

        //            if (mean == 0) {
        //                _ic = 0.001;
        //                _test_ic = 0.001;
        //            } else {
        //                _ic = std::min(5.,mean);
        //                _test_ic = std::min(1., std::sqrt(_ic));
        //            }
        //        }
    }

    void compute(double t, bool /* update */) {
        //  indice de competition - Proposition
        // compute_IC(t);

        //  day_demand
        if (_plant_phase == plant::NOGROWTH or _plant_phase == plant::NOGROWTH3 or
                _plant_phase == plant::NOGROWTH4) {
            _day_demand = 0;
        } else {
            if (_demand_sum == 0) {
                _day_demand = _leaf_last_demand_sum + _internode_last_demand_sum;
            } else {
                _day_demand = _demand_sum +  _leaf_last_demand_sum + _internode_last_demand_sum;;
            }
        }
        _day_demand_[2] = _day_demand_[1];
        _day_demand_[1] = _day_demand_[0];
        _day_demand_[0] = _day_demand;


        //  seed_res
        if (t == _parameters.beginDate) {
            _seed_res = _gdw - _day_demand;
        } else {
            if (_seed_res > _day_demand) {
                _seed_res = _seed_res - _day_demand;
            } else {
                _seed_res = 0;
            }
        }
        _seed_res_[2] = _seed_res_[1];
        _seed_res_[1] = _seed_res_[0];
        _seed_res_[0] = _seed_res;

        //  supply
        _supply = _assim;
        _supply_[2] = _supply_[1];
        _supply_[1] = _supply_[0];
        _supply_[0] = _supply;


        //  reservoir_dispo @TODO : vérifier s'il faut borner au dessus de 0
        _reservoir_dispo = std::max(0.,_leaf_stock_max * _leaf_biomass_sum - _stock);

        //  stock
        if (_state == plant::ELONG) {
            _stock = _culm_stock;
            _deficit = _culm_deficit;
        } else {
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
        }

        //  surplus
        if (_state == plant::ELONG) {
            _surplus = _culm_surplus_sum;
        } else {
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
        }

        // Realloc biomass
        if (_deleted_leaf_biomass > 0) {
            double qty = _deleted_leaf_biomass * _realocationCoeff;
            _stock = std::max(0., qty + _deficit);
            _deficit = std::min(0., qty + _deficit);
        }
    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        //permet le passage du get à t0 en mimant un isComputed au temps t
        last_time = t-1;

        // parameters
        _parameters = parameters;

        //    parameters variables
        _gdw = _parameters.get < double >("gdw");
        _leaf_stock_max = parameters.get < double >("leaf_stock_max");
        _realocationCoeff = _parameters.get < double >("realocationCoeff");

        //    computed variables (internal)
        _day_demand = 0;
        _ic = 0; //@TODO check initialization value
        _ic_1 = 0; //@TODO check initialization value
        _test_ic = 0; //@TODO check initialization value
        _reservoir_dispo = 0;
        _seed_res = 0;
        _stock = 1e-10; //@TODO check initialization value
        _deficit = 0;
        _supply = 0;
        _surplus = 0;
        for (int i = 0; i < 3; ++i) {
            _ic_[i] = _seed_res_[i] = _supply_[i] = _day_demand_[i] = 0;
        }
    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters
    double _gdw;
    double _leaf_stock_max;
    double _realocationCoeff;

    //    parameters(t)


    //local vars
    double _seed_res_[3];
    double _supply_[3];
    double _day_demand_[3];

    //    internals - computed
    double _day_demand;
    double _ic;
    double _ic_1;
    double _test_ic;
    double _ic_[3];
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
    int _plant_phase;
    double _leaf_biomass_sum;
    double _deleted_leaf_biomass;
    double _realloc_biomass_sum;
    int _state;
    double _culm_stock;
    double _culm_deficit;
    double _culm_surplus_sum;
    double _assim;

};

} // namespace model
#endif
