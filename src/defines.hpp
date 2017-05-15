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

enum plant_state { NO_STATE = 0,
                   NOGROWTH = 1,
                   NEW_PHYTOMER = 2,
                   LIG = 4,
                   KILL = 8 };

enum plant_phase {  INITIAL = 0,
                    VEGETATIVE = 1,
                    ELONG = 2,
                    PI = 3,
                    PRE_FLO = 4,
                    FLO = 5,
                    END_FILLING = 6,
                    MATURITY = 7,
                    DEAD = 8 };

inline bool operator&(plant_state a, plant_state b)
{return (static_cast<int>(a) & static_cast<int>(b)) != 0;}
inline void operator<<(plant_state& a, plant_state b)
{a = static_cast<plant_state>(static_cast<int>(a) | static_cast<int>(b));}
inline void operator>>(plant_state& a, plant_state b)
{a = static_cast<plant_state>(static_cast<int>(a) & !static_cast<int>(b));}

//enum plant_phase { INIT = 0,
//                   INITIAL = 1,
//                   GROWTH = 2,
//                   NOGROWTH = 3,
//                   NEW_PHYTOMER = 5,
//                   NOGROWTH2 = 18,
//                   NOGROWTH3 = 19,
//                   NOGROWTH4 = 20,
//                   NEW_PHYTOMER3 = 23,
//                   LIG = 24,
//                   KILL = 25 };

//enum plant_state {  VEGETATIVE = 0,
//                    PRE_ELONG,
//                    ELONG,
//                    PRE_PI,
//                    PI,
//                    PRE_FLO,
//                    FLO,
//                    END_FILLING,
//                    MATURITY,
//                    DEAD };
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
