#pragma once

#include <asw/asw.h>
#include <array>

#include "../button.h"
#include "../globals.h"
#include "./state.h"

class Game : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override;

  void update(float dt) override;

  void draw() override;

  static int players;
  static int difficulty;

 private:
  // Performs unique one player actions
  void gameOne();

  // Performs unique two player actions
  void gameTwo();

  // Creates images
  asw::Texture grid;
  asw::Texture img_x;
  asw::Texture img_o;
  asw::Texture xwin;
  asw::Texture owin;
  asw::Texture catsgame;
  std::array<asw::Texture, 3> cursor;
  asw::Texture main_menu;
  std::array<asw::Texture, 4> selected;

  // Create sounds
  asw::Sample win;
  asw::Sample lose;
  asw::Sample cat;
  asw::Sample place;

  // Creates Butons
  Button menu;

  // Creates variables
  int x;
  int y;
  int turn;
  int selector;

  // Array of the game board
  std::array<std::array<int, 3>, 3> gridarray = {{
      {0, 0, 0},
      {0, 0, 0},
      {0, 0, 0},
  }};
};