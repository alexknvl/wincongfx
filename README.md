# Windows Console Graphics Library

Super fast console ASCII rendering for windows.

## Usage

```cpp
#include <WinConGfx/Console.hpp>

int main(int, char**) {
  WinConGfx::Console console;
  
  console.SetSize(80, 25);
  console.SetCursorVisiblity(false);
  
  int counter = 0;
  while (true) {
    counter += 1;
    // Clear the back buffer.
    console.Clear();

    // Draw stuff.
    size_t size = counter;
    console.Fill(0, 0, size, size, WinConGfx::Console::BG_BLUE, WinConGfx::Console::FG_YELLOW, '$');

    // Render the changes.
    console.SwapBuffers();
    
    if (counter == 10) counter = 0;
  }
}
```

See Snake game example [here](/blob/master/src/SampleApp/main.cpp).
