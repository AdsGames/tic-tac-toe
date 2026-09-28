#pragma once

#include <asw/asw.h>
#include <string>

namespace ui {

// Load a button image from assets/images/buttons
inline asw::Texture button_texture(const std::string& name) {
  return asw::assets::load_texture("assets/images/buttons/" + name + ".png");
}

// Make an image button from assets/images/buttons/<name>.png and
// <name>_hover.png, sized to the image
inline void set_button_images(asw::ui::Button& button,
                              const std::string& name) {
  button.set_images(button_texture(name), button_texture(name + "_hover"));
}

}  // namespace ui
