#include <allegro.h>
#include <loadpng.h>

#include "button.h"
#include "mouseListener.h"

#include "aexlib/draw/animation.h"
#include "aexlib/draw/primitive.h"

// Mouse listener
mouseListener m_listener;

// Creates images
BITMAP* buffer;
BITMAP* grid;
BITMAP* img_x;
BITMAP* img_o;
BITMAP* blank;
BITMAP* xwin;
BITMAP* owin;
BITMAP* catsgame;
BITMAP* cursor[3];
BITMAP* main_menu;
BITMAP* selected[4];

// Create sounds
SAMPLE* win;
SAMPLE* lose;
SAMPLE* cat;
SAMPLE* place;

// Creates Butons
Button one_player;
Button two_player;
Button quit;
Button menu;
Button sound;
Button difficulty_b;

// Creates variables
int x;
int y;
int turn;
int selector;
int gridimage;
int difficulty;
int mode;
int soundfx;
int gameScreen;
bool gameRunning;

// Draw function
void draw(bool to_screen);

// Array of the game board
int gridarray[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

// Random number generator. Use int random(lowest,highest);
int random(int newLowest, int newHighest) {
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest + int(range * rand() / (RAND_MAX + 1.0));
  return randomNumber;
}

// Sets up game (bitmaps, sounds, ect.)
void setup(bool first) {
  // Assigns variables
  x = 0;
  y = 0;
  gridimage = 0;

  // Refreshes game board
  for (int i = 0; i < 4; i++) {
    for (int t = 0; t < 4; t++) {
      gridarray[i][t] = 0;
    }
  }

  if (first) {
    // Allegro Stuff
    allegro_init();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ".");
    set_window_title("A.D.S. Games - Tic Tac Toe");

    // Graphics
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 300, 300, 0, 0);

    // Init variables
    gameRunning = true;
    turn = 0;
    selector = 0;
    soundfx = true;
    difficulty = 1;
    gameScreen = 1;

    // Sets button images
    one_player.set_images("images/buttons/one_player.png",
                          "images/buttons/one_player_hover.png");
    two_player.set_images("images/buttons/two_player.png",
                          "images/buttons/two_player_hover.png");
    quit.set_images("images/buttons/quit.png", "images/buttons/quit_hover.png");
    menu.set_images("images/buttons/menu.png", "images/buttons/menu_hover.png");
    sound.set_images("images/buttons/sound_on.png",
                     "images/buttons/sound_on_hover.png");
    difficulty_b.set_images("images/buttons/medium.png",
                            "images/buttons/medium_hover.png");

    // Assigns bitmaps
    buffer = create_bitmap(300, 300);
    grid = load_png("images/grid.png", NULL);
    img_x = load_png("images/x.png", NULL);
    img_o = load_png("images/o.png", NULL);
    blank = load_png("images/none.png", NULL);
    xwin = load_png("images/xwin.png", NULL);
    owin = load_png("images/owin.png", NULL);
    catsgame = load_png("images/catsgame.png", NULL);
    main_menu = load_png("images/main_menu.png", NULL);

    cursor[0] = load_png("images/cursor.png", NULL);
    cursor[1] = load_png("images/cursor_x.png", NULL);
    cursor[2] = load_png("images/cursor_o.png", NULL);

    selected[0] = load_png("images/selected.png", NULL);
    selected[1] = load_png("images/selected2.png", NULL);
    selected[2] = load_png("images/selected3.png", NULL);
    selected[3] = load_png("images/selected4.png", NULL);

    // Assigns Sounds
    win = load_sample("sfx/win.wav");
    lose = load_sample("sfx/lose.wav");
    cat = load_sample("sfx/catsgame.wav");
    place = load_sample("sfx/place.wav");

    // Seeds random number generator
    srand(time(NULL));

    // Sets button positions
    one_player.set_position(50, 70);
    two_player.set_position(50, 130);
    quit.set_position(50, 190);
    menu.set_position(0, 270);
    sound.set_position(110, 250);
    difficulty_b.set_position(150, 250);
  }
}

