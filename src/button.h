/*
 * Button Class
 * Allan Legemaate
 * 31/12/2016
 * Redone to be more intuitive
 */
#pragma once

#include <asw/asw.h>
#include <array>

class Button {
 public:
  Button();

  void set_images(const std::string& image1, const std::string& image2);

  bool get_hover() const;

  bool is_clicked() const;

  void draw() const;

  void set_position(int newX, int newY);

  int get_x() const;
  int get_y() const;

 private:
  asw::Quad<float> transform;

  std::array<asw::Texture, 2> images;
};
