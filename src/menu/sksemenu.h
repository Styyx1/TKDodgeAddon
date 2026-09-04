#pragma once

#include "RE/I/InputEvent.h"
namespace Dodge
{

namespace MENU
{

namespace SKSEMENU
{

inline bool capture_key_input = false;
inline bool abort_key_input   = false;
void RegisterSKSEMenu();
void OnOpen();
void OnClose();
void RegisterOpenClose();
void DrawSettings();
void DrawSystem();
void DrawHotkeySelection();
void DrawDefaultDodge();
bool __stdcall OnInput(RE::InputEvent* a_event);

void __stdcall RenderMenu();
} // namespace SKSEMENU

} // namespace MENU

} // namespace Dodge