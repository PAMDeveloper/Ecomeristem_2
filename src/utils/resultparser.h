#ifndef RESULTPARSER_H
#define RESULTPARSER_H

#include <defines.hpp>

#include <observer/PlantView.hpp>
#include <utils/ParametersReader.hpp>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/lexical_cast.hpp>

#include <QDebug>
class ResultParser
{
public:
    ResultParser() {

    }

    //1 seule view
    map<string, vector<double>>  resultsToMap(EcomeristemSimulator * simulator) {
        map<string, vector<double>> result;
        const Observer& observer = simulator->observer();
        const Observer::Views& views = observer.views();
        Observer::Views::const_iterator it = views.begin();
        View::Values values = it->second->values();
        double begin = it->second->begin();
        double end = it->second->end();

        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
            string s = itv->first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            result.insert(std::pair<string,vector<double> >(s, vector<double>()) );
        }

        // write values
        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
            View::Value::const_iterator itp = itv->second.begin();
            string s = itv->first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);

            for (double t = begin; t <= end; ++t) {
                while (itp != itv->second.end() and itp->first < t) {
                    ++itp;
                }

                if (itp != itv->second.end()) {
                    string c = itp->second;
                    char* p;
                    double converted = strtod(c.c_str(), &p);
                    if (*p) {
                        result[s].push_back(nan(""));
                    } else {
                        result[s].push_back(converted);
                    }
                } else {
                    result[s].push_back(nan(""));
                }
            }
        }
        return result;
    }

    map<string, vector<double>> filterVObs( map<string, vector<double>> vObs,
                                            double dayMax,
                                            map<string, double> constraints = map<string,double>(),
                                            string dayId = "day")
    {
        map<string, vector<double>> filteredVObs;
        for(auto const &token : vObs) {
            string * s = new string(token.first);
            transform(s->begin(), s->end(), s->begin(), ::tolower);
            filteredVObs.insert( pair<string,vector<double> >(*s, vector<double>()) );
        }

        for (int i = 0; i < vObs[dayId].size(); ++i) {
            bool valid = true;

            valid &= vObs[dayId][i] <= dayMax;
            if(valid){
                for(auto const &token : vObs)  {
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    if(constraints.find(*s) != constraints.end()) {
                        valid &= token.second[i] == constraints[*s];
                    }
                }
            }

            if(valid){
                for(auto token : vObs) {
                    string * h = new string(token.first);
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    filteredVObs[*s].push_back(vObs[*h][i]);
                }
            }
        }


            return filteredVObs;
        }


    void display(map<string, vector<double>> map){
        for(auto token: map) {
            if(token.first != "lig")
                continue;
            qDebug() << "**************" << QString::fromStdString(token.first) << "**************";
            QString vals = "";
            for(double val: token.second) {
                vals += QString::number(val) + ",";
            }
            qDebug() << vals;
        }
    }

    map<string, vector<double>> reduceResults(map<string, vector<double> > results,
                                              map<string, vector<double> > vObs,
                                              map<string, double> constraints = map<string,double>(),
                                              string dayId = "day") {

        map<string, vector<double>> filteredVObs;
        if(constraints.size() > 0)
            filteredVObs = filterVObs(vObs, results.begin()->second.size(), constraints, dayId);
        else
            filteredVObs = vObs;

        map<string, vector<double>> reducedResults;
        for(auto const &token : filteredVObs) {
            string s = token.first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            if(results.find(s) != results.end()) {
                reducedResults.insert( pair<string,vector<double> >(s, vector<double>()) );
            }
        }


       for(auto const &r : reducedResults) {
            for (int i = 0; i < filteredVObs[dayId].size(); ++i) {
                int day = filteredVObs[dayId][i];
                if(day <= results[r.first].size())
                    reducedResults[r.first].push_back(results[r.first][day-1]);
            }
        }

        return reducedResults;
    }
};

#endif // RESULTPARSER_H
