/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/cstring.h"

#include "doticu_skylib/form.h"

#include "doticu_skylib/virtual_arguments.h"
#include "doticu_skylib/virtual_array.h"
#include "doticu_skylib/virtual_callback.h"
#include "doticu_skylib/virtual_class.h"
#include "doticu_skylib/virtual_machine.h"
#include "doticu_skylib/virtual_macros.h"
#include "doticu_skylib/virtual_object.h"
#include "doticu_skylib/virtual_utils.h"
#include "doticu_skylib/virtual_variable.h"

#include "doticu_mcmlib/consts.h"
#include "doticu_mcmlib/config_base.h"
#include "doticu_mcmlib/config_manager.h"

namespace doticu_mcmlib {

    String_t            Config_Manager_t::Class_Name()  { DEFINE_CLASS_NAME("SKI_ConfigManager"); }
    V::Class_t*         Config_Manager_t::Class()       { DEFINE_CLASS(); }
    V::Object_t*        Config_Manager_t::Object()      { DEFINE_OBJECT(); }
    Config_Manager_t*   Config_Manager_t::Self()        { return static_cast<Config_Manager_t*>(Consts_t::SkyUI_Config_Manager_Quest()); }

    V::Array_Variable_t<String_t>*          Config_Manager_t::Config_Names_Variable()   { DEFINE_ARRAY_VARIABLE(String_t, "_modNames"); }
    V::Array_Variable_t<Config_Base_t*>*    Config_Manager_t::Config_Bases_Variable()   { DEFINE_ARRAY_VARIABLE(Config_Base_t*, "_modConfigs"); }

    V::Array_t* Config_Manager_t::Config_Names()    { return Config_Names_Variable()->Array(); }
    V::Array_t* Config_Manager_t::Config_Bases()    { return Config_Bases_Variable()->Array(); }

    void Config_Manager_t::Change_Config_Base_Name(Config_Base_t* config_base, String_t new_name)
    {
        if (config_base) {
            V::Array_t* config_names = Config_Names();
            V::Array_t* config_bases = Config_Bases();
            if (config_names && config_bases && config_names->count == config_bases->count) {
                for (Index_t idx = 0, end = config_bases->count; idx < end; idx += 1) {
                    V::Variable_t* config_base_variable = config_bases->Point(idx);
                    if (config_base_variable && config_base_variable->Unpack<Quest_t*>() == config_base) {
                        V::Variable_t* config_name_variable = config_names->Point(idx);
                        if (config_name_variable) {
                            config_name_variable->String(new_name);
                            config_base->Mod_Name(new_name);
                        }
                        return;
                    }
                }
            }
        }
    }

    void Config_Manager_t::Register_Me(V::Machine_t* machine)
    {
        #define METHOD(STR_FUNC_, ARG_NUM_, RETURN_, METHOD_, ...)  \
        SKYLIB_M                                                    \
            BIND_METHOD(machine, Class_Name(), Config_Manager_t,    \
                        STR_FUNC_, ARG_NUM_,                        \
                        RETURN_, METHOD_, __VA_ARGS__);             \
        SKYLIB_W

        #undef METHOD
    }

}
