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

#include <defines.hpp>
#include <plant/processes/ThermalTimeModel.hpp>
#include <plant/processes/WaterBalanceModel.hpp>
#include <plant/processes/PlantStockModel.hpp>
#include <plant/processes/AssimilationModel.hpp>
#include <plant/processes/TilleringModel.hpp>
#include <plant/root/RootModel.hpp>
#include <plant/culm/CulmModel.hpp>

using namespace model;

class PlantModel : public CoupledModel < PlantModel >
{
public:
    enum submodels { THERMAL_TIME, WATER_BALANCE, STOCK, ASSIMILATION,
                     TILLERING, ROOT, CULMS};

    /* LAI, DELTA_T, DD, EDD, IH, LIGULO_VISU, PHENO_STAGE,
                     PLASTO_VISU, TT, TT_LIG, BOOL_CROSSED_PLASTO,
                     ASSIM, CSTR, ROOT_DEMAND_COEF, ROOT_DEMAND,
                     ROOT_BIOMASS, GROW, SUPPLY, DEFICIT, IC,
                     SURPLUS, TEST_IC, DAY_DEMAND, RESERVOIR_DISPO,
                     SEED_RES, */
    enum internals { LIG, LEAF_BIOMASS_SUM, INTERNODE_BIOMASS_SUM,
                     SENESC_DW_SUM, LEAF_LAST_DEMAND_SUM,
                     INTERNODE_LAST_DEMAND_SUM, LEAF_DEMAND_SUM,
                     INTERNODE_DEMAND_SUM,
                     PLANT_PHASE, PLANT_STATE, PAI, HEIGHT, PLASTO, TT_LIG, IH,
                     LEAF_BIOM_STRUCT };

    PlantModel():
        _thermal_time_model(new ThermalTimeModel),
        _water_balance_model(new WaterBalanceModel),
        _stock_model(new PlantStockModel),
        _assimilation_model(new AssimilationModel),
        _tillering_model(new TilleringModel),
        _root_model(new RootModel)
    {
        // submodels
        Submodels( ((THERMAL_TIME, _thermal_time_model.get())) );
        Submodels( ((WATER_BALANCE, _water_balance_model.get())) );
        Submodels( ((STOCK, _stock_model.get())) );
        Submodels( ((ASSIMILATION, _assimilation_model.get())) );
        Submodels( ((TILLERING, _tillering_model.get())) );
        Submodels( ((ROOT, _root_model.get())) );

        // local internals
        Internal(LIG, &PlantModel::_lig);
        Internal( LEAF_BIOMASS_SUM, &PlantModel::_leaf_biomass_sum );
        Internal( LEAF_DEMAND_SUM, &PlantModel::_leaf_demand_sum );
        Internal( LEAF_LAST_DEMAND_SUM, &PlantModel::_leaf_last_demand_sum );
        Internal( INTERNODE_BIOMASS_SUM, &PlantModel::_internode_biomass_sum );
        Internal( INTERNODE_DEMAND_SUM, &PlantModel::_internode_demand_sum );
        Internal( INTERNODE_LAST_DEMAND_SUM, &PlantModel::_internode_last_demand_sum );
        Internal( SENESC_DW_SUM, &PlantModel::_senesc_dw_sum );
        Internal( PAI, &PlantModel::_leaf_blade_area_sum );
        Internal( HEIGHT, &PlantModel::_height );
        Internal( PLANT_STATE, &PlantModel::_state );
        Internal( PLANT_PHASE, &PlantModel::_phase );
        Internal( PLASTO, &PlantModel::_plasto );
        Internal( TT_LIG, &PlantModel::_TT_lig );
        Internal( IH, &PlantModel::_IH );
        Internal( LEAF_BIOM_STRUCT, &PlantModel::_leaf_biom_struct );
    }

    virtual ~PlantModel()
    {

        //        std::vector < culm::CulmModel* >::const_iterator it = culm_models.begin();

        //        while (it != culm_models.end()) {
        //            delete *it;
        //            ++it;
        //        }
    }


