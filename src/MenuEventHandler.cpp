#include "MenuEventHandler.h"

RE::BSEventNotifyControl MenuEventHandler::MenuEvent::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
{
    auto util         = Utility::GetSingleton();
    auto journal_menu = RE::JournalMenu::MENU_NAME;

    if (!event) {
        return RE::BSEventNotifyControl::kContinue;
    }
    if (event->opening) {
        return RE::BSEventNotifyControl::kContinue;
    }
    if (event->menuName != journal_menu)
        return RE::BSEventNotifyControl::kContinue;

    if (event->menuName == journal_menu) {
        if (!event->opening) {
        }
    }
    return RE::BSEventNotifyControl::kContinue;
};
