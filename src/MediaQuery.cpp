#include "MediaQuery.h"
#include <optional>

#ifdef _WIN32

#include <windows.h>

#else

#include <sys/ioctl.h>
#include <unistd.h>

#endif

MaybeScreenDimensions MediaQuery::get_terminal_dimensions() {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if ( GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ),
                                   &csbi ) ) {
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return ScreenDimensions{ .width = columns, .height = rows };
  }
#else
  struct winsize ws;
  if ( ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws ) == 0 ) {
    return ScreenDimensions{ .width = ws.ws_col, .height = ws.ws_row };
  }
#endif
  return std::nullopt;
}
