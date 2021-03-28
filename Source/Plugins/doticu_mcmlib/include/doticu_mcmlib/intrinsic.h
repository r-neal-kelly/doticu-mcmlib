/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

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

    template <typename T>
    using none          = skylib::none<T>;
    template <typename T>
    using maybe         = skylib::maybe<T>;
    template <typename T>
    using some          = skylib::some<T>;
    template <typename T>
    using unique        = skylib::unique<T>;

    template <typename T>
    using Enum_t        = skylib::Enum_t<T>;
    template <typename T>
    using Vector_t      = skylib::Vector_t<T>;
    template <typename ...Ts>
    using Callback_i    = skylib::Callback_i<Ts...>;

    using Form_t        = skylib::Form_t;
    using Game_t        = skylib::Game_t;
    using Mod_t         = skylib::Mod_t;
    using Quest_t       = skylib::Quest_t;
    using UI_t          = skylib::UI_t;

    namespace V {

        using Arguments_t       = skylib::Virtual::Arguments_t;
        using Array_t           = skylib::Virtual::Array_t;
        using Callback_i        = skylib::Virtual::Callback_i;
        using Callback_t        = skylib::Virtual::Callback_t;
        using Class_t           = skylib::Virtual::Class_t;
        using Machine_t         = skylib::Virtual::Machine_t;
        using Object_t          = skylib::Virtual::Object_t;
        using Variable_t        = skylib::Virtual::Variable_t;
        using Utility_t         = skylib::Virtual::Utility_t;

        template <typename T>
        using Variable_tt       = skylib::Virtual::Variable_tt<T>;

    }

}
