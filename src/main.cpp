/**
 * @file app/main.cpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS file
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

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <qtapp/mainwindow.h>
#include <QApplication>
#include <QDebug>
#include <QDate>
#include <iostream>

#include <defines.hpp>
#include <artis/utils/DateTime.hpp>
#include <artis/utils/Trace.hpp>

#include <observer/PlantView.hpp>
#include <plant/PlantModel.hpp>
#include <utils/ParametersReader.hpp>
#include <utils/resultparser.h>


//using namespace artis::kernel;

void display(map<string, vector<double>> map){
    for(auto token: map) {
        //        if(token.first != "LIG" && token.first != "lig")
        //            continue;
        qDebug() << "**************" << QString::fromStdString(token.first) << "**************";
        QString vals = "";
        for(double val: token.second) {
            vals += QString::number(val) + ",";
        }
        qDebug() << vals;
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //    std::string dirName = "D:/PAMStudio_dev/data/ecomeristem/sample";
    //    std::string dirName = "D:/Samples/ecomeristem_og_testSample";


//    std::string dirName = "D:/PAMStudio_dev/data/ecomeristem/ng";
//    ecomeristem::ModelParameters parameters;
//    utils::ParametersReader reader;
//    reader.loadParametersFromFiles(dirName, parameters);
//    qDebug() << parameters.beginDate << parameters.get("EndDate");

    //  ::Trace::trace().clear();

//    auto vobsMap = reader.loadVObsFromFile("D:/PAMStudio_dev/data/ecomeristem/ng/vobs_G1_C_BFF2015.txt");
//    for (int i = 0; i < 200000; ++i) {
//        GlobalParameters globalParameters;
//        EcomeristemContext context(parameters.get("BeginDate"), parameters.get("EndDate"));
//        PlantModel * m = new PlantModel;
//        EcomeristemSimulator simulator(m, globalParameters);
//        observer::PlantView *view = new observer::PlantView();
//        simulator.attachView("plant", view);
//        simulator.init(parameters.get("BeginDate"), parameters);
//        simulator.run(context);

//        ResultParser parser;
//        auto resultMap = parser.resultsToMap(&simulator);
//        auto vobsFilteredMap = parser.filterVObs(vobsMap, resultMap);
//        auto reducedRMap = parser.reduceResults(resultMap, vobsMap);
//        qDebug() << i ;
//    }

    //  string lis= "";
    //  for(auto token: resultMap) {
    //      lis += " " + token.first;
    //  }
    //  qDebug() << QString::fromStdString(lis);
    //  qDebug() << "RESULT";
    //  display(resultMap);
    //  qDebug() << "VOBS";
    //  display(vobsMap);
    //  qDebug() << "FILTERED VOBS";
    //  display(vobsFilteredMap);
    //  qDebug() << "REDUCED";
    //  display(reducedRMap);

    //    std::ofstream out("Trace.txt");
    //    out << std::fixed << ::Trace::trace().elements().to_string();
    //    out.close();

    MainWindow w;
    w.show();
    //    w.show_trace();
    //  w.displayData(view, QString::fromStdString(dirName), &parameters,
    //                parameters.get("BeginDate"),
    //                parameters.get("EndDate"));

    a.exec();
    //  _CrtDumpMemoryLeaks();
    return 0;
}
