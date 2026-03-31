#include "button.h"

Button::Button() {
  transform.set_position(0, 0);
  transform.set_size(10, 10);
}

void Button::set_images(const std::string& image1, const std::string& image2) {
  // Set new images
  images[0] = asw::assets::load_texture(image1);
  images[1] = asw::assets::load_texture(image2);

  transform.size = asw::util::get_texture_size(images[0]);
}

bool Button::get_hover() const {
  const auto& mouse = asw::input::get_mouse();
  return transform.contains(mouse.position.x, mouse.position.y);
}

bool Button::is_clicked() const {
  return get_hover() &&
         asw::input::get_mouse_button_down(asw::input::MouseButton::Left);
}

void Button::set_position(int x, int y) {
  transform.position.x = x;
  transform.position.y = y;
}

int Button::get_x() const {
  return transform.position.x;
}

int Button::get_y() const {
  return transform.position.y;
}

void Button::draw() const {
  // Check hover state
  if (get_hover()) {
    asw::draw::sprite(images[1], transform.position);

  } else {
    asw::draw::sprite(images[0], transform.position);
  }
}
