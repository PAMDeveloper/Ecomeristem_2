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

    enum internals { LENGTH_PREDIM, DIAM_PREDIM, REDUCTION_INER };
    enum externals { INTERNODE_LENGTH_PREDIM, INTERNODE_DIAM_PREDIM, INTERNODE_INDEX, FTSW };


    PeduncleModel()
    {
        Internal(LENGTH_PREDIM, &PeduncleModel::_length_predim);
        Internal(DIAM_PREDIM, &PeduncleModel::_diam_predim);
        Internal(REDUCTION_INER, &PeduncleModel::_reduction_iner);

        External(INTERNODE_LENGTH_PREDIM, &PeduncleModel::_internode_length);
        External(INTERNODE_DIAM_PREDIM, &PeduncleModel::_internode_diam);
        External(INTERNODE_INDEX, &PeduncleModel::_internode_index);
         External(FTSW, &PeduncleModel::_ftsw);
    }

    virtual ~PeduncleModel()
    { }


    void compute(double /* t */, bool /* update */)
    {
        _p = _parameters.get(t).P;

        //predim  @TODO nettoyer les inits hors du compute
        _length_predim = _internode_length * _ratio_in_ped;
        _diam_predim = _internode_diam * _peduncle_diam;

        //ReductionINER
        if (_ftsw < _thresINER) {
            _reduction_iner = max(1e-4, ((1./_thresINER) * _ftsw)  * //@TODO vérifier l'équation
                                       (1. + (_p * _respINER)));
        } else {
            _reduction_iner = 1. + _p * _respINER;
        }

        //INER
        _iner = _inter_predim * _reduction_iner / (_plasto + _internode_index * (_ligulo - _plasto));


    }

    void init(double /* t */,
              const ecomeristem::ModelParameters&  parameters )
    {
        _parameters = parameters;
        // parameters
        _ratio_in_ped = _parameters.get < double >("ratio_INPed");
        _peduncle_diam = _parameters.get < double >("peduncle_diam");
        _thresINER = parameters.get < double >("thresINER");
        _respINER = parameters.get < double >("resp_LER");

        // internals
        _length_predim = 0;
        _diam_predim = 0;
        _reduction_iner = 0;
//        _peduncle_phase = peduncle::INITIAL;
//        _time_from_app = 0;
    }
private:
    ecomeristem::ModelParameters  _parameters;
    // parameters
    double _ratio_in_ped;
    double _peduncle_diam;
    double _thresINER;
    double _respINER;

    // externals
    double _internode_length;
    double _internode_diam;
    double _internode_index;
    double _ftsw;
    double _p;

//    culm::culm_phase _culm_phase;
//    plant::plant_state _plant_state;
//    plant::plant_phase _plant_phase;

    // internals
    double _length_predim;
    double _diam_predim;
    double _reduction_iner;
//    double _time_from_app;
//    peduncle::peduncle_phase _peduncle_phase;
//    double _volume;
//    double _DDInitiation;
//    double _DDRealization;
//    double _stock;

};

} } // namespace ecomeristem peduncle
