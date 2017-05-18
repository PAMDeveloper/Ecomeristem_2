/**
 * @file ecomeristem/peduncle/Model.hpp
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

#include <defines.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

namespace model {

class PeduncleModel : public AtomicModel < PeduncleModel >
{
public:

    enum internals {  };
    enum externals { PLANT_PHASE };


    PeduncleModel()
    {
        Internal(LENGTH_PREDIM, &PeduncleModel::_length_predim);

        External(PLANT_PHASE, &PeduncleModel::_plant_phase);

    }

    virtual ~PeduncleModel()
    { }


    void compute(double t, bool /* update */)
    {
        _p = _parameters.get(t).P;

        if (_plant_phase == plant::FLO) {
            // FLO, rien à calculer
        } else if (_plant_phase == plant::END_FILLING) {
            // END_FILLING, rien à calculer
        } else if (_plant_phase == plant::MATURITY) {
            // MATURITY
                // ComputeInternodeExpTime

                // SumOfBiomassOnCulmInInternodeRec
        } else {
            if(_is_predim == false) {
                // INIT
                    // ComputePeduncleLengthPredim

                    // ComputePeduncleDiameterPredim

                    // ComputeReductionINER

                    // ComputeINER

                    // initLen : Mult2Values(INER, DDRealization)

                    // ComputeInternodeVolume

                    // ComputeInternodeExpTime

                    // ComputeInternodeBiomass

                    // SumOfBiomassOnCulmInInternodeRec
                _is_predim = true;
                _is_in_transition = true;
            } else if (_is_in_transition) {
                // TRANSITION, rien à calculer
                _is_in_transition = false;
            } else {
                // REALIZATION
                    // ComputeReductionINER

                    // ComputeINER_LE

                    // ComputeInternodeExpTime

                    // UpdateInternodeLength

                    // ComputeInternodeVolume

                    // ComputeInternodeDemand

                    // ComputeInternodeBiomass

                    // SumOfBiomassOnCulmInInternodeRec

                    // TransitionToMatureState
            }
        }

    }

    void init(double /* t */,
              const ecomeristem::ModelParameters&  parameters )
    {
        _parameters = parameters;

        // parameters

        // internals
        _is_predim = false;
        _is_in_transition = false;

    }
private:
    ecomeristem::ModelParameters  _parameters;

    // parameters

    // internals
    bool _is_predim;
    bool _is_in_transition;

    // externals
    plant::plant_phase _plant_phase;

};

}
