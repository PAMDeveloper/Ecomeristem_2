#ifndef CULMSTOCKMODEL_H
#define CULMSTOCKMODEL_H

#include <defines.hpp>

namespace model {

class CulmStockModel : public AtomicModel < CulmStockModel >
{
public:
    enum internals { STOCK, SUPPLY, MAX_RESERVOIR_DISPO, INTERMEDIATE, DEFICIT, SURPLUS, FIRST_DAY };

    enum externals { ASSIM, LEAF_BIOMASS_SUM, PLANT_LEAF_BIOMASS_SUM,
                     INTERNODE_BIOMASS_SUM, PLANT_BIOMASS_SUM, PLANT_STOCK,
                     PLANT_DEFICIT, INTERNODE_DEMAND_SUM, LEAF_DEMAND_SUM,
                     INTERNODE_LAST_DEMAND_SUM, LEAF_LAST_DEMAND_SUM,
                     REALLOC_BIOMASS_SUM, PLANT_STATE};


    CulmStockModel() {
        Internal(STOCK, &CulmStockModel::_stock);
        Internal(SUPPLY, &CulmStockModel::_supply);
        Internal(MAX_RESERVOIR_DISPO, &CulmStockModel::_max_reservoir_dispo);
        Internal(DEFICIT, &CulmStockModel::_deficit);
        Internal(SURPLUS, &CulmStockModel::_surplus);
        Internal(FIRST_DAY, &CulmStockModel::_first_day);
        Internal(INTERMEDIATE, &CulmStockModel::_intermediate);

        External(ASSIM, &CulmStockModel::_assim);
        External(LEAF_BIOMASS_SUM, &CulmStockModel::_leaf_biomass_sum);
        External(PLANT_LEAF_BIOMASS_SUM, &CulmStockModel::_plant_leaf_biomass_sum);
        External(INTERNODE_BIOMASS_SUM,
                 &CulmStockModel::_internode_biomass_sum);
        External(PLANT_BIOMASS_SUM, &CulmStockModel::_plant_biomass_sum);
        External(PLANT_STOCK, &CulmStockModel::_plant_stock);
        External(PLANT_DEFICIT, &CulmStockModel::_plant_deficit);
        External(INTERNODE_DEMAND_SUM, &CulmStockModel::_internode_demand_sum);
        External(LEAF_DEMAND_SUM, &CulmStockModel::_leaf_demand_sum);
        External(INTERNODE_LAST_DEMAND_SUM,
                 &CulmStockModel::_internode_last_demand_sum);
        External(LEAF_LAST_DEMAND_SUM, &CulmStockModel::_leaf_last_demand_sum);
        External(REALLOC_BIOMASS_SUM, &CulmStockModel::_realloc_biomass_sum);
        External(PLANT_STATE, &CulmStockModel::_plant_state);

    }

    virtual ~CulmStockModel()
    {}


    void compute(double t, bool /* update */) {
        if (_plant_state != plant::ELONG) {
            return;
        }

        //MaxReservoirDispo
        _max_reservoir_dispo = _maximum_reserve_in_internode *
                _internode_biomass_sum + _leaf_stock_max * _leaf_biomass_sum;

        //CulmSupply
        _supply = _assim * _leaf_biomass_sum / _plant_leaf_biomass_sum;

        //Intermediate
        double stock = _plant_stock *
                       (_leaf_biomass_sum + _internode_biomass_sum) /
                       _plant_biomass_sum;
        double deficit = _plant_deficit *
                        (_leaf_biomass_sum + _internode_biomass_sum) /
                        _plant_biomass_sum;

        _intermediate = stock + deficit + _supply - _internode_demand_sum -
                _leaf_demand_sum - _leaf_last_demand_sum -
                _internode_last_demand_sum + _realloc_biomass_sum;

        //Deficit
        _deficit = std::min(0., _intermediate);

        //CulmSurplus
        if (_plant_state == plant::ELONG) {
            if (_first_day == t) {
                _surplus = std::max(0., _plant_stock - _internode_demand_sum -
                                    _leaf_demand_sum - _leaf_last_demand_sum -
                                    _internode_last_demand_sum + _supply -
                                    _max_reservoir_dispo +
                                    _realloc_biomass_sum);
            } else {
                _surplus = std::max(0., _stock - _internode_demand_sum -
                                    _leaf_demand_sum - _leaf_last_demand_sum -
                                    _internode_last_demand_sum + _supply -
                                    _max_reservoir_dispo +
                                    _realloc_biomass_sum);
            }
        } else {
            _surplus = 0;
        }


        //CulmStock
        _stock = std::max(0., std::min(_max_reservoir_dispo, _intermediate));
    }


    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;
        //    parameters variables
        _maximum_reserve_in_internode =
                parameters.get < double >("maximumReserveInInternode");
        _leaf_stock_max = parameters.get < double >("leaf_stock_max");
        _realocationCoeff = parameters.get < double >("realocationCoeff");

        //    computed variables (internal)
        _stock = 0;
        _supply = 0;
        _max_reservoir_dispo = 0;
        _intermediate = 0;
        _deficit = 0;
        _surplus = 0;
        _first_day = t;
    }

private:
    ecomeristem::ModelParameters _parameters;

    // parameters
    double _maximum_reserve_in_internode;
    double _leaf_stock_max;
    double _realocationCoeff;

    //    internals - computed
    double _first_day;
    double _stock;
    double _supply;
    double _max_reservoir_dispo;
    double _intermediate;
    double _deficit;
    double _surplus;

    //    externals
    int    _plant_state;
    double _assim;
    double _leaf_biomass_sum;
    double _internode_biomass_sum;
    double _plant_leaf_biomass_sum;
    double _plant_biomass_sum;
    double _plant_stock;
    double _plant_deficit;
    double _internode_demand_sum;
    double _leaf_demand_sum;
    double _internode_last_demand_sum;
    double _leaf_last_demand_sum;
    double _realloc_biomass_sum;
};

} // namespace model

#endif // CULMSTOCKMODEL_H
