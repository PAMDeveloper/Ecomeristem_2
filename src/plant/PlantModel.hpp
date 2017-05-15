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
#include <QDebug>
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
                     LEAF_BIOM_STRUCT, REALLOC_BIOMASS_SUM };

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
        Internal( PLANT_PHASE, &PlantModel::_plant_phase );
        Internal( PLANT_STATE, &PlantModel::_plant_state );
        Internal( PLASTO, &PlantModel::_plasto );
        Internal( TT_LIG, &PlantModel::_TT_lig );
        Internal( IH, &PlantModel::_IH );
        Internal( LEAF_BIOM_STRUCT, &PlantModel::_leaf_biom_struct );
        Internal( REALLOC_BIOMASS_SUM, &PlantModel::_realloc_biomass_sum );

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

        double stock = _stock_model->get <double> (t-1, PlantStockModel::STOCK);
        double ic = _stock_model->get <double> (t-1, PlantStockModel::IC);
        double phenostage = _thermal_time_model->get<int> (t, ThermalTimeModel::PHENO_STAGE);
        double bool_crossed_plasto = _thermal_time_model->get<double> (t, ThermalTimeModel::BOOL_CROSSED_PLASTO);
        double FTSW  = _water_balance_model->get<double> (t, WaterBalanceModel::FTSW);

        if (FTSW <= 0 or ic <= -1) {
            _plant_state = plant::KILL;
            _plant_phase = plant::DEAD;
            return;
        }

        //Globals states
        _plant_state >> plant::NEW_PHYTOMER;
        if (stock <= 0) {
            _plant_state << plant::NOGROWTH;
            return;
        } else {
            _plant_state >> plant::NOGROWTH;
        }

        switch (_plant_phase) {
        case plant::INITIAL: {
            _plant_phase = plant::VEGETATIVE;
            break;
        }
        case plant::VEGETATIVE: {
            if ( bool_crossed_plasto >= 0) {
                _plant_state << plant::NEW_PHYTOMER;

                if ( phenostage == _nb_leaf_stem_elong and phenostage < _nb_leaf_pi - 1) {
                    _plant_phase = plant::ELONG;
                } else if(phenostage == _nb_leaf_pi - 1) {
                    _plant_phase = plant::PI;
                }
            }
            break;
        }
        case plant::ELONG: {
            if (bool_crossed_plasto >= 0) {
                _plant_state << plant::NEW_PHYTOMER;
                if (phenostage == _nb_leaf_pi - 1) {
                    _plant_phase = plant::PI;
                }
            }
            break;
        }
        case plant::PI: {
            if (bool_crossed_plasto >= 0) {
                if (phenostage < _nb_leaf_pi + _nb_leaf_max_after_pi) {
                    _plant_state << plant::NEW_PHYTOMER;
                } else if (phenostage > _nb_leaf_pi + _nb_leaf_max_after_pi) {
                    _plant_phase = plant::PRE_FLO;
                }
            }
            break;
        }
        case plant::PRE_FLO: {
            if (phenostage == _nb_leaf_pi + _nb_leaf_max_after_pi + 1 + _phenostage_pre_flo_to_flo) {
                _plant_phase = plant::FLO;
            }
            break;
        }
        case plant::FLO: {
            if (phenostage == _phenostage_to_end_filling) {
                _plant_phase = plant::END_FILLING;
            }
            break;
        }
        case plant::END_FILLING: {
            if (phenostage == _phenostage_to_maturity) {
                _plant_phase = plant::MATURITY;
            }
            break;
        }}
    }


    void compute(double t, bool /* update */) {
        std::string date = artis::utils::DateTime::toJulianDayFmt(t, artis::utils::DATE_FORMAT_YMD);

        //Compute IC
        _stock_model->compute_IC(t);

        //Thermal time
        _thermal_time_model->put < double >(t, ThermalTimeModel::PLASTO, _plasto);
        _thermal_time_model->put < double >(t, ThermalTimeModel::PLASTO_DELAY, 0); //@TODO voir le plasto delay
        _thermal_time_model->put < plant::plant_state >(t, ThermalTimeModel::PLANT_STATE, _plant_state);
        (*_thermal_time_model)(t);

        //Water balance
        _water_balance_model->put < double >(t, WaterBalanceModel::INTERC,
                         _assimilation_model->get < double >(t-1, AssimilationModel::INTERC));
        (*_water_balance_model)(t);

        // Manager
        qDebug() << "BEFORE" << QString::fromStdString(date) << "state:" << _plant_state << " - phase:" << _plant_phase;
        step_state(t);
        qDebug() << "AFTER" << QString::fromStdString(date) << "state:" << _plant_state << " - phase:" << _plant_phase;

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
        if(_plant_state & plant::NEW_PHYTOMER)
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
        _root_model->put < plant::plant_state >(t, RootModel::PLANT_STATE, _plant_state);
        _root_model->put < plant::plant_phase >(t, RootModel::PLANT_PHASE, _plant_phase);
        _root_model->put < double >(t, RootModel::CULM_SURPLUS_SUM, _culm_surplus_sum);
        (*_root_model)(t);

//        search_deleted_leaf(t); //on passe avant pour le realloc biomass

        // Stock
         double demand_sum = _leaf_demand_sum + _internode_demand_sum + _root_model->get < double >(t, RootModel::ROOT_DEMAND);
        _stock_model->put < double >(t, PlantStockModel::DEMAND_SUM, demand_sum);
        _stock_model->put < double >(t, PlantStockModel::LEAF_LAST_DEMAND_SUM, _leaf_last_demand_sum);
        _stock_model->put < double >(t, PlantStockModel::INTERNODE_LAST_DEMAND_SUM, _internode_last_demand_sum);
        _stock_model->put < plant::plant_state >(t, PlantStockModel::PLANT_STATE, _plant_state);
        _stock_model->put < double >(t, PlantStockModel::LEAF_BIOMASS_SUM, _leaf_biomass_sum);
        _stock_model->put < double >(t, PlantStockModel::DELETED_LEAF_BIOMASS, 0);
        _stock_model->put < double >(t, PlantStockModel::REALLOC_BIOMASS_SUM, _realloc_biomass_sum);
        _stock_model->put < double >(t, PlantStockModel::ASSIM,
                                     _assimilation_model->get < double >(t, AssimilationModel::ASSIM));
        _stock_model->put < double >(t, PlantStockModel::CULM_STOCK, _culm_stock_sum);
        _stock_model->put < double >(t, PlantStockModel::CULM_DEFICIT, _culm_deficit_sum);
        _stock_model->put < double >(t, PlantStockModel::CULM_SURPLUS_SUM, _culm_surplus_sum);
        _stock_model->put < plant::plant_phase >(t, PlantStockModel::PLANT_PHASE, _plant_phase);
        (*_stock_model)(t);

        _leaf_biom_struct = _leaf_biomass_sum + _stock_model->get < double >(t,PlantStockModel::STOCK);

        compute_height(t);
    }


    void create_culm(double t, int n)
    {
        for (int i = 0; i < n; ++i) {
            CulmModel* meristem = new CulmModel(_culm_models.size() + 1, _plasto, _ligulo, _LL_BL);
            setsubmodel(CULMS, meristem);
            meristem->init(t, _parameters);
            _culm_models.push_back(meristem);
        }
    }

    void create_phytomer(double t)
    {
        std::deque < CulmModel* >::const_iterator it = _culm_models.begin();
        while (it != _culm_models.end()) {
            (*it)->create_phytomer(t, _plasto, _ligulo, _LL_BL);
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
            (*it)->put < plant::plant_state >(t, CulmModel::PLANT_STATE, _plant_state);
            (*it)->put < plant::plant_phase >(t, CulmModel::PLANT_PHASE, _plant_phase);
            (*it)->put(t, CulmModel::TEST_IC, _stock_model->get < double >(t-1, PlantStockModel::TEST_IC));
            (*it)->put(t, CulmModel::PLANT_STOCK, _stock_model->get < double >(t-1, PlantStockModel::STOCK));
            (*it)->put(t, CulmModel::PLANT_DEFICIT, _stock_model->get < double >(t-1, PlantStockModel::DEFICIT));
            (*it)->put(t, CulmModel::PLANT_BIOMASS_SUM, _leaf_biomass_sum + _internode_biomass_sum);
            (*it)->put(t, CulmModel::PLANT_LEAF_BIOMASS_SUM, _leaf_biomass_sum);
            (*it)->put(t, CulmModel::PLANT_BLADE_AREA_SUM, _leaf_blade_area_sum);
            (*it)->put(t, CulmModel::ASSIM, _assimilation_model->get < double >(t-1, AssimilationModel::ASSIM));
            (*it)->put(t, CulmModel::MGR, _MGR);
            (**it)(t);
            ++it;
        }

        _leaf_biomass_sum = 0;
        _last_leaf_biomass_sum = 0;
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
            _last_leaf_biomass_sum += (*it)->get < double, CulmModel >(t, CulmModel::LAST_LEAF_BIOMASS_SUM);
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
        /* height delphi, proposition à vérifer
         _height = 0;
         if (_culm_models.empty()) {
            return;
         }

         auto it = _culm_models.begin();
         _height += (*it)->get < double, CulmModel >(t, CulmModel::INTERNODE_LEN_SUM);
         if ((*it)->_phytomer_models.size() == 1) {
            _height += (1 - 1 / _LL_BL) * (*it)->get < double, CulmModel >(t, CulmModel::FIRST_LEAF_LEN);
         } else {
             _height += (1 - 1 / _LL_BL) * (*it)->get < double, CulmModel >(t, CulmModel::LAST_LIGULATED_LEAF_LEN);
         }
        end */

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
        _nb_leaf_pi = _parameters.get < double >("nbleaf_pi");
        _nb_leaf_stem_elong = _parameters.get < double >("nb_leaf_stem_elong");
        _nb_leaf_max_after_pi = _parameters.get < double >("nb_leaf_max_after_PI");
        _phenostage_pre_flo_to_flo  = _parameters.get < double >("phenostage_PRE_FLO_to_FLO");
        _phenostage_to_end_filling = _parameters.get < double >("phenostage_to_end_filling");
        _phenostage_to_maturity = _parameters.get < double >("phenostage_to_maturity");

        //Attributes for culmmodel
        _plasto = parameters.get < double >("plasto_init");
        _ligulo = _plasto * _coef_ligulo;
        _LL_BL = _LL_BL_init;

        //local init
        CulmModel* meristem = new CulmModel(1, _plasto, _ligulo, _LL_BL);
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
        _plant_phase = plant::VEGETATIVE;
        _plant_state = plant::NO_STATE;
        _height = 0;
        _MGR = parameters.get < double >("MGR_init");
        _TT_lig = 0;
        _IH = 0;
        _leaf_biom_struct = 0;
        _last_leaf_biomass_sum = 0;

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
    double _nb_leaf_pi;
    double _nb_leaf_stem_elong;
    double _nb_leaf_max_after_pi;
    double _phenostage_pre_flo_to_flo;
    double _phenostage_to_end_filling;
    double _phenostage_to_maturity;

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
    double _last_leaf_biomass_sum;

    //internal states
    plant::plant_state _plant_state;
    plant::plant_phase _plant_phase;

    //    double _demand_sum;
    //    bool _culm_is_computed;
    //    double _lig;
    //    double _deleted_leaf_biomass;
    //    double _deleted_leaf_blade_area;
};

#endif //PLANT_MODEL_HPP
