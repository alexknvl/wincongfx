#ifndef WINCONGFX_CONSOLE_HPP_
#define WINCONGFX_CONSOLE_HPP_

#ifdef _WIN32
  #ifdef WINCONGFX_DLL
    #define WINCONGFX_API __declspec(dllexport)
  #else
    #define WINCONGFX_API __declspec(dllimport)
  #endif
#else
  #define WINCONGFX_API
#endif

namespace WinConGfx {

class Console {
public:
  // background colors
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
  
  // foreground colors
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
  WINCONGFX_API Console();
  WINCONGFX_API ~Console();

  // Returns the size of the console.
  WINCONGFX_API void GetSize(size_t& width, size_t& height) const;
  
  // Sets the size of the console.
  // Width and height must be positive 
  // and less or equal to 0x7FFF.
  WINCONGFX_API void SetSize(size_t width, size_t height);
  
  // Sets the title of the console.
  WINCONGFX_API void SetTitle(const std::string& title);
  
  // Changes the cursor visibility.
  WINCONGFX_API void SetCursorVisiblity(bool flag);

  // Puts character at given coordinates.
  // Coordinates must satisfy 0 <= x < width, 0 <= y < height.
  WINCONGFX_API void Put(size_t x, size_t y, char c);
  
  // Changes background color at given coordinates.
  // Coordinates must satisfy 0 <= x < width, 0 <= y < height.
  WINCONGFX_API void Put(size_t x, size_t y, BGColor attr);
  
  // Changes foreground color at given coordinates.
  // Coordinates must satisfy 0 <= x < width, 0 <= y < height.
  WINCONGFX_API void Put(size_t x, size_t y, FGColor attr);

  // Clears the console.
  // Is equivalent to Clear(0, 0, width, height).
  WINCONGFX_API void Clear();
  
  // Clears a part of the console.
  // All points being cleared must satisfy 0 <= x < width, 0 <= y < height.
  // Is equivalent to Fill(0, 0, width, height, BG_BLACK, FG_WHITE, ' ').
  WINCONGFX_API void Clear(size_t top, size_t left, size_t width, size_t height);
  
  // Fills a part of the console with the given character and 
  // sets the background and foreground colors.
  WINCONGFX_API void Fill(size_t top, size_t left, size_t width, size_t height, BGColor bg, FGColor fg, char c);

  // Renders only lines that have been changed.
  WINCONGFX_API void SwapBuffers();
  
  // Renders everything.
  WINCONGFX_API void SwapBuffersFull();
  
  // Reads a key from the console. If blocking is true, waits for one.
  WINCONGFX_API int ReadKey(bool blocking = false);

private:
  // forward declaration to hide away the implementation and
  // reduce dependencies (i.e. windows.h)
  struct InternalData;
  InternalData* data;
};

}; // namespace WinConGfx

#endif//WINCONGFX_CONSOLE_HPP_