#include "controls.h"

#include <asw/asw.h>

void controls::bind() {
  asw::input::bind_action(
      CLICK, asw::input::MouseButtonBinding{asw::input::MouseButton::Left});
  asw::input::bind_action(CYCLE_SELECTOR,
                          asw::input::KeyBinding{asw::input::Key::S});
}