    void step_state(double t) {
        int old_phase;

        double nbleaf_pi = _parameters.get < double >("nbleaf_pi");
        double nbleaf_culm_elong = _parameters.get < double >("nb_leaf_stem_elong");
        double stock = _stock_model->get <double> (t-1, PlantStockModel::STOCK);
        double phenoStage = _thermal_time_model->get<int> (t, ThermalTimeModel::PHENO_STAGE);
        double boolCrossedPlasto = _thermal_time_model->get<double> (t, ThermalTimeModel::BOOL_CROSSED_PLASTO);

        do {
            old_phase = (int)_phase;

            switch (_phase) {
            case plant::INIT: {
                _phase = plant::INITIAL;
                _state = plant::VEGETATIVE;
                break;
            }
            case plant::INITIAL: {
                if (stock > 0 and phenoStage < nbleaf_pi) {
                    _phase = plant::GROWTH;
                }
                else {
                    _phase = plant::KILL;
                }
                break;
            }
            case plant::GROWTH: {
                if (boolCrossedPlasto > 0 and stock > 0) {
                    _phase = plant::NEW_PHYTOMER;
                }
                if (stock <= 0) {
                    _phase = plant::NOGROWTH2;
                }
                break;
            }
            case plant::NOGROWTH: {
                if (stock > 0) {
                    _phase = plant::GROWTH;
                }
                break;
            }
            case plant::KILL: break;
            case plant::NEW_PHYTOMER: {
                if (phenoStage == nbleaf_culm_elong) {
                    _state = plant::ELONG;
                }
                _phase = plant::NEW_PHYTOMER3;
                break;
            }
            case plant::NOGROWTH2: {
                _last_time = t;
                _phase = plant::NOGROWTH3;
                break;
            }
            case plant::NOGROWTH3: {
                if (t == _last_time + 1) {
                    _phase = plant::NOGROWTH4;
                }
                break;
            }
            case plant::NOGROWTH4: {
                if (stock > 0) {
                    _phase = plant::GROWTH;
                }
                break;
            }
            case plant::NEW_PHYTOMER3: {
                if (boolCrossedPlasto <= 0) {
                    _phase = plant::GROWTH;
                }
                if (stock <= 0) {
                    _phase = plant::NOGROWTH2;
                }
                break;
            }
            case plant::LIG: break;
            };
        } while (old_phase != _phase);
    }


