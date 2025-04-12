#include "./game.h"

// Static init
int Game::players = 1;
int Game::difficulty = 1;

void Game::init() {
  // Assigns variables
  x = 0;
  y = 0;

  // Refreshes game board
  gridarray = {{
      {0, 0, 0},
      {0, 0, 0},
      {0, 0, 0},
  }};

  // Init variables
  turn = 0;
  selector = 0;

  // Assigns bitmaps
  grid = asw::assets::loadTexture("assets/images/grid.png");
  img_x = asw::assets::loadTexture("assets/images/x.png");
  img_o = asw::assets::loadTexture("assets/images/o.png");
  xwin = asw::assets::loadTexture("assets/images/xwin.png");
  owin = asw::assets::loadTexture("assets/images/owin.png");
  catsgame = asw::assets::loadTexture("assets/images/catsgame.png");

  cursor[0] = asw::assets::loadTexture("assets/images/cursor.png");
  cursor[1] = asw::assets::loadTexture("assets/images/cursor_x.png");
  cursor[2] = asw::assets::loadTexture("assets/images/cursor_o.png");

  selected[0] = asw::assets::loadTexture("assets/images/selected.png");
  selected[1] = asw::assets::loadTexture("assets/images/selected2.png");
  selected[2] = asw::assets::loadTexture("assets/images/selected3.png");
  selected[3] = asw::assets::loadTexture("assets/images/selected4.png");

  // Assigns Sounds
  win = asw::assets::loadSample("assets/sfx/win.wav");
  lose = asw::assets::loadSample("assets/sfx/lose.wav");
  cat = asw::assets::loadSample("assets/sfx/catsgame.wav");
  place = asw::assets::loadSample("assets/sfx/place.wav");

  // Sets button positions
  menu.set_images("assets/images/buttons/menu.png",
                  "assets/images/buttons/menu_hover.png");
  menu.set_position(0, 270);
}

// Performs unique one player actions
void Game::gameOne() {
  // Places x or o respectively
  if (turn == 0) {
    if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT) &&
        gridarray[x][y] == 0) {
      gridarray[x][y] = 1;
      if (soundfx) {
        asw::sound::play(place, 255, 122, 0);
      }
      turn = 1;
    }
  }
  // Opponent AI
  else if (turn == 1) {
    // If a move has been made
    bool move_made = false;

    // Smart AI
    for (int t = difficulty; t > 0; t--) {
      for (int i = 0; i < 3; i++) {
        // Across
        if (!move_made && ((gridarray[i][0] == t && gridarray[i][1] == t &&
                            gridarray[i][2] == 0) ||
                           (gridarray[i][0] == t && gridarray[i][1] == 0 &&
                            gridarray[i][2] == t) ||
                           (gridarray[i][0] == 0 && gridarray[i][1] == t &&
                            gridarray[i][2] == t))) {
          if (gridarray[i][0] == 0) {
            gridarray[i][0] = 2;
          } else if (gridarray[i][1] == 0) {
            gridarray[i][1] = 2;
          } else if (gridarray[i][2] == 0) {
            gridarray[i][2] = 2;
          }
          move_made = true;
          break;
        }
        // Down
        else if (!move_made && ((gridarray[0][i] == t && gridarray[1][i] == t &&
                                 gridarray[2][i] == 0) ||
                                (gridarray[0][i] == t && gridarray[1][i] == 0 &&
                                 gridarray[2][i] == t) ||
                                (gridarray[0][i] == 0 && gridarray[1][i] == t &&
                                 gridarray[2][i] == t))) {
          if (gridarray[0][i] == 0) {
            gridarray[0][i] = 2;
          } else if (gridarray[1][i] == 0) {
            gridarray[1][i] = 2;
          } else if (gridarray[2][i] == 0) {
            gridarray[2][i] = 2;
          }
          move_made = true;
          break;
        }
      }
      // Diagonal top left to bottom right
      if (!move_made && ((gridarray[0][0] == t && gridarray[1][1] == t &&
                          gridarray[2][2] == 0) ||
                         (gridarray[0][0] == t && gridarray[1][1] == 0 &&
                          gridarray[2][2] == t) ||
                         (gridarray[0][0] == 0 && gridarray[1][1] == t &&
                          gridarray[2][2] == t))) {
        if (gridarray[0][0] == 0) {
          gridarray[0][0] = 2;
        } else if (gridarray[1][1] == 0) {
          gridarray[1][1] = 2;
        } else if (gridarray[2][2] == 0) {
          gridarray[2][2] = 2;
        }
        move_made = true;
        break;
      }
      // Diagonal top right to bottom left
      else if (!move_made && ((gridarray[2][0] == t && gridarray[1][1] == t &&
                               gridarray[0][2] == 0) ||
                              (gridarray[2][0] == t && gridarray[1][1] == 0 &&
                               gridarray[0][2] == t) ||
                              (gridarray[2][0] == 0 && gridarray[1][1] == t &&
                               gridarray[0][2] == t))) {
        if (gridarray[2][0] == 0) {
          gridarray[2][0] = 2;
        } else if (gridarray[1][1] == 0) {
          gridarray[1][1] = 2;
        } else if (gridarray[0][2] == 0) {
          gridarray[0][2] = 2;
        }
        move_made = true;
        break;
      }
    }
    // Revert to dumb ai
    while (!move_made) {
      // No more moves available!
      if (gridarray[0][0] != 0 && gridarray[0][1] != 0 &&
          gridarray[0][2] != 0 && gridarray[1][0] != 0 &&
          gridarray[1][1] != 0 && gridarray[1][2] != 0 &&
          gridarray[2][0] != 0 && gridarray[2][1] != 0 &&
          gridarray[2][2] != 0) {
        break;
      }

      // Choose random place
      const int random_integer = asw::random::between(0, 2);
      const int random_integer2 = asw::random::between(0, 2);
      if (gridarray[random_integer][random_integer2] == 0) {
        gridarray[random_integer][random_integer2] = 2;
        move_made = true;
      }
    }
    if (soundfx) {
      asw::sound::play(place, 255, 122, 0);
    }

    if (move_made) {
      turn = 0;
    }
  }
}

