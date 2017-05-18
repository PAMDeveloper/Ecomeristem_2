/**
 * @file ecomeristem/plant/thermal-time/Model.hpp
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


#ifndef SAMPLE_ATOMIC_MODEL_HPP
#define SAMPLE_ATOMIC_MODEL_HPP

#include <defines.hpp>
#include <plant/processes/CulmStockModel.hpp>
#include <plant/phytomer/PhytomerModel.hpp>

namespace model {

class CulmModel : public CoupledModel < CulmModel >
{
public:

    enum submodels { CULM_STOCK, PHYTOMERS };


    enum internals { STOCK, DEFICIT, SURPLUS, NB_LIG, STEM_LEAF_PREDIM,
                     LEAF_BIOMASS_SUM, LEAF_LAST_DEMAND_SUM, LEAF_DEMAND_SUM,
                     INTERNODE_DEMAND_SUM, INTERNODE_LAST_DEMAND_SUM,
                     INTERNODE_BIOMASS_SUM, INTERNODE_LEN_SUM,
                     LEAF_BLADE_AREA_SUM,
                     REALLOC_BIOMASS_SUM, SENESC_DW_SUM,
                     LAST_LIGULATED_LEAF, LAST_LIGULATED_LEAF_LEN,
                     LAST_LEAF_BIOMASS_SUM };

    enum externals { DD, DELTA_T, FTSW, FCSTR, PHENO_STAGE,
                     PREDIM_LEAF_ON_MAINSTEM, SLA, PLANT_PHASE,
                     PLANT_STATE, TEST_IC, PLANT_STOCK,
                     PLANT_DEFICIT, PLANT_LEAF_BIOMASS_SUM,
                     PLANT_BIOMASS_SUM, PLANT_BLADE_AREA_SUM, ASSIM, MGR,
                     PLASTO, LIGULO, LL_BL};


    CulmModel(int index, double plasto, double ligulo, double plant_LL_BL):
        _index(index), _is_first_culm(index == 1),
        _culm_stock_model(new CulmStockModel),
        _plasto(plasto),
        _ligulo(ligulo),
        _LL_BL(plant_LL_BL)
    {
        // submodels
        Submodels( ((CULM_STOCK, _culm_stock_model.get())) );

        //    internals

        //@TODO regarder pourquoi ca pointe dans le vide
        InternalS(STOCK, _culm_stock_model.get(), CulmStockModel::STOCK);
        InternalS(DEFICIT, _culm_stock_model.get(), CulmStockModel::DEFICIT);
        InternalS(SURPLUS, _culm_stock_model.get(), CulmStockModel::SURPLUS);


        Internal(NB_LIG, &CulmModel::_nb_lig);
        Internal(STEM_LEAF_PREDIM, &CulmModel::_stem_leaf_predim);
        Internal(LEAF_BIOMASS_SUM, &CulmModel::_leaf_biomass_sum);
        Internal(LEAF_LAST_DEMAND_SUM, &CulmModel::_leaf_last_demand_sum);
        Internal(LEAF_DEMAND_SUM, &CulmModel::_leaf_demand_sum);
        Internal(INTERNODE_LAST_DEMAND_SUM, &CulmModel::_internode_last_demand_sum);
        Internal(INTERNODE_DEMAND_SUM, &CulmModel::_internode_demand_sum);
        Internal(INTERNODE_BIOMASS_SUM, &CulmModel::_internode_biomass_sum);
        Internal(INTERNODE_LEN_SUM, &CulmModel::_internode_len_sum);
        Internal(LEAF_BLADE_AREA_SUM, &CulmModel::_leaf_blade_area_sum);
        Internal(REALLOC_BIOMASS_SUM, &CulmModel::_realloc_biomass_sum);
        Internal(SENESC_DW_SUM, &CulmModel::_senesc_dw_sum);
        Internal(LAST_LIGULATED_LEAF, &CulmModel::_last_ligulated_leaf);
        Internal(LAST_LIGULATED_LEAF_LEN, &CulmModel::_last_ligulated_leaf_len);
        Internal(LAST_LEAF_BIOMASS_SUM, &CulmModel::_last_leaf_biomass_sum);

        //    externals
        External(DD, &CulmModel::_dd);
        External(DELTA_T, &CulmModel::_delta_t);
        External(FTSW, &CulmModel::_ftsw);
        External(FCSTR, &CulmModel::_fcstr);
        External(PHENO_STAGE, &CulmModel::_plant_phenostage);
        External(PREDIM_LEAF_ON_MAINSTEM, &CulmModel::_predim_leaf_on_mainstem);
        External(SLA, &CulmModel::_sla);
        External(PLANT_STATE, &CulmModel::_plant_state);
        External(PLANT_PHASE, &CulmModel::_plant_phase);
        External(TEST_IC, &CulmModel::_test_ic);
        External(PLANT_STOCK, &CulmModel::_plant_stock);
        External(PLANT_DEFICIT, &CulmModel::_plant_deficit);
        External(PLANT_BIOMASS_SUM, &CulmModel::_plant_biomass_sum);
        External(PLANT_LEAF_BIOMASS_SUM, &CulmModel::_plant_leaf_biomass_sum);
        External(PLANT_BLADE_AREA_SUM, &CulmModel::_plant_blade_area_sum);
        External(ASSIM, &CulmModel::_assim);
        External(MGR, &CulmModel::_MGR);
        External(PLASTO, &CulmModel::_plasto);
        External(LIGULO, &CulmModel::_ligulo);
        External(LL_BL, &CulmModel::_LL_BL);
    }

    virtual ~CulmModel()
    {
        //        auto it = _phytomer_models.begin();
        //        while (it != _phytomer_models.end()) {
        //            delete *it;
        //            ++it;
        //        }
    }


    bool is_phytomer_creatable() {
        return (_culm_phase == culm::VEGETATIVE
                || _culm_phase == culm::ELONG
                || _culm_phase == culm::PI
                )
                && (get_phytomer_number() < _nb_leaf_pi + _nb_leaf_max_after_pi);
    }

    void step_state(double t) {

        if(_plant_phase == plant::PI && _lag == -1) {
            _lag = true;
            _culm_phenostage_at_lag = _culm_phenostage;
        }

        if(_lag) {
            if(_culm_phenostage == _culm_phenostage_at_lag + _coeff_pi_lag && get_phytomer_number() >= 3) {
                _culm_phase = culm::PI;
                _lag = false;
            }
        }

        if( _plant_phase == plant::PI && _is_first_culm) {
            _culm_phase = culm::PI;
        }

        switch( _culm_phase  ) {
        case culm::INITIAL: {
            _culm_phase  = culm::VEGETATIVE;
            break;
        }
        case culm::VEGETATIVE: {
            if(_plant_phase == plant::ELONG) {
                if( _nb_lig > 0) {
                    _culm_phase  = culm::ELONG;
                }
            }
            break;
        }
        case culm::ELONG: {
            break;
        }
        case culm::PI: {
            _last_phase = _culm_phase ;
            if( _plant_state & plant::NEW_PHYTOMER_AVAILABLE) {
                if( _plant_phase == plant::PI) {
                    if(!_started_PI) {
                        _started_PI = true;
                        //create_panicle();
                        //create_peduncle();
                    }
                } else if (_culm_phenostage == _nb_leaf_pi + _nb_leaf_max_after_pi + 1 ) {
                    //peduncle_elongation()
                    _culm_phase  = culm::PRE_FLO;
                }
            }
            break;
        }
        case culm::PRE_FLO: {
            _last_phase = _culm_phase ;
            if( _plant_phenostage == _nb_leaf_pi + _nb_leaf_max_after_pi + 1 + _phenostage_pre_flo_to_flo ) {
    //            PanicleTransitionToFLO( );
    //            PeduncleTransitionToFLO( );
                _culm_phase  = culm::FLO;
            }
            break;
        }
      }
    }

    //@TODO gérer le deleteLeaf et le reallocBiomassSum var
    void compute(double t, bool /* update */) {
        if( ( _plant_state & plant::NEW_PHYTOMER_AVAILABLE ) && is_phytomer_creatable()) {
            create_phytomer(t);
        }

        step_state(t);

        auto it = _phytomer_models.begin();
        std::deque < PhytomerModel* >::iterator previous_it;
        int i = 0;
        _nb_lig = 0;
        _leaf_biomass_sum = 0;
        _last_leaf_biomass_sum = 0;
        _leaf_last_demand_sum = 0;
        _leaf_demand_sum = 0;
        _internode_last_demand_sum = 0;
        _internode_demand_sum = 0;
        _internode_biomass_sum = 0;
        _internode_len_sum = 0;
        _leaf_blade_area_sum = 0;
        _last_ligulated_leaf = -1;
        _last_ligulated_leaf_len = 0;
        _realloc_biomass_sum = 0;

        // Modifs PHT, à vérifier
        //_first_leaf_len = (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_LEN);
        //auto it = _phytomer_models.begin();

        while (it != _phytomer_models.end()) {
            //Phytomers
            compute_phytomers(it, previous_it, i, t);

            //Sum
            compute_vars(it, previous_it, i, t);

            previous_it = it;
            ++it;
            ++i;
        }

        //Floral_organs
        compute_peduncle(t);
        compute_panicle(t);


        //StockModel
        compute_stock(t);
    }

    void compute_peduncle(double t) {
        //put FindFirstNonVegetativePredimOrgan length (from top) INTERNODE_LENGTH_PREDIM
        //put FindFirstNonVegetativePredimOrgan diam (from top) INTERNODE_DIAM_PREDIM

    }

    void compute_panicle(double t) {

    }

    void compute_stock(double t) {
        _culm_stock_model->put(t, CulmStockModel::PLANT_DEFICIT, _plant_deficit);
        _culm_stock_model->put(t, CulmStockModel::PLANT_STOCK, _plant_stock);
        _culm_stock_model->put(t, CulmStockModel::LEAF_BIOMASS_SUM, _leaf_biomass_sum);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_BIOMASS_SUM, _internode_biomass_sum);
        _culm_stock_model->put(t, CulmStockModel::PLANT_LEAF_BIOMASS_SUM, _plant_leaf_biomass_sum);
        _culm_stock_model->put(t, CulmStockModel::ASSIM, _assim);
        _culm_stock_model->put(t, CulmStockModel::PLANT_BIOMASS_SUM, _plant_biomass_sum);
        _culm_stock_model->put(t, CulmStockModel::LEAF_DEMAND_SUM, _leaf_demand_sum);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_DEMAND_SUM, _internode_demand_sum);
        _culm_stock_model->put(t, CulmStockModel::LEAF_LAST_DEMAND_SUM, _leaf_last_demand_sum);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_LAST_DEMAND_SUM, _internode_last_demand_sum);
        _culm_stock_model->put(t, CulmStockModel::REALLOC_BIOMASS_SUM, _realloc_biomass_sum);
        _culm_stock_model->put(t, CulmStockModel::PLANT_PHASE, _plant_phase);
        (*_culm_stock_model)(t);
    }

    void compute_phytomers(std::deque < PhytomerModel* >::iterator it, std::deque < PhytomerModel* >::iterator previous_it, int i, double t) {
        (*it)->put(t, PhytomerModel::DD, _dd);
        (*it)->put(t, PhytomerModel::DELTA_T, _delta_t);
        (*it)->put(t, PhytomerModel::FTSW, _ftsw);
        (*it)->put(t, PhytomerModel::FCSTR, _fcstr);
        (*it)->put(t, PhytomerModel::SLA, _sla);
        (*it)->put < plant::plant_state >(t, PhytomerModel::PLANT_STATE, _plant_state);
        (*it)->put < plant::plant_phase >(t, PhytomerModel::PLANT_PHASE, _plant_phase);
        (*it)->put(t, PhytomerModel::TEST_IC, _test_ic);
        (*it)->leaf()->put(t, LeafModel::MGR, _MGR);
        (*it)->internode()->put(t, InternodeModel::CULM_PHASE, _culm_phase);

        if (_is_first_culm) {
            if (i == 0) {
                (*it)->put(t, PhytomerModel::PREDIM_LEAF_ON_MAINSTEM, 0.);
            } else {
                (*it)->put(
                            t, PhytomerModel::PREDIM_LEAF_ON_MAINSTEM,
                            (*previous_it)->get < double, LeafModel >(t, PhytomerModel::LEAF_PREDIM));
            }
        } else {
            (*it)->put(t, PhytomerModel::PREDIM_LEAF_ON_MAINSTEM,
                       _predim_leaf_on_mainstem);
        }


        if (i == 0) {
            (*it)->put(t, PhytomerModel::PREDIM_PREVIOUS_LEAF, 0.);
        } else {
            (*it)->put(t, PhytomerModel::PREDIM_PREVIOUS_LEAF,
                       (*previous_it)->get < double, LeafModel >(t, PhytomerModel::LEAF_PREDIM));
        }
        (**it)(t);
    }

    void compute_vars(std::deque < PhytomerModel* >::iterator it, std::deque < PhytomerModel* >::iterator previous_it, int i, double t) {


        if (not (*it)->is_leaf_dead()) {
            if((*it)->is_leaf_lig(t)) {
                ++_nb_lig;
            }

            if (_index == 1 or (*it)->is_leaf_lig(t)) {
                _stem_leaf_predim = (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_PREDIM);
            }
            if (_index == 1 and (*it)->is_leaf_lig(t)) {
                _last_ligulated_leaf = i;
                _last_ligulated_leaf_len = (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_LEN);
            }
        }

            _leaf_biomass_sum += (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_BIOMASS);

            if ((*it)->leaf()->get < double >(t, LeafModel::LAST_LEAF_BIOMASS) == 0) {
                _last_leaf_biomass_sum += (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_BIOMASS);
            } else {
                _last_leaf_biomass_sum += (*it)->leaf()->get < double >(t, LeafModel::LAST_LEAF_BIOMASS);
            }

        _leaf_last_demand_sum +=
                (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_LAST_DEMAND);
        _leaf_demand_sum += (*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_DEMAND);
        _internode_last_demand_sum +=
                (*it)->get < double, InternodeModel >(t, PhytomerModel::INTERNODE_LAST_DEMAND);
        _internode_demand_sum +=
                (*it)->get < double, InternodeModel >(
                    t, PhytomerModel::INTERNODE_DEMAND);
        _internode_biomass_sum += (*it)->get < double, InternodeModel >(
                    t, PhytomerModel::INTERNODE_BIOMASS);
        _internode_len_sum += (*it)->get < double, InternodeModel >(
                    t, PhytomerModel::INTERNODE_LEN);

//        if ((*it)->get < double, LeafModel >(t, PhytomerModel::LEAF_CORRECTED_BLADE_AREA) == 0) {
            _leaf_blade_area_sum +=
                    (*it)->get < double, LeafModel >(
                        t, PhytomerModel::LEAF_BLADE_AREA);
//        } else {
//            _leaf_blade_area_sum +=
//                    (*it)->get < double, LeafModel >(
//                        t, PhytomerModel::LEAF_CORRECTED_BLADE_AREA);
//        }

        _realloc_biomass_sum +=
                (*it)->get < double, LeafModel >(
                    t, PhytomerModel::REALLOC_BIOMASS);
        _senesc_dw_sum +=
                (*it)->get < double, LeafModel >(
                    t, PhytomerModel::SENESC_DW);


    }


    void create_phytomer(double t)
    {
        if (t != _parameters.beginDate) {
            int index;
            if (_phytomer_models.empty()) {
                index = 1;
            } else {
                index = _phytomer_models.back()->get_index() + 1;
            }

            PhytomerModel* phytomer = new PhytomerModel(index, _is_first_culm, _plasto, _ligulo, _LL_BL);
            setsubmodel(PHYTOMERS, phytomer);
            phytomer->init(t, _parameters);
            _phytomer_models.push_back(phytomer);
        }
    }

    int get_phytomer_number() const
    { return _phytomer_models.size(); }



    // Proposition (florian) pour delete_leaf :
