#ifndef CULMSTOCKMODEL_H
#define CULMSTOCKMODEL_H

#include <defines.hpp>

namespace model {

class CulmStockModel : public AtomicModel < CulmStockModel >
{
public:
    enum internals { STOCK, SUPPLY, MAX_RESERVOIR_DISPO, INTERMEDIATE, DEFICIT, SURPLUS,
                     FIRST_DAY, STOCK_LEAF_CULM, LEAF_STOCK, IN_STOCK, IN_DEFICIT };

    enum externals { ASSIM, LEAF_BIOMASS_SUM, PLANT_LEAF_BIOMASS_SUM,
                     INTERNODE_BIOMASS_SUM, PLANT_BIOMASS_SUM, PLANT_STOCK,
                     PLANT_DEFICIT, INTERNODE_DEMAND_SUM, LEAF_DEMAND_SUM,
                     INTERNODE_LAST_DEMAND_SUM, LEAF_LAST_DEMAND_SUM,
                     REALLOC_BIOMASS_SUM, PLANT_PHASE, CULM_PHASE,
                     PANICLE_DAY_DEMAND, PANICLE_WEIGHT, LAST_LEAF_BIOMASS_SUM,
                     LAST_PLANT_LEAF_BIOMASS_SUM, IS_FIRST_DAY_PI };


    CulmStockModel() {
        Internal(STOCK, &CulmStockModel::_stock);
        Internal(SUPPLY, &CulmStockModel::_supply);
        Internal(MAX_RESERVOIR_DISPO, &CulmStockModel::_max_reservoir_dispo);
        Internal(DEFICIT, &CulmStockModel::_deficit);
        Internal(SURPLUS, &CulmStockModel::_surplus);
        Internal(FIRST_DAY, &CulmStockModel::_first_day);
        Internal(INTERMEDIATE, &CulmStockModel::_intermediate);
        Internal(STOCK_LEAF_CULM, &CulmStockModel::_stock_leaf_culm);
        Internal(LEAF_STOCK, &CulmStockModel::_leaf_stock);
        Internal(IN_STOCK, &CulmStockModel::_in_stock);
        Internal(IN_DEFICIT, &CulmStockModel::_in_deficit);

        External(PLANT_BIOMASS_SUM, &CulmStockModel::_plant_biomass_sum);
        External(LAST_PLANT_LEAF_BIOMASS_SUM, &CulmStockModel::_last_plant_biomass_sum);
        External(PLANT_LEAF_BIOMASS_SUM, &CulmStockModel::_plant_leaf_biomass_sum);

        External(LAST_LEAF_BIOMASS_SUM, &CulmStockModel::_last_leaf_biomass_sum);
        External(ASSIM, &CulmStockModel::_assim);
        External(LEAF_BIOMASS_SUM, &CulmStockModel::_leaf_biomass_sum);
        External(INTERNODE_BIOMASS_SUM,&CulmStockModel::_internode_biomass_sum);
        External(PLANT_STOCK, &CulmStockModel::_plant_stock);
        External(PLANT_DEFICIT, &CulmStockModel::_plant_deficit);
        External(INTERNODE_DEMAND_SUM, &CulmStockModel::_internode_demand_sum);
        External(LEAF_DEMAND_SUM, &CulmStockModel::_leaf_demand_sum);
        External(INTERNODE_LAST_DEMAND_SUM,&CulmStockModel::_internode_last_demand_sum);
        External(LEAF_LAST_DEMAND_SUM, &CulmStockModel::_leaf_last_demand_sum);
        External(REALLOC_BIOMASS_SUM, &CulmStockModel::_realloc_biomass_sum);
        External(PLANT_PHASE, &CulmStockModel::_plant_phase);
        External(CULM_PHASE, &CulmStockModel::_culm_phase);
        External(PANICLE_DAY_DEMAND, &CulmStockModel::_panicle_day_demand);
        External(PANICLE_WEIGHT, &CulmStockModel::_panicle_weight);
        External(IS_FIRST_DAY_PI, &CulmStockModel::_is_first_day_pi);

    }

