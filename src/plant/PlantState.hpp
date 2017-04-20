/**
 * @file ecomeristem/plant/Manager.hpp
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

#ifndef __ECOMERISTEM_PLANT_MANAGER_HPP
#define __ECOMERISTEM_PLANT_MANAGER_HPP

#include <defines.hpp>


class PlantState : public AtomicModel < PlantState >
{
public:
    enum plant_phase { INIT = 0,
                       INITIAL = 1,
                       GROWTH = 2,
                       NOGROWTH = 3,
                       NEW_PHYTOMER = 5,
                       NOGROWTH2 = 18,
                       NOGROWTH3 = 19,
                       NOGROWTH4 = 20,
                       NEW_PHYTOMER3 = 23,
                       LIG = 24,
                       KILL = 25 };

    enum plant_state {  VEGETATIVE = 0, PRE_ELONG, ELONG, PRE_PI, PI, PRE_FLO,
                        FLO, END_FILLING, MATURITY, DEAD };

    enum internals { PHASE, STATE };
    enum externals { STOCK, PHENO_STAGE, BOOL_CROSSED_PLASTO, FTSW, IC };

    PlantState()
    {
//        external(STOCK, &PlantManager::_stock);
//        external(PHENO_STAGE, &PlantManager::_phenoStage);
//        external(BOOL_CROSSED_PLASTO, &PlantManager::_boolCrossedPlasto);
//        external(FTSW, &PlantManager::_FTSW);
//        external(IC, &PlantManager::_IC);

//        internal(PHASE, &PlantManager::_phase);
//        internal(STATE, &PlantManager::_state);
    }

    virtual ~PlantState()
    { }

    virtual bool check(double t) const;

    virtual void init(double /* t */,
                      const ecomeristem::ModelParameters& parameters)
    {
//        nbleaf_pi = 10; // parameters.get < double >("nbleaf_pi");
//        nbleaf_culm_elong = parameters.get < double >("nb_leaf_stem_elong");
//        nbleaf_max_after_pi = parameters.get < double >("nb_leaf_max_after_PI");

//        culm_number = 1;
//        leaf_number = 1;
//        first_leaf = true;

//        // TODO
//        _stock = 1e-10;
//        _stock_1 = 1e-10;
//        _phenoStage = 0;
//        _boolCrossedPlasto = 0;

//        _phase = ecomeristem::plant::INIT;
//        _state = ecomeristem::plant::VEGETATIVE;
    }

    virtual void compute(double t, bool /* update */);

    virtual void put(double t, unsigned int index, double value);

private:
//// parameters
//    double nbleaf_pi;
//    double nbleaf_culm_elong;
//    double nbleaf_max_after_pi;

//// internal variables
//    double _phase;
//    double _state;
//    unsigned int leaf_number;
//    unsigned int culm_number;
//    bool first_leaf;
//    double _last_time;

//// external variables
//    double _stock;
//    double _stock_1;
//    double _phenoStage;
//    double _boolCrossedPlasto;
//    double _FTSW;
//    double _IC;
};

#endif
