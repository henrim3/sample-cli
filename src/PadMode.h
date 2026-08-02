#include "AppMode.h"

class PadMode : public IAppMode {
  ModeResponse handle_key( Key key, AppContext & context );
  ModeResponse handle_action( const Action & action, AppContext & context );
};
