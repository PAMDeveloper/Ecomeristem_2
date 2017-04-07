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


#ifndef WATER_BALANCE_MODEL_HPP
#define WATER_BALANCE_MODEL_HPP

#include <defines.hpp>
#include <plant/PlantState.hpp>

namespace model {

class WaterBalanceModel : public AtomicModel < WaterBalanceModel >
{
public:
    enum internals { };

    enum externals { };


    WaterBalanceModel() {
        //    computed variables
        Internal(IIII, &WaterBalanceModel::_iiii);


        //    external variables
        External(XXX, &WaterBalanceModel::_xxxx);

    }

    virtual ~WaterBalanceModel()
    {}


    void compute(double t, bool /* update */) {

    }



    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

        //    paramaters variables

        //    computed variables

    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters variables

    //    temporal parameters variables

    //    computed variables

    //    external variables
};

} // namespace model
#endif