// Performs unique two player actions
void Game::gameTwo() {
  // Places x or o respectively
  if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT) &&
      gridarray[x][y] == 0 && !menu.get_hover()) {
    gridarray[x][y] = turn + 1;
    turn = (turn + 1) % 2;
    if (soundfx) {
      asw::sound::play(place, 255, 122, 0);
    }
  }
}

void Game::update(float dt) {
  // Chooses which tile is selected based on mouse position
  x = asw::input::mouse.x / 100;
  y = asw::input::mouse.y / 100;

  // Check and perform x winning action
  if ((gridarray[0][0] == 1 && gridarray[0][1] == 1 && gridarray[0][2] == 1) ||
      (gridarray[1][0] == 1 && gridarray[1][1] == 1 && gridarray[1][2] == 1) ||
      (gridarray[2][0] == 1 && gridarray[2][1] == 1 && gridarray[2][2] == 1) ||
      (gridarray[0][0] == 1 && gridarray[1][0] == 1 && gridarray[2][0] == 1) ||
      (gridarray[0][1] == 1 && gridarray[1][1] == 1 && gridarray[2][1] == 1) ||
      (gridarray[0][2] == 1 && gridarray[1][2] == 1 && gridarray[2][2] == 1) ||
      (gridarray[0][0] == 1 && gridarray[1][1] == 1 && gridarray[2][2] == 1) ||
      (gridarray[2][0] == 1 && gridarray[1][1] == 1 && gridarray[0][2] == 1)) {
    if (gridarray[0][0] == 1 && gridarray[0][1] == 1 && gridarray[0][2] == 1) {
      asw::draw::line(asw::Vec2<float>(50, 50), asw::Vec2<float>(50, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[1][0] == 1 && gridarray[1][1] == 1 && gridarray[1][2] == 1) {
      asw::draw::line(asw::Vec2<float>(150, 50), asw::Vec2<float>(150, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[2][0] == 1 && gridarray[2][1] == 1 && gridarray[2][2] == 1) {
      asw::draw::line(asw::Vec2<float>(250, 50), asw::Vec2<float>(250, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][0] == 1 && gridarray[1][0] == 1 && gridarray[2][0] == 1) {
      asw::draw::line(asw::Vec2<float>(50, 50), asw::Vec2<float>(250, 50),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][1] == 1 && gridarray[1][1] == 1 && gridarray[2][1] == 1) {
      asw::draw::line(asw::Vec2<float>(50, 150), asw::Vec2<float>(250, 150),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][2] == 1 && gridarray[1][2] == 1 && gridarray[2][2] == 1) {
      asw::draw::line(asw::Vec2<float>(50, 250), asw::Vec2<float>(250, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][0] == 1 && gridarray[1][1] == 1 && gridarray[2][2] == 1) {
      asw::draw::line(asw::Vec2<float>(50, 50), asw::Vec2<float>(250, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[2][0] == 1 && gridarray[1][1] == 1 && gridarray[0][2] == 1) {
      asw::draw::line(asw::Vec2<float>(250, 50), asw::Vec2<float>(50, 250),
                      asw::util::makeColor(255, 0, 0));
    }

    if (soundfx) {
      asw::sound::play(win, 255, 122, 0);
    }

    asw::draw::sprite(xwin, asw::Vec2<float>(0, 0));
    // rest(2000);
    turn = 0;
    init();
  }

  // Check and perform o winning action
  else if ((gridarray[0][0] == 2 && gridarray[0][1] == 2 &&
            gridarray[0][2] == 2) ||
           (gridarray[1][0] == 2 && gridarray[1][1] == 2 &&
            gridarray[1][2] == 2) ||
           (gridarray[2][0] == 2 && gridarray[2][1] == 2 &&
            gridarray[2][2] == 2) ||
           (gridarray[0][0] == 2 && gridarray[1][0] == 2 &&
            gridarray[2][0] == 2) ||
           (gridarray[0][1] == 2 && gridarray[1][1] == 2 &&
            gridarray[2][1] == 2) ||
           (gridarray[0][2] == 2 && gridarray[1][2] == 2 &&
            gridarray[2][2] == 2) ||
           (gridarray[0][0] == 2 && gridarray[1][1] == 2 &&
            gridarray[2][2] == 2) ||
           (gridarray[2][0] == 2 && gridarray[1][1] == 2 &&
            gridarray[0][2] == 2)) {
    if (gridarray[0][0] == 2 && gridarray[0][1] == 2 && gridarray[0][2] == 2) {
      asw::draw::line(asw::Vec2<float>(50, 50), asw::Vec2<float>(50, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[1][0] == 2 && gridarray[1][1] == 2 && gridarray[1][2] == 2) {
      asw::draw::line(asw::Vec2<float>(150, 50), asw::Vec2<float>(150, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[2][0] == 2 && gridarray[2][1] == 2 && gridarray[2][2] == 2) {
      asw::draw::line(asw::Vec2<float>(250, 50), asw::Vec2<float>(250, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][0] == 2 && gridarray[1][0] == 2 && gridarray[2][0] == 2) {
      asw::draw::line(asw::Vec2<float>(50, 50), asw::Vec2<float>(250, 50),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][1] == 2 && gridarray[1][1] == 2 && gridarray[2][1] == 2) {
      asw::draw::line(asw::Vec2<float>(50, 150), asw::Vec2<float>(250, 150),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][2] == 2 && gridarray[1][2] == 2 && gridarray[2][2] == 2) {
      asw::draw::line(asw::Vec2<float>(50, 250), asw::Vec2<float>(250, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[0][0] == 2 && gridarray[1][1] == 2 && gridarray[2][2] == 2) {
      asw::draw::line(asw::Vec2<float>(50, 50), asw::Vec2<float>(250, 250),
                      asw::util::makeColor(255, 0, 0));
    }
    if (gridarray[2][0] == 2 && gridarray[1][1] == 2 && gridarray[0][2] == 2) {
      asw::draw::line(asw::Vec2<float>(250, 50), asw::Vec2<float>(50, 250),
                      asw::util::makeColor(255, 0, 0));
    }

    if (soundfx) {
      asw::sound::play(lose, 255, 122, 0);
    }
    asw::draw::sprite(owin, asw::Vec2<float>(0, 0));
    // rest(2000);
    turn = 1;
    init();
  }

  // Check and perform cats game action
  else if (gridarray[0][0] != 0 && gridarray[1][0] != 0 &&
           gridarray[2][0] != 0 && gridarray[0][1] != 0 &&
           gridarray[1][1] != 0 && gridarray[2][1] != 0 &&
           gridarray[0][2] != 0 && gridarray[1][2] != 0 &&
           gridarray[2][2] != 0) {
    if (soundfx) {
      asw::sound::play(cat, 255, 122, 0);
    }
    asw::draw::sprite(catsgame, asw::Vec2<float>(0, 0));
    // rest(2000);
    turn = 1;
    init();
  }

  // Runs game function
  if (Game::players == 1) {
    gameOne();
  } else {
    gameTwo();
  }

  // Change selector sprite
  if (asw::input::wasKeyPressed(asw::input::Key::S)) {
    selector = (selector + 1) % 4;
  }

  // Checks for mouse press
  if (menu.is_clicked()) {
    sceneManager.setNextScene(States::Menu);
  }
}

void Game::draw() {
  // Draws grid
  asw::draw::sprite(grid, asw::Vec2<float>(0, 0));

  // Draws tiles on board
  for (int i = 0; i < 4; i++) {
    for (int t = 0; t < 4; t++) {
      if (gridarray[i][t] == 1) {
        asw::draw::sprite(img_x, asw::Vec2<float>(i * 100, t * 100));
      } else if (gridarray[i][t] == 2) {
        asw::draw::sprite(img_o, asw::Vec2<float>(i * 100, t * 100));
      }
    }
  }

  // Draws selection tile
  asw::draw::sprite(selected[selector], asw::Vec2<float>(x * 100, y * 100));

  // Draws Menu Button
  menu.draw();

  if (Game::players == 1) {
    asw::draw::sprite(
        cursor[1], asw::Vec2<float>(asw::input::mouse.x, asw::input::mouse.y));
  } else {
    asw::draw::sprite(
        cursor[(turn + 1)],
        asw::Vec2<float>(asw::input::mouse.x, asw::input::mouse.y));
  }
}