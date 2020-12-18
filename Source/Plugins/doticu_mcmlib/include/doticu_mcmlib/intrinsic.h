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

}