// Performs unique one player actions
void gameOne() {
  // Places x or o respectively
  if (turn == 0) {
    if (mouseListener::buttonPressed[1] && gridarray[x][y] == 0) {
      gridarray[x][y] = 1;
      if (soundfx)
        play_sample(place, 255, 122, 1000, 0);
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
        if (!move_made && (gridarray[i][0] == t && gridarray[i][1] == t &&
                               gridarray[i][2] == 0 ||
                           gridarray[i][0] == t && gridarray[i][1] == 0 &&
                               gridarray[i][2] == t ||
                           gridarray[i][0] == 0 && gridarray[i][1] == t &&
                               gridarray[i][2] == t)) {
          if (gridarray[i][0] == 0)
            gridarray[i][0] = 2;
          else if (gridarray[i][1] == 0)
            gridarray[i][1] = 2;
          else if (gridarray[i][2] == 0)
            gridarray[i][2] = 2;
          move_made = true;
          break;
        }
        // Down
        else if (!move_made && (gridarray[0][i] == t && gridarray[1][i] == t &&
                                    gridarray[2][i] == 0 ||
                                gridarray[0][i] == t && gridarray[1][i] == 0 &&
                                    gridarray[2][i] == t ||
                                gridarray[0][i] == 0 && gridarray[1][i] == t &&
                                    gridarray[2][i] == t)) {
          if (gridarray[0][i] == 0)
            gridarray[0][i] = 2;
          else if (gridarray[1][i] == 0)
            gridarray[1][i] = 2;
          else if (gridarray[2][i] == 0)
            gridarray[2][i] = 2;
          move_made = true;
          break;
        }
      }
      // Diagonal top left to bottom right
      if (!move_made && (gridarray[0][0] == t && gridarray[1][1] == t &&
                             gridarray[2][2] == 0 ||
                         gridarray[0][0] == t && gridarray[1][1] == 0 &&
                             gridarray[2][2] == t ||
                         gridarray[0][0] == 0 && gridarray[1][1] == t &&
                             gridarray[2][2] == t)) {
        if (gridarray[0][0] == 0)
          gridarray[0][0] = 2;
        else if (gridarray[1][1] == 0)
          gridarray[1][1] = 2;
        else if (gridarray[2][2] == 0)
          gridarray[2][2] = 2;
        move_made = true;
        break;
      }
      // Diagonal top right to bottom left
      else if (!move_made && (gridarray[2][0] == t && gridarray[1][1] == t &&
                                  gridarray[0][2] == 0 ||
                              gridarray[2][0] == t && gridarray[1][1] == 0 &&
                                  gridarray[0][2] == t ||
                              gridarray[2][0] == 0 && gridarray[1][1] == t &&
                                  gridarray[0][2] == t)) {
        if (gridarray[2][0] == 0)
          gridarray[2][0] = 2;
        else if (gridarray[1][1] == 0)
          gridarray[1][1] = 2;
        else if (gridarray[0][2] == 0)
          gridarray[0][2] = 2;
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
      int random_integer = random(0, 2);
      int random_integer2 = random(0, 2);
      if (gridarray[random_integer][random_integer2] == 0) {
        gridarray[random_integer][random_integer2] = 2;
        move_made = true;
      }
    }
    if (soundfx)
      play_sample(place, 255, 122, 1500, 0);

    if (move_made)
      turn = 0;
  }
}

// Performs unique two player actions
void gameTwo() {
  // Places x or o respectively
  if (mouseListener::buttonPressed[1] && gridarray[x][y] == 0 &&
      !menu.get_hover()) {
    gridarray[x][y] = turn + 1;
    turn = (turn + 1) % 2;
    if (soundfx)
      play_sample(place, 255, 122, 1000, 0);
  }
}

