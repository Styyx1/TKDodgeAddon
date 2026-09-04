#include "menu-defines.h"

#include "Settings.h"

namespace Dodge::MENU
{


void FUN::RestoreDefaults()
{
    VAR::enable_sneak_key_dodge     = false;
    VAR::enable_dodge_in_place      = true;
    VAR::step_dodge                 = false;
    VAR::enable_sneak_dodge         = false;
    VAR::enable_dodge_attack_cancel = true;
    VAR::use_sprint_key             = false;
    VAR::use_mco_recover_window     = false;
    VAR::use_perk_lock              = false;
    VAR::use_percentage_cost        = false;
    VAR::use_double_tap             = false;
    VAR::only_cancel_light          = false;
    VAR::remove_forward             = false;
    VAR::disable_in_third           = false;

    VAR::i_frame_duration         = 0.3f;
    VAR::sprinting_press_duration = 0.5f;
    VAR::sneaking_press_duration  = 0.5f;
    VAR::dodge_cost               = 15.0f;


    VAR::default_dodge_event = "TKDodgeBack";
    VAR::dodge_key           = 274;
    VAR::perk_mod_name       = "";
    VAR::spell_mod_name      = "";

    CONF::enable_sneak_key_dodge.SetValue(VAR::enable_sneak_key_dodge);
    CONF::enable_dodge_in_place.SetValue(VAR::enable_dodge_in_place);
    CONF::step_dodge.SetValue(VAR::step_dodge);
    CONF::enable_sneak_dodge.SetValue(VAR::enable_sneak_dodge);
    CONF::enable_dodge_attack_cancel.SetValue(VAR::enable_dodge_attack_cancel);
    CONF::only_cancel_light.SetValue(VAR::only_cancel_light);
    CONF::i_frame_duration.SetValue(VAR::i_frame_duration);
    CONF::default_dodge_event.SetValue(VAR::default_dodge_event);
    CONF::sprinting_press_duration.SetValue(VAR::sprinting_press_duration);
    CONF::sneaking_press_duration.SetValue(VAR::sneaking_press_duration);

    CONF::dodge_cost.SetValue(VAR::dodge_cost);
    CONF::dodge_key.SetValue(VAR::dodge_key);
    CONF::use_sprint_key.SetValue(VAR::use_sprint_key);
    CONF::use_mco_recover_window.SetValue(VAR::use_mco_recover_window);

    CONF::use_perk_lock.SetValue(VAR::use_perk_lock);
    CONF::use_percentage_cost.SetValue(VAR::use_percentage_cost);

    CONF::use_double_tap.SetValue(VAR::use_double_tap);
    CONF::remove_forward.SetValue(VAR::remove_forward);
    CONF::disable_in_third.SetValue(VAR::disable_in_third);

    // Save Settings
    CONF::UpdateSettings(true);
}
void FUN::LoadFromConfig()
{
    VAR::enable_sneak_key_dodge     = CONF::enable_sneak_key_dodge.GetValue();
    VAR::enable_dodge_in_place      = CONF::enable_dodge_in_place.GetValue();
    VAR::step_dodge                 = CONF::step_dodge.GetValue();
    VAR::enable_sneak_dodge         = CONF::enable_sneak_dodge.GetValue();
    VAR::enable_dodge_attack_cancel = CONF::enable_dodge_attack_cancel.GetValue();
    VAR::only_cancel_light          = CONF::only_cancel_light.GetValue();
    VAR::i_frame_duration           = CONF::i_frame_duration.GetValue();
    VAR::default_dodge_event        = CONF::default_dodge_event.GetValue();
    VAR::sprinting_press_duration   = CONF::sprinting_press_duration.GetValue();
    VAR::sneaking_press_duration    = CONF::sneaking_press_duration.GetValue();

    VAR::dodge_cost               = CONF::dodge_cost.GetValue();
    VAR::dodge_key                = CONF::dodge_key.GetValue();
    VAR::use_sprint_key           = CONF::use_sprint_key.GetValue();
    VAR::use_mco_recover_window   = CONF::use_mco_recover_window.GetValue();
    VAR::only_light_attack_cancel = CONF::only_cancel_light.GetValue();

    VAR::use_perk_lock       = CONF::use_perk_lock.GetValue();
    VAR::use_percentage_cost = CONF::use_percentage_cost.GetValue();


    VAR::use_double_tap   = CONF::use_double_tap.GetValue();
    VAR::remove_forward   = CONF::remove_forward.GetValue();
    VAR::disable_in_third = CONF::disable_in_third.GetValue();
}
} // namespace Dodge::MENU