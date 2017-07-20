///**
// * @file utils/ParametersReader.hpp
// * @author The Ecomeristem Development Team
// * See the AUTHORS file
// */
//
///*
// * Copyright (C) 2012-2015 ULCO http://www.univ-littoral.fr
// * Copyright (C) 2005-2015 Cirad http://www.cirad.fr
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// */
//
//#ifndef UTILS_PARAMETERS_READER_HPP
//#define UTILS_PARAMETERS_READER_HPP
//
//#include <model/models/ModelParameters.hpp>
//
//#include <utils/Connections.hpp>
//
//namespace utils {
//
//class ParametersReader
//{
//public:
//    ParametersReader()
//    { }
//
//    virtual ~ParametersReader()
//    { }
//
//    void load(const std::string& id,
//              model::models::ModelParameters& parameters);
//
//private:
//    void load_data(pqxx::connection& connection,
//                   const std::string& table,
//                   const std::string& id,
//                   const std::vector < std::string >& names,
//                   model::models::ModelParameters& parameters);
//    void load_simulation(const std::string& id,
//                         pqxx::connection& connection,
//                         model::models::ModelParameters& parameters);
//};
//
//} // namespace utils
//
//#endif

/*
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
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

#ifndef UTILS_PARAMETERS_READER_HPP
#define UTILS_PARAMETERS_READER_HPP

#include <ModelParameters.hpp>

#include <utils/Connections.hpp>

using namespace ecomeristem;
using namespace std;
namespace utils {

	class ParametersReader {
	public:
		ParametersReader()
		{ }

		virtual ~ParametersReader()
		{ }

		void loadParametersFromProstgresql(const std::string &id,
            ModelParameters &parameters);

		void loadParametersFromFiles(const std::string &folder,
            ModelParameters &parameters);

        map<string, vector<double>> loadVObsFromFile(const std::string &file_path);

	private:
        void load_meteo(PGconn* connection, ModelParameters &parameters);
		void load_data(PGconn* connection,
			const std::string &table,
			const std::string &id,
			const std::vector < std::string > &names,
            ModelParameters &parameters);

		void load_simulation(const std::string &id,
			PGconn* connection,
            ModelParameters &parameters);
	};
} // namespace utils

#endif

