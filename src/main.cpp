#include <asw/asw.h>

#include "./states/game.h"
#include "./states/init.h"
#include "./states/menu.h"

// Program starts here
int main() {
  // Allegro Stuff
  asw::core::init(300, 300);
  asw::display::setTitle("A.D.S. Games - Tic Tac Toe");

  // Scene manager
  auto app = asw::scene::SceneManager<States>();
  app.registerScene<Init>(States::Init, app);
  app.registerScene<Menu>(States::Menu, app);
  app.registerScene<Game>(States::Game, app);
  app.setNextScene(States::Init);
  app.start();

  // End
  return 0;
}
