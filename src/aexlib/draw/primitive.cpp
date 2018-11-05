/*
 * Additional Primitive Drawing Functionality for Allegro 4
 * Allan Legemaate
 * 05/11/2018
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <allegro.h>
#include <math.h>

namespace aexlib {
  namespace draw {
    namespace primitive {
      // Thick Lines
      void thick_line(BITMAP *bmp, float x, float y, float x_, float y_,float thickness, int color) {
        float dx = x - x_;
        float dy = y - y_;
        float d = sqrtf(dx * dx + dy * dy);
        if (!d)
          return;

        int v[4 * 2];

        /* left up */
        v[0] = x - thickness * dy / d;
        v[1] = y + thickness * dx / d;
        /* right up */
        v[2] = x + thickness * dy / d;
        v[3] = y - thickness * dx / d;
        /* right down */
        v[4] = x_ + thickness * dy / d;
        v[5] = y_ - thickness * dx / d;
        /* left down */
        v[6] = x_ - thickness * dy / d;
        v[7] = y_ + thickness * dx / d;

        polygon(bmp, 4, v, color);
      }
    }
  }
}

#endif
