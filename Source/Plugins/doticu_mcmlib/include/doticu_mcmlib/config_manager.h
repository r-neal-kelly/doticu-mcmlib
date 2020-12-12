/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/quest.h"

#include "doticu_mcmlib/intrinsic.h"

namespace doticu_mcmlib {

    namespace V {

        using Arguments_t       = skylib::Virtual::Arguments_t;
        using Array_t           = skylib::Virtual::Array_t;
        using Callback_i        = skylib::Virtual::Callback_i;
        using Callback_t        = skylib::Virtual::Callback_t;
        using Class_t           = skylib::Virtual::Class_t;
        using Machine_t         = skylib::Virtual::Machine_t;
        using Object_t          = skylib::Virtual::Object_t;
        using Variable_t        = skylib::Virtual::Variable_t;
        using String_Variable_t = skylib::Virtual::String_Variable_t;
        using Utils_t           = skylib::Virtual::Utils_t;

        template <typename Type_t>
        using Array_Variable_t  = skylib::Virtual::Array_Variable_t<Type_t>;

    }

    class Config_Base_t;

    class Config_Manager_t : public skylib::Quest_t {
    public:
        static String_t             Class_Name();
        static V::Class_t*          Class();
        V::Object_t*                Object();
        static Config_Manager_t*    Self();

    public:
        V::Array_Variable_t<String_t>*          Config_Names_Variable();
        V::Array_Variable_t<Config_Base_t*>*    Config_Bases_Variable(); // SKI_ConfigBase[]

    public:
        V::Array_t* Config_Names();
        V::Array_t* Config_Bases();

    public:
        void Change_Config_Base_Name(Config_Base_t* config_base, String_t new_name);

    public:
        static void Register_Me(V::Machine_t* machine);
    };

}
