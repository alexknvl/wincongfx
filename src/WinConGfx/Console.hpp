#ifndef WINCONGFX_CONSOLE_HPP_
#define WINCONGFX_CONSOLE_HPP_

namespace WinConGfx {

class Console {
public:
  enum BGColor {
    BG_BLACK = 0,

    BG_RED = 0x0040,
    BG_GREEN = 0x0020,
    BG_BLUE = 0x0010,
    BG_INTENSIVE = 0x0080,

    BG_CYAN = BG_GREEN | BG_BLUE,
    BG_MAGENTA = BG_RED | BG_BLUE,
    BG_YELLOW = BG_RED | BG_GREEN,
    BG_WHITE = BG_RED | BG_GREEN | BG_BLUE, 

    BG_MASK = BG_RED | BG_GREEN | BG_BLUE | BG_INTENSIVE
  };
  enum FGColor {
    FG_BLACK = 0,

    FG_RED = 0x0004,
    FG_GREEN = 0x0002,
    FG_BLUE = 0x0001,
    FG_INTENSIVE = 0x0008,

    FG_CYAN = FG_GREEN | FG_BLUE,
    FG_MAGENTA = FG_RED | FG_BLUE,
    FG_YELLOW = FG_RED | FG_GREEN,
    FG_WHITE = FG_RED | FG_GREEN | FG_BLUE, 

    FG_MASK = FG_RED | FG_GREEN | FG_BLUE | FG_INTENSIVE
  };
  
public:
  Console();
  ~Console();

  void GetSize(size_t& width, size_t& height) const;
  void SetSize(size_t width, size_t height);
  void SetTitle(const std::string& title);
  void SetCursorVisiblity(bool flag);

  void Put(size_t x, size_t y, char c);
  void Put(size_t x, size_t y, BGColor attr);
  void Put(size_t x, size_t y, FGColor attr);

  void Clear();
  void Clear(size_t top, size_t left, size_t width, size_t height);
  void Fill(size_t top, size_t left, size_t width, size_t height, BGColor bg, FGColor fg, char c);

  void SwapBuffers();
  void SwapBuffersFull();

  int ReadKey(bool blocking = false);

private:
  struct InternalData;
  InternalData* data;
};

}; // namespace WinConGfx

#endif//WINCONGFX_CONSOLE_HPP_