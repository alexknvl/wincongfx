#include <windows.h>
#include <vector>
#include <cassert>

#include "Console.hpp"

namespace WinConGfx {

struct Console::InternalData {
  HANDLE inputHandle;
  HANDLE outputHandle;

  SHORT screenWidth;
  SHORT screenHeight;
  std::vector<CHAR_INFO> screenBuffer;
  std::vector<CHAR_INFO> backBuffer;
};

Console::Console() {
  data = new InternalData();
  data->inputHandle = ::GetStdHandle(STD_INPUT_HANDLE);
  data->outputHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
  SetSize(80, 25);
}

Console::~Console() {
  delete data;
}

void Console::GetSize(size_t& width, size_t& height) const {
  width = data->screenWidth;
  height = data->screenHeight;
}
void Console::SetSize(size_t width, size_t height) {
  assert(width >= 1 && width <= 0x7FFF);
  assert(height >= 1 && height <= 0x7FFF);

  // save the params
  data->screenWidth = static_cast<SHORT>(width);
  data->screenHeight = static_cast<SHORT>(height);

  // Resize buffers.
  data->screenBuffer.resize(data->screenWidth * data->screenHeight);
  data->backBuffer.resize(data->screenWidth * data->screenHeight);

  // Set the actual screen buffer size.
  COORD screenSize = { 
    data->screenWidth, 
    data->screenHeight
  };
  ::SetConsoleScreenBufferSize(data->outputHandle, screenSize);

  // Set window size.
  SMALL_RECT wndSize = { 
    0, 
    0, 
    data->screenWidth - 1, 
    data->screenHeight - 1
  };
  ::SetConsoleWindowInfo(data->outputHandle, TRUE, &wndSize);

  // Copy console output back to the buffers.
  COORD dwBufferCoord = { 
    0, 
    0 
  };
  COORD dwBufferSize = { 
    data->screenWidth, 
    data->screenHeight
  };
  SMALL_RECT srReadRegion = { 
    0, 
    0, 
    data->screenWidth - 1, 
    data->screenHeight - 1
  };
  ::ReadConsoleOutputA(data->outputHandle, &data->screenBuffer[0], 
    dwBufferSize, dwBufferCoord, &srReadRegion);
  ::ReadConsoleOutputA(data->outputHandle, &data->backBuffer[0], 
    dwBufferSize, dwBufferCoord, &srReadRegion);
}
void Console::SetTitle(const std::string& title) {
  ::SetConsoleTitleA(title.c_str());
}
void Console::SetCursorVisiblity(bool flag) {
  CONSOLE_CURSOR_INFO cursorInfo;
  ::GetConsoleCursorInfo(data->outputHandle, &cursorInfo);
  cursorInfo.bVisible = (BOOL) flag;
  ::SetConsoleCursorInfo(data->outputHandle, &cursorInfo);
}

void Console::Put(size_t x, size_t y, char c) {
  // Check that we can cast to SHORT
  assert(x <= 0x7FFF);
  assert(y <= 0x7FFF);

  // Check that the coordinates are inside the screen buffer.
  assert(static_cast<SHORT>(x) < data->screenWidth);
  assert(static_cast<SHORT>(y) < data->screenHeight);

  CHAR_INFO& ci = data->backBuffer[data->screenWidth * y + x];
  ci.Char.AsciiChar = c;
}
void Console::Put(size_t x, size_t y, BGColor attr) {
  // Check that we can cast to SHORT
  assert(x <= 0x7FFF);
  assert(y <= 0x7FFF);

  // Check that the coordinates are inside the screen buffer.
  assert(static_cast<SHORT>(x) < data->screenWidth);
  assert(static_cast<SHORT>(y) < data->screenHeight);

  CHAR_INFO& ci = data->backBuffer[data->screenWidth * y + x];
  ci.Attributes &= ~BG_MASK;
  ci.Attributes |= attr;
}
void Console::Put(size_t x, size_t y, FGColor attr) {
  // Check that we can cast to SHORT
  assert(x <= 0x7FFF);
  assert(y <= 0x7FFF);
  
  // Check that the coordinates are inside the screen buffer.
  assert(static_cast<SHORT>(x) < data->screenWidth);
  assert(static_cast<SHORT>(y) < data->screenHeight);

  CHAR_INFO& ci = data->backBuffer[data->screenWidth * y + x];
  ci.Attributes &= ~FG_MASK;
  ci.Attributes |= attr;
}

void Console::Clear() {
  Clear(0, 0, data->screenWidth, data->screenHeight);
}
void Console::Clear(size_t top, size_t left, size_t width, size_t height) {
  Fill(top, left, width, height, BG_BLACK, FG_WHITE, ' ');
}
void Console::Fill(size_t top, size_t left, size_t width, size_t height, BGColor bg, FGColor fg, char c) {
  assert(width >= 1);
  assert(height >= 1);

  // Check that we can cast to SHORT
  assert(left + width <= 0x7FFF);
  assert(top + height <= 0x7FFF);
  
  // Check that the rect is inside the screen buffer.
  assert(static_cast<SHORT>(top + height) <= data->screenHeight);
  assert(static_cast<SHORT>(left + width) <= data->screenWidth);
  
  // Fill the rect
  for (SHORT y = static_cast<SHORT>(top); y < static_cast<SHORT>(top + height); y++) {
    for (SHORT x = static_cast<SHORT>(left); x < static_cast<SHORT>(left + width); x++) {
      CHAR_INFO& ci = data->backBuffer[data->screenWidth * y + x];

      ci.Attributes &= ~BG_MASK;
      ci.Attributes &= ~FG_MASK;
      ci.Attributes |= bg;
      ci.Attributes |= fg;

      ci.Char.AsciiChar = c;
    }
  }
}

void Console::SwapBuffers() {
  // Loop over all lines.
  for (SHORT y = 0; y < data->screenHeight; y++) {
    // Check whether the current line is changed.
    // Note: this is why we keep screen buffer in data instead of simply
    //       rendering back buffer straight to the console.
    bool changed = ::memcmp(
      &data->screenBuffer[y * data->screenWidth], 
      &data->backBuffer[y * data->screenWidth],
      sizeof(CHAR_INFO) * data->screenWidth
    ) != 0;

    if (changed) {
      // If the current line was changed, render it.
      COORD dwBufferCoord = { 0, y };
      COORD dwBufferSize = { data->screenWidth, data->screenHeight };
      SMALL_RECT srWriteRegion = { 0, y, data->screenWidth - 1, y };

      WriteConsoleOutputA(data->outputHandle, &data->backBuffer[0], 
        dwBufferSize, dwBufferCoord, &srWriteRegion);
      
      // Copy the line from the back buffer to the screen buffer. 
      ::memcpy(
        &data->screenBuffer[y * data->screenWidth],
        &data->backBuffer[y * data->screenWidth],
        sizeof(CHAR_INFO) * data->screenWidth
      );
    }
  }
}

void Console::SwapBuffersFull() {
  // Render everything.
  COORD dwBufferCoord = { 0, 0 };
  COORD dwBufferSize = { data->screenWidth, data->screenHeight };
  SMALL_RECT srWriteRegion = { 0, 0, data->screenWidth - 1, data->screenHeight - 1};

  WriteConsoleOutputA(data->outputHandle, &data->backBuffer[0], 
    dwBufferSize, dwBufferCoord, &srWriteRegion);
  
  // Copy everything from the back buffer to the screen buffer.
  ::memcpy(
    &data->screenBuffer[0],
    &data->backBuffer[0],
    sizeof(CHAR_INFO) * data->screenWidth * data->screenHeight
  );
}

int Console::ReadKey(bool blocking) {
  while (true) {
    DWORD eventCount = 0;
    ::GetNumberOfConsoleInputEvents(data->inputHandle, &eventCount);

    while (eventCount > 0) {
      // Read one event.
      INPUT_RECORD record;
      DWORD read;
      ::ReadConsoleInputA(data->inputHandle, &record, 1, &read);

      // XXX[10.8.2012 alex]: what if `read` is 0?
      if (read > 0) {
        eventCount--;
        
        // If it's a key event and key was pressed,
        // return its virtual key code.
        if (record.EventType == KEY_EVENT) {
          KEY_EVENT_RECORD& keyEvent = record.Event.KeyEvent;

          if (keyEvent.bKeyDown) {
            return keyEvent.wVirtualKeyCode;
          }
        }
      } else {
        // FIXME[10.8.2012 alex]
        assert(0);
      }
    }
    
    // If there are no events and we are reading in
    // a non-blocking way, return.
    if (!blocking) {
      return 0;
    }
  }
}

}; // namespace WinConGfx