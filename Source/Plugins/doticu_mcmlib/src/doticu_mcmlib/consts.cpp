/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/maybe.h"

#include "doticu_skylib/game.h"
#include "doticu_skylib/game_macros.h"
#include "doticu_skylib/mod.h"
#include "doticu_skylib/quest.h"

#include "doticu_mcmlib/consts.h"

namespace doticu_mcmlib {

    maybe<Mod_t*>   Consts_t::SkyUI_Mod()                   { DEFINE_MOD(SKYUI_ESP_NAME); }

    some<Quest_t*>  Consts_t::SkyUI_Config_Manager_Quest()  { DEFINE_FORM(SkyUI_Mod(), Quest_t, 0x000802); }

}
