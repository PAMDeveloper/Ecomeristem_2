/**
 * @file ecomeristem/phytomer/Model.hpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2016 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2016 ULCO http://www.univ-littoral.fr
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

#include <defines.hpp>
#include <plant/culm/phytomer/internode/InternodeModel.hpp>
#include <plant/culm/phytomer/leaf/LeafModel.hpp>

namespace model {

class PhytomerModel : public CoupledModel < PhytomerModel >
{
public:
    enum submodels { LEAF, INTERNODE };

    enum internals { LEAF_PREDIM };
    enum externals { DD, DELTA_T, FTSW, FCSTR, P, PHENO_STAGE,
                     PREDIM_LEAF_ON_MAINSTEM, PREDIM_PREVIOUS_LEAF,
                     SLA, GROW, PLANT_PHASE, STATE, TEST_IC,
                     PLANT_STATE};

    PhytomerModel(int index, bool is_on_mainstem) :
        _index(index),
        _is_first_phytomer(index == 1),
        _is_on_mainstem(is_on_mainstem),
        _internode_model(new InternodeModel(_index, _is_on_mainstem)),
        _leaf_model(new LeafModel(_index, _is_on_mainstem))
    {
        // submodels
        Submodels( ((LEAF, _leaf_model.get())) );
        Submodels( ((INTERNODE, _internode_model.get())) );

        // internals
        InternalS(LEAF_PREDIM,  _leaf_model.get(), LeafModel::PREDIM);

        // externals
        External(PLANT_PHASE, &PhytomerModel::_plant_phase);
        External(PLANT_STATE, &PhytomerModel::_plant_state);
        External(TEST_IC, &PhytomerModel::_test_ic);
        External(FCSTR, &PhytomerModel::_fcstr);
        External(PREDIM_LEAF_ON_MAINSTEM, &PhytomerModel::_predim_leaf_on_mainstem);
        External(PREDIM_PREVIOUS_LEAF, &PhytomerModel::_predim_previous_leaf);
        External(FTSW, &PhytomerModel::_ftsw);
        External(P, &PhytomerModel::_p);
        External(DD, &PhytomerModel::_dd);
        External(DELTA_T, &PhytomerModel::_delta_t);
        External(GROW, &PhytomerModel::_grow);
        External(SLA, &PhytomerModel::_sla);
        External(PHENO_STAGE, &PhytomerModel::_pheno_stage);
    }

    virtual ~PhytomerModel()
    {
//        if (internode_model) delete internode_model;
//        if (leaf_model) delete leaf_model;
    }

    void init(double t, const ecomeristem::ModelParameters& parameters)
    {
        _internode_model->init(t, parameters);
        _leaf_model->init(t, parameters);
    }

    void compute(double t, bool /* update */)
    {
        if (_leaf_model) {
            _leaf_model->put(t, LeafModel::DD, _dd);
            _leaf_model->put(t, LeafModel::DELTA_T, _delta_t);
            _leaf_model->put(t, LeafModel::FTSW, _ftsw);
            _leaf_model->put(t, LeafModel::FCSTR, _fcstr);
            _leaf_model->put(t, LeafModel::P, _p);
            _leaf_model->put(t, LeafModel::PHENO_STAGE, _pheno_stage);
            _leaf_model->put(t, LeafModel::PREDIM_LEAF_ON_MAINSTEM, _predim_leaf_on_mainstem);
            _leaf_model->put(t, LeafModel::PREDIM_PREVIOUS_LEAF, _predim_previous_leaf);
            _leaf_model->put(t, LeafModel::SLA, _sla);
            _leaf_model->put(t, LeafModel::GROW, _grow);
            _leaf_model->put(t, LeafModel::TEST_IC, _test_ic);
            (*_leaf_model)(t);
        }

        _internode_model->put(t, InternodeModel::DD, _dd);
        _internode_model->put(t, InternodeModel::DELTA_T, _delta_t);
        _internode_model->put(t, InternodeModel::FTSW, _ftsw);
        _internode_model->put(t, InternodeModel::P, _p);
        _internode_model->put(t, InternodeModel::PLANT_PHASE, _plant_phase);
        _internode_model->put(t, InternodeModel::PLANT_STATE, _plant_state);
        _internode_model->put(t, InternodeModel::LIG,
                             _leaf_model->get < double > (t, LeafModel::LIG_T));
        _internode_model->put(t, InternodeModel::LEAF_PREDIM,
                             _leaf_model->get < double > (t, LeafModel::PREDIM));
        (*_internode_model)(t);
    }

    void delete_leaf(double t)
    {

//#ifdef WITH_TRACE
//        utils::Trace::trace()
//            << utils::TraceElement("CULM", t, artis::utils::COMPUTE)
//            << "ADD LIG ; DELETE index = " << _index;
//        utils::Trace::trace().flush();
//#endif

//        delete leaf_model;
//        leaf_model = 0;
//        change_internal(LEAF_BIOMASS, &PhytomerModel::_null);
//        change_internal(LEAF_BLADE_AREA, &PhytomerModel::_null);
//        change_internal(LEAF_DEMAND, &PhytomerModel::_null);
//        change_internal(LEAF_LAST_DEMAND, &PhytomerModel::_null);
//        change_internal(PREDIM, &PhytomerModel::_null);
//        change_internal(PLASTO_DELAY, &PhytomerModel::_null);
//        change_internal(REALLOC_BIOMASS, &PhytomerModel::_null);
//        change_internal(SENESC_DW, &PhytomerModel::_null);
//        change_internal(SENESC_DW_SUM, &PhytomerModel::_null);
//        change_internal(LEAF_CORRECTED_BIOMASS, &PhytomerModel::_null);
//        change_internal(LEAF_CORRECTED_BLADE_AREA, &PhytomerModel::_null);
//        change_internal(LEAF_LEN, &PhytomerModel::_null);
    }

//    double get_blade_area() const
//    { return leaf_model->get_blade_area(); }

//    const LeafModel& leaf() const
//    { return *leaf_model; }

//    const internode::InternodeModel& internode() const
//    { return *internode_model; }

//    int get_index() const
//    { return _index; }

    bool is_leaf_dead() const
    { return _leaf_model.get() != nullptr; }

    bool is_leaf_lig(double t) const {
        return !is_leaf_dead() &&
                _leaf_model->get < bool > (t, LeafModel::IS_LIG);
    }

private:
    //  attribute
    int _index;
    bool _is_first_phytomer;
    bool _is_on_mainstem;

    // submodels
    std::unique_ptr < InternodeModel > _internode_model;
    std::unique_ptr < LeafModel > _leaf_model;

    // external variables
    double _ftsw;
    double _p;
    double _plant_phase;
    double _plant_state;
    double _len;
    double _fcstr;
    double _predim_leaf_on_mainstem;
    double _predim_previous_leaf;
    double _test_ic;
    double _dd;
    double _delta_t;
    double _grow;
    double _sla;
    double _pheno_stage;
};

} // namespace model
