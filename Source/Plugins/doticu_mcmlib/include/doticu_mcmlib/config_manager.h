/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/quest.h"

#include "doticu_mcmlib/intrinsic.h"

namespace doticu_mcmlib {

    class Config_Base_t;

    class Config_Manager_t : public skylib::Quest_t
    {
    public:
        static String_t                 Class_Name();
        static V::Class_t*              Class();
        V::Object_t*                    Object();
        static some<Config_Manager_t*>  Self();

    public:
        V::Variable_tt<Vector_t<String_t>>&         Config_Names();
        V::Variable_tt<Vector_t<Config_Base_t*>>&   Config_Bases(); // SKI_ConfigBase[]

    public:
        void Change_Config_Base_Name(some<Config_Base_t*> config_base, String_t new_name);

    public:
        static void Register_Me(V::Machine_t* machine);
    };

}
