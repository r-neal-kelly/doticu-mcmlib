/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/intrinsic.h"

namespace doticu_mcmlib {

    #define DEFINE_OPTION()         \
    SKYLIB_M                        \
        static Int_t option = -1;   \
        return option;              \
    SKYLIB_W

}