    void compute(double t, bool /* update */) {
        std::string date = artis::utils::DateTime::toJulianDayFmt(t, artis::utils::DATE_FORMAT_YMD);

        //Compute IC
        _stock_model->compute_IC(t);

        //Thermal time
        _thermal_time_model->put < double >(t, ThermalTimeModel::PLASTO, _plasto);
        _thermal_time_model->put < double >(t, ThermalTimeModel::PLASTO_DELAY, 0); //@TODO voir le plasto delay
        _thermal_time_model->put < int >(t, ThermalTimeModel::PLANT_PHASE, _phase);
        (*_thermal_time_model)(t);

        //Water balance
        _water_balance_model->put < double >(t, WaterBalanceModel::INTERC,
                         _assimilation_model->get < double >(t-1, AssimilationModel::INTERC));
        (*_water_balance_model)(t);

        // Manager
        step_state(t);

        //LLBL - Plasto
        std::deque < CulmModel* >::const_iterator mainstem = _culm_models.begin();

        int nb_leaves = (*mainstem)->get_phytomer_number();
        if ( nb_leaves == _nb_leaf_param2 - 1 and
             _thermal_time_model->get<double> (t, ThermalTimeModel::BOOL_CROSSED_PLASTO) > 0 and
             _stock_model->get <double> (t-1, PlantStockModel::STOCK) > 0)
        {

            _plasto = _plasto * _coeff_Plasto_PI;
            _ligulo = _ligulo * _coeff_Ligulo_PI;
            _LL_BL = _LL_BL_init + _slope_LL_BL_at_PI * (nb_leaves - 1 -_nb_leaf_param2);
            _MGR = _MGR * _coeff_MGR_PI;
        } else if ( nb_leaves >= _nb_leaf_param2 - 1 and
                    _thermal_time_model->get<double> (t, ThermalTimeModel::BOOL_CROSSED_PLASTO) > 0 and
                     nb_leaves < _nb_Leaf_PI + _nb_Leaf_Max_After_PI + 1)
        {
            _LL_BL = _LL_BL_init + _slope_LL_BL_at_PI * (nb_leaves - 1 -_nb_leaf_param2);
        }



        //Phytomer creation
        if(_phase == plant::NEW_PHYTOMER or _phase == plant::NEW_PHYTOMER3) //@TODO virer un état
            create_phytomer(t);

        //Tillering
        _tillering_model->put < double >(t, TilleringModel::IC,
                                         _stock_model->get < double >(t-1, PlantStockModel::IC));
        _tillering_model->put < double >(t, TilleringModel::BOOL_CROSSED_PLASTO,
                                         _thermal_time_model->get < double >(t, ThermalTimeModel::BOOL_CROSSED_PLASTO));

        std::deque < CulmModel* >::const_iterator it = _culm_models.begin();
        double n = 0;
        while(it != _culm_models.end()) {
            if ((*it)->get_phytomer_number() >=
                    _nbleaf_enabling_tillering) {
                ++n;
            }
            it++;
        }
        _tillering_model->put <double> (t, TilleringModel::TAE, n);
        (*_tillering_model)(t);

        //culm creation
        if (_tillering_model->get < double >(t, TilleringModel::CREATE) > 0
                and _tillering_model->get < double >(t, TilleringModel::NB_TILLERS) > 0) {
            create_culm(t, _tillering_model->get < double >(t, TilleringModel::NB_TILLERS));
        }

        //CulmModel
        compute_culms(t);

        //Lig update @TODO : vérifier placement équations TT_lig et IH
        _lig_1 = _lig;
        mainstem = _culm_models.begin();
        _lig = (*mainstem)->get <double, CulmModel>(t, CulmModel::NB_LIG);
        //TT_Lig
        if (t != _parameters.beginDate) {
            if (_lig_1 == _lig) {
                if (_thermal_time_model->get < int >(t, ThermalTimeModel::STATE) == ThermalTimeModel::STOCK_AVAILABLE) {
                    _TT_lig = _TT_lig + _thermal_time_model->get < double >(t, ThermalTimeModel::EDD);
                }
            } else {
                _TT_lig = 0;
            }
        }
        //IH
        if (_thermal_time_model->get < int >(t, ThermalTimeModel::STATE) == ThermalTimeModel::STOCK_AVAILABLE) {
            _IH = _lig + std::min(1., _TT_lig / _thermal_time_model->get < double >(t, ThermalTimeModel::LIGULO_VISU));
        }

        //Assimilation
        _assimilation_model->put < double >(t, AssimilationModel::CSTR,
                                            _water_balance_model->get < double >(t, WaterBalanceModel::CSTR));
        _assimilation_model->put < double >(t, AssimilationModel::FCSTR,
                                            _water_balance_model->get < double >(t, WaterBalanceModel::FCSTR));
        _assimilation_model->put < double >(t, AssimilationModel::PAI, _leaf_blade_area_sum);
        _assimilation_model->put < double >(t, AssimilationModel::LEAFBIOMASS, _leaf_biomass_sum);
        _assimilation_model->put < double >(t, AssimilationModel::INTERNODEBIOMASS, _internode_biomass_sum);
        (*_assimilation_model)(t);

        //Root
        _root_model->put < double >(t, RootModel::LEAF_DEMAND_SUM, _leaf_demand_sum);
        _root_model->put < double >(t, RootModel::LEAF_LAST_DEMAND_SUM, _leaf_last_demand_sum);
        _root_model->put < double >(t, RootModel::INTERNODE_DEMAND_SUM, _internode_demand_sum);
        _root_model->put < double >(t, RootModel::INTERNODE_LAST_DEMAND_SUM, _internode_last_demand_sum);
        _root_model->put < double >(t, RootModel::PLANT_PHASE, _phase);
        _root_model->put < double >(t, RootModel::PLANT_STATE, _state);
        _root_model->put < double >(t, RootModel::CULM_SURPLUS_SUM, _culm_surplus_sum);
        (*_root_model)(t);

//        search_deleted_leaf(t); //on passe avant pour le realloc biomass

        // Stock
         double demand_sum = _leaf_demand_sum + _internode_demand_sum + _root_model->get < double >(t, RootModel::ROOT_DEMAND);
        _stock_model->put < double >(t, PlantStockModel::DEMAND_SUM, demand_sum);
        _stock_model->put < double >(t, PlantStockModel::LEAF_LAST_DEMAND_SUM, _leaf_last_demand_sum);
        _stock_model->put < double >(t, PlantStockModel::INTERNODE_LAST_DEMAND_SUM, _internode_last_demand_sum);
        _stock_model->put < int >(t, PlantStockModel::PLANT_PHASE, _phase);
        _stock_model->put < double >(t, PlantStockModel::LEAF_BIOMASS_SUM, _leaf_biomass_sum);
        _stock_model->put < double >(t, PlantStockModel::DELETED_LEAF_BIOMASS, 0);
        _stock_model->put < double >(t, PlantStockModel::REALLOC_BIOMASS_SUM, _realloc_biomass_sum);
        _stock_model->put < double >(t, PlantStockModel::ASSIM,
                                     _assimilation_model->get < double >(t, AssimilationModel::ASSIM));
        _stock_model->put < double >(t, PlantStockModel::CULM_STOCK, _culm_stock_sum);
        _stock_model->put < double >(t, PlantStockModel::CULM_DEFICIT, _culm_deficit_sum);
        _stock_model->put < double >(t, PlantStockModel::CULM_SURPLUS_SUM, _culm_surplus_sum);
        _stock_model->put < int >(t, PlantStockModel::PLANT_STATE, _state);
        (*_stock_model)(t);

        _leaf_biom_struct = _leaf_biomass_sum + _stock_model->get < double >(t,PlantStockModel::STOCK);

        compute_height(t);
    }


