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
#include <plant/culm/processes/CulmStockModel.hpp>
#include <plant/culm/phytomer/PhytomerModel.hpp>

namespace model {

class CulmModel : public CoupledModel < CulmModel >
{
public:
    enum submodels { CULM_STOCK, PHYTOMERS };

    enum internals { NB_LIG, STEM_LEAF_PREDIM };

    enum externals { DD, DELTA_T, FTSW, FCSTR, P, PHENO_STAGE,
                     PREDIM_LEAF_ON_MAINSTEM, SLA, GROW, PLANT_PHASE,
                     PLANT_STATE, STOP, TEST_IC, PLANT_STOCK,
                     PLANT_DEFICIT, PLANT_LEAF_BIOMASS_SUM,
                     PLANT_BIOMASS_SUM, PLANT_BLADE_AREA_SUM, ASSIM };


    CulmModel(int index):
        _index(index), _is_first_culm(index == 1),
        _culm_stock_model(new CulmStockModel)
    {
        // submodels
        Submodels( ((CULM_STOCK, _culm_stock_model.get())) );

        //    internals
        Internal(NB_LIG, &CulmModel::_nb_lig);
        Internal(STEM_LEAF_PREDIM, &CulmModel::_stem_leaf_predim);

        //    externals
        External(DD, &CulmModel::_dd);
        External(DELTA_T, &CulmModel::_delta_t);
        External(FTSW, &CulmModel::_ftsw);
        External(FCSTR, &CulmModel::_fcstr);
        External(P, &CulmModel::_p);
        External(PHENO_STAGE, &CulmModel::_pheno_stage);
        External(PREDIM_LEAF_ON_MAINSTEM, &CulmModel::_predim_leaf_on_mainstem);
        External(SLA, &CulmModel::_sla);
        External(GROW, &CulmModel::_grow);
        External(PLANT_PHASE, &CulmModel::_plant_phase);
        External(PLANT_STATE, &CulmModel::_plant_state);
        External(TEST_IC, &CulmModel::_test_ic);
        External(PLANT_STOCK, &CulmModel::_plant_stock);
        External(PLANT_DEFICIT, &CulmModel::_plant_deficit);
        External(PLANT_BIOMASS_SUM, &CulmModel::_plant_biomass_sum);
        External(PLANT_LEAF_BIOMASS_SUM, &CulmModel::_plant_leaf_biomass_sum);
        External(PLANT_BLADE_AREA_SUM, &CulmModel::_plant_blade_area_sum);
        External(ASSIM, &CulmModel::_assim);

    }

    virtual ~CulmModel()
    {
        //        auto it = _phytomer_models.begin();
        //        while (it != _phytomer_models.end()) {
        //            delete *it;
        //            ++it;
        //        }
    }


    //@TODO gérer le deleteLeaf et le reallocBiomassSum var
    void compute(double t, bool /* update */) {
        auto it = _phytomer_models.begin();
        std::deque < PhytomerModel* >::iterator previous_it;
        int i = 0;
        while (it != _phytomer_models.end()) {
            //Phytomers
            compute_phytomers(it, previous_it, i, t);

            //Sum
            compute_vars(it, previous_it, i, t);

            previous_it = it;
            ++it;
            ++i;
        }

        //StockModel
        if (_plant_state == PlantState::ELONG) {
            compute_stock(t);
        }
    }

    void compute_stock(double t) {
        _culm_stock_model->put(t, CulmStockModel::PLANT_DEFICIT, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_STOCK, 0);
        _culm_stock_model->put(t, CulmStockModel::LEAF_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_LEAF_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::ASSIM, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::LEAF_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::LEAF_LAST_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::INTERNODE_LAST_DEMAND_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::REALLOC_BIOMASS_SUM, 0);
        _culm_stock_model->put(t, CulmStockModel::PLANT_STATE, PlantState::VEGETATIVE);
        (*_culm_stock_model)(t);
    }

