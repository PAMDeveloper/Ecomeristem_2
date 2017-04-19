/**
 * @file ecomeristem/plant/stock/Model.hpp
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


#ifndef TILLERING_MODEL_HPP
#define TILLERING_MODEL_HPP

#include <defines.hpp>
#include <plant/PlantState.hpp>

namespace model {

class TilleringModel : public AtomicModel < TilleringModel >
{
public:
    enum internals { NB_TILLERS, CREATE };

    enum externals { IC, BOOL_CROSSED_PLASTO, TAE };


    TilleringModel() {
        //    computed variables
        Internal(NB_TILLERS, &TilleringModel::_nb_tillers);
        Internal(CREATE, &TilleringModel::_create);

        //    external variables
        External(IC, &TilleringModel::_ic);
        External(BOOL_CROSSED_PLASTO, &TilleringModel::_boolCrossedPlasto);
        External(TAE, &TilleringModel::_tae);
    }

    virtual ~TilleringModel()
    {}

    void compute(double t, bool /* update */) {
        // Tillering
        _create = 0;
        if (_ic > _Ict) {
            _nb_tillers = _nb_tillers + _nbExistingTillers;
        }
        if (_boolCrossedPlasto > 0 and _nb_tillers >= 1 and _ic > _Ict * ((_P * _resp_Ict) + 1)) {
            _nb_tillers = std::min(_nb_tillers, _tae);
            _nbExistingTillers = _nbExistingTillers + _nb_tillers;
            _create = 1;
        }
    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //    parameters variables
        _Ict = _parameters.get < double >("Ict");
        //@TODO implement test pour tillering
//        _nbleaf_enabling_tillering = _parameters.get < double >("nbleaf_enabling_tillering");
        _P = 0 /* _parameters.get < double >("P") */; // pas encore dans le fichier parametre
        _resp_Ict = _parameters.get < double >("resp_Ict");
        //    parameters variables (t)


        //    computed variables (internal)
        _nb_tillers = 0;
        _nbExistingTillers = 1;
        _create = 0;

    }

private:
    ecomeristem::ModelParameters _parameters;

    //    parameters
    double _Ict;
//    double _nbleaf_enabling_tillering;
    double _P;
    double _resp_Ict;

    //    parameters(t)

    //    internals - computed
    double _nb_tillers;
    double _nbExistingTillers;
    double _create;

    //    externals
    double _ic;
    double _boolCrossedPlasto;
    double _tae;

};

} // namespace model
#endif
