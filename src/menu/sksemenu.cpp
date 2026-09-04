#include "sksemenu.h"

#include "API/SKSEMenuFramework.h"
#include "CLIBUtil/hotkeys.hpp"
#include "RE/I/InputDevices.h"
#include "REX/STRING.h"
#include "SKSE/InputMap.h"
#include "Settings.h"
#include "menu-defines.h"
#include "mod-data.h"
#include "st-ui.h"

namespace Dodge
{

namespace MENU
{
void SKSEMENU::RegisterSKSEMenu()
{
    if (!SKSEMenuFramework::IsInstalled())
    {
        return;
    }
    SKSEMenuFramework::SetSection(TIT::MENU_TITLE);
    SKSEMenuFramework::AddSectionItem(TIT::MENU_TITLE, RenderMenu);
    RegisterOpenClose();
    SKSEMenuFramework::AddInputEvent(OnInput);
}
void SKSEMENU::OnOpen()
{
    FUN::LoadFromConfig();
}
void SKSEMENU::OnClose()
{
    CONF::UpdateSettings(true);
}

static void __stdcall MenuEventListener(SKSEMenuFramework::Model::EventType eventType)
{
    switch (eventType)
    {
        case SKSEMenuFramework::Model::kOpenMenu:
            SKSEMENU::OnOpen();
            break;
        case SKSEMenuFramework::Model::kCloseMenu:
            SKSEMENU::OnClose();
            break;
        default:
            break;
    }
}

void SKSEMENU::RegisterOpenClose()
{
    static SKSEMenuFramework::Model::Event event(MenuEventListener, 1.256f);
}
void SKSEMENU::DrawSettings()
{

    using ux = SKSEMenuUtil;
    ux::GreenTitleText(TIT::MENU_TITLE);

    ux::SetCheckbox(LAB::enable_sneak_key_dodge, VAR::enable_sneak_key_dodge, CONF::enable_sneak_key_dodge,
                    HEL::enable_sneak_key_dodge);
    SameLine();
    ux::SetCheckbox(LAB::enable_dodge_in_place, VAR::enable_dodge_in_place, CONF::enable_dodge_in_place,
                    HEL::enable_dodge_in_place);

    ux::SetCheckbox(LAB::step_dodge, VAR::step_dodge, CONF::step_dodge, HEL::step_dodge);
    SameLine();
    ux::SetCheckbox(LAB::enable_sneak_dodge, VAR::enable_sneak_dodge, CONF::enable_sneak_dodge,
                    HEL::enable_sneak_dodge);

    ux::SetCheckbox(LAB::enable_dodge_attack_cancel, VAR::enable_dodge_attack_cancel, CONF::enable_dodge_attack_cancel,
                    HEL::enable_dodge_attack_cancel);
    SameLine();
    ux::SetCheckbox(LAB::use_sprint_key, VAR::use_sprint_key, CONF::use_sprint_key, HEL::use_sprint_key);

    ux::SetCheckbox(LAB::use_mco_recover_window, VAR::use_mco_recover_window, CONF::use_mco_recover_window,
                    HEL::use_mco_recover_window);
    SameLine();
    ux::SetCheckbox(LAB::use_perk_lock, VAR::use_perk_lock, CONF::use_perk_lock, HEL::use_perk_lock);

    ux::SetCheckbox(LAB::use_percentage_cost, VAR::use_percentage_cost, CONF::use_percentage_cost,
                    HEL::use_percentage_cost);
    SameLine();
    ux::SetCheckbox(LAB::use_double_tap, VAR::use_double_tap, CONF::use_double_tap, HEL::use_double_tap);

    ux::SetCheckbox(LAB::only_cancel_light, VAR::only_cancel_light, CONF::only_cancel_light, HEL::only_cancel_light);
    SameLine();
    ux::SetCheckbox(LAB::remove_forward, VAR::remove_forward, CONF::remove_forward, HEL::remove_forward);

    ux::SetCheckbox(LAB::disable_in_third, VAR::disable_in_third, CONF::disable_in_third, HEL::disable_in_third);


    ux::SetSliderFloat(LAB::i_frame_duration, VAR::i_frame_duration, CONF::i_frame_duration, 0.0f, 4.0f,
                       HEL::i_frame_duration, "%.2f sec");
    ux::SetSliderFloat(LAB::sprinting_press_duration, VAR::sprinting_press_duration, CONF::sprinting_press_duration,
                       0.01f, 4.0f, HEL::sprinting_press_duration, "%.2f sec");

    ux::SetSliderFloat(LAB::sneaking_press_duration, VAR::sneaking_press_duration, CONF::sneaking_press_duration, 0.01f,
                       4.0f, HEL::sneaking_press_duration, "%.2f sec");

    ux::SetSliderFloat(LAB::dodge_cost, VAR::dodge_cost, CONF::dodge_cost, 0.0f, 100.0f, HEL::dodge_cost);
    DrawDefaultDodge();
}
void SKSEMENU::DrawSystem()
{
    if (Button("Restore Default Settings"))
    {
        FUN::RestoreDefaults();
    }
}
void SKSEMENU::DrawHotkeySelection()
{

    if (VAR::dodge_key <= 1)
    {
        Text("UNBOUND");
    }
    else
    {
        auto key_name = hotkeys::details::GetNameByKey(CONF::dodge_key.GetValue());
        auto show_key = REX::STR::TO_UPPER(key_name);
        Text(std::format("Hotkey: {}", show_key).c_str());
    }
    SameLine();

    if (!capture_key_input)
    {
        if (Button("Rebind"))
        {
            capture_key_input = true;
            VAR::dodge_key    = 0;
        }
        SameLine();
        SKSEMenuUtil::HelpMarker("Press the desired key to rebind the dodge key");
    }
    else
    {
        Text("Press any key...");
        SameLine();
        if (Button("Cancel"))
        {
            capture_key_input = false;
            VAR::dodge_key    = CONF::dodge_key.GetValue();
        }
        SameLine();
        if (Button("Unbind"))
        {
            CONF::dodge_key.SetValue(VAR::dodge_key);
            capture_key_input = false;
        }
    }

    if (VAR::dodge_key != 0)
    {
        CONF::dodge_key.SetValue(VAR::dodge_key);
        capture_key_input = false;
    }
}
void SKSEMENU::DrawDefaultDodge()
{

    if (BeginCombo(LAB::default_dodge_event, CONF::default_dodge_event.GetValue().c_str()))
    {

        Bullet();
        SameLine();
        if (Button("Forward Dodge"))
        {
            CONF::default_dodge_event.SetValue("TKDodgeForward");
        }
        Bullet();
        SameLine();
        if (Button("Dodge Backwards"))
        {
            CONF::default_dodge_event.SetValue("TKDodgeBack");
        }
        Bullet();
        SameLine();
        if (Button("Dodge Left"))
        {
            CONF::default_dodge_event.SetValue("TKDodgeLeft");
        }
        Bullet();
        SameLine();
        if (Button("Dodge Right"))
        {
            CONF::default_dodge_event.SetValue("TKDodgeRight");
        }
        EndCombo();
    }
}

bool __stdcall SKSEMENU::OnInput(RE::InputEvent* a_event)
{

    bool block_input = false;
    if (capture_key_input)
    {
        for (auto e = a_event; e; e = e->next)
        {
            auto button = e->AsButtonEvent();
            if (!button || !button->HasIDCode())
            {
                continue;
            }
            if (!button->IsDown())
            {
                continue;
            }
            uint32_t key = button->GetIDCode();
            switch (button->GetDevice())
            {
                case RE::INPUT_DEVICE::kMouse:
                    key += SKSE::InputMap::kMacro_MouseButtonOffset;
                    break;
                case RE::INPUT_DEVICE::kGamepad:
                    key = SKSE::InputMap::GamepadMaskToKeycode(key);
                    break;
                default:
                    break;
            }
            if (key == 256)
            {
                VAR::dodge_key = 1;
                CONF::dodge_key.SetValue(1);
                return block_input;
            }
            VAR::dodge_key = key;
        }
    }
    return block_input;
}


void __stdcall SKSEMENU::RenderMenu()
{
    if (CollapsingHeader(TIT::MENU_TITLE, ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawSettings();
    }

    if (CollapsingHeader(TIT::KEY_SEC, ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawHotkeySelection();
    }

    if (CollapsingHeader(TIT::SYS_SEC, ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawSystem();
    }
}


} // namespace MENU
} // namespace Dodge