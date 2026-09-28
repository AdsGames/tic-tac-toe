#pragma once

#include <asw/asw.h>
#include <array>
#include <cstddef>
#include <string>

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

    // Create buttons. Arrows, Tab, Return, Space and a controller move
    // through them; the hover image shows focus, so there is no focus ring
    gui.root.clear_children();
    gui.ctx.navigation = asw::ui::bind_default_navigation();
    gui.ctx.theme.focus_ring.width = 0;

    auto& one_player = add_button("one_player", 50, 70);
    one_player.on_click = [this]() {
      Game::players = 1;
      manager.set_next_scene(States::Game);
    };

    auto& two_player = add_button("two_player", 50, 130);
    two_player.on_click = [this]() {
      Game::players = 2;
      manager.set_next_scene(States::Game);
    };

    auto& quit = add_button("quit", 50, 190);
    quit.on_click = []() { asw::core::exit(); };

    // Sound toggle, checked while sound is on
    auto& sound = add_button<asw::ui::Checkbox>("sound_off", 110, 250);
    sound.texture_checked = ui::button_texture("sound_on");
    sound.texture_checked_hover = ui::button_texture("sound_on_hover");
    sound.checked = asw::sound::get_sfx_volume() > 0.0F;
    sound.on_change = [](bool on) {
      asw::sound::set_sfx_volume(on ? 1.0F : 0.0F);
    };

    // Difficulty, cycles easy, medium, hard. Left and right move focus to the
    // sound toggle beside it
    static constexpr std::array<const char*, 3> difficulties = {
        "easy", "medium", "hard"};
    auto& difficulty = add_button<asw::ui::Choice>(difficulties[0], 150, 250);
    difficulty.adjust_on_left_right = false;
    for (const auto* name : difficulties) {
      difficulty.images.push_back(ui::button_texture(name));
    }
    difficulty.select(static_cast<std::size_t>(Game::difficulty));
    difficulty.texture_hover =
        ui::button_texture(std::string(difficulties.at(difficulty.index)) +
                           "_hover");
    difficulty.on_change = [&difficulty](std::size_t index) {
      Game::difficulty = static_cast<int>(index);
      difficulty.texture_hover =
          ui::button_texture(std::string(difficulties.at(index)) + "_hover");
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
  // Add an image button at a position
  template <typename T = asw::ui::Button>
  T& add_button(const std::string& name, float x, float y) {
    auto& button = gui.root.add_child<T>();
    ui::set_button_images(button, name);
    button.transform.position = asw::Vec2f(x, y);
    return button;
  }

  asw::ui::Root gui;

  asw::Texture main_menu;
  asw::Texture grid;
};