    void create_culm(double t, int n)
    {
        for (int i = 0; i < n; ++i) {
            CulmModel* meristem = new CulmModel(_culm_models.size() + 1);
            setsubmodel(CULMS, meristem);
            meristem->init(t, _parameters);
            _culm_models.push_back(meristem);
        }
    }

    void create_phytomer(double t)
    {
        std::deque < CulmModel* >::const_iterator it = _culm_models.begin();
        while (it != _culm_models.end()) {
            (*it)->create_phytomer(t);
            ++it;
        }
    }

    void compute_culms(double t)
    {
        std::deque < CulmModel* >::const_iterator it = _culm_models.begin();
        while (it != _culm_models.end()) {
            (*it)->put(t, CulmModel::DD, _thermal_time_model->get < double >(t, ThermalTimeModel::DD));
            (*it)->put(t, CulmModel::DELTA_T, _thermal_time_model->get < double >(t, ThermalTimeModel::DELTA_T));
            (*it)->put(t, CulmModel::FTSW, _water_balance_model->get < double >(t, WaterBalanceModel::FTSW));
            (*it)->put(t, CulmModel::FCSTR, _water_balance_model->get < double >(t, WaterBalanceModel::FCSTR));
            (*it)->put < int > (t, CulmModel::PHENO_STAGE, _thermal_time_model->get < int >(t, ThermalTimeModel::PHENO_STAGE));
            (*it)->put(t, CulmModel::PREDIM_LEAF_ON_MAINSTEM, _predim_leaf_on_mainstem);
            (*it)->put(t, CulmModel::SLA, _thermal_time_model->get < double >(t, ThermalTimeModel::SLA));
            (*it)->put < int >(t, CulmModel::PLANT_PHASE, _phase);
            (*it)->put < int >(t, CulmModel::PLANT_STATE, _state);
            (*it)->put(t, CulmModel::TEST_IC, _stock_model->get < double >(t-1, PlantStockModel::TEST_IC));
            (*it)->put(t, CulmModel::PLANT_STOCK, _stock_model->get < double >(t-1, PlantStockModel::STOCK));
            (*it)->put(t, CulmModel::PLANT_DEFICIT, _stock_model->get < double >(t-1, PlantStockModel::DEFICIT));
            (*it)->put(t, CulmModel::PLANT_BIOMASS_SUM, _leaf_biomass_sum + _internode_biomass_sum);
            (*it)->put(t, CulmModel::PLANT_LEAF_BIOMASS_SUM, _leaf_biomass_sum);
            (*it)->put(t, CulmModel::PLANT_BLADE_AREA_SUM, _leaf_blade_area_sum);
            (*it)->put(t, CulmModel::ASSIM, _assimilation_model->get < double >(t-1, AssimilationModel::ASSIM));
            (*it)->put(t, CulmModel::LL_BL, _LL_BL);
            (*it)->put(t, CulmModel::PLASTO, _plasto);
            (*it)->put(t, CulmModel::LIGULO, _ligulo);
            (*it)->put(t, CulmModel::MGR, _MGR);
            (**it)(t);
            ++it;
        }

        _leaf_biomass_sum = 0;
        _leaf_last_demand_sum = 0;
        _leaf_demand_sum = 0;
        _internode_last_demand_sum = 0;
        _internode_demand_sum = 0;
        _internode_biomass_sum = 0;
        _leaf_blade_area_sum = 0;
        _realloc_biomass_sum = 0;
        _senesc_dw_sum = 0;
        _culm_stock_sum = 0;
        _culm_deficit_sum = 0;
        _culm_surplus_sum = 0;

        it = _culm_models.begin();
        _predim_leaf_on_mainstem = (*it)->get <double, CulmModel> (t, CulmModel::STEM_LEAF_PREDIM);

        while (it != _culm_models.end()) {
            _leaf_biomass_sum += (*it)->get < double, CulmModel >(t, CulmModel::LEAF_BIOMASS_SUM);
            _leaf_last_demand_sum += (*it)->get < double, CulmModel>(t, CulmModel::LEAF_LAST_DEMAND_SUM);
            _leaf_demand_sum += (*it)->get < double, CulmModel >(t, CulmModel::LEAF_DEMAND_SUM);
            _internode_last_demand_sum += (*it)->get < double, CulmModel >(t, CulmModel::INTERNODE_LAST_DEMAND_SUM);
            _internode_demand_sum += (*it)->get < double, CulmModel >(t, CulmModel::INTERNODE_DEMAND_SUM);
            _internode_biomass_sum += (*it)->get < double, CulmModel >(t, CulmModel::INTERNODE_BIOMASS_SUM);
            _leaf_blade_area_sum += (*it)->get < double, CulmModel>(t, CulmModel::LEAF_BLADE_AREA_SUM);
            _realloc_biomass_sum += (*it)->get < double, CulmModel>(t, CulmModel::REALLOC_BIOMASS_SUM);
            _senesc_dw_sum += (*it)->get < double, CulmModel>(t, CulmModel::SENESC_DW_SUM);
            _culm_stock_sum += (*it)->get < double, CulmStockModel >(t, CulmModel::STOCK);
            _culm_deficit_sum += (*it)->get < double, CulmStockModel >(t, CulmModel::DEFICIT);
            _culm_surplus_sum += (*it)->get < double, CulmStockModel >(t, CulmModel::SURPLUS);
            ++it;
        }
    }

