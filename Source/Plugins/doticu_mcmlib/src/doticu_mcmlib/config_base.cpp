/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/utils.h"
#include "doticu_skylib/form.h"
#include "doticu_skylib/virtual_array.h"
#include "doticu_skylib/virtual_arguments.h"
#include "doticu_skylib/virtual_callback.h"
#include "doticu_skylib/virtual_object.h"
#include "doticu_skylib/virtual_macros.h"
#include "doticu_skylib/virtual_utils.h"

#include "doticu_mcmlib/config_base.h"

namespace doticu_mcmlib {

    namespace Virtual = skylib::Virtual;

    using UI_t = skylib::UI_t;

    String_t Config_Base_t::Class_Name()    { DEFINE_CLASS_NAME("SKI_ConfigBase"); }
    Class_t* Config_Base_t::Class()         { DEFINE_CLASS(); }
    Object_t* Config_Base_t::Object()       { DEFINE_OBJECT(); }

    Variable_t* Config_Base_t::Current_Page_Name_Variable()         { DEFINE_VARIABLE("_currentPage"); }
    Variable_t* Config_Base_t::Current_Page_Number_Variable()       { DEFINE_VARIABLE("_currentPageNum"); }
    Variable_t* Config_Base_t::Current_State_Variable()             { DEFINE_VARIABLE("_state"); }
    Variable_t* Config_Base_t::Cursor_Position_Variable()           { DEFINE_VARIABLE("_cursorPosition"); }
    Variable_t* Config_Base_t::Cursor_Fill_Mode_Variable()          { DEFINE_VARIABLE("_cursorFillMode"); }
    Variable_t* Config_Base_t::Flags_Variable()                     { DEFINE_VARIABLE("_optionFlagsBuf"); }
    Variable_t* Config_Base_t::Labels_Variable()                    { DEFINE_VARIABLE("_textBuf"); }
    Variable_t* Config_Base_t::String_Values_Variable()             { DEFINE_VARIABLE("_strValueBuf"); }
    Variable_t* Config_Base_t::Number_Values_Variable()             { DEFINE_VARIABLE("_numValueBuf"); }
    Variable_t* Config_Base_t::States_Variable()                    { DEFINE_VARIABLE("_stateOptionMap"); }
    Variable_t* Config_Base_t::Info_Text_Variable()                 { DEFINE_VARIABLE("_infoText"); }
    Variable_t* Config_Base_t::Slider_Parameters_Variable()         { DEFINE_VARIABLE("_sliderParams"); }
    Variable_t* Config_Base_t::Menu_Parameters_Variable()           { DEFINE_VARIABLE("_menuParams"); }
    Variable_t* Config_Base_t::Is_Waiting_For_Message_Variable()    { DEFINE_VARIABLE("_waitForMessage"); }
    Variable_t* Config_Base_t::Message_Result_Variable()            { DEFINE_VARIABLE("_messageResult"); }

    Variable_t* Config_Base_t::Mod_Name_Property()                  { DEFINE_PROPERTY("ModName"); }
    Variable_t* Config_Base_t::Pages_Property()                     { DEFINE_PROPERTY("Pages"); }

    String_t Config_Base_t::Current_Page_Name()
    {
        return Current_Page_Name_Variable()->String();
    }

    Int_t Config_Base_t::Current_Page_Number()
    {
        return Current_Page_Number_Variable()->Int();
    }

    void Config_Base_t::Current_Page(String_t name)
    {
        Array_t* pages = Pages_Property()->Array();
        if (pages) {
            for (size_t idx = 0, count = pages->count; idx < count; idx += 1) {
                String_t page = pages->Point(idx)->String();
                if (skylib::CString_t::Is_Same(page, name, true)) {
                    Current_Page_Name_Variable()->String(name);
                    Current_Page_Number_Variable()->Int(idx + 1);
                    return;
                }
            }
            Current_Page_Name_Variable()->String("");
            Current_Page_Number_Variable()->Int(-1);
        } else {
            Current_Page_Name_Variable()->String("");
            Current_Page_Number_Variable()->Int(-1);
        }
    }

    State_e Config_Base_t::Current_State()
    {
        return static_cast<State_e>(Current_State_Variable()->Int());
    }

    Int_t Config_Base_t::Cursor_Position()
    {
        return Cursor_Position_Variable()->Int();
    }

    Cursor_e Config_Base_t::Cursor_Fill_Mode()
    {
        return static_cast<Cursor_e>(Cursor_Fill_Mode_Variable()->Int());
    }

