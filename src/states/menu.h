#pragma once

#include <asw/asw.h>

#include "../ui.h"
#include "./game.h"
#include "./state.h"

class Menu : public asw::scene::Scene<States> {
 public:
  using asw::scene::Scene<States>::Scene;

  void init() override {
    // Load sprites
    grid = asw::assets::load_texture("assets/images/grid.png");

    main_menu = asw::assets::load_texture("assets/images/main_menu.png");

    // Create buttons
    ui::reset_root(gui);

    auto& one_player = ui::add_image_button(gui.root, "one_player", 50, 70);
    one_player.on_click = [this]() {
      Game::players = 1;
      manager.set_next_scene(States::Game);
    };

    auto& two_player = ui::add_image_button(gui.root, "two_player", 50, 130);
    two_player.on_click = [this]() {
      Game::players = 2;
      manager.set_next_scene(States::Game);
    };

    auto& quit = ui::add_image_button(gui.root, "quit", 50, 190);
    quit.on_click = []() { asw::core::exit(); };

    // Sound toggle, checked while sound is on
    auto& sound = ui::add_image_button<asw::ui::Checkbox>(gui.root,
                                                          "sound_off", 110, 250);
    sound.texture_checked =
        asw::assets::load_texture("assets/images/buttons/sound_on.png");
    sound.texture_checked_hover =
        asw::assets::load_texture("assets/images/buttons/sound_on_hover.png");
    sound.checked = asw::sound::get_sfx_volume() > 0.0F;
    sound.on_change = [](bool on) {
      asw::sound::set_sfx_volume(on ? 1.0F : 0.0F);
    };

    difficulty_b = &ui::add_image_button(gui.root, difficulty_name(), 150, 250);
    difficulty_b->on_click = [this]() {
      Game::difficulty = (Game::difficulty + 1) % 3;
      ui::set_button_images(*difficulty_b, difficulty_name());
    };
  }

  void update(float /*dt*/) override { gui.update(); }

  void draw() override {
    // Draws grid
    asw::draw::sprite(grid, asw::Vec2f(0, 0));

    // Draws menu
    asw::draw::sprite(main_menu, asw::Vec2f(0, 0));

    // Draws Buttons
    gui.draw();
  }

 private:
  static const char* difficulty_name() {
    if (Game::difficulty == 0) {
      return "easy";
    }
    if (Game::difficulty == 1) {
      return "medium";
    }
    return "hard";
  }

  asw::ui::Root gui;
  asw::ui::Button* difficulty_b = nullptr;

  asw::Texture main_menu;
  asw::Texture grid;
};