    void compute_height(double t)
    {
        _height = 0;
        if (_culm_models.empty()) {
            return;
        }

        auto it = _culm_models.begin();
        _height += (*it)->get < double, CulmModel >(t, CulmModel::INTERNODE_LEN_SUM);
        _height += (1 - 1 / _LL_BL) * (*it)->get < double, CulmModel >(t, CulmModel::LAST_LIGULATED_LEAF_LEN);
    }

    void init(double t, const ecomeristem::ModelParameters& parameters)
    {
        //parameters
        _parameters = parameters;
        _nbleaf_enabling_tillering =
                parameters.get < double >("nb_leaf_enabling_tillering");
//        _realocationCoeff = parameters.get < double >("realocationCoeff");
        _LL_BL_init = parameters.get < double >("LL_BL_init");
        _nb_leaf_param2 = parameters.get < double >("nb_leaf_param2");
        _slope_LL_BL_at_PI = parameters.get < double >("slope_LL_BL_at_PI");
        _nb_Leaf_PI = parameters.get < double >("nbleaf_pi");
        _nb_Leaf_Max_After_PI = parameters.get < double >("nb_leaf_max_after_PI");
        _coef_ligulo = parameters.get < double >("coef_ligulo1");
        _coeff_Plasto_PI = parameters.get < double >("coef_plasto_PI");
        _coeff_Ligulo_PI = parameters.get < double >("coef_ligulo_PI");
        _coeff_MGR_PI = parameters.get < double >("coef_MGR_PI");

        //local init
        CulmModel* meristem = new CulmModel(1);
        setsubmodel(CULMS, meristem);
        meristem->init(t, parameters);
        _culm_models.push_back(meristem);

        //submodels
        _thermal_time_model->init(t, parameters);
        _water_balance_model->init(t, parameters);
        _stock_model->init(t, parameters);
        _assimilation_model->init(t, parameters);
        _tillering_model->init(t, parameters);
        _root_model->init(t, parameters);

        //vars
        _predim_leaf_on_mainstem = 0;

        //internal variables (local)
        _lig = 0;
        _lig_1 = 0;
        _leaf_biomass_sum = 0;
        _leaf_demand_sum = 0;
        _leaf_last_demand_sum = 0;
        _internode_demand_sum = 0;
        _internode_last_demand_sum = 0;
        _internode_biomass_sum = 0;
        _leaf_blade_area_sum = 0;
        _senesc_dw_sum = 0;
        _culm_stock_sum = 0;
        _culm_deficit_sum = 0;
        _culm_surplus_sum = 0;
        _state = plant::VEGETATIVE;
        _phase = plant::INIT;
        _height = 0;
        _LL_BL = _LL_BL_init;
        _plasto = parameters.get < double >("plasto_init");
        _ligulo = _plasto * _coef_ligulo;
        _MGR = parameters.get < double >("MGR_init");
        _TT_lig = 0;
        _IH = 0;
        _leaf_biom_struct = 0;

        //
        _last_time = 0;
    }
    bool is_dead() const
    { /*return not culm_models.empty() and culm_models[0]->is_dead();*/ }


private:
    //@TODO vérif
    double _last_time;

