/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/quest.h"

#include "doticu_mcmlib/intrinsic.h"

namespace doticu_mcmlib {

    class Flag_e : public Enum_t<Int_t>
    {
    public:
        enum : Int_t
        {
            NONE    = 0,
            DISABLE = 1 << 0,
            HIDE    = 1 << 1,
            UNMAP   = 1 << 2,
        };
        using Enum_t::Enum_t;
    };

    class Cursor_e : public Enum_t<Int_t>
    {
    public:
        enum : Int_t
        {
            LEFT_TO_RIGHT   = 1,
            TOP_TO_BOTTOM   = 2,
        };
        using Enum_t::Enum_t;
    };

    class State_e : public Enum_t<Int_t>
    {
    public:
        enum : Int_t
        {
            DEFAULT = 0,
            RESET   = 1,
            SLIDER  = 2,
            MENU    = 3,
            COLOR   = 4,
        };
        using Enum_t::Enum_t;
    };

    class Option_e : public Enum_t<Int_t>
    {
    public:
        enum : Int_t
        {
            EMPTY   = 0,
            HEADER  = 1,
            TEXT    = 2,
            TOGGLE  = 3,
            SLIDER  = 4,
            MENU    = 5,
            COLOR   = 6,
            KEYMAP  = 7,
            INPUT   = 8,
        };
        using Enum_t::Enum_t;
    };

    class Option_t
    {
    public:
        u16 page;
        u16 position;

        Option_t(u16 page, u16 position);
        Option_t(u32 packed_option);
        Option_t(Int_t packed_option);

        operator u32();
        operator Int_t();
    };

    class Config_Base_t : public skylib::Quest_t
    {
    public:
        static constexpr const char*    JOURNAL_MENU    = "Journal Menu";
        static constexpr const char*    ROOT_MENU       = "_root.ConfigPanelFader.configPanel";
        static constexpr size_t         MAX_POSITIONS   = 128;

        static String_t     Add_Font(some<const char*> string,
                                     maybe<const char*> color = "#FFFFFF",
                                     maybe<const char*> alpha = "#FF",
                                     maybe<const char*> size = "20");

        static String_t     Class_Name();
        static V::Class_t*  Class();
        V::Object_t*        Object();

    public:
        V::Variable_tt<String_t>&           Mod_Name();

        V::Variable_tt<String_t>&           Current_Page_Name();
        V::Variable_tt<Int_t>&              Current_Page_Number();
        V::Variable_tt<String_t>&           Current_Info_Text();
        V::Variable_tt<State_e>&            Current_State();
        V::Variable_tt<Int_t>&              Current_Cursor_Position();
        V::Variable_tt<Cursor_e>&           Current_Cursor_Mode();

        V::Variable_tt<Vector_t<String_t>>& Pages();
        V::Variable_tt<Vector_t<Int_t>>&    Flags();
        V::Variable_tt<Vector_t<String_t>>& Labels();
        V::Variable_tt<Vector_t<String_t>>& Strings();
        V::Variable_tt<Vector_t<Float_t>>&  Floats();
        V::Variable_tt<Vector_t<String_t>>& States();
        V::Variable_tt<Vector_t<Float_t>>&  Slider_Parameters();
        V::Variable_tt<Vector_t<Int_t>>&    Menu_Parameters();

        V::Variable_tt<Bool_t>&             Is_Waiting_For_Message();
        V::Variable_tt<Bool_t>&             Message_Result();

    public:
        void        Current_Page(String_t name);

        Int_t       Pack_Flags(Flag_e flags, Option_e option_type);

        void        Clear_Flags();
        void        Clear_Labels();
        void        Clear_Strings();
        void        Clear_Numbers();
        void        Clear_States();
        void        Clear_Buffers();

        void        Write_Buffers();

        void        Title_Text(String_t title);

        Bool_t      Can_Add_Options(Int_t count = 1);

    public:
        Int_t       Add_Option(Option_e option_type, String_t label, String_t string, Float_t number, Flag_e flags);
        Int_t       Add_Empty_Option();
        Int_t       Add_Header_Option(String_t label, Flag_e flags = Flag_e::NONE);
        Int_t       Add_Text_Option(String_t label, String_t value, Flag_e flags = Flag_e::NONE);
        Int_t       Add_Toggle_Option(String_t label, Bool_t is_toggled, Flag_e flags = Flag_e::NONE);
        Int_t       Add_Slider_Option(String_t label, Float_t value, String_t format = "{0}", Flag_e flags = Flag_e::NONE);
        Int_t       Add_Menu_Option(String_t label, String_t value, Flag_e flags = Flag_e::NONE);
        Int_t       Add_Color_Option(String_t label, Int_t color, Flag_e flags = Flag_e::NONE);
        Int_t       Add_Keymap_Option(String_t label, Int_t key_code, Flag_e flags = Flag_e::NONE);
        Int_t       Add_Input_Option(String_t label, String_t value, Flag_e flags = Flag_e::NONE);

        void        Number_Option_Value(Int_t index, Float_t value, Bool_t do_render);
        void        String_Option_Value(Int_t index, String_t value, Bool_t do_render);
        void        Both_Option_Values(Int_t index, String_t string_value, Float_t number_value, Bool_t do_render);

        void        Slider_Dialog_Current_Value(Float_t value);
        void        Slider_Dialog_Default_Value(Float_t value);
        void        Slider_Dialog_Range_Values(Float_t min, Float_t max);
        void        Slider_Dialog_Interval_Value(Float_t value);
        void        Menu_Dialog_Values(Vector_t<String_t> values);
        void        Menu_Dialog_Default(Int_t index);

        void        Text_Option_Value(Int_t option, String_t text, Bool_t do_render = true);
        void        Toggle_Option_Value(Int_t option, Bool_t value, Bool_t do_render = true);
        void        Slider_Option_Value(Int_t option, Float_t value, String_t format = "{0}", Bool_t do_render = true);
        void        Menu_Option_Value(Int_t option, String_t value, Bool_t do_render = true);
        void        Input_Option_Value(Int_t option, String_t value, Bool_t do_render = true);
        void        Keymap_Option_Value(Int_t option, Int_t key_code, Bool_t do_render = true);

        void        Option_Flags(Int_t option, Flag_e flags, Bool_t do_render = true);

        void        Enable_Option(Int_t option, Bool_t do_render = true, Bool_t with_unmap = false);
        void        Disable_Option(Int_t option, Bool_t do_render = true);
        void        Show_Option(Int_t option, Bool_t do_render = true, Bool_t with_unmap = false);
        void        Hide_Option(Int_t option, Bool_t do_render = true);
        void        Flicker_Option(Int_t option, Callback_i<>* ucallback = nullptr);

        void        Show_Message(String_t message,
                                 Bool_t allow_cancel = true,
                                 String_t accept_label = "$Accept",
                                 String_t cancel_label = "$Cancel",
                                 Callback_i<Bool_t>* user_callback = nullptr);

        void        Reset_Page();
        void        Unlock();
        void        Open_Page(String_t page_name);

    public:
        static void Register_Me(some<V::Machine_t*> machine);
    };

}
