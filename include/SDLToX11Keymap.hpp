#ifndef SDLTOX11KEYMAP_HPP
#define SDLTOX11KEYMAP_HPP

#include <SDL2/SDL.h>
#include <X11/Xlib.h>
#include <unordered_map>

class SDLToX11Keymap {
public:
  SDLToX11Keymap();
  KeySym convert(SDL_Keycode sdlKey) const;

private:
  std::unordered_map<SDL_Keycode, KeySym> m_keyMap;
};

#endif
