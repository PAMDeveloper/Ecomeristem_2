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
    enum internals { IC, TEST_IC, NB_TILLERS, CREATE };

    enum externals { SEED_RES, SUPPLY, DAY_DEMAND, BOOL_CROSSED_PLASTO, TAE };


    TilleringModel() {
        //    computed variables
        Internal(IC, &TilleringModel::_ic);
        Internal(TEST_IC, &TilleringModel::_test_ic);
        Internal(NB_TILLERS, &TilleringModel::_nb_tillers);
        Internal(CREATE, &TilleringModel::_create);

        //    external variables
        External(SEED_RES, &TilleringModel::_seed_res);
        External(SUPPLY, &TilleringModel::_supply);
        External(DAY_DEMAND, &TilleringModel::_day_demand);
        External(BOOL_CROSSED_PLASTO, &TilleringModel::_boolCrossedPlasto);
        External(TAE, &TilleringModel::_tae);
    }

    virtual ~TilleringModel()
    {}

    void compute(double t, bool /* update */) {

        //  indice de competition - Proposition
        if (t != _parameters.beginDate) {
            double mean;
            unsigned int n = 2;
            if (_day_demand != 0) {
                _ic_[0] = std::max(0., _seed_res + _supply / _day_demand;
            } else {
                _ic_[0] = _ic;
            }

            mean = 2. * _ic_[0];

            for (unsigned int i = 1; i < 3; i++) {
                if (_ic_[i] != 0) {
                    mean = mean + _ic_[i];
                    n = n + 1;
                }
            }
            mean = mean / n;

            if (mean == 0) {
                _ic = 0.001;
                _test_ic = 0.001;
            } else {
                _ic = std::min(5.,mean);
                _test_ic = std::min(1., std::sqrt(_ic));
            }
        }

        // Day step
        _ic_[2] = _ic_[1];
        _ic_[1] = _ic_[0];

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
        _nbleaf_enabling_tillering = _parameters.get < double >("nbleaf_enabling_tillering");
        _P = 0 /* _parameters.get < double >("P") */; // pas encore dans le fichier parametre
        _resp_Ict = _parameters.get < double >("resp_Ict");
        //    parameters variables (t)


        //    computed variables (internal)
        _ic = 0;
        _test_ic = 0;
        _ic_[2] = _ic_[1] = _ic_[0] = 0;
        _nb_tillers = 0;
        _nbExistingTillers = 1;
        _create = 0;
        //    external variables

    }

private:
    ecomeristem::ModelParameters _parameters;

    //    parameters
    double _Ict;
    double _nbleaf_enabling_tillering;
    double _P;
    double _resp_Ict;

    //    parameters(t)

    //    internals - computed
    double _ic;
    double _test_ic;
    double _ic_[3];
    double _nb_tillers;
    double _nbExistingTillers;
    double _create;

    //    externals
    double _seed_res;
    double _supply;
    double _day_demand;
    double _boolCrossedPlasto;
    double _tae;

};

} // namespace model
#endif
