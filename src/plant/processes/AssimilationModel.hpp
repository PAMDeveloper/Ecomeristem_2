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


#ifndef ASSIMILATION_MODEL_HPP
#define ASSIMILATION_MODEL_HPP

#include <defines.hpp>
#include <plant/PlantState.hpp>

namespace model {

class AssimilationModel : public AtomicModel < AssimilationModel >
{
public:
    enum internals { ASSIM, ASSIM_POT, INTERC, LAI, RESP_MAINT };

    enum externals { CSTR, FCSTR, PAI, LEAFBIOMASS, INTERNODEBIOMASS };


    AssimilationModel() {
        //  computed variables
        Internal(ASSIM, &AssimilationModel::_assim);
        Internal(ASSIM_POT, &AssimilationModel::_assim_pot);
        Internal(RESP_MAINT, &AssimilationModel::_resp_maint);
        Internal(ASSIM_POT, &AssimilationModel::_assim_pot);
        Internal(INTERC, &AssimilationModel::_interc);
        Internal(LAI, &AssimilationModel::_lai);

        //  external variables
        External(CSTR, &AssimilationModel::_cstr);
        External(FCSTR, &AssimilationModel::_fcstr);
        External(PAI, &AssimilationModel::_PAI);
        External(LEAFBIOMASS, &AssimilationModel::_LeafBiomass);
        External(INTERNODEBIOMASS, &AssimilationModel::_InternodeBiomass);
    }

    virtual ~AssimilationModel()
    {}

    void compute(double t, bool /* update */) {
        //  assim
        _assim = std::max(0., _assim_pot / _density - _resp_maint);

        //  assimPot
        _radiation = _parameters.get(t).Par;
        _assim_pot = std::pow(_cstr, _power_for_cstr) * _interc * _epsib * _radiation * _kpar;
        //  interc
        _interc = 1. - std::exp(-_kdf * _lai);

        //  lai
        _lai = _PAI * (_rolling_B + _rolling_A * _fcstr) * _density / 1.e4;

        //  respMaint (== à _resp_maint ?)
        _Ta = _parameters.get(t).Temperature;
        _resp_maint = (_Kresp_leaf * _LeafBiomass + _Kresp_internode * _InternodeBiomass) *
                std::pow(2., (_Ta - _Tresp) / 10.);

    }

    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //  parameters variables
        _density = parameters.get < double >("density");
        _power_for_cstr = parameters.get < double >("power_for_cstr");
        _kpar = 1 /* parameters.get < double >("kpar") */;
        _epsib = parameters.get < double >("Epsib");
        _kdf = parameters.get < double >("Kdf");
        _rolling_A = parameters.get < double >("Rolling_A");
        _rolling_B = parameters.get < double >("Rolling_B");
        _Kresp_leaf = parameters.get < double >("Kresp");
        _Kresp_internode = parameters.get < double >("Kresp_internode");
        _Tresp = parameters.get < double >("Tresp");

        //  parameters variables (t)

        //  computed variables (internal)
        _assim = 0;
        _resp_maint = 0;
        _assim_pot = 0;
        _interc = 0;
        _lai = 0;

        //  external variables

    }

private:
    ecomeristem::ModelParameters _parameters;

    //  parameters
    double _density;
    double _power_for_cstr;
    double _kpar; // pas encore un paramètre dans le fichier txt
    double _epsib;
    double _kdf;
    double _rolling_A;
    double _rolling_B;
    double _Kresp_leaf;
    double _Kresp_internode;
    double _Tresp;

    //  parameters(t)
    double _radiation;
    double _Ta;

    //  internals - computed
    double _assim;
    double _resp_maint;
    double _assim_pot;
    double _interc;
    double _lai;

    //  externals
    double _cstr;
    double _fcstr;
    double _PAI;
    double _LeafBiomass;
    double _InternodeBiomass;
};

} // namespace model
#endif
