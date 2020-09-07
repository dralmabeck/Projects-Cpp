#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  int numberOfLines = 20;
  NCursesDisplay::Display(system, numberOfLines);
}