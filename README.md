# Windows Console Graphics Library

Super fast console ASCII rendering for windows.

## Usage

```cpp
#include <WinConGfx/Console.hpp>

int main(int, char**) {
  WinConGfx::Console console;
  
  console.SetSize(80, 25);
  console.SetCursorVisiblity(false);
  
  // Clear the back buffer.
  console.Clear();
  
  // Draw stuff.
  console.Fill(0, 0, 10, 10, WinConGfx::Console::BG_BLUE, WinConGfx::Console::FG_YELLOW, '$');
  
  // Renders changes.
  console.SwapBuffers();
}
```

See example [here](/blob/master/src/SampleApp/main.cpp).