//    void delete_leaf(double t, int index)
//    {
//        // Nécessite de placer delete leaf avant le compute culms
//        double biomass = _phytomer_models[index]->get < double, PhytomerModel >(t, PhytomerModel::LEAF_BIOMASS);
//        _deleted_senesc_dw = (1 - _realocationCoeff) * biomass

//        //    delete phytomer_models[index]; @TODO : phytomer à détruire si feuille morte ? Intrenoeud mort aussi ?
//        //    phytomer_models.erase(phytomer_models.begin() + index);

//        _phytomer_models[index]->delete_leaf(t);
//        ++_deleted_leaf_number;
//    }

    // Code c++ de base :
    //    void CulmModel::delete_leaf(double t, int index)
    //    {
    //        _deleted_senesc_dw =
    //                (1 - _parameters->get < double > ("realocationCoeff")) *
    //                get_leaf_biomass(t - 1, index);
    //        _deleted_senesc_dw_computed = true;

    //        //    delete phytomer_models[index];
    //        //    phytomer_models.erase(phytomer_models.begin() + index);

    //        phytomer_models[index]->delete_leaf(t);

    //        ++_deleted_leaf_number;

    //    }

    //    double CulmModel::get_leaf_biomass(double t, int index) const
    //    {
    //        double biomass = phytomer_models[index]->get < double, PhytomerModel >(
    //                    t, PhytomerModel::LEAF_BIOMASS);
    //        double corrected_biomass =
    //                phytomer_models[index]->get < double, PhytomerModel >(
    //                    t, PhytomerModel::LEAF_CORRECTED_BIOMASS);

    //        if (corrected_biomass > 0) {
    //            return corrected_biomass;
    //        } else {
    //            return biomass;
    //        }
    //    }

    //    double CulmModel::get_leaf_blade_area(double t, int index) const
    //    {
    //        double blade_area = phytomer_models[index]->get < double, leaf::BladeArea >(
    //                    t, PhytomerModel::LEAF_BLADE_AREA);
    //        double corrected_blade_area =
    //                phytomer_models[index]->get < double, leaf::BladeArea >(
    //                    t, PhytomerModel::LEAF_CORRECTED_BLADE_AREA);

    //        if (corrected_blade_area > 0) {
    //            return corrected_blade_area;
    //        } else {
    //            return blade_area;
    //        }
    //    }

    //    double CulmModel::get_leaf_len(double t, int index) const
    //    {
    //        return phytomer_models[index]->get < double, leaf::LeafLen >(
    //                    t, PhytomerModel::LEAF_LEN);
    //    }

    //    int CulmModel::get_first_ligulated_leaf_index(double t) const
    //    {
    //        std::deque < PhytomerModel* >::const_iterator it =
    //                phytomer_models.begin();
    //        int i = 0;

    //        while (it != phytomer_models.end()) {
    //            if (not (*it)->is_leaf_dead() and
    //                    (*it)->get < double, leaf::LeafLen >(
    //                        t, PhytomerModel::LEAF_LEN) ==
    //                    (*it)->get < double, leaf::LeafPredim >(t, PhytomerModel::PREDIM)) {
    //                break;
    //            }
    //            ++it;
    //            ++i;
    //        }
    //        if (it != phytomer_models.end()) {
    //            return i;
    //        } else {
    //            return -1;
    //        }
    //    }

    //    int CulmModel::get_first_alive_leaf_index(double /* t */) const
    //    {
    //        std::deque < PhytomerModel* >::const_iterator it =
    //                phytomer_models.begin();
    //        int i = 0;
    //        int index = -1;

    //        while (it != phytomer_models.end()) {
    //            if (not (*it)->is_leaf_dead()) {
    //                index = i;
    //                break;
    //            }
    //            ++it;
    //            ++i;
    //        }
    //        return index;
    //    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        PhytomerModel* first_phytomer = new PhytomerModel(1, _is_first_culm, _plasto, _ligulo, _LL_BL);

        setsubmodel(PHYTOMERS, first_phytomer);
        first_phytomer->init(t, parameters);
        _phytomer_models.push_back(first_phytomer);
        _culm_stock_model->init(t, parameters);

        //parameters
        _parameters = parameters;
        _nb_leaf_pi = _parameters.get < double >("nbleaf_pi");
        _nb_leaf_max_after_pi = _parameters.get < double >("nb_leaf_max_after_PI");
        _phenostage_pre_flo_to_flo  = _parameters.get < double >("phenostage_PRE_FLO_to_FLO");
        _coeff_pi_lag = _parameters.get < double >("coeff_PI_lag");

        //    internals
        _nb_lig = 0;
        _stem_leaf_predim = 0;
        _leaf_biomass_sum = 0;
        _leaf_last_demand_sum = 0;
        _leaf_demand_sum = 0;
        _internode_last_demand_sum = 0;
        _internode_demand_sum = 0;
        _internode_biomass_sum = 0;
        _internode_len_sum = 0;
        _leaf_blade_area_sum = 0;
        _last_ligulated_leaf = -1;
        _last_ligulated_leaf_len = 0;
        _realloc_biomass_sum = 0;
        _senesc_dw_sum = 0;
        _last_leaf_biomass_sum = 0;

        _started_PI = false;
        _culm_phase = culm::INITIAL;
        _last_phase = _culm_phase;
        _culm_phenostage = 1;
        _culm_phenostage_at_lag = 0;
        _lag = false;
    }

