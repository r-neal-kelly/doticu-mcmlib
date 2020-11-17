/*
    Copyright � 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/intrinsic.h"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/quest.h"
#include "doticu_skylib/virtual.h"
#include "doticu_skylib/virtual_class.h"
#include "doticu_skylib/virtual_machine.h"

#include "doticu_mcmlib/intrinsic.h"

namespace doticu_mcmlib {

    using Variable_t = skylib::Virtual::Variable_t;
    using Array_t = skylib::Virtual::Array_t;
    using Class_t = skylib::Virtual::Class_t;
    using Object_t = skylib::Virtual::Object_t;
    using Machine_t = skylib::Virtual::Machine_t;

    enum class Flag_e : Int_t
    {
        NONE = 0,
        DISABLE = 1 << 0,
        HIDE = 1 << 1,
        UNMAP = 1 << 2,
    };

    enum class Cursor_e : Int_t {
        LEFT_TO_RIGHT = 1,
        TOP_TO_BOTTOM = 2,
    };

    enum class State_e : Int_t {
        DEFAULT = 0,
        RESET = 1,
        SLIDER = 2,
        MENU = 3,
        COLOR = 4,
    };

    enum class Option_e : Int_t {
        EMPTY = 0,
        HEADER = 1,
        TEXT = 2,
        TOGGLE = 3,
        SLIDER = 4,
        MENU = 5,
        COLOR = 6,
        KEYMAP = 7,
        INPUT = 8,
    };

    class Config_Base_t : public skylib::Quest_t {
    public:
        static constexpr const char* JOURNAL_MENU   = "Journal Menu";
        static constexpr const char* ROOT_MENU      = "_root.ConfigPanelFader.configPanel";

        static String_t Class_Name();
        static Class_t* Class();

    public:
        Object_t* Object();

        Variable_t* Current_Page_Name_Variable();
        Variable_t* Current_Page_Number_Variable();
        Variable_t* Current_State_Variable(); // Int_t
        Variable_t* Cursor_Position_Variable();
        Variable_t* Cursor_Fill_Mode_Variable();
        Variable_t* Flags_Variable(); // Array_t* of Int_t
        Variable_t* Labels_Variable(); // Array_t* of String_t
        Variable_t* String_Values_Variable(); // Array_t* of String_t
        Variable_t* Number_Values_Variable(); // Array_t* of Float_t
        Variable_t* States_Variable();
        Variable_t* Info_Text_Variable();
        Variable_t* Slider_Parameters_Variable(); // Array_t* of Float_t
        Variable_t* Menu_Parameters_Variable(); // Array_t* of Int_t
        Variable_t* Is_Waiting_For_Message_Variable(); // Bool_t
        Variable_t* Message_Result_Variable(); // Bool_t

        Variable_t* Mod_Name_Property(); // String_t
        Variable_t* Pages_Property(); // Array_t* of String_t

        String_t Current_Page_Name();
        Int_t Current_Page_Number();
        void Current_Page(String_t name);
        State_e Current_State();
        Int_t Cursor_Position();
        Cursor_e Cursor_Fill_Mode();
        Array_t* Flags();
        Array_t* Labels();
        Array_t* String_Values();
        Array_t* Number_Values();
        Array_t* States();

        void Cursor_Position(Int_t cursor_position);
        void Cursor_Fill_Mode(Cursor_e cursor_fill_mode);

        Int_t Pack_Flags(Flag_e flags, Option_e option_type);
        Int_t Pack_Option_ID(Int_t page_number, Int_t cursor_position);

        void Clear_Buffers();
        void Write_Buffers();

        void Title_Text(String_t title);

        String_t Info_Text();
        void Info_Text(String_t info);

        Int_t Add_Option(Option_e option_type, String_t label, String_t string, Float_t number, Flag_e flags);
        Int_t Add_Empty_Option();
        Int_t Add_Header_Option(String_t label, Flag_e flags = Flag_e::NONE);
        Int_t Add_Text_Option(String_t label, String_t value, Flag_e flags = Flag_e::NONE);
        Int_t Add_Toggle_Option(String_t label, Bool_t is_toggled, Flag_e flags = Flag_e::NONE);
        Int_t Add_Slider_Option(String_t label, Float_t value, String_t format = "{0}", Flag_e flags = Flag_e::NONE);
        Int_t Add_Menu_Option(String_t label, String_t value, Flag_e flags = Flag_e::NONE);
        Int_t Add_Color_Option(String_t label, Int_t color, Flag_e flags = Flag_e::NONE);
        Int_t Add_Keymap_Option(String_t label, Int_t key_code, Flag_e flags = Flag_e::NONE);
        Int_t Add_Input_Option(String_t label, String_t value, Flag_e flags = Flag_e::NONE);

        void Number_Option_Value(Int_t index, Float_t value, Bool_t do_render);
        void String_Option_Value(Int_t index, String_t value, Bool_t do_render);
        void Both_Option_Values(Int_t index, String_t string_value, Float_t number_value, Bool_t do_render);

        void Slider_Dialog_Current_Value(Float_t value);
        void Slider_Dialog_Default_Value(Float_t value);
        void Slider_Dialog_Range_Values(Float_t min, Float_t max);
        void Slider_Dialog_Interval_Value(Float_t value);
        void Menu_Dialog_Values(Vector_t<String_t> values);
        void Menu_Dialog_Default(Int_t index);

        void Text_Option_Value(Int_t option, String_t text, Bool_t do_render = true);
        void Toggle_Option_Value(Int_t option, Bool_t value, Bool_t do_render = true);
        void Slider_Option_Value(Int_t option, Float_t value, String_t format = "{0}", Bool_t do_render = true);
        void Menu_Option_Value(Int_t option, String_t value, Bool_t do_render = true);
        void Input_Option_Value(Int_t option, String_t value, Bool_t do_render = true);
        void Keymap_Option_Value(Int_t option, Int_t key_code, Bool_t do_render = true);

        void Option_Flags(Int_t option, Flag_e flags, Bool_t do_render = true);

        void Enable_Option(Int_t option, Bool_t do_render = true, Bool_t with_unmap = false);
        void Disable_Option(Int_t option, Bool_t do_render = true);
        void Show_Option(Int_t option, Bool_t do_render = true, Bool_t with_unmap = false);
        void Hide_Option(Int_t option, Bool_t do_render = true);
        void Flicker_Option(Int_t option, Callback_i<>* ucallback = nullptr);

        void Show_Message(String_t message,
                          Bool_t allow_cancel = true,
                          String_t accept_label = "$Accept",
                          String_t cancel_label = "$Cancel",
                          Callback_i<Bool_t>* user_callback = nullptr);

        void Reset_Page();
        void Unlock();
        void Open_Page(String_t page_name);

    public:
        static void Register_Me(Machine_t* machine);
    };

}
