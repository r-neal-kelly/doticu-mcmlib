/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/cstring.h"
#include "doticu_skylib/form.h"
#include "doticu_skylib/interface.inl"
#include "doticu_skylib/scrap_array.inl"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/ui.inl"
#include "doticu_skylib/virtual_arguments.h"
#include "doticu_skylib/virtual_array.h"
#include "doticu_skylib/virtual_callback.h"
#include "doticu_skylib/virtual_class.h"
#include "doticu_skylib/virtual_machine.h"
#include "doticu_skylib/virtual_macros.h"
#include "doticu_skylib/virtual_object.h"
#include "doticu_skylib/virtual_utility.h"
#include "doticu_skylib/virtual_variable.h"

#include "doticu_mcmlib/config_base.h"

namespace doticu_mcmlib {

    Option_t::Option_t(u16 page, u16 position) :
        page(page), position(position)
    {
    }

    Option_t::Option_t(u32 packed_option) :
        page(packed_option >> 8), position(packed_option & 0xFF)
    {
    }

    Option_t::Option_t(Int_t packed_option) :
        Option_t(static_cast<u32>(packed_option))
    {
    }

    Option_t::operator u32()
    {
        return (static_cast<skylib::u32>(page) << 8) | static_cast<skylib::u32>(position & 0xFF);
    }

    Option_t::operator Int_t()
    {
        return static_cast<Int_t>(operator u32());
    }

    String_t Config_Base_t::Add_Font(some<const char*> string,
                                     maybe<const char*> color,
                                     maybe<const char*> alpha,
                                     maybe<const char*> size)
    {
        SKYLIB_ASSERT_SOME(string);

        std::string font = "<font";

        if (color && color[0]) {
            font = font + " color='" + color() + "'";
        }
        if (alpha && alpha[0]) {
            font = font + " alpha='" + alpha() + "'";
        }
        if (size && size[0]) {
            font = font + " size='" + size() + "'";
        }

        return font + ">" + string() + "</font>";
    }

    String_t                            Config_Base_t::Class_Name()                 { DEFINE_CLASS_NAME("SKI_ConfigBase"); }
    V::Class_t*                         Config_Base_t::Class()                      { DEFINE_CLASS(); }
    V::Object_t*                        Config_Base_t::Object()                     { DEFINE_OBJECT_METHOD(); }

    V::Variable_tt<String_t>&           Config_Base_t::Mod_Name()                   { DEFINE_VARIABLE_REFERENCE(String_t, "::ModName_var"); }

    V::Variable_tt<String_t>&           Config_Base_t::Current_Page_Name()          { DEFINE_VARIABLE_REFERENCE(String_t, "_currentPage"); }
    V::Variable_tt<Int_t>&              Config_Base_t::Current_Page_Number()        { DEFINE_VARIABLE_REFERENCE(Int_t, "_currentPageNum"); }
    V::Variable_tt<String_t>&           Config_Base_t::Current_Info_Text()          { DEFINE_VARIABLE_REFERENCE(String_t, "_infoText"); }
    V::Variable_tt<State_e>&            Config_Base_t::Current_State()              { DEFINE_VARIABLE_REFERENCE(State_e, "_state"); }
    V::Variable_tt<Int_t>&              Config_Base_t::Current_Cursor_Position()    { DEFINE_VARIABLE_REFERENCE(Int_t, "_cursorPosition"); }
    V::Variable_tt<Cursor_e>&           Config_Base_t::Current_Cursor_Mode()        { DEFINE_VARIABLE_REFERENCE(Cursor_e, "_cursorFillMode"); }

