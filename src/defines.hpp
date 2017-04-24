#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <artis/kernel/AbstractAtomicModel.hpp>
#include <artis/kernel/AbstractCoupledModel.hpp>
#include <artis/kernel/Simulator.hpp>
#include <artis/observer/Observer.hpp>
#include <artis/observer/View.hpp>
#include <artis/utils/DoubleTime.hpp>
#include <ModelParameters.hpp>

class PlantModel;

// Plant enums
namespace plant {
enum plant_phase { INIT = 0,
                   INITIAL = 1,
                   GROWTH = 2,
                   NOGROWTH = 3,
                   NEW_PHYTOMER = 5,
                   NOGROWTH2 = 18,
                   NOGROWTH3 = 19,
                   NOGROWTH4 = 20,
                   NEW_PHYTOMER3 = 23,
                   LIG = 24,
                   KILL = 25 };

enum plant_state {  VEGETATIVE = 0, PRE_ELONG, ELONG, PRE_PI, PI, PRE_FLO,
                    FLO, END_FILLING, MATURITY, DEAD };
}


struct GlobalParameters
{ };

using Model = artis::kernel::AbstractModel < artis::utils::DoubleTime,
                                             ecomeristem::ModelParameters >;

using Trace = artis::utils::Trace < artis::utils::DoubleTime >;

using TraceElement = artis::utils::TraceElement < artis::utils::DoubleTime >;

template < typename T >
using AtomicModel = artis::kernel::AbstractAtomicModel <
    T, artis::utils::DoubleTime, ecomeristem::ModelParameters >;

template < typename T >
using CoupledModel = artis::kernel::AbstractCoupledModel <
    T, artis::utils::DoubleTime, ecomeristem::ModelParameters, GlobalParameters >;

typedef artis::observer::Observer < artis::utils::DoubleTime,
                                    ecomeristem::ModelParameters > Observer;

typedef artis::observer::View < artis::utils::DoubleTime,
                                ecomeristem::ModelParameters > View;

typedef artis::kernel::Simulator < PlantModel,
                                   artis::utils::DoubleTime,
                                   ecomeristem::ModelParameters,
                                   GlobalParameters > EcomeristemSimulator;

typedef artis::context::Context < artis::utils::DoubleTime > EcomeristemContext;

#endif // DEFINES_HPP
