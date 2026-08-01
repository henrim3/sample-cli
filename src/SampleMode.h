#include "AppMode.h"

class SampleMode : public IAppMode {
public:
  ModeResponse handle_key( SpecialKey key, AppContext & context ) override;
  ModeResponse handle_action( const Action & action, AppContext & context ) override;
};
