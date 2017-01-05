/*
 * Button Class
 * Allan Legemaate
 * 31/12/2016
 * Redone to be more intuitive
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <allegro.h>
#include <string>

class Button{
  public:
    Button();
    ~Button();

    void set_images( std::string image1, std::string image2);

    bool get_hover();

    void draw(BITMAP* tempBitmap);

    void set_position( int newX, int newY);

    void set_type(int newType);
    void set_value(int newValue);

    int get_x();
    int get_y();
  private:
    int button_height;
    int button_width;

    int x;
    int y;

    BITMAP *images[2];
};

#endif