// Performs main game actions
void game() {
  // Listen to mouse
  m_listener.update();

  // Menu
  if (gameScreen == 1) {
    // Checks for mouse press
    if (mouseListener::buttonPressed[1]) {
      if (one_player.get_hover()) {
        gameScreen = 2;
        turn = 0;
        setup(false);
        aexlib::draw::animation::highcolor_fade_out(16);
        draw(false);
        aexlib::draw::animation::highcolor_fade_in(buffer, 16);
      } else if (two_player.get_hover()) {
        gameScreen = 3;
        turn = 0;
        setup(false);
        aexlib::draw::animation::highcolor_fade_out(16);
        draw(false);
        aexlib::draw::animation::highcolor_fade_in(buffer, 16);
      } else if (difficulty_b.get_hover() == true) {
        if (difficulty == 2)
          difficulty_b.set_images("images/buttons/easy.png",
                                  "images/buttons/easy_hover.png");
        else if (difficulty == 0)
          difficulty_b.set_images("images/buttons/medium.png",
                                  "images/buttons/medium_hover.png");
        else if (difficulty == 1)
          difficulty_b.set_images("images/buttons/hard.png",
                                  "images/buttons/hard_hover.png");
        difficulty = (difficulty + 1) % 3;
      } else if (sound.get_hover() == true) {
        if (soundfx == true) {
          soundfx = false;
          sound.set_images("images/buttons/sound_off.png",
                           "images/buttons/sound_off_hover.png");
        } else if (soundfx == false) {
          soundfx = true;
          sound.set_images("images/buttons/sound_on.png",
                           "images/buttons/sound_on_hover.png");
        }
      } else if (quit.get_hover() == true) {
        gameRunning = false;
      }
    }
  }

  // One player and 2 player
  else if (gameScreen == 2 || gameScreen == 3) {
    // Chooses which tile is selected based on mouse position
    x = mouse_x / 100;
    y = mouse_y / 100;

    // Check and perform x winning action
    if (gridarray[0][0] == 1 && gridarray[0][1] == 1 && gridarray[0][2] == 1 ||
        gridarray[1][0] == 1 && gridarray[1][1] == 1 && gridarray[1][2] == 1 ||
        gridarray[2][0] == 1 && gridarray[2][1] == 1 && gridarray[2][2] == 1 ||
        gridarray[0][0] == 1 && gridarray[1][0] == 1 && gridarray[2][0] == 1 ||
        gridarray[0][1] == 1 && gridarray[1][1] == 1 && gridarray[2][1] == 1 ||
        gridarray[0][2] == 1 && gridarray[1][2] == 1 && gridarray[2][2] == 1 ||
        gridarray[0][0] == 1 && gridarray[1][1] == 1 && gridarray[2][2] == 1 ||
        gridarray[2][0] == 1 && gridarray[1][1] == 1 && gridarray[0][2] == 1) {
      if (gridarray[0][0] == 1 && gridarray[0][1] == 1 && gridarray[0][2] == 1)
        aexlib::draw::primitive::thick_line(buffer, 50, 50, 50, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[1][0] == 1 && gridarray[1][1] == 1 && gridarray[1][2] == 1)
        aexlib::draw::primitive::thick_line(buffer, 150, 50, 150, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[2][0] == 1 && gridarray[2][1] == 1 && gridarray[2][2] == 1)
        aexlib::draw::primitive::thick_line(buffer, 250, 50, 250, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][0] == 1 && gridarray[1][0] == 1 && gridarray[2][0] == 1)
        aexlib::draw::primitive::thick_line(buffer, 50, 50, 250, 50, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][1] == 1 && gridarray[1][1] == 1 && gridarray[2][1] == 1)
        aexlib::draw::primitive::thick_line(buffer, 50, 150, 250, 150, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][2] == 1 && gridarray[1][2] == 1 && gridarray[2][2] == 1)
        aexlib::draw::primitive::thick_line(buffer, 50, 250, 250, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][0] == 1 && gridarray[1][1] == 1 && gridarray[2][2] == 1)
        aexlib::draw::primitive::thick_line(buffer, 50, 50, 250, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[2][0] == 1 && gridarray[1][1] == 1 && gridarray[0][2] == 1)
        aexlib::draw::primitive::thick_line(buffer, 250, 50, 50, 250, 10,
                                            makecol(255, 0, 0));

      if (soundfx)
        play_sample(win, 255, 122, 1000, 0);

      draw_trans_sprite(buffer, xwin, 0, 0);
      draw_sprite(screen, buffer, 0, 0);
      rest(2000);
      turn = 0;
      setup(false);
    }

    // Check and perform o winning action
    else if (gridarray[0][0] == 2 && gridarray[0][1] == 2 &&
                 gridarray[0][2] == 2 ||
             gridarray[1][0] == 2 && gridarray[1][1] == 2 &&
                 gridarray[1][2] == 2 ||
             gridarray[2][0] == 2 && gridarray[2][1] == 2 &&
                 gridarray[2][2] == 2 ||
             gridarray[0][0] == 2 && gridarray[1][0] == 2 &&
                 gridarray[2][0] == 2 ||
             gridarray[0][1] == 2 && gridarray[1][1] == 2 &&
                 gridarray[2][1] == 2 ||
             gridarray[0][2] == 2 && gridarray[1][2] == 2 &&
                 gridarray[2][2] == 2 ||
             gridarray[0][0] == 2 && gridarray[1][1] == 2 &&
                 gridarray[2][2] == 2 ||
             gridarray[2][0] == 2 && gridarray[1][1] == 2 &&
                 gridarray[0][2] == 2) {
      if (gridarray[0][0] == 2 && gridarray[0][1] == 2 && gridarray[0][2] == 2)
        aexlib::draw::primitive::thick_line(buffer, 50, 50, 50, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[1][0] == 2 && gridarray[1][1] == 2 && gridarray[1][2] == 2)
        aexlib::draw::primitive::thick_line(buffer, 150, 50, 150, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[2][0] == 2 && gridarray[2][1] == 2 && gridarray[2][2] == 2)
        aexlib::draw::primitive::thick_line(buffer, 250, 50, 250, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][0] == 2 && gridarray[1][0] == 2 && gridarray[2][0] == 2)
        aexlib::draw::primitive::thick_line(buffer, 50, 50, 250, 50, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][1] == 2 && gridarray[1][1] == 2 && gridarray[2][1] == 2)
        aexlib::draw::primitive::thick_line(buffer, 50, 150, 250, 150, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][2] == 2 && gridarray[1][2] == 2 && gridarray[2][2] == 2)
        aexlib::draw::primitive::thick_line(buffer, 50, 250, 250, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[0][0] == 2 && gridarray[1][1] == 2 && gridarray[2][2] == 2)
        aexlib::draw::primitive::thick_line(buffer, 50, 50, 250, 250, 10,
                                            makecol(255, 0, 0));
      if (gridarray[2][0] == 2 && gridarray[1][1] == 2 && gridarray[0][2] == 2)
        aexlib::draw::primitive::thick_line(buffer, 250, 50, 50, 250, 10,
                                            makecol(255, 0, 0));

      if (soundfx)
        play_sample(lose, 255, 122, 1000, 0);
      draw_trans_sprite(buffer, owin, 0, 0);
      draw_sprite(screen, buffer, 0, 0);
      rest(2000);
      turn = 1;
      setup(false);
    }

    // Check and perform cats game action
    else if (gridarray[0][0] != 0 && gridarray[1][0] != 0 &&
             gridarray[2][0] != 0 && gridarray[0][1] != 0 &&
             gridarray[1][1] != 0 && gridarray[2][1] != 0 &&
             gridarray[0][2] != 0 && gridarray[1][2] != 0 &&
             gridarray[2][2] != 0) {
      if (soundfx)
        play_sample(cat, 255, 122, 1000, 0);
      draw_trans_sprite(buffer, catsgame, 0, 0);
      draw_sprite(screen, buffer, 0, 0);
      rest(2000);
      turn = 1;
      setup(false);
    }

    // Runs game function
    if (gameScreen == 2)
      gameOne();
    else if (gameScreen == 3)
      gameTwo();

    // Change selector sprite
    if (key[KEY_S]) {
      selector = (selector + 1) % 4;
      rest(200);
    }

    // Checks for mouse press
    if (mouseListener::buttonPressed[1]) {
      if (menu.get_hover() == true) {
        gameScreen = 1;
        aexlib::draw::animation::highcolor_fade_out(16);
        draw(false);
        aexlib::draw::animation::highcolor_fade_in(buffer, 16);
      }
    }
  }
}