    virtual ~CulmStockModel()
    {}


    void compute(double t, bool /* update */) {
        if (_plant_phase == plant::INITIAL or _plant_phase == plant::VEGETATIVE) {
            _surplus = 0;
            return;
        }

        //stockleafculm
        //@TODO : erreur en delphi, ((_last)_plant_biomass_sum à corriger
        if(_is_first_day_pi) {
            _leaf_stock = _plant_stock * (_last_leaf_biomass_sum / _last_plant_biomass_sum);
            _in_stock = _plant_stock * (_internode_biomass_sum / _last_plant_biomass_sum);
            _in_deficit = _plant_deficit * (_internode_biomass_sum / _last_plant_biomass_sum);
        } else {
            _leaf_stock = _plant_stock * (_leaf_biomass_sum / _plant_biomass_sum);
            _in_stock = _plant_stock * (_internode_biomass_sum / _plant_biomass_sum);
            _in_deficit = _plant_deficit * (_internode_biomass_sum / _plant_biomass_sum);
        }

        _stock_leaf_culm = std::min(_leaf_stock_max * _leaf_biomass_sum,
                                    _leaf_stock - _in_stock + _in_deficit -
                                    (_leaf_demand_sum + _internode_demand_sum
                                     + _leaf_last_demand_sum + _internode_last_demand_sum));

        //MaxReservoirDispo
        _max_reservoir_dispo = (_maximum_reserve_in_internode *
                                _internode_biomass_sum) + (_leaf_stock_max * _leaf_biomass_sum);

        //Intermediate
        double stock = _plant_stock *
                (_leaf_biomass_sum + _internode_biomass_sum) /
                _plant_biomass_sum;
        double deficit = _plant_deficit *
                (_leaf_biomass_sum + _internode_biomass_sum) /
                _plant_biomass_sum;

        //CulmSupply
        _supply = _assim * _leaf_biomass_sum / _plant_leaf_biomass_sum;


        _intermediate = stock + deficit + _supply - _internode_demand_sum -
                _leaf_demand_sum - _leaf_last_demand_sum - _panicle_day_demand -
                _internode_last_demand_sum + _realloc_biomass_sum;

        //Deficit
        _deficit = std::min(0., _intermediate);

        //CulmSurplus
        if (_first_day == t) {
            _surplus = std::max(0., _plant_stock - _internode_demand_sum -
                                _leaf_demand_sum - _leaf_last_demand_sum -
                                _internode_last_demand_sum + _supply -
                                _max_reservoir_dispo + _realloc_biomass_sum);
        } else {
            _surplus = std::max(0., _stock - _internode_demand_sum -
                                _leaf_demand_sum - _leaf_last_demand_sum -
                                _internode_last_demand_sum - _panicle_day_demand
                                + _supply - _max_reservoir_dispo +
                                _realloc_biomass_sum);
        }


        //CulmStock
        //@TODO : n'est fait qu'à partir de PI
        //Avant Pi ("pre-pi" en delphi) stock = sum leaf_stock
        if(_culm_phase != culm::INITIAL and _culm_phase != culm::VEGETATIVE) {
            _stock = std::max(0., std::min(_max_reservoir_dispo, _intermediate));
        } else {
            _stock = _leaf_stock + _in_stock;
        }
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
        _stock_leaf_culm = 0;
        _first_day = t;
        _leaf_stock = 0;
        _in_stock = 0;
        _in_deficit = 0;
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
    double _stock_leaf_culm;
    double _leaf_stock;
    double _in_stock;
    double _in_deficit;

    //    externals
    plant::plant_phase _plant_phase;
    culm::culm_phase _culm_phase;
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
    double _panicle_day_demand;
    double _panicle_weight;
    double _last_leaf_biomass_sum;
    double _last_plant_biomass_sum;
    bool _is_first_day_pi;

};

} // namespace model

#endif // CULMSTOCKMODEL_H
