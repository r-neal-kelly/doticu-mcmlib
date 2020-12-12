/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/maybe.h"

#include "doticu_skylib/game.h"
#include "doticu_skylib/mod.h"
#include "doticu_skylib/quest.h"

#include "doticu_mcmlib/consts.h"

namespace doticu_mcmlib {

    #define DEFINE_MOD(NAME_)                                           \
    SKYLIB_M                                                            \
        static skylib::Mod_t* mod = skylib::Mod_t::Active_Mod(NAME_);   \
        return mod;                                                     \
    SKYLIB_W

    #define DEFINE_FORM(MOD_PTR_, TYPE_, LOWER_FORM_ID_)                        \
    SKYLIB_M                                                                    \
        SKYLIB_ASSERT(MOD_PTR_);                                                \
        static skylib::maybe<TYPE_*> form = static_cast<skylib::maybe<TYPE_*>>  \
            (skylib::Game_t::Form(MOD_PTR_, LOWER_FORM_ID_));                   \
        SKYLIB_ASSERT(form);                                                    \
        return form;                                                            \
    SKYLIB_W

    skylib::Mod_t*      Consts_t::SkyUI_Mod()                   { DEFINE_MOD(SKYUI_ESP_NAME); }

    skylib::Quest_t*    Consts_t::SkyUI_Config_Manager_Quest()  { DEFINE_FORM(SkyUI_Mod(), skylib::Quest_t, 0x000802); }

    #undef DEFINE_MOD
    #undef DEFINE_FORM

}
