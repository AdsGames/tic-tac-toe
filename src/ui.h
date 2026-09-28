#pragma once

#include <asw/asw.h>
#include <string>

namespace ui {

// Set a button's image and hover image from assets/images/buttons, and size
// the button to the image
inline void set_button_images(asw::ui::Button& button,
                              const std::string& name) {
  const std::string path = "assets/images/buttons/" + name;
  button.texture_hover = asw::assets::load_texture(path + "_hover.png");
  button.set_texture(asw::assets::load_texture(path + ".png"), true);
}

// Add an image-only button at a position. The buttons are mouse-only: ASW
// focuses the first focusable widget and draws the hover image while
// focused, so a focusable button would always look hovered.
template <typename T = asw::ui::Button>
T& add_image_button(asw::ui::Widget& parent,
                    const std::string& name,
                    float x,
                    float y) {
  auto& button = parent.add_child<T>();
  button.draw_background = false;
  button.focusable = false;
  set_button_images(button, name);
  button.transform.position = asw::Vec2f(x, y);
  return button;
}

// Clear a UI root and size it to the screen, with no background
inline void reset_root(asw::ui::Root& root) {
  root.root.children.clear();
  root.root.bg = asw::color::transparent;
  const auto size = asw::display::get_logical_size();
  root.set_size(static_cast<float>(size.x), static_cast<float>(size.y));
}

}  // namespace ui