    Array_t* Config_Base_t::Flags()
    {
        return Flags_Variable()->Array();
    }

    Array_t* Config_Base_t::Labels()
    {
        return Labels_Variable()->Array();
    }

    Array_t* Config_Base_t::String_Values()
    {
        return String_Values_Variable()->Array();
    }

    Array_t* Config_Base_t::Number_Values()
    {
        return Number_Values_Variable()->Array();
    }

    Array_t* Config_Base_t::States()
    {
        return States_Variable()->Array();
    }

    void Config_Base_t::Cursor_Position(Int_t cursor_position)
    {
        SKYLIB_ASSERT(cursor_position < 128 && cursor_position >= -1);

        Cursor_Position_Variable()->Int(cursor_position);
    }

    void Config_Base_t::Cursor_Fill_Mode(Cursor_e cursor_fill_mode)
    {
        SKYLIB_ASSERT(cursor_fill_mode == Cursor_e::LEFT_TO_RIGHT ||
                      cursor_fill_mode == Cursor_e::TOP_TO_BOTTOM);

        Cursor_Fill_Mode_Variable()->Int(static_cast<Int_t>(cursor_fill_mode));
    }

    Int_t Config_Base_t::Pack_Flags(Int_t flags, Option_e option_type)
    {
        return (flags * 0x100) + static_cast<Int_t>(option_type);
    }

    Int_t Config_Base_t::Pack_Option_ID(Int_t page_number, Int_t cursor_position)
    {
        return (page_number * 0x100) + cursor_position;
    }

    void Config_Base_t::Clear_Buffers()
    {
        Array_t* flags = Flags();
        Array_t* labels = Labels();
        Array_t* strings = String_Values();
        Array_t* numbers = Number_Values();
        Array_t* states = States();

        for (size_t idx = 0, count = 128; idx < count; idx += 1) {
            flags->Point(idx)->Int(0);
            labels->Point(idx)->String("");
            strings->Point(idx)->String("");
            numbers->Point(idx)->Float(0.0f);
            states->Point(idx)->String("");
        }

        Cursor_Position(0);
        Cursor_Fill_Mode(Cursor_e::LEFT_TO_RIGHT);
    }

