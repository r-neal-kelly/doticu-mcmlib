/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/utils.h"

namespace doticu_mcmlib {

    #define DEFINE_OPTION()         \
    M                               \
        static Int_t option = -1;   \
        return option;              \
    W

}