    void compute_phytomers(std::deque < PhytomerModel* >::iterator it, std::deque < PhytomerModel* >::iterator previous_it, int i, double t) {
        (*it)->put(t, PhytomerModel::DD, _dd);
        (*it)->put(t, PhytomerModel::DELTA_T, _delta_t);
        (*it)->put(t, PhytomerModel::FTSW, _ftsw);
        (*it)->put(t, PhytomerModel::FCSTR, _fcstr);
        (*it)->put(t, PhytomerModel::P, _p);
        (*it)->put(t, PhytomerModel::PHENO_STAGE, _pheno_stage);
        (*it)->put(t, PhytomerModel::SLA, _sla);
        (*it)->put(t, PhytomerModel::GROW, _grow);
        (*it)->put(t, PhytomerModel::PLANT_PHASE, _plant_phase);
        (*it)->put(t, PhytomerModel::STATE, _plant_state);
        (*it)->put(t, PhytomerModel::TEST_IC, _test_ic);

        if (_is_first_culm) {
            if (i == 0) {
                (*it)->put(t, PhytomerModel::PREDIM_LEAF_ON_MAINSTEM, 0.);
            } else {
                (*it)->put(
                            t, PhytomerModel::PREDIM_LEAF_ON_MAINSTEM,
                            (*previous_it)->get < double, PhytomerModel >(t, PhytomerModel::LEAF_PREDIM));
            }
        } else {
            (*it)->put(t, PhytomerModel::PREDIM_LEAF_ON_MAINSTEM,
                       _predim_leaf_on_mainstem);
        }


        if (i == 0) {
            (*it)->put(t, PhytomerModel::PREDIM_PREVIOUS_LEAF, 0.);
        } else {
            (*it)->put(t, PhytomerModel::PREDIM_PREVIOUS_LEAF,
                       (*previous_it)->get < double, PhytomerModel >(t, PhytomerModel::LEAF_PREDIM));
        }
        (**it)(t);
    }