    void Config_Base_t::Write_Buffers()
    {
        Array_t* flags = Flags();
        Int_t option_count = 0;
        for (size_t idx = 0, count = 128; idx < count; idx += 1) {
            Int_t flag = flags->Point(idx)->Int();
            if (flag != static_cast<Int_t>(Option_e::EMPTY)) {
                option_count += 1;
            }
        }

        UI_t::Run<Int_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionFlagsBuffer", flags);
        UI_t::Run<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionTextBuffer", Labels());
        UI_t::Run<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionStrValueBuffer", String_Values());
        UI_t::Run<Float_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setOptionNumValueBuffer", Number_Values());
        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".flushOptionBuffers", option_count);
    }

    void Config_Base_t::Title_Text(String_t title)
    {
        UI_t::Run<String_t>(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setTitleText", title);
    }

    String_t Config_Base_t::Info_Text()
    {
        return Info_Text_Variable()->String();
    }

    void Config_Base_t::Info_Text(String_t info)
    {
        Info_Text_Variable()->String(info);
    }

    Int_t Config_Base_t::Add_Option(Option_e option_type, String_t label, String_t string, Float_t number, Int_t flags)
    {
        SKYLIB_ASSERT(label);
        SKYLIB_ASSERT(string);
        SKYLIB_ASSERT(Current_State() == State_e::RESET);

        Int_t position = Cursor_Position();
        if (position != -1) {
            Flags()->Point(position)->Int(Pack_Flags(flags, option_type));
            Labels()->Point(position)->String(label);
            String_Values()->Point(position)->String(string);
            Number_Values()->Point(position)->Float(number);

            Int_t new_position = position + static_cast<Int_t>(Cursor_Fill_Mode());
            if (new_position >= 128) {
                Cursor_Position(-1);
            } else {
                Cursor_Position(new_position);
            }

            return Pack_Option_ID(Current_Page_Number(), position);
        } else {
            return -1;
        }
    }

    Int_t Config_Base_t::Add_Empty_Option()
    {
        return Add_Option(Option_e::EMPTY, "", "", 0.0f, 0);
    }

    Int_t Config_Base_t::Add_Header_Option(String_t label, Int_t flags)
    {
        return Add_Option(Option_e::HEADER, label, "", 0.0f, flags);
    }

    Int_t Config_Base_t::Add_Text_Option(String_t label, String_t value, Int_t flags)
    {
        return Add_Option(Option_e::TEXT, label, value, 0.0f, flags);
    }

    Int_t Config_Base_t::Add_Toggle_Option(String_t label, Bool_t is_toggled, Int_t flags)
    {
        return Add_Option(Option_e::TOGGLE, label, "", static_cast<Float_t>(is_toggled), flags);
    }

    Int_t Config_Base_t::Add_Slider_Option(String_t label, Float_t value, String_t format, Int_t flags)
    {
        return Add_Option(Option_e::SLIDER, label, format, value, flags);
    }

    Int_t Config_Base_t::Add_Menu_Option(String_t label, String_t value, Int_t flags)
    {
        return Add_Option(Option_e::MENU, label, value, 0.0f, flags);
    }

    Int_t Config_Base_t::Add_Color_Option(String_t label, Int_t color, Int_t flags)
    {
        return Add_Option(Option_e::COLOR, label, "", static_cast<Float_t>(color), flags);
    }

    Int_t Config_Base_t::Add_Keymap_Option(String_t label, Int_t key_code, Int_t flags)
    {
        return Add_Option(Option_e::KEYMAP, label, "", static_cast<Float_t>(key_code), flags);
    }

    Int_t Config_Base_t::Add_Input_Option(String_t label, String_t value, Int_t flags)
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

        Slider_Parameters_Variable()->Array()->Point(0)->Float(value);
    }

    void Config_Base_t::Slider_Dialog_Default_Value(Float_t value)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        Slider_Parameters_Variable()->Array()->Point(1)->Float(value);
    }

    void Config_Base_t::Slider_Dialog_Range_Values(Float_t min, Float_t max)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        Array_t* slider_parameters = Slider_Parameters_Variable()->Array();
        slider_parameters->Point(2)->Float(min);
        slider_parameters->Point(3)->Float(max);
    }

    void Config_Base_t::Slider_Dialog_Interval_Value(Float_t value)
    {
        SKYLIB_ASSERT(Current_State() == State_e::SLIDER);

        Slider_Parameters_Variable()->Array()->Point(4)->Float(value);
    }

    void Config_Base_t::Menu_Dialog_Values(Vector_t<String_t> values)
    {
        SKYLIB_ASSERT(Current_State() == State_e::MENU);

        UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".setMenuDialogOptions", values);
    }

    void Config_Base_t::Menu_Dialog_Default(Int_t index)
    {
        SKYLIB_ASSERT(Current_State() == State_e::MENU);

        Menu_Parameters_Variable()->Array()->Point(1)->Int(index);
    }

    void Config_Base_t::Text_Option_Value(Int_t option, String_t text, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::TEXT);

        String_Option_Value(index, text, do_render);
    }

    void Config_Base_t::Toggle_Option_Value(Int_t option, Bool_t value, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::TOGGLE);

        Number_Option_Value(index, static_cast<Float_t>(value), do_render);
    }

    void Config_Base_t::Slider_Option_Value(Int_t option, Float_t value, String_t format, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::SLIDER);

        Both_Option_Values(index, format, value, do_render);
    }

    void Config_Base_t::Menu_Option_Value(Int_t option, String_t value, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::MENU);

        String_Option_Value(index, value, do_render);
    }

    void Config_Base_t::Input_Option_Value(Int_t option, String_t value, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::INPUT);

        String_Option_Value(index, value, do_render);
    }

    void Config_Base_t::Keymap_Option_Value(Int_t option, Int_t key_code, Bool_t do_render)
    {
        Int_t index = option % 0x100;
        Option_e type = static_cast<Option_e>
            (Flags()->Point(index)->Int() % 0x100);
        SKYLIB_ASSERT(type == Option_e::KEYMAP);

        Number_Option_Value(index, static_cast<Float_t>(key_code), do_render);
    }

    void Config_Base_t::Option_Flags(Int_t option, Flag_e flags, Bool_t do_render)
    {
        SKYLIB_ASSERT(Current_State() != State_e::RESET);

        Int_t flags_int = static_cast<Int_t>(flags);

        Int_t index = option % 0x100;

        Int_t old_flags = Flags()->Point(index)->Int();
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

    void Config_Base_t::Flicker_Option(Int_t option, Callback_i<>* ucallback)
    {
        using UCallback_t = Callback_i<>;

        Option_Flags(option, Flag_e::DISABLE, true);

        struct VCallback : public Virtual::Callback_t
        {
            Config_Base_t* self;
            Int_t option;
            UCallback_t* ucallback;
            VCallback(Config_Base_t* self, Int_t option, UCallback_t* ucallback) :
                self(self), option(option), ucallback(ucallback)
            {
            }
            void operator()(Variable_t* result)
            {
                if (self->Current_State() != State_e::RESET) {
                    self->Option_Flags(option, Flag_e::NONE, true);
                }

                if (ucallback) {
                    ucallback->operator()();
                    delete ucallback;
                }
            }
        };
        Virtual::Utils_t::Wait(0.2f, new VCallback(this, option, ucallback));
    }

    void Config_Base_t::Show_Message(String_t message,
                                     Bool_t allow_cancel,
                                     String_t accept_label,
                                     String_t cancel_label,
                                     Callback_i<Bool_t>* user_callback)
    {
        using UCallback_t = Callback_i<Bool_t>;

        if (!Is_Waiting_For_Message_Variable()->Bool()) {
            Is_Waiting_For_Message_Variable()->Bool(true);
            Message_Result_Variable()->Bool(false);

            Vector_t<String_t> args;
            args.reserve(3);
            args.push_back(message);
            args.push_back(accept_label);
            if (allow_cancel) {
                args.push_back(cancel_label);
            } else {
                args.push_back("");
            }

            Register_Mod_Event("SKICP_messageDialogClosed", "OnMessageDialogClose");
            UI_t::Run(JOURNAL_MENU, "_root.ConfigPanelFader.configPanel" ".showMessageDialog", args);

            struct Waiter : Virtual::Callback_t {
                Config_Base_t* self;
                UCallback_t* user_callback;
                Waiter(Config_Base_t* self, UCallback_t* user_callback) :
                    self(self), user_callback(user_callback)
                {
                }
                void operator()(Variable_t* result)
                {
                    if (self->Is_Waiting_For_Message_Variable()->Bool()) {
                        Virtual::Utils_t::Wait(0.1f, new Waiter(self, user_callback));
                    } else {
                        self->Unregister_Mod_Event("SKICP_messageDialogClosed");
                        if (user_callback) {
                            user_callback->operator()(self->Message_Result_Variable()->Bool());
                            delete user_callback;
                        }
                    }
                }
            };
            Virtual::Utils_t::Wait(0.1f, new Waiter(this, user_callback));
        } else {
            if (user_callback) {
                user_callback->operator()(false);
                delete user_callback;
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

    void Config_Base_t::Open_Page(String_t page_name)
    {
        if (Current_State() == State_e::RESET) {
            struct VCallback : public Virtual::Callback_t {
                Config_Base_t* self;
                String_t page_name;
                VCallback(Config_Base_t* self, String_t page_name) :
                    self(self), page_name(page_name)
                {
                }
                void operator()(Variable_t* result)
                {
                    self->Open_Page(page_name);
                }
            };
            Virtual::Utils_t::Wait(0.1f, new VCallback(this, page_name));
        } else {
            Current_Page(page_name);
            if (page_name && page_name.data && page_name.data[0]) {
                Title_Text(page_name);
            } else {
                Title_Text(Mod_Name_Property()->String());
            }

            Current_State_Variable()->Int(static_cast<Int_t>(State_e::RESET));
            Clear_Buffers();

            struct VArguments : public Virtual::Arguments_t {
                String_t page_name;
                VArguments(String_t page_name) :
                    page_name(page_name)
                {
                }
                Bool_t operator()(Array_t* arguments)
                {
                    arguments->Resize(1);
                    arguments->At(0)->String(page_name);
                    return true;
                }
            } varguments(page_name);
            struct VCallback : public Virtual::Callback_t {
                Config_Base_t* self;
                VCallback(Config_Base_t* self) :
                    self(self)
                {
                }
                void operator()(Variable_t* result)
                {
                    self->Write_Buffers();
                    self->Current_State_Variable()->Int(static_cast<Int_t>(State_e::DEFAULT));
                }
            };
            Virtual::Callback_i* vcallback = new VCallback(this);
            Virtual::Machine_t::Self()->Call_Method(this, Class_Name(), "OnPageReset", &varguments, &vcallback);
        }
    }

    void Config_Base_t::Register_Me(Machine_t* machine)
    {
        #define METHOD(STR_FUNC_, ARG_NUM_, RETURN_, METHOD_, ...)  \
        M                                                           \
            BIND_METHOD(machine, Class_Name(), Config_Base_t,       \
                        STR_FUNC_, ARG_NUM_,                        \
                        RETURN_, METHOD_, __VA_ARGS__);             \
        W

        METHOD("ClearOptionBuffers", 0, void, Clear_Buffers);

        #undef METHOD
    }

}
