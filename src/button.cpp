#include "button.h"

Button::Button() {
  transform.setPosition(0, 0);
  transform.setSize(10, 10);
}

void Button::set_images(const std::string& image1, const std::string& image2) {
  // Set new images
  images[0] = asw::assets::loadTexture(image1);
  images[1] = asw::assets::loadTexture(image2);

  transform.size = asw::util::getTextureSize(images[0]);
}

bool Button::get_hover() const {
  return transform.contains(asw::input::mouse.x, asw::input::mouse.y);
}

bool Button::is_clicked() const {
  return get_hover() &&
         asw::input::wasButtonPressed(asw::input::MouseButton::LEFT);
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