    void compute_vars(std::deque < PhytomerModel* >::iterator it, std::deque < PhytomerModel* >::iterator previous_it, int i, double t) {
        if (not (*it)->is_leaf_dead()) {
            if((*it)->is_leaf_lig(t)) {
                ++_nb_lig;
            }

            //@TODO inutile de le calculer ailleurs que sur le mainstem (_is_first_culm)
            if (i == 0 or (*it)->is_leaf_lig(t)) {
                _stem_leaf_predim = (*it)->get < double, PhytomerModel >(t, PhytomerModel::LEAF_PREDIM);
            }
        }

        //            if ((*it)->get < double, leaf::LeafBiomass >(
        //                    t, PhytomerModel::LEAF_CORRECTED_BIOMASS) == 0) {
        //                _leaf_biomass_sum +=
        //                    (*it)->get < double, leaf::LeafBiomass >(
        //                        t, PhytomerModel::LEAF_BIOMASS);
        //            } else {
        //                _leaf_biomass_sum +=
        //                    (*it)->get < double, leaf::LeafBiomass >(
        //                        t, PhytomerModel::LEAF_CORRECTED_BIOMASS);
        //            }

        //            _leaf_last_demand_sum +=
        //                (*it)->get < double, leaf::LeafLastDemand >(
        //                    t, PhytomerModel::LEAF_LAST_DEMAND);
        //            _leaf_demand_sum += (*it)->get < double, leaf::LeafDemand >(
        //                t, PhytomerModel::LEAF_DEMAND);
        //            _internode_last_demand_sum +=
        //                (*it)->get < double, internode::InternodeLastDemand >(
        //                    t, PhytomerModel::INTERNODE_LAST_DEMAND);
        //            _internode_demand_sum +=
        //                (*it)->get < double, internode::InternodeDemand >(
        //                    t, PhytomerModel::INTERNODE_DEMAND);
        //            _internode_biomass_sum += (*it)->get < double, internode::Biomass >(
        //                t, PhytomerModel::INTERNODE_BIOMASS);
        //            _internode_len_sum += (*it)->get < double, internode::InternodeLen >(
        //                t, PhytomerModel::INTERNODE_LEN);

        //            if ((*it)->get < double, leaf::BladeArea >(
        //                    t, PhytomerModel::LEAF_CORRECTED_BLADE_AREA) == 0) {
        //                _leaf_blade_area_sum +=
        //                    (*it)->get < double, leaf::BladeArea >(
        //                        t, PhytomerModel::LEAF_BLADE_AREA);
        //            } else {
        //                _leaf_blade_area_sum +=
        //                    (*it)->get < double, leaf::BladeArea >(
        //                        t, PhytomerModel::LEAF_CORRECTED_BLADE_AREA);
        //            }

        //            _realloc_biomass_sum +=
        //                (*it)->get < double, leaf::LeafBiomass >(
        //                    t, PhytomerModel::REALLOC_BIOMASS);
        //            _senesc_dw_sum +=
        //                (*it)->get < double, leaf::LeafBiomass >(
        //                    t, PhytomerModel::SENESC_DW);

    }


//    void CulmModel::create_phytomer(double t)
//    {
//        if (t != _first_day) {
//            int index;

//            if (phytomer_models.empty()) {
//                index = 1;
//            } else {
//                index = phytomer_models.back()->get_index() + 1;
//            }

//#ifdef WITH_TRACE
//            utils::Trace::trace()
//                    << utils::TraceElement("CULM", t, artis::utils::COMPUTE)
//                    << "CREATE PHYTOMER: " << index
//                    << " ; index = " << _index;
//            utils::Trace::trace().flush();
//#endif

//            PhytomerModel* phytomer = new PhytomerModel(index, _is_first_culm);

//            setsubmodel(PHYTOMERS, phytomer);
//            phytomer->init(t, *_parameters);
//            phytomer_models.push_back(phytomer);
//            compute(t, true);
//        }
//    }

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

//#ifdef WITH_TRACE
//        utils::Trace::trace()
//                << utils::TraceElement("CULM", t, artis::utils::COMPUTE)
//                << "DELETE LEAF: " << _index
//                << " ; index = " << index
//                << " ; nb = " << _deleted_leaf_number;
//        utils::Trace::trace().flush();
//#endif

//    }

//    double CulmModel::get_leaf_biomass(double t, int index) const
//    {
//        double biomass = phytomer_models[index]->get < double, leaf::LeafBiomass >(
//                    t, PhytomerModel::LEAF_BIOMASS);
//        double corrected_biomass =
//                phytomer_models[index]->get < double, leaf::LeafBiomass >(
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

//    int CulmModel::get_last_ligulated_leaf_index(double t) const
//    {
//        std::deque < PhytomerModel* >::const_iterator it =
//                phytomer_models.begin();
//        int i = 0;
//        int index = -1;

//        while (it != phytomer_models.end()) {
//            if (not (*it)->is_leaf_dead() and
//                    (*it)->get < double, leaf::LeafLen >(
//                        t, PhytomerModel::LEAF_LEN) ==
//                    (*it)->get < double, leaf::LeafPredim >(t, PhytomerModel::PREDIM)) {
//                index = i;
//            }
//            ++it;
//            ++i;
//        }
//        return index;
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
        _parameters = parameters;

        //    parameters variables

        //    parameters variables (t)

        //    internals
        _nb_lig = 0;
        _stem_leaf_predim = 0;

    }

private:
    ecomeristem::ModelParameters _parameters;

    //  submodels
    std::unique_ptr < CulmStockModel > _culm_stock_model;
    std::deque < PhytomerModel* > _phytomer_models;

    //    attributes
    double _index;
    bool _is_first_culm;

    //    internals
    double _nb_lig;
    double _stem_leaf_predim;

    //    externals
    double _dd;
    double _delta_t;
    double _ftsw;
    double _fcstr;
    double _p;
    int _pheno_stage;
    double _predim_leaf_on_mainstem;
    double _sla;
    double _grow;
    double _stop;
    double _test_ic;
    double _plant_phase;
    double _plant_state;
    double _plant_stock;
    double _plant_deficit;
    double _plant_biomass_sum;
    double _plant_leaf_biomass_sum;
    double _plant_blade_area_sum;
    double _assim;
};

} // namespace model
#endif
