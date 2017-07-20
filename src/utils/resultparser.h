#ifndef RESULTPARSER_H
#define RESULTPARSER_H

#include <defines.hpp>

#include <observer/PlantView.hpp>
#include <utils/ParametersReader.hpp>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/lexical_cast.hpp>

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
        unsigned int gindex = 0;

        Observer::Views::const_iterator it = views.begin();
        View::Values values = it->second->values();
        double begin = it->second->begin();
        double end = it->second->end();

        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
            result.insert(std::pair<string,vector<double> >(itv->first, vector<double>()) );
        }

        // write values
        unsigned int index = 1;
        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
            View::Value::const_iterator itp = itv->second.begin();
            for (double t = begin; t <= end; ++t) {
                while (itp != itv->second.end() and itp->first < t) {
                    ++itp;
                }

                if (itp != itv->second.end()) {
                    string s = itp->second;
                    char* p;
                    double converted = strtod(s.c_str(), &p);
                    if (*p) {
                        result[itv->first].push_back(nan(""));
                    } else {
                        result[itv->first].push_back(converted);
                    }
                } else {
                    result[itv->first].push_back(nan(""));
                }
            }
        }
        return result;
    }

    map<string, vector<double>> filterVObs( map<string, vector<double>> vObs,
                                            map<string, double> constraints = map<string,double>(),
                                            string dayId = "Day")
    {
        map<string, vector<double>> filteredVObs;
        for(auto const &token : vObs) {
            filteredVObs.insert( pair<string,vector<double> >(token.first, vector<double>()) );
        }

        for (int i = 0; i < vObs[dayId].size(); ++i) {
            bool valid = true;
            for(auto const &token : vObs)  {
                if(constraints.find(token.first) != constraints.end()) {
                    valid |= vObs[token.first][i] == constraints[token.first];
                }
            }
            if(valid){
                for(auto const &token : vObs) {
                    filteredVObs[token.first].push_back(vObs[token.first][i]);
                }
            }
        }
        return filteredVObs;
    }

    map<string, vector<double>> reduceResults(map<string, vector<double>> results,
                                              map<string, vector<double>> vObs,
                                              map<string, double> constraints = map<string,double>(),
                                              string dayId = "Day") {
        map<string, vector<double>> filteredVObs;
        if(constraints.size() > 0)
            filteredVObs = filterVObs(vObs, constraints, dayId);
        else
            filteredVObs = vObs;

        map<string, vector<double>> reducedResults;
        for(auto const &token : filteredVObs) {
            if(results.find(token.first) != results.end()) {
                reducedResults.insert( pair<string,vector<double> >(token.first, vector<double>()) );
            }

            for(auto const &token : reducedResults) {
                for (int i = 0; i < filteredVObs[dayId].size(); ++i) {

                }
            }

        }
    }
};

#endif // RESULTPARSER_H
