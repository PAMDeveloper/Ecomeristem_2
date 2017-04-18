#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <artis/kernel/AbstractAtomicModel.hpp>
#include <artis/kernel/AbstractCoupledModel.hpp>
#include <artis/observer/Observer.hpp>
#include <artis/observer/View.hpp>
#include <artis/utils/DoubleTime.hpp>
#include <ModelParameters.hpp>

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


#endif // DEFINES_HPP