private:
    ecomeristem::ModelParameters _parameters;

    //  submodels
    std::unique_ptr < CulmStockModel > _culm_stock_model;
    std::deque < PhytomerModel* > _phytomer_models;

    //    attributes
    double _index;
    bool _is_first_culm;


    //parameters
    double _nb_leaf_pi;
    double _nb_leaf_max_after_pi;
    double _phenostage_pre_flo_to_flo;
    double _coeff_pi_lag;

    //    internals
    bool _started_PI;
    double _culm_phenostage;
    double _culm_phenostage_at_lag;
    culm::culm_phase _culm_phase;
    culm::culm_phase _last_phase;
    bool _lag;
    double _nb_lig;
    double _stem_leaf_predim;
    double _leaf_biomass_sum;
    double _leaf_last_demand_sum;
    double _leaf_demand_sum;
    double _internode_last_demand_sum;
    double _internode_demand_sum;
    double _internode_biomass_sum;
    double _internode_len_sum;
    double _leaf_blade_area_sum;
    double _realloc_biomass_sum;
    double _senesc_dw_sum;
    int _last_ligulated_leaf;
    double _last_ligulated_leaf_len;
    double _last_leaf_biomass_sum;
    //        double _lig;
    //        double _deleted_leaf_number;
    //        double _deleted_senesc_dw;
    //        bool _deleted_senesc_dw_computed;

    //    externals
    int _plant_phenostage;
    plant::plant_state _plant_state;
    plant::plant_phase _plant_phase;
    double _MGR;
    double _LL_BL;
    double _plasto;
    double _ligulo;
    double _dd;
    double _delta_t;
    double _ftsw;
    double _fcstr;
    double _predim_leaf_on_mainstem;
    double _sla;
    double _test_ic;
    double _plant_stock;
    double _plant_deficit;
    double _plant_biomass_sum;
    double _plant_leaf_biomass_sum;
    double _plant_blade_area_sum;
    double _assim;
};

} // namespace model
#endif