/**
 * @file ecomeristem/culm/Model.cpp
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

#include <plant/culm/CulmModel.hpp>

namespace ecomeristem { namespace culm {






//void CulmModel::create_phytomer(double t)
//{
//    if (t != _first_day) {
//        int index;

//        if (phytomer_models.empty()) {
//            index = 1;
//        } else {
//            index = phytomer_models.back()->get_index() + 1;
//        }

//#ifdef WITH_TRACE
//        utils::Trace::trace()
//            << utils::TraceElement("CULM", t, artis::utils::COMPUTE)
//            << "CREATE PHYTOMER: " << index
//            << " ; index = " << _index;
//        utils::Trace::trace().flush();
//#endif

//        phytomer::PhytomerModel* phytomer = new phytomer::PhytomerModel(index, _is_first_culm);

//        setsubmodel(PHYTOMERS, phytomer);
//        phytomer->init(t, *_parameters);
//        phytomer_models.push_back(phytomer);
//        compute(t, true);
//    }
//}

//void CulmModel::delete_leaf(double t, int index)
//{
//    _deleted_senesc_dw =
//        (1 - _parameters->get < double > ("realocationCoeff")) *
//        get_leaf_biomass(t - 1, index);
//    _deleted_senesc_dw_computed = true;

////    delete phytomer_models[index];
////    phytomer_models.erase(phytomer_models.begin() + index);

//    phytomer_models[index]->delete_leaf(t);

//    ++_deleted_leaf_number;

//#ifdef WITH_TRACE
//        utils::Trace::trace()
//            << utils::TraceElement("CULM", t, artis::utils::COMPUTE)
//            << "DELETE LEAF: " << _index
//            << " ; index = " << index
//            << " ; nb = " << _deleted_leaf_number;
//        utils::Trace::trace().flush();
//#endif

//}

//double CulmModel::get_leaf_biomass(double t, int index) const
//{
//    double biomass = phytomer_models[index]->get < double, leaf::LeafBiomass >(
//        t, phytomer::PhytomerModel::LEAF_BIOMASS);
//    double corrected_biomass =
//        phytomer_models[index]->get < double, leaf::LeafBiomass >(
//            t, phytomer::PhytomerModel::LEAF_CORRECTED_BIOMASS);

//    if (corrected_biomass > 0) {
//        return corrected_biomass;
//    } else {
//        return biomass;
//    }
//}

//double CulmModel::get_leaf_blade_area(double t, int index) const
//{
//    double blade_area = phytomer_models[index]->get < double, leaf::BladeArea >(
//        t, phytomer::PhytomerModel::LEAF_BLADE_AREA);
//    double corrected_blade_area =
//        phytomer_models[index]->get < double, leaf::BladeArea >(
//            t, phytomer::PhytomerModel::LEAF_CORRECTED_BLADE_AREA);

//    if (corrected_blade_area > 0) {
//        return corrected_blade_area;
//    } else {
//        return blade_area;
//    }
//}

//double CulmModel::get_leaf_len(double t, int index) const
//{
//    return phytomer_models[index]->get < double, leaf::LeafLen >(
//        t, phytomer::PhytomerModel::LEAF_LEN);
//}

//int CulmModel::get_first_ligulated_leaf_index(double t) const
//{
//    std::deque < phytomer::PhytomerModel* >::const_iterator it =
//        phytomer_models.begin();
//    int i = 0;

//    while (it != phytomer_models.end()) {
//        if (not (*it)->is_leaf_dead() and
//            (*it)->get < double, leaf::LeafLen >(
//                t, phytomer::PhytomerModel::LEAF_LEN) ==
//            (*it)->get < double, leaf::LeafPredim >(t, phytomer::PhytomerModel::PREDIM)) {
//            break;
//        }
//        ++it;
//        ++i;
//    }
//    if (it != phytomer_models.end()) {
//        return i;
//    } else {
//        return -1;
//    }
//}

//int CulmModel::get_last_ligulated_leaf_index(double t) const
//{
//    std::deque < phytomer::PhytomerModel* >::const_iterator it =
//        phytomer_models.begin();
//    int i = 0;
//    int index = -1;

//    while (it != phytomer_models.end()) {
//        if (not (*it)->is_leaf_dead() and
//            (*it)->get < double, leaf::LeafLen >(
//                t, phytomer::PhytomerModel::LEAF_LEN) ==
//            (*it)->get < double, leaf::LeafPredim >(t, phytomer::PhytomerModel::PREDIM)) {
//            index = i;
//        }
//        ++it;
//        ++i;
//    }
//    return index;
//}

//int CulmModel::get_first_alive_leaf_index(double /* t */) const
//{
//    std::deque < phytomer::PhytomerModel* >::const_iterator it =
//        phytomer_models.begin();
//    int i = 0;
//    int index = -1;

//    while (it != phytomer_models.end()) {
//        if (not (*it)->is_leaf_dead()) {
//            index = i;
//            break;
//        }
//        ++it;
//        ++i;
//    }
//    return index;
//}

} } // namespace ecomeristem culm
