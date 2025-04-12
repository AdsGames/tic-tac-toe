#pragma once

#include <asw/asw.h>

#include "../button.h"
#include "../globals.h"
#include "./game.h"
#include "./state.h"

class Menu : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override {
    // Sets button images
    one_player.set_images("assets/images/buttons/one_player.png",
                          "assets/images/buttons/one_player_hover.png");
    two_player.set_images("assets/images/buttons/two_player.png",
                          "assets/images/buttons/two_player_hover.png");
    quit.set_images("assets/images/buttons/quit.png",
                    "assets/images/buttons/quit_hover.png");
    set_sound_images();
    set_difficulty_images();

    // Load sprites
    grid = asw::assets::loadTexture("assets/images/grid.png");
    main_menu = asw::assets::loadTexture("assets/images/main_menu.png");

    // Set positions
    one_player.set_position(50, 70);
    two_player.set_position(50, 130);
    quit.set_position(50, 190);
    sound.set_position(110, 250);
    difficulty_b.set_position(150, 250);
  }

  void update(float dt) override {
    // Checks for mouse press
    if (one_player.is_clicked()) {
      Game::players = 1;
      sceneManager.setNextScene(States::Game);
    } else if (two_player.is_clicked()) {
      Game::players = 2;
      sceneManager.setNextScene(States::Game);
    } else if (difficulty_b.is_clicked()) {
      Game::difficulty = (Game::difficulty + 1) % 3;
      set_difficulty_images();
    } else if (sound.is_clicked()) {
      soundfx = !soundfx;
      set_sound_images();
    } else if (quit.is_clicked()) {
      asw::core::exit = true;
    }
  }

  void draw() override {
    // Draws grid
    asw::draw::sprite(grid, asw::Vec2<float>(0, 0));

    // Draws menu
    asw::draw::sprite(main_menu, asw::Vec2<float>(0, 0));

    // Draws Buttons
    one_player.draw();
    two_player.draw();
    quit.draw();
    sound.draw();
    difficulty_b.draw();
  }

 private:
  void set_difficulty_images() {
    if (Game::difficulty == 0) {
      difficulty_b.set_images("assets/images/buttons/easy.png",
                              "assets/images/buttons/easy_hover.png");
    } else if (Game::difficulty == 1) {
      difficulty_b.set_images("assets/images/buttons/medium.png",
                              "assets/images/buttons/medium_hover.png");
    } else if (Game::difficulty == 2) {
      difficulty_b.set_images("assets/images/buttons/hard.png",
                              "assets/images/buttons/hard_hover.png");
    }
  }

  void set_sound_images() {
    if (soundfx) {
      sound.set_images("assets/images/buttons/sound_on.png",
                       "assets/images/buttons/sound_on_hover.png");
    } else {
      sound.set_images("assets/images/buttons/sound_off.png",
                       "assets/images/buttons/sound_off_hover.png");
    }
  }

  Button one_player;
  Button two_player;
  Button quit;
  Button sound;
  Button difficulty_b;

  asw::Texture main_menu;
  asw::Texture grid;
};