// Draw loop
void draw(bool to_screen) {
  // Allow transparency
  set_alpha_blender();

  // Menu
  if (gameScreen == 1) {
    // Draws grid
    draw_sprite(buffer, grid, 0, 0);

    // Draws tiles on board
    for (int i = 0; i < 4; i++) {
      for (int t = 0; t < 4; t++) {
        if (gridarray[i][t] == 0)
          draw_sprite(buffer, blank, (i)*100, (t)*100);
        else if (gridarray[i][t] == 1)
          draw_sprite(buffer, img_x, (i)*100, (t)*100);
        else if (gridarray[i][t] == 2)
          draw_sprite(buffer, img_o, (i)*100, (t)*100);
      }
    }

    // Draws menu
    draw_trans_sprite(buffer, main_menu, 0, 0);

    // Draws Buttons
    one_player.draw(buffer);
    two_player.draw(buffer);
    quit.draw(buffer);
    sound.draw(buffer);
    difficulty_b.draw(buffer);
  }
  // One player and 2 player
  else if (gameScreen == 2 || gameScreen == 3) {
    // Draws grid
    draw_sprite(buffer, grid, 0, 0);

    // Draws tiles on board
    for (int i = 0; i < 4; i++) {
      for (int t = 0; t < 4; t++) {
        if (gridarray[i][t] == 0)
          draw_sprite(buffer, blank, i * 100, t * 100);
        else if (gridarray[i][t] == 1)
          draw_sprite(buffer, img_x, i * 100, t * 100);
        else if (gridarray[i][t] == 2)
          draw_sprite(buffer, img_o, i * 100, t * 100);
      }
    }

    // Draws selection tile
    draw_sprite(buffer, selected[selector], x * 100, y * 100);

    // Draws Menu Button
    menu.draw(buffer);
  }

  // Draws mouse cursor
  draw_sprite(buffer, cursor[(turn + 1) * (gameScreen != 1)], mouse_x, mouse_y);

  // Draws buffer
  if (to_screen)
    draw_sprite(screen, buffer, 0, 0);
}

// Program starts here
int main() {
  // Setup for first time
  setup(true);

  // Run game loop while game running
  while (!key[KEY_ESC] && gameRunning) {
    game();
    draw(true);
  }

  // Destroy images
  destroy_bitmap(buffer);
  destroy_bitmap(grid);
  destroy_bitmap(img_x);
  destroy_bitmap(img_o);
  destroy_bitmap(blank);
  destroy_bitmap(selected[0]);
  destroy_bitmap(selected[1]);
  destroy_bitmap(selected[2]);
  destroy_bitmap(selected[3]);
  destroy_bitmap(xwin);
  destroy_bitmap(owin);
  destroy_bitmap(catsgame);

  // Destroy sounds
  destroy_sample(win);
  destroy_sample(lose);
  destroy_sample(cat);
  destroy_sample(place);

  // End
  return 0;
}
END_OF_MAIN()
