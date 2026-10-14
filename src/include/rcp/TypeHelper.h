/*
********************************************************************
* RabbitControl - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of RabbitControl for c++.
*
* Written by Ingo Randolf, 2018-2026
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_TYPEHELPER_H
#define RCP_TYPEHELPER_H

#include <type_traits>

namespace rcp {


// true for arithmetic, non-bool scalar types
template<typename T>
struct is_scalar_element
    : std::integral_constant<bool, std::is_arithmetic<T>::value && !std::is_same<T, bool>::value>
{
};

// convenience variable template
template<typename T>
inline constexpr bool is_scalar_element_v = is_scalar_element<T>::value;


// primary trait: defaults to the scalar check
template<typename T>
struct is_valid_parameter_number_element : is_scalar_element<T>
{
};

// for VectorX<T>
template<template<typename, typename> class VectorT, typename T>
struct is_valid_parameter_number_element<VectorT<T, void>> : is_scalar_element<T>
{
};

// convenience variable template
template<typename T>
inline constexpr bool is_valid_parameter_number_element_v = is_valid_parameter_number_element<T>::value;

}

#endif // TYPEHELPER_H
