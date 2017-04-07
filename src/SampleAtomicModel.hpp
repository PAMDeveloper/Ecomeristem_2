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

namespace model {

class SampleAtomicModel : public AtomicModel < SampleAtomicModel >
{
public:
    enum internals { };

    enum externals { };


    SampleAtomicModel() {
        //    computed variables
        Internal(IIII, &SampleAtomicModel::_iiii);


        //    external variables
        External(XXX, &SampleAtomicModel::_xxxx);

    }

    virtual ~SampleAtomicModel()
    {}


    void compute(double t, bool /* update */) {
		_tttt = _parameters.get(t).Temperature;
#ifdef WITH_TRACE
		Trace::trace() << TraceElement( path(this), t, artis::utils::COMPUTE)
				<< artis::utils::KernelInfo("IIII", true, boost::lexical_cast<std::string>(_iiii));
		Trace::trace().flush();
#endif
    }



    void init(double t, const ecomeristem::ModelParameters& parameters) {
        _parameters = parameters;

		//    parameters variables
		_gggg = _parameters.get < double >("GGGG");;
		
		//    parameters variables (t)
		_tttt = 0;

		//    computed variables (internal)
		_iiii = 0;

		//    external variables
		_xxxx = 0;

    }

private:
    ecomeristem::ModelParameters _parameters;
    //    parameters variables
	double _gggg;
	
    //    parameters variables (t)
	double _tttt;

    //    computed variables (internal)
	double _iiii;

    //    external variables
	double _xxxx;
};

} // namespace model
#endif
