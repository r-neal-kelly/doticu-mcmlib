/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/collections.h"
#include "doticu_skylib/interface.h"
#include "doticu_skylib/intrinsic.h"
#include "doticu_skylib/maybe.h"
#include "doticu_skylib/string.h"

#include "doticu_skylib/skylib.h"
#include "doticu_skylib/virtual.h"

namespace doticu_skylib {}
namespace skylib = doticu_skylib;

namespace doticu_mcmlib {

    using u16           = skylib::u16;
    using u32           = skylib::u32;

    using Bool_t        = skylib::Bool_t;
    using Int_t         = skylib::Int_t;
    using Float_t       = skylib::Float_t;
    using String_t      = skylib::String_t;

    using Index_t       = skylib::Index_t;

    template <typename Type>
    using none          = skylib::none<Type>;
    template <typename Type>
    using some          = skylib::some<Type>;
    template <typename Type>
    using maybe         = skylib::maybe<Type>;

    template <typename Type>
    using Enum_t        = skylib::Enum_t<Type>;
    template <typename T>
    using Vector_t      = skylib::Vector_t<T>;
    template <typename ...T>
    using Callback_i    = skylib::Callback_i<T...>;

    namespace V {

        using Arguments_t       = skylib::Virtual::Arguments_t;
        using Array_t           = skylib::Virtual::Array_t;
        using Callback_i        = skylib::Virtual::Callback_i;
        using Callback_t        = skylib::Virtual::Callback_t;
        using Class_t           = skylib::Virtual::Class_t;
        using Machine_t         = skylib::Virtual::Machine_t;
        using Object_t          = skylib::Virtual::Object_t;
        using Variable_t        = skylib::Virtual::Variable_t;
        using Int_Variable_t    = skylib::Virtual::Int_Variable_t;
        using Float_Variable_t  = skylib::Virtual::Float_Variable_t;
        using String_Variable_t = skylib::Virtual::String_Variable_t;
        using Utils_t           = skylib::Virtual::Utils_t;

        template <typename Type_t>
        using Array_Variable_t  = skylib::Virtual::Array_Variable_t<Type_t>;

    }

}
