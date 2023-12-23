#include "SDLToX11Keymap.hpp"
#include "X11/keysym.h"

SDLToX11Keymap::SDLToX11Keymap(){
    m_keyMap[SDLK_UNKNOWN] = NoSymbol;
    m_keyMap[SDLK_BACKSPACE] = XK_BackSpace;
    m_keyMap[SDLK_TAB] = XK_Tab;
    m_keyMap[SDLK_RETURN] = XK_Return;
    m_keyMap[SDLK_ESCAPE] = XK_Escape;
    m_keyMap[SDLK_DELETE] = XK_Delete;
    m_keyMap[SDLK_CAPSLOCK] = XK_Caps_Lock;
    m_keyMap[SDLK_F1] = XK_F1;
    m_keyMap[SDLK_F2] = XK_F2;
    m_keyMap[SDLK_F3] = XK_F3;
    m_keyMap[SDLK_F4] = XK_F4;
    m_keyMap[SDLK_F5] = XK_F5;
    m_keyMap[SDLK_F6] = XK_F6;
    m_keyMap[SDLK_F7] = XK_F7;
    m_keyMap[SDLK_F8] = XK_F8;
    m_keyMap[SDLK_F9] = XK_F9;
    m_keyMap[SDLK_F10] = XK_F10;
    m_keyMap[SDLK_F11] = XK_F11;
    m_keyMap[SDLK_F12] = XK_F12;
    m_keyMap[SDLK_PRINTSCREEN] = 0XFD1D;
    m_keyMap[SDLK_SCROLLLOCK] = XK_Scroll_Lock;
    m_keyMap[SDLK_PAUSE] = XK_Pause;
    m_keyMap[SDLK_INSERT] = XK_Insert;
    m_keyMap[SDLK_HOME] = XK_Home;
    m_keyMap[SDLK_PAGEUP] = XK_Page_Up;
    m_keyMap[SDLK_END] = XK_End;
    m_keyMap[SDLK_PAGEDOWN] = XK_Page_Down;
    m_keyMap[SDLK_RIGHT] = XK_Right;
    m_keyMap[SDLK_LEFT] = XK_Left;
    m_keyMap[SDLK_DOWN] = XK_Down;
    m_keyMap[SDLK_UP] = XK_Up;
    m_keyMap[SDLK_NUMLOCKCLEAR] = XK_Num_Lock;
    m_keyMap[SDLK_KP_DIVIDE] = XK_KP_Divide;
    m_keyMap[SDLK_KP_MULTIPLY] = XK_KP_Multiply;
    m_keyMap[SDLK_KP_MINUS] = XK_minus;
    m_keyMap[SDLK_KP_PLUS] = XK_plus;
    m_keyMap[SDLK_KP_ENTER] = XK_KP_Enter;
    m_keyMap[SDLK_KP_1] = XK_KP_1;
    m_keyMap[SDLK_KP_2] = XK_KP_2;
    m_keyMap[SDLK_KP_3] = XK_KP_3;
    m_keyMap[SDLK_KP_4] = XK_KP_4;
    m_keyMap[SDLK_KP_5] = XK_KP_5;
    m_keyMap[SDLK_KP_6] = XK_KP_6;
    m_keyMap[SDLK_KP_7] = XK_KP_7;
    m_keyMap[SDLK_KP_8] = XK_KP_8;
    m_keyMap[SDLK_KP_9] = XK_KP_9;
    m_keyMap[SDLK_KP_0] = XK_KP_0;
    m_keyMap[SDLK_KP_PERIOD] = XK_period;
    // SDLK Application
    // SDLK Power
    m_keyMap[SDLK_KP_EQUALS] = XK_KP_Equal;
    m_keyMap[SDLK_F13] = XK_F13;
    m_keyMap[SDLK_F14] = XK_F14;
    m_keyMap[SDLK_F15] = XK_F15;
    m_keyMap[SDLK_F16] = XK_F16;
    m_keyMap[SDLK_F17] = XK_F17;
    m_keyMap[SDLK_F18] = XK_F18;
    m_keyMap[SDLK_F19] = XK_F19;
    m_keyMap[SDLK_F20] = XK_F20;
    m_keyMap[SDLK_F21] = XK_F21;
    m_keyMap[SDLK_F22] = XK_F22;
    m_keyMap[SDLK_F23] = XK_F23;
    m_keyMap[SDLK_F24] = XK_F24;
    m_keyMap[SDLK_EXECUTE] = XK_Execute;
    m_keyMap[SDLK_HELP] = XK_Help;
    m_keyMap[SDLK_MENU] = XK_Menu;
    m_keyMap[SDLK_SELECT] = XK_Select;
    m_keyMap[SDLK_STOP] = XK_Cancel;
    m_keyMap[SDLK_AGAIN] = XK_Redo;
    m_keyMap[SDLK_UNDO] = XK_Undo;
    // SDLK_Cut
    // SDLK_Copy
    // SDLK_Paste
    m_keyMap[SDLK_FIND] = XK_Find;
    // SDLK_Mute
    // SDLK_VOLUMEUP
    // SDLK_VOLUMDOWN
    m_keyMap[SDLK_KP_COMMA] = XK_KP_Separator;
    // SDLK_KP_EQUALSAS40
    // SDLK_ALTERASE
    m_keyMap[SDLK_SYSREQ] = XK_Sys_Req;
    m_keyMap[SDLK_CANCEL] = XK_Cancel;
    m_keyMap[SDLK_CLEAR] = XK_Clear;
    m_keyMap[SDLK_PRIOR] = XK_Prior;
    m_keyMap[SDLK_RETURN2] = XK_Return; // Not sure about this conversion
    // SDLK_SEPARATOR = XK_KP_Separator?
    // SDLK_OPER
    m_keyMap[SDLK_OPER] = XK_asciitilde; // Tilde?
    // SDLK_CLEARAGAIN
    // SDLK_CRSEL
    // SDLK_EXSEL
    // SDLK_THOUSANDSEPERATOR
    // SDLK_DECIMALSEPARATOR
    m_keyMap[SDLK_CURRENCYUNIT] = XK_currency;
    // SDLK_CURRENCYSUBUNIT
    // SDLK_KP_LEFTPAREN
    // SDLK_KP_RIGHTPAREN
    m_keyMap[SDLK_KP_LEFTBRACE] = XK_braceleft;
    m_keyMap[SDLK_KP_RIGHTBRACE] = XK_braceright;
    m_keyMap[SDLK_KP_TAB] = XK_KP_Tab;
    // SDLK_KP_BACKSPACE
    // SDLK_KP_A -> SDKL_KP_F
    // SDLK_KP_XOR
    // SDLK_KP_POWER
    // SDLK_KP_PERCENT
    // SDLK_KP_LESS
    // SDLK_KP_GREATER
    // SDLK_KP_AMPERSAND
    // SDLK_KP_DBLAMPERSAND
    m_keyMap[SDLK_KP_VERTICALBAR] = XK_bar;
    // SLDK_KP_DBLVERTICALBAR -> SDLK_KP_MEMDIVID
    m_keyMap[SDLK_KP_PLUSMINUS] = XK_plusminus;
    // SDLK_KP_CLEAR
    // SDLK_KP_CLEARENTRY
    // SDLK_KP_BINARY
    // SDLK_KP_OCTAL
    // SDLK_KP_DECIMAL
    // SDLK_KP_HEXADECIMAL
    m_keyMap[SDLK_LCTRL] = XK_Control_L;
    m_keyMap[SDLK_LSHIFT] = XK_Shift_L;
    m_keyMap[SDLK_LALT] = XK_Alt_L;
    m_keyMap[SDLK_LGUI] = XK_Super_L;
    m_keyMap[SDLK_RCTRL] = XK_Control_R;
    m_keyMap[SDLK_RSHIFT] = XK_Shift_R;
    m_keyMap[SDLK_RALT] = XK_Alt_R;
    m_keyMap[SDLK_RGUI] = XK_Super_R;
    // SDLK_MODE -> SDLK_SLEEP
    


}

KeySym SDLToX11Keymap::convert(SDL_Keycode sdlKey) const{
    // These keys match already
    if ((sdlKey >= SDLK_SPACE) && (sdlKey <= SDLK_z)){
        return sdlKey;
    }

    std::unordered_map<SDL_Keycode, KeySym>::const_iterator key = m_keyMap.find(sdlKey);
    if (key != m_keyMap.end()){
        return key->second;
    }
    return NoSymbol;
}
