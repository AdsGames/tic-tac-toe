/*
 * Screen Animation Functionality for Allegro 4
 * Allan Legemaate
 * 05/11/2018
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <allegro.h>

namespace aexlib {
  namespace draw {
    namespace animation {
      // Fade in
      void highcolor_fade_in(BITMAP *bmp_orig, int speed) {
        // Create bitmaps
        BITMAP *bmp_buff;
        if (!(bmp_buff = create_bitmap(SCREEN_W,SCREEN_H))) {
          return;
        }

        int a;
        if (speed <= 0)
          speed = 1;

        // Fade
        for (a = 0; a < 255 - speed; a += speed) {
          clear(bmp_buff);
          set_trans_blender(0, 0, 0, a);
          draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
          vsync();
          blit(bmp_buff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }

        // Clean up
        destroy_bitmap(bmp_buff);
      }

      // Fade out
      void highcolor_fade_out(int speed) {
        // Create bitmaps
        BITMAP *bmp_orig, *bmp_buff;
        if (!(bmp_orig = create_bitmap(SCREEN_W,SCREEN_H)) ||
            !(bmp_buff = create_bitmap(SCREEN_W,SCREEN_H))) {
          return;
        }

        int a;
        blit(screen, bmp_orig, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        if (speed <= 0)
          speed = 16;

        // Fade
        for (a = 255; a > speed; a -= speed) {
           clear(bmp_buff);
           set_trans_blender(0, 0, 0, a);
           draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
           vsync();
           blit(bmp_buff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }

        // Clean up
        destroy_bitmap(bmp_buff);
        destroy_bitmap(bmp_orig);
      }
    }
  }
}

#endif
