#include <asw/asw.h>

#include "./states/game.h"
#include "./states/init.h"
#include "./states/menu.h"

// Program starts here
int main() {
  // Allegro Stuff
  asw::core::init(300, 300);
  asw::display::set_title("A.D.S. Games - Tic Tac Toe");
  asw::display::set_icon("assets/icon.ico");

  // Scene manager
  auto app = asw::scene::SceneManager<States>();
  app.register_scene<Init>(States::Init, app);
  app.register_scene<Menu>(States::Menu, app);
  app.register_scene<Game>(States::Game, app);
  app.set_next_scene(States::Init);
  app.start();

  asw::core::shutdown();

  // End
  return 0;
}
