/*
    Copyright � 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "intrinsic.h"

namespace doticu_mcmlib {

    class Consts_t {
    public:
        static constexpr const char* SKYUI_ESP_NAME = "SkyUI_SE.esp";

        static skylib::Mod_t*   SkyUI_Mod();

        static skylib::Quest_t* SkyUI_Config_Manager_Quest();
    };

}