    V::Variable_tt<Vector_t<String_t>>& Config_Base_t::Pages()                      { DEFINE_VARIABLE_REFERENCE(Vector_t<String_t>, "::Pages_var"); }
    V::Variable_tt<Vector_t<Int_t>>&    Config_Base_t::Flags()                      { DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "_optionFlagsBuf"); }
    V::Variable_tt<Vector_t<String_t>>& Config_Base_t::Labels()                     { DEFINE_VARIABLE_REFERENCE(Vector_t<String_t>, "_textBuf"); }
    V::Variable_tt<Vector_t<String_t>>& Config_Base_t::Strings()                    { DEFINE_VARIABLE_REFERENCE(Vector_t<String_t>, "_strValueBuf"); }
    V::Variable_tt<Vector_t<Float_t>>&  Config_Base_t::Floats()                     { DEFINE_VARIABLE_REFERENCE(Vector_t<Float_t>, "_numValueBuf"); }
    V::Variable_tt<Vector_t<String_t>>& Config_Base_t::States()                     { DEFINE_VARIABLE_REFERENCE(Vector_t<String_t>, "_stateOptionMap"); }
    V::Variable_tt<Vector_t<Float_t>>&  Config_Base_t::Slider_Parameters()          { DEFINE_VARIABLE_REFERENCE(Vector_t<Float_t>, "_sliderParams"); }
    V::Variable_tt<Vector_t<Int_t>>&    Config_Base_t::Menu_Parameters()            { DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "_menuParams"); }

    V::Variable_tt<Bool_t>&             Config_Base_t::Is_Waiting_For_Message()     { DEFINE_VARIABLE_REFERENCE(Bool_t, "_waitForMessage"); }
    V::Variable_tt<Bool_t>&             Config_Base_t::Message_Result()             { DEFINE_VARIABLE_REFERENCE(Bool_t, "_messageResult"); }

    void Config_Base_t::Current_Page(String_t name)
    {
        V::Array_t* pages = Pages().Array();
        if (pages) {
            for (size_t idx = 0, count = pages->count; idx < count; idx += 1) {
                String_t page = pages->Point(idx)->String();
                if (skylib::CString_t::Is_Same(page, name, true)) {
                    Current_Page_Name() = name;
                    Current_Page_Number() = idx + 1;
                    return;
                }
            }
            Current_Page_Name() = "";
            Current_Page_Number() = -1;
        } else {
            Current_Page_Name() = "";
            Current_Page_Number() = -1;
        }
    }

    Int_t Config_Base_t::Pack_Flags(Flag_e flags, Option_e option_type)
    {
        return (static_cast<Int_t>(flags) * 0x100) + static_cast<Int_t>(option_type);
    }

    void Config_Base_t::Clear_Flags()
    {
        V::Array_t* flags = Flags().Array();
        if (flags) {
            for (Index_t idx = 0, count = flags->count; idx < count; idx += 1) {
                auto var = static_cast<V::Variable_tt<Int_t>*>(flags->Point(idx));
                if (var) {
                    *var = 0;
                }
            }
        }
    }

    void Config_Base_t::Clear_Labels()
    {
        V::Array_t* labels = Labels().Array();
        if (labels) {
            for (Index_t idx = 0, count = labels->count; idx < count; idx += 1) {
                auto var = static_cast<V::Variable_tt<String_t>*>(labels->Point(idx));
                if (var) {
                    *var = "";
                }
            }
        }
    }

    void Config_Base_t::Clear_Strings()
    {
        V::Array_t* strings = Strings().Array();
        if (strings) {
            for (Index_t idx = 0, count = strings->count; idx < count; idx += 1) {
                auto var = static_cast<V::Variable_tt<String_t>*>(strings->Point(idx));
                if (var) {
                    *var = "";
                }
            }
        }
    }

    void Config_Base_t::Clear_Numbers()
    {
        V::Array_t* numbers = Floats().Array();
        if (numbers) {
            for (Index_t idx = 0, count = numbers->count; idx < count; idx += 1) {
                auto var = static_cast<V::Variable_tt<Float_t>*>(numbers->Point(idx));
                if (var) {
                    *var = 0.0f;
                }
            }
        }
    }

    void Config_Base_t::Clear_States()
    {
        V::Array_t* states = States().Array();
        if (states) {
            for (Index_t idx = 0, count = states->count; idx < count; idx += 1) {
                auto var = static_cast<V::Variable_tt<String_t>*>(states->Point(idx));
                if (var) {
                    *var = "";
                }
            }
        }
    }

    void Config_Base_t::Clear_Buffers()
    {
        // this could be optimized into one loop but there are concerns of stability
        // with unknown data-states in broken scripts that SkyUI doesn't handle properly.

        Clear_Flags();
        Clear_Labels();
        Clear_Strings();
        Clear_Numbers();
        Clear_States();

        Current_Cursor_Position() = 0;
        Current_Cursor_Mode() = Cursor_e::LEFT_TO_RIGHT;
    }

    void Config_Base_t::Write_Buffers()
    {
        V::Array_t* flags = Flags().Array();
        Int_t option_count = 0;
        for (size_t idx = 0, count = MAX_POSITIONS; idx < count; idx += 1) {
            Int_t flag = flags->Point(idx)->Int();
            if (flag != static_cast<Int_t>(Option_e::EMPTY)) {
                option_count += 1;
            }
        }

        UI_t::Run<Int_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionFlagsBuffer", flags);
        UI_t::Run<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionTextBuffer", Labels().Array());
        UI_t::Run<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionStrValueBuffer", Strings().Array());
        UI_t::Run<Float_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionNumValueBuffer", Floats().Array());
        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".flushOptionBuffers", option_count);
    }

    void Config_Base_t::Title_Text(String_t title)
    {
        UI_t::Run<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setTitleText", title);
    }

    Bool_t Config_Base_t::Can_Add_Options(Int_t count)
    {
        if (count < 0) {
            count = 0;
        }
        Int_t end = count + Current_Cursor_Position();
        return end > -1 && end <= MAX_POSITIONS;
    }

    Int_t Config_Base_t::Add_Option(Option_e option_type, String_t label, String_t string, Float_t number, Flag_e flags)
    {
        SKYLIB_ASSERT(Current_State() == State_e::RESET);

        Int_t position = Current_Cursor_Position();
        if (position > -1) {
            Flags().Array()->Point(position)->Int(Pack_Flags(flags, option_type));
            Labels().Array()->Point(position)->String(label);
            Strings().Array()->Point(position)->String(string);
            Floats().Array()->Point(position)->Float(number);

            Cursor_e cursor_mode = Current_Cursor_Mode();
            Int_t new_position = position + static_cast<Int_t>(cursor_mode);
            if (new_position >= MAX_POSITIONS) {
                Current_Cursor_Position() = -1;
            } else {
                Current_Cursor_Position() = new_position;
            }

            return Option_t(Current_Page_Number(), position);
        } else {
            return -1;
        }
    }

    Int_t Config_Base_t::Add_Empty_Option()
    {
        return Add_Option(Option_e::EMPTY, "", "", 0.0f, Flag_e::NONE);
    }

    Int_t Config_Base_t::Add_Header_Option(String_t label, Flag_e flags)
    {
        return Add_Option(Option_e::HEADER, label, "", 0.0f, flags);
    }

    Int_t Config_Base_t::Add_Text_Option(String_t label, String_t value, Flag_e flags)
    {
        return Add_Option(Option_e::TEXT, label, value, 0.0f, flags);
    }

    Int_t Config_Base_t::Add_Toggle_Option(String_t label, Bool_t is_toggled, Flag_e flags)
    {
        return Add_Option(Option_e::TOGGLE, label, "", static_cast<Float_t>(is_toggled), flags);
    }

    Int_t Config_Base_t::Add_Slider_Option(String_t label, Float_t value, String_t format, Flag_e flags)
    {
        return Add_Option(Option_e::SLIDER, label, format, value, flags);
    }

    Int_t Config_Base_t::Add_Menu_Option(String_t label, String_t value, Flag_e flags)
    {
        return Add_Option(Option_e::MENU, label, value, 0.0f, flags);
    }

    Int_t Config_Base_t::Add_Color_Option(String_t label, Int_t color, Flag_e flags)
    {
        return Add_Option(Option_e::COLOR, label, "", static_cast<Float_t>(color), flags);
    }

    Int_t Config_Base_t::Add_Keymap_Option(String_t label, Int_t key_code, Flag_e flags)
    {
        return Add_Option(Option_e::KEYMAP, label, "", static_cast<Float_t>(key_code), flags);
    }

    Int_t Config_Base_t::Add_Input_Option(String_t label, String_t value, Flag_e flags)
    {
        return Add_Option(Option_e::INPUT, label, value, 0.0f, flags);
    }

    void Config_Base_t::Number_Option_Value(Int_t index, Float_t value, Bool_t do_render)
    {
        SKYLIB_ASSERT(Current_State() != State_e::RESET);

        UI_t::Value<Int_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursorIndex", index);
        UI_t::Value<Float_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursor.numValue", value);
        if (do_render) {
            UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".invalidateOptionData");
        }
    }

    void Config_Base_t::String_Option_Value(Int_t index, String_t value, Bool_t do_render)
    {
        SKYLIB_ASSERT(Current_State() != State_e::RESET);

        UI_t::Value<Int_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursorIndex", index);
        UI_t::Value<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursor.strValue", value);
        if (do_render) {
            UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".invalidateOptionData");
        }
    }

    void Config_Base_t::Both_Option_Values(Int_t index, String_t string_value, Float_t number_value, Bool_t do_render)
    {
        SKYLIB_ASSERT(Current_State() != State_e::RESET);

        UI_t::Value<Int_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursorIndex", index);
        UI_t::Value<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursor.strValue", string_value);
        UI_t::Value<Float_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".optionCursor.numValue", number_value);
        if (do_render) {
            UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".invalidateOptionData");
        }
    }

    void Config_Base_t::Slider_Dialog_Current_Value(Float_t value)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        Slider_Parameters().Array()->Point(0)->Float(value);
    }

    void Config_Base_t::Slider_Dialog_Default_Value(Float_t value)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        Slider_Parameters().Array()->Point(1)->Float(value);
    }

    void Config_Base_t::Slider_Dialog_Range_Values(Float_t min, Float_t max)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        V::Array_t* slider_parameters = Slider_Parameters().Array();
        slider_parameters->Point(2)->Float(min);
        slider_parameters->Point(3)->Float(max);
    }

    void Config_Base_t::Slider_Dialog_Interval_Value(Float_t value)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        Slider_Parameters().Array()->Point(4)->Float(value);
    }

    void Config_Base_t::Menu_Dialog_Values(Vector_t<String_t> values)
    {
        SKYLIB_ASSERT(Current_State() == State_e::MENU);

        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setMenuDialogOptions", values);
    }

    void Config_Base_t::Menu_Dialog_Default(Int_t index)
    {
        SKYLIB_ASSERT(Current_State() == State_e::MENU);

        Menu_Parameters().Array()->Point(1)->Int(index);
    }

    void Config_Base_t::Text_Option_Value(Int_t option, String_t text, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags().Array()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::TEXT);

        String_Option_Value(index, text, do_render);
    }

    void Config_Base_t::Toggle_Option_Value(Int_t option, Bool_t value, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags().Array()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::TOGGLE);

        Number_Option_Value(index, static_cast<Float_t>(value), do_render);
    }

    void Config_Base_t::Slider_Option_Value(Int_t option, Float_t value, String_t format, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags().Array()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::SLIDER);

        Both_Option_Values(index, format, value, do_render);
    }

    void Config_Base_t::Menu_Option_Value(Int_t option, String_t value, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags().Array()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::MENU);

        String_Option_Value(index, value, do_render);
    }

    void Config_Base_t::Input_Option_Value(Int_t option, String_t value, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags().Array()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::INPUT);

        String_Option_Value(index, value, do_render);
    }

    void Config_Base_t::Keymap_Option_Value(Int_t option, Int_t key_code, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags().Array()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::KEYMAP);

        Number_Option_Value(index, static_cast<Float_t>(key_code), do_render);
    }

    void Config_Base_t::Option_Flags(Int_t option, Flag_e flags, Bool_t do_render)
    {
        SKYLIB_ASSERT(Current_State() != State_e::RESET);

        Int_t flags_int = static_cast<Int_t>(flags);

        Int_t index = option % 0x100;

        Int_t old_flags = Flags().Array()->Point(index)->Int();
        old_flags %= 0x100;
        old_flags += flags_int * 0x100;

        Vector_t<Int_t> arguments;
        arguments.reserve(2);
        arguments.push_back(index);
        arguments.push_back(flags_int);

        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionFlags", arguments);
        if (do_render) {
            UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".invalidateOptionData");
        }
    }

    void Config_Base_t::Enable_Option(Int_t option, Bool_t do_render, Bool_t with_unmap)
    {
        Option_Flags(option, with_unmap ? Flag_e::UNMAP : Flag_e::NONE, do_render);
    }

    void Config_Base_t::Disable_Option(Int_t option, Bool_t do_render)
    {
        Option_Flags(option, Flag_e::DISABLE, do_render);
    }

    void Config_Base_t::Show_Option(Int_t option, Bool_t do_render, Bool_t with_unmap)
    {
        Enable_Option(option, do_render, with_unmap);
    }

    void Config_Base_t::Hide_Option(Int_t option, Bool_t do_render)
    {
        Option_Flags(option, Flag_e::HIDE, do_render);
    }

    void Config_Base_t::Flicker_Option(Int_t option, maybe<unique<Callback_i<>>> callback)
    {
        using Callback = maybe<unique<Callback_i<>>>;

        class Waiter :
            public V::Callback_t
        {
        public:
            Config_Base_t*  self;
            Int_t           option;
            Callback        callback;

        public:
            Waiter(Config_Base_t* self, Int_t option, Callback callback) :
                self(self), option(option), callback(std::move(callback))
            {
            }

        public:
            void operator ()(V::Variable_t*)
            {
                if (this->self->Current_State() != State_e::RESET) {
                    // this can cause a crash if the menu is closed before hand
                    this->self->Option_Flags(this->option, Flag_e::NONE, true);
                }

                if (this->callback) {
                    (*this->callback)();
                }
            }
        };

        Option_Flags(option, Flag_e::DISABLE, true);
        V::Utility_t::Wait_Even_In_Menu(0.2f, new Waiter(this, option, std::move(callback)));
    }

    void Config_Base_t::Show_Message(String_t message,
                                     Bool_t allow_cancel,
                                     String_t accept_label,
                                     String_t cancel_label,
                                     maybe<unique<Callback_i<Bool_t>>> callback)
    {
        using Callback = maybe<unique<Callback_i<Bool_t>>>;

        class Waiter :
            public V::Callback_t
        {
        public:
            Config_Base_t*  self;
            Callback        callback;

        public:
            Waiter(Config_Base_t* self, Callback callback) :
                self(self), callback(std::move(callback))
            {
            }

        public:
            void operator ()(V::Variable_t*)
            {
                if (this->self->Is_Waiting_For_Message()) {
                    V::Utility_t::Wait_Even_In_Menu(0.1f, new Waiter(this->self, std::move(this->callback)));
                } else {
                    this->self->Unregister_SKSE_Event("SKICP_messageDialogClosed", none<V::Callback_i*>());
                    if (this->callback) {
                        (*this->callback)(this->self->Message_Result());
                    }
                }
            }
        };

        if (!Is_Waiting_For_Message()) {
            Is_Waiting_For_Message() = true;
            Message_Result() = false;

            Register_SKSE_Event("SKICP_messageDialogClosed", "OnMessageDialogClose", none<V::Callback_i*>());

            Vector_t<String_t> args;
            args.reserve(3);
            args.push_back(message);
            args.push_back(accept_label);
            if (allow_cancel) {
                args.push_back(cancel_label);
            } else {
                args.push_back("");
            }
            UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".showMessageDialog", args);

            V::Utility_t::Wait_Even_In_Menu(0.1f, new Waiter(this, std::move(callback)));
        } else {
            if (callback) {
                (*callback)(false);
            }
        }
    }

    void Config_Base_t::Reset_Page()
    {
        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".forcePageReset");
    }

    void Config_Base_t::Unlock()
    {
        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".unlock", true);
    }

    void Config_Base_t::Register_Me(some<V::Machine_t*> machine)
    {
        SKYLIB_ASSERT_SOME(machine);

        String_t class_name = Class_Name();

        #define METHOD(NAME_, WAITS_FOR_FRAME_, RETURN_, METHOD_, ...)              \
        SKYLIB_M                                                                    \
            BIND_METHOD(machine, class_name, Config_Base_t,                         \
                        NAME_, WAITS_FOR_FRAME_, RETURN_, METHOD_, __VA_ARGS__);    \
        SKYLIB_W

        METHOD("ClearOptionBuffers", false, void, Clear_Buffers);

        #undef METHOD
    }

}
