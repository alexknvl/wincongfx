#include <deque>
#include <vector>
#include <ctime>

#include <WinConGfx/Console.hpp>
#include <Windows.h>

struct Point {
  size_t x;
  size_t y;
  
  Point(size_t x, size_t y) : x(x), y(y) { }
};

int main(int, char**) {
  WinConGfx::Console console;
  
  console.SetSize(80, 25);
  console.SetCursorVisiblity(false);
  std::deque<Point> snake;
  
  snake.push_back(Point(40, 12));
  snake.push_back(Point(40, 12));
  snake.push_back(Point(40, 12));
  snake.push_back(Point(40, 12));
  snake.push_back(Point(40, 12));
  
  clock_t lastUpdate = clock();
  int key = 0;
  bool stop = false;
  
  while (!stop) {
    clock_t time = clock();
    
    int newKey = console.ReadKey(false);
    if (newKey != 0) {
      key = newKey;
    }
    
    // if not enough time has passed, just read any new keys and continue
    if ((time - lastUpdate) / (double) CLOCKS_PER_SEC >= 0.25) {
      // update snake position
      if (key == 0) {
        continue;
      } else switch (key) {
        case 'W': {
          Point p = snake.back();
          snake.push_back(Point(p.x, (p.y + 24) % 25));
          snake.pop_front();
          break;
        }
        case 'A': {
          Point p = snake.back();
          snake.push_back(Point((p.x + 79) % 80, p.y));
          snake.pop_front();
          break;
        }
        case 'S': {
          Point p = snake.back();
          snake.push_back(Point(p.x, (p.y + 1) % 25));
          snake.pop_front();
          break;
        }
        case 'D': {
          Point p = snake.back();
          snake.push_back(Point((p.x + 1) % 80, p.y));
          snake.pop_front();
          break;
        }
        case 'Q': {
          stop = true;
          break;
        }
      }
      
      lastUpdate = time;
    }
    
    // render
    console.Clear();
    console.Fill(0, 0, 10, 10, WinConGfx::Console::BG_BLUE, WinConGfx::Console::FG_YELLOW, '$');
    for (std::deque<Point>::const_iterator it = snake.begin(); it != snake.end(); ++it) {
      console.Put(it->x, it->y, '#');
    }
    console.SwapBuffers();
  }
}