    ecomeristem::ModelParameters _parameters;
    // submodels
    std::deque < CulmModel* > _culm_models;
    std::unique_ptr < model::ThermalTimeModel > _thermal_time_model;
    std::unique_ptr < model::WaterBalanceModel > _water_balance_model;
    std::unique_ptr < model::PlantStockModel > _stock_model;
    std::unique_ptr < model::AssimilationModel > _assimilation_model;
    std::unique_ptr < model::TilleringModel > _tillering_model;
    std::unique_ptr < model::RootModel > _root_model;

    // parameters
    double _coeff_Plasto_PI;
    double _coeff_Ligulo_PI;
    double _coeff_MGR_PI;
    double _nbleaf_enabling_tillering;
    double _nb_leaf_param2;
    double _slope_LL_BL_at_PI;
    double _nb_Leaf_PI;
    double _nb_Leaf_Max_After_PI;
    double _LL_BL_init;
    double _coef_ligulo;

    // vars
    double _predim_leaf_on_mainstem;
    // internals
    double _plasto;
    double _ligulo;
    double _MGR;
    double _LL_BL;
    double _lig;
    double _leaf_biomass_sum;
    double _leaf_demand_sum;
    double _leaf_blade_area_sum;
    double _leaf_last_demand_sum;
    double _internode_biomass_sum;
    double _internode_demand_sum;
    double _internode_last_demand_sum;
    double _senesc_dw_sum;
    double _realloc_biomass_sum;
    double _culm_stock_sum;
    double _culm_deficit_sum;
    double _culm_surplus_sum;
    double _height;
    double _lig_1;
    double _TT_lig;
    double _IH;
    double _leaf_biom_struct;

    //internal states
    int _phase;
    int _state;

    //    double _demand_sum;
    //    bool _culm_is_computed;
    //    double _lig;
    //    double _deleted_leaf_biomass;
    //    double _deleted_leaf_blade_area;
};

#endif //PLANT_MODEL_HPP
