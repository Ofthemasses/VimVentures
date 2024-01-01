#ifndef RECT2D_HPP
#define RECT2D_HPP

#include <SDL2/SDL.h>

class Rect2D {
protected:
  SDL_Rect m_rect{0, 0, 0, 0};

public:
  Rect2D() : m_rect{0, 0, 0, 0} {}
  Rect2D(int x, int y, int w, int h) : m_rect{x, y, w, h} {}

  void SetPosition(int x, int y) {
    m_rect.x = x;
    m_rect.y = y;
  }

  void SetSize(int w, int h) {
    m_rect.w = w;
    m_rect.h = h;
  }

  int GetX() const { return m_rect.x; }
  int GetY() const { return m_rect.y; }
  int GetWidth() const { return m_rect.w; }
  int GetHeight() const { return m_rect.h; }

  SDL_Rect GetSDLRect() const { return m_rect; }

  virtual ~Rect2D() = default;
};

#endif
