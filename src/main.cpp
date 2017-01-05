#include <allegro.h>
#include <alpng.h>
#include <math.h>

#include "button.h"

//Creates images
BITMAP* buffer;
BITMAP* grid;
BITMAP* X;
BITMAP* O;
BITMAP* blank;
BITMAP* selected;
BITMAP* xwin;
BITMAP* owin;
BITMAP* catsgame;
BITMAP* cursor;
BITMAP* main_menu;

//Create sounds
SAMPLE* win;
SAMPLE* lose;
SAMPLE* cat;
SAMPLE* place;

//Creates Butons
Button one_player;
Button two_player;
Button quit;
Button menu;
Button sound;
Button difficulty_b;

//Creates variables
int x;
int y;
int turn;
int selector;
int gridimage;
int difficulty;
int mode;
int soundfx;
int gameScreen;
bool gameRunning;

//Array of the game board
int gridarray[3][3]={
  {0,0,0},
  {0,0,0},
  {0,0,0}
};

int selectedarray[3][3]={
  {1,0,0},
  {0,0,0},
  {0,0,0}
};

//Thick Lines
void thick_line(BITMAP *bmp, float x, float y, float x_, float y_,float thickness, int color){
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

//Fade in
void highcolor_fade_in(BITMAP* bmp_orig, int speed){
  BITMAP* bmp_buff;

  if((bmp_buff=create_bitmap(SCREEN_W,SCREEN_H))){
    int a;
    if (speed<=0)speed=16;

    for(a=0;a<256;a+=speed){
      clear(bmp_buff);
      set_trans_blender(0,0,0,a);
      draw_trans_sprite(bmp_buff,bmp_orig,0,0);
      vsync();
      blit(bmp_buff,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    destroy_bitmap(bmp_buff);
  }
  blit(bmp_orig,screen,0,0,0,0,SCREEN_W,SCREEN_H);
}

//Fade out
void highcolor_fade_out(int speed){
  BITMAP* bmp_orig, *bmp_buff;

  if((bmp_orig=create_bitmap(SCREEN_W,SCREEN_H))){
    if((bmp_buff=create_bitmap(SCREEN_W,SCREEN_H))){
      int a;
      blit(screen,bmp_orig,0,0,0,0,SCREEN_W,SCREEN_H);
      if (speed<=0)speed=16;

      for(a=255-speed;a>0;a-=speed){
         clear(bmp_buff);
         set_trans_blender(0,0,0,a);
         draw_trans_sprite(bmp_buff,bmp_orig,0,0);
         vsync();
         blit(bmp_buff,screen,0,0,0,0,SCREEN_W,SCREEN_H);
      }
    destroy_bitmap(bmp_buff);
    }
    destroy_bitmap(bmp_orig);
  }
  rectfill(screen,0,0,SCREEN_W,SCREEN_H,makecol(255,0,0));
}

//Sets up game (bitmaps, sounds, ect.)
void setup(bool first){

  //Assigns variables
  x = 0;
  y = 0;
  selector = 0;
  gridimage = 0;

  //Refreshes game board
  for (int i = 0; i < 4; i++){
    for (int t = 0; t < 4; t++){
      gridarray[i][t] = 0;
    }
  }

  if(first){
    allegro_init();
    alpng_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 300, 300, 0, 0);
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");
    set_window_title("Tic Tac Toe");

    gameRunning = true;

    turn = 0;

    soundfx = true;
    difficulty = 1;
    gameScreen = 0;

    //Sets button images
    one_player.set_images( "images/buttons/one_player.png", "images/buttons/one_player_hover.png");
    two_player.set_images( "images/buttons/two_player.png", "images/buttons/two_player_hover.png");
    quit.set_images( "images/buttons/quit.png", "images/buttons/quit_hover.png");
    menu.set_images( "images/buttons/menu.png", "images/buttons/menu_hover.png");
    sound.set_images( "images/buttons/sound_on.png", "images/buttons/sound_on_hover.png");
    difficulty_b.set_images( "images/buttons/medium.png", "images/buttons/medium_hover.png");

    //Assigns bitmaps
    buffer = create_bitmap( 300, 300);
    grid = load_bitmap( "images/grid.png", NULL);
    X = load_bitmap( "images/x.png", NULL);
    O = load_bitmap( "images/o.png", NULL);
    blank = load_bitmap( "images/none.png", NULL);
    selected = load_bitmap( "images/selected.png", NULL);
    xwin = load_bitmap( "images/xwin.png", NULL);
    owin = load_bitmap( "images/owin.png", NULL);
    catsgame = load_bitmap( "images/catsgame.png", NULL);
    cursor = load_bitmap( "images/cursor.png", NULL);
    main_menu = load_bitmap( "images/main_menu.png", NULL);

    //Assigns Sounds
    win=load_sample( "sfx/win.wav");
    lose=load_sample( "sfx/lose.wav");
    cat=load_sample( "sfx/catsgame.wav");
    place=load_sample( "sfx/place.wav");

    //Creates random number generator
    srand ( time(NULL) );

    //Sets button positions
    one_player.set_position( 50, 70);
    two_player.set_position( 50, 130);
    quit.set_position( 50, 190);
    menu.set_position( 0, 270);
    sound.set_position( 110, 250);
    difficulty_b.set_position( 150, 250);
  }
}

//Performs unique one player actions
void gameOne(){

  //Chooses which tile is selected based on mouse position
  for( int i = 0; i < 3; i++){
    for( int t = 0; t < 3; t++){
      selectedarray[i][t] = 0;
      if( (mouse_x / 100) == i && (mouse_y / 100) == t && !menu.get_hover()){
        selectedarray[i][t] = 1;
        x = i;
        y = t;
      }
    }
  }

  //Places x or o respectively
  if( mouse_b & 1 && gridarray[x][y] == 0){
    if( turn == 0){
      gridarray[x][y] = 1;
      if( soundfx)
        play_sample(place,255,122,1000,0);
      turn = 1;
    }
  }

  //Opponent AI
  if(turn == 1){

    //Ai checks for 3 in line os
    //first row
    if( gridarray[0][0] == 2 && gridarray[0][2] == 2 && gridarray[0][1] == 0 && difficulty > 1){
      gridarray[0][1] = 2;
    }
    else if( gridarray[0][0] == 2 && gridarray[0][1] == 2 && gridarray[0][2] == 0 && difficulty > 1){
      gridarray[0][2] = 2;
    }
    else if( gridarray[0][1] == 2 &&gridarray[0][2] == 2 && gridarray[0][0] == 0 && difficulty > 1){
      gridarray[0][0] = 2;
    }

    //second row
    else if(gridarray[1][0]==2&&gridarray[1][2]==2 &&gridarray[1][1]==0 && difficulty>1){
      gridarray[1][1]=2;
    }
    else if(gridarray[1][0]==2&&gridarray[1][1]==2 &&gridarray[1][2]==0 && difficulty>1){
      gridarray[1][2]=2;
    }
    else if(gridarray[1][1]==2&&gridarray[1][2]==2 &&gridarray[1][0]==0 && difficulty>1){
      gridarray[1][0]=2;
    }

    //third row
    else if(gridarray[2][0]==2&&gridarray[2][2]==2 &&gridarray[2][1]==0 && difficulty>1){
      gridarray[2][1]=2;
    }
    else if(gridarray[2][0]==2&&gridarray[2][1]==2 &&gridarray[2][2]==0 && difficulty>1){
      gridarray[2][2]=2;
    }
    else if(gridarray[2][1]==2&&gridarray[2][2]==2 &&gridarray[2][0]==0 && difficulty>1){
      gridarray[2][0]=2;
    }

    //down left
    else if(gridarray[0][0]==2&&gridarray[2][0]==2 &&gridarray[1][0]==0 && difficulty>1){
      gridarray[1][0]=2;
    }
    else if(gridarray[0][0]==2&&gridarray[1][0]==2 &&gridarray[2][0]==0 && difficulty>1){
      gridarray[2][0]=2;
    }
    else if(gridarray[1][0]==2&&gridarray[2][0]==2 &&gridarray[0][0]==0 && difficulty>1){
      gridarray[0][0]=2;
    }

    //down center
    else if(gridarray[0][1]==2&&gridarray[2][1]==2 &&gridarray[1][1]==0 && difficulty>1){
      gridarray[1][1]=2;
    }
    else if(gridarray[0][1]==2&&gridarray[1][1]==2 &&gridarray[2][1]==0 && difficulty>1){
      gridarray[2][1]=2;
    }
    else if(gridarray[1][0]==2&&gridarray[2][1]==2 &&gridarray[0][1]==0 && difficulty>1){
      gridarray[0][1]=2;
    }

    //down right
    else if(gridarray[0][2]==2&&gridarray[2][2]==2 &&gridarray[1][2]==0 && difficulty>1){
      gridarray[1][2]=2;
    }
    else if(gridarray[0][2]==2&&gridarray[1][2]==2 &&gridarray[2][2]==0 && difficulty>1){
      gridarray[2][2]=2;
    }
    else if(gridarray[1][2]==2&&gridarray[2][2]==2 &&gridarray[0][2]==0 && difficulty>1){
      gridarray[0][2]=2;
    }

    //diagonal top left to bottom right
    else if(gridarray[0][0]==2&&gridarray[2][2]==2 &&gridarray[1][1]==0 && difficulty>1){
      gridarray[1][1]=2;
    }
    else if(gridarray[0][0]==2&&gridarray[1][1]==2 &&gridarray[2][2]==0 && difficulty>1){
      gridarray[2][2]=2;
    }
    else if(gridarray[1][1]==2&&gridarray[2][2]==2 &&gridarray[0][0]==0 && difficulty>1){
      gridarray[0][0]=2;
    }

    //diagonal top right to bottom left
    else if(gridarray[2][0]==2&&gridarray[0][2]==2 &&gridarray[1][1]==0 && difficulty>1){
      gridarray[1][1]=2;
    }
    else if(gridarray[2][0]==2&&gridarray[1][1]==2 &&gridarray[0][2]==0 && difficulty>1){
      gridarray[0][2]=2;
    }
    else if(gridarray[1][1]==2&&gridarray[0][2]==2 &&gridarray[2][0]==0 && difficulty>1){
      gridarray[2][0]=2;
    }



    //Ai checks for 3 in line xs
    //first row
    else if(gridarray[0][0]==1&&gridarray[0][2]==1 &&gridarray[0][1]==0 && difficulty>0){
      gridarray[0][1]=2;
    }
    else if(gridarray[0][0]==1&&gridarray[0][1]==1 &&gridarray[0][2]==0 && difficulty>0){
      gridarray[0][2]=2;
    }
    else if(gridarray[0][1]==1&&gridarray[0][2]==1 &&gridarray[0][0]==0 && difficulty>0){
      gridarray[0][0]=2;
    }

    //second row
    else if(gridarray[1][0]==1&&gridarray[1][2]==1 &&gridarray[1][1]==0 && difficulty>0){
      gridarray[1][1]=2;
    }
    else if(gridarray[1][0]==1&&gridarray[1][1]==1 &&gridarray[1][2]==0 && difficulty>0){
      gridarray[1][2]=2;
    }
    else if(gridarray[1][1]==1&&gridarray[1][2]==1 &&gridarray[1][0]==0 && difficulty>0){
      gridarray[1][0]=2;
    }

    //third row
    else if(gridarray[2][0]==1&&gridarray[2][2]==1 &&gridarray[2][1]==0 && difficulty>0){
      gridarray[2][1]=2;
    }
    else if(gridarray[2][0]==1&&gridarray[2][1]==1 &&gridarray[2][2]==0 && difficulty>0){
      gridarray[2][2]=2;
    }
    else if(gridarray[2][1]==1&&gridarray[2][2]==1 &&gridarray[2][0]==0 && difficulty>0){
      gridarray[2][0]=2;
    }

    //down left
    else if(gridarray[0][0]==1&&gridarray[2][0]==1 &&gridarray[1][0]==0 && difficulty>0){
      gridarray[1][0]=2;
    }
    else if(gridarray[0][0]==1&&gridarray[1][0]==1 &&gridarray[2][0]==0 && difficulty>0){
      gridarray[2][0]=2;
    }
    else if(gridarray[1][0]==1&&gridarray[2][0]==1 &&gridarray[0][0]==0 && difficulty>0){
      gridarray[0][0]=2;
    }

    //down center
    else if(gridarray[0][1]==1&&gridarray[2][1]==1 &&gridarray[1][1]==0 && difficulty>0){
      gridarray[1][1]=2;
    }
    else if(gridarray[0][1]==1&&gridarray[1][1]==1 &&gridarray[2][1]==0 && difficulty>0){
      gridarray[2][1]=2;
    }
    else if(gridarray[1][0]==1&&gridarray[2][1]==1 &&gridarray[0][1]==0 && difficulty>0){
      gridarray[0][1]=2;
    }

    //down right
    else if(gridarray[0][2]==1&&gridarray[2][2]==1 &&gridarray[1][2]==0 && difficulty>0){
      gridarray[1][2]=2;
    }
    else if(gridarray[0][2]==1&&gridarray[1][2]==1 &&gridarray[2][2]==0 && difficulty>0){
      gridarray[2][2]=2;
    }
    else if(gridarray[1][2]==1&&gridarray[2][2]==1 &&gridarray[0][2]==0 && difficulty>0){
      gridarray[0][2]=2;
    }

    //diagonal top left to bottom right
    else if(gridarray[0][0]==1&&gridarray[2][2]==1 &&gridarray[1][1]==0 && difficulty>0){
      gridarray[1][1]=2;
    }
    else if(gridarray[0][0]==1&&gridarray[1][1]==1 &&gridarray[2][2]==0 && difficulty>0){
      gridarray[2][2]=2;
    }
    else if(gridarray[1][1]==1&&gridarray[2][2]==1 &&gridarray[0][0]==0 && difficulty>0){
      gridarray[0][0]=2;
    }

    //diagonal top right to bottom left
    else if(gridarray[2][0]==1&&gridarray[0][2]==1 &&gridarray[1][1]==0 && difficulty>0){
      gridarray[1][1]=2;
    }
    else if(gridarray[2][0]==1&&gridarray[1][1]==1 &&gridarray[0][2]==0 && difficulty>0){
      gridarray[0][2]=2;
    }
    else if(gridarray[1][1]==1&&gridarray[0][2]==1 &&gridarray[2][0]==0 && difficulty>0){
      gridarray[2][0]=2;
    }

    //else randomize place if above checks result in false
    else{
      int highest=3;
      int lowest=0;
      int range=(highest-lowest);
      int random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
      int random_integer2 = lowest+int(range*rand()/(RAND_MAX + 1.0));
      do{
        if(gridarray[random_integer][random_integer2]==0){
          gridarray[random_integer][random_integer2]=2;
          turn=0;
        }
        else{
          random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
          random_integer2 = lowest+int(range*rand()/(RAND_MAX + 1.0));
        }
        if(gridarray[0][0]!=0 && gridarray[0][1]!=0 && gridarray[0][2]!=0&&gridarray[1][0]!=0 && gridarray[1][1]!=0 && gridarray[1][2]!=0&&gridarray[2][0]!=0 && gridarray[2][1]!=0 && gridarray[2][2]!=0){
          break;
        }
      }while(turn!=0);
    }
    if(soundfx==true)
      play_sample(place,255,122,1500,0);
    turn=0;
  }
}

//Performs unique two player actions
void gameTwo(){

  //Chooses which tile is selected based on mouse position
  for( int i=0; i<3; i++){
    for( int t=0; t<3; t++){
      selectedarray[i][t] = 0;
      if( ( mouse_x / 100) == i && (mouse_y / 100) == t){
        selectedarray[i][t] = 1;
        x = i;
        y = t;
      }
    }
  }

  //Places x or o respectively
  if( mouse_b & 1 && gridarray[x][y] == 0 && !menu.get_hover()){
    if( turn == 0){
      gridarray[x][y] = 1;
      if(soundfx)
        play_sample( place, 255, 122, 1000, 0);
      turn = 1;
      cursor = load_bitmap( "images/cursor_o.png", NULL);
    }
    else if(turn==1){
      gridarray[x][y]=2;
      if(soundfx)
        play_sample( place, 255, 122, 1000, 0);
      turn = 0;
      cursor = load_bitmap( "images/cursor_x.png", NULL);
    }
  }
}


//Performs main game actions
void game(){
  set_alpha_blender();

  // Splash
  if(gameScreen==0){
    gameScreen = 1;
  }

  // Menu
  else if(gameScreen == 1){
    //Draws grid
    draw_sprite(buffer,grid,0,0);

    //Draws tiles on board
    for (int i = 0; i < 4; i++){
      for (int t = 0; t < 4; t++){
        if(gridarray[i][t]==0){draw_sprite( buffer, blank, (i)*100, (t)*100);}
        else if(gridarray[i][t]==1){draw_sprite( buffer, X, (i)*100, (t)*100);}
        else if(gridarray[i][t]==2){draw_sprite( buffer, O, (i)*100, (t)*100);}
      }
    }

    //Draws menu
    draw_trans_sprite( buffer, main_menu, 0, 0);

    //Draws Buttons
    one_player.draw(buffer);
    two_player.draw(buffer);
    quit.draw(buffer);
    sound.draw(buffer);
    difficulty_b.draw(buffer);

    //Checks for mouse press
    if( mouse_b & 1){
      if( one_player.get_hover()){
        gameScreen = 2;
        cursor=load_bitmap( "images/cursor_x.png", NULL);
        setup( false);
        highcolor_fade_out( 16);
        highcolor_fade_in( grid, 16);
      }
      else if(two_player.get_hover()){
        gameScreen = 3;
        cursor=load_bitmap( "images/cursor_x.png", NULL);
        setup(false);
        highcolor_fade_out(16);
        highcolor_fade_in(grid, 16);
      }
      else if(difficulty_b.get_hover()==true){
        if(difficulty==2){
          difficulty=0;
          difficulty_b.set_images("images/buttons/easy.png","images/buttons/easy_hover.png");
          rest(160);
        }
        else if(difficulty==0){
          difficulty=1;
          difficulty_b.set_images("images/buttons/medium.png","images/buttons/medium_hover.png");
          rest(160);
        }
        else if(difficulty==1){
          difficulty=2;
          difficulty_b.set_images("images/buttons/hard.png","images/buttons/hard_hover.png");
          rest(160);
        }
      }
      else if(sound.get_hover()==true){
        if(soundfx==true){
          soundfx=false;
          sound.set_images("images/buttons/sound_off.png","images/buttons/sound_off_hover.png");
          rest(160);
        }
        else if(soundfx==false){
          soundfx=true;
          sound.set_images("images/buttons/sound_on.png","images/buttons/sound_on_hover.png");
          rest(160);
        }
      }
      else if(quit.get_hover()==true){
        gameRunning = false;
      }
    }
  }

  // One player and 2 player
  else if(gameScreen==2||gameScreen==3){
    //Check and perform x winning action
    if(gridarray[0][0]==1 && gridarray[0][1]==1 && gridarray[0][2]==1||gridarray[1][0]==1 && gridarray[1][1]==1 && gridarray[1][2]==1||gridarray[2][0]==1 && gridarray[2][1]==1 && gridarray[2][2]==1||gridarray[0][0]==1 && gridarray[1][0]==1 && gridarray[2][0]==1||gridarray[0][1]==1 && gridarray[1][1]==1 && gridarray[2][1]==1||gridarray[0][2]==1 && gridarray[1][2]==1 && gridarray[2][2]==1||gridarray[0][0]==1 && gridarray[1][1]==1 && gridarray[2][2]==1||gridarray[2][0]==1 && gridarray[1][1]==1 && gridarray[0][2]==1){
      if(gridarray[0][0]==1 && gridarray[0][1]==1 && gridarray[0][2]==1)thick_line(buffer, 50, 50, 50, 250, 10, makecol(255,0,0));
      if(gridarray[1][0]==1 && gridarray[1][1]==1 && gridarray[1][2]==1)thick_line(buffer, 150, 50, 150, 250, 10, makecol(255,0,0));
      if(gridarray[2][0]==1 && gridarray[2][1]==1 && gridarray[2][2]==1)thick_line(buffer, 250, 50, 250, 250, 10, makecol(255,0,0));
      if(gridarray[0][0]==1 && gridarray[1][0]==1 && gridarray[2][0]==1)thick_line(buffer, 50, 50, 250, 50, 10, makecol(255,0,0));
      if(gridarray[0][1]==1 && gridarray[1][1]==1 && gridarray[2][1]==1)thick_line(buffer, 50, 150, 250, 150, 10, makecol(255,0,0));
      if(gridarray[0][2]==1 && gridarray[1][2]==1 && gridarray[2][2]==1)thick_line(buffer, 50, 250, 250, 250, 10, makecol(255,0,0));
      if(gridarray[0][0]==1 && gridarray[1][1]==1 && gridarray[2][2]==1)thick_line(buffer, 50, 50, 250, 250, 10, makecol(255,0,0));
      if(gridarray[2][0]==1 && gridarray[1][1]==1 && gridarray[0][2]==1)thick_line(buffer, 250, 50, 50, 250, 10, makecol(255,0,0));

      if(soundfx==true){
        play_sample(win,255,122,1000,0);
      }
      draw_trans_sprite(buffer,xwin,0,0);
      draw_sprite(screen,buffer,0,0);
      rest(2000);
      if(gameScreen == 3){
        cursor = load_bitmap( "images/cursor_x.png", NULL);
      }
      turn = 0;
      setup(false);
    }

    //Check and perform o winning action
    if(gridarray[0][0]==2 && gridarray[0][1]==2 && gridarray[0][2]==2||gridarray[1][0]==2 && gridarray[1][1]==2 && gridarray[1][2]==2||gridarray[2][0]==2 && gridarray[2][1]==2 && gridarray[2][2]==2||gridarray[0][0]==2 && gridarray[1][0]==2 && gridarray[2][0]==2||gridarray[0][1]==2 && gridarray[1][1]==2 && gridarray[2][1]==2||gridarray[0][2]==2 && gridarray[1][2]==2 && gridarray[2][2]==2||gridarray[0][0]==2 && gridarray[1][1]==2 && gridarray[2][2]==2||gridarray[2][0]==2 && gridarray[1][1]==2 && gridarray[0][2]==2){
      if(gridarray[0][0]==2 && gridarray[0][1]==2 && gridarray[0][2]==2)thick_line(buffer, 50, 50, 50, 250, 10, makecol(255,0,0));
      if(gridarray[1][0]==2 && gridarray[1][1]==2 && gridarray[1][2]==2)thick_line(buffer, 150, 50, 150, 250, 10, makecol(255,0,0));
      if(gridarray[2][0]==2 && gridarray[2][1]==2 && gridarray[2][2]==2)thick_line(buffer, 250, 50, 250, 250, 10, makecol(255,0,0));
      if(gridarray[0][0]==2 && gridarray[1][0]==2 && gridarray[2][0]==2)thick_line(buffer, 50, 50, 250, 50, 10, makecol(255,0,0));
      if(gridarray[0][1]==2 && gridarray[1][1]==2 && gridarray[2][1]==2)thick_line(buffer, 50, 150, 250, 150, 10, makecol(255,0,0));
      if(gridarray[0][2]==2 && gridarray[1][2]==2 && gridarray[2][2]==2)thick_line(buffer, 50, 250, 250, 250, 10, makecol(255,0,0));
      if(gridarray[0][0]==2 && gridarray[1][1]==2 && gridarray[2][2]==2)thick_line(buffer, 50, 50, 250, 250, 10, makecol(255,0,0));
      if(gridarray[2][0]==2 && gridarray[1][1]==2 && gridarray[0][2]==2)thick_line(buffer, 250, 50, 50, 250, 10, makecol(255,0,0));

      if(soundfx==true)
        play_sample(lose,255,122,1000,0);
      draw_trans_sprite(buffer,owin,0,0);
      draw_sprite(screen,buffer,0,0);
      rest(2000);
      if(gameScreen == 3){
        cursor = load_bitmap( "images/cursor_o.png", NULL);
      }
      turn = 1;
      setup(false);
    }

    //Check and perform cats game action
    if(gridarray[0][0] !=0 && gridarray[1][0] !=0 && gridarray[2][0] !=0 && gridarray[0][1] !=0&& gridarray[1][1] !=0&& gridarray[2][1] !=0&& gridarray[0][2] !=0&& gridarray[1][2] !=0&& gridarray[2][2] !=0){
      if(soundfx==true)play_sample(cat,255,122,1000,0);
      draw_trans_sprite(buffer,catsgame,0,0);
      draw_sprite(screen,buffer,0,0);
      rest(2000);
      turn = 1;
      setup(false);
    }

    //Runs game function
    if(gameScreen == 2){
      gameOne();
    }
    else if(gameScreen == 3){
      gameTwo();
    }


    //Draws grid
    draw_sprite(buffer,grid,0,0);

    //Draws tiles on board
    for (int i = 0; i < 4; i++){
      for (int t = 0; t < 4; t++){
        if(gridarray[i][t]==0){draw_sprite( buffer, blank, (i)*100, (t)*100);}
        else if(gridarray[i][t]==1){draw_sprite( buffer, X, (i)*100, (t)*100);}
        else if(gridarray[i][t]==2){draw_sprite( buffer, O, (i)*100, (t)*100);}
      }
    }

    //Draws selection tile
    for (int i = 0; i < 4; i++){
      for (int t = 0; t < 4; t++){
        if(selectedarray[i][t]==1){draw_sprite( buffer, selected, (i)*100, (t)*100);}
      }
    }

    //Change selector sprite
    if( key[KEY_S]){
      if(selector==0){
        selected=load_bitmap( "images/selected2.png", NULL);
        selector=1;
        rest(200);
      }
      else if(selector==1){
        selected=load_bitmap( "images/selected3.png", NULL);
        selector=2;
        rest(200);
      }
      else if(selector==2){
        selected=load_bitmap( "images/selected4.png", NULL);
        selector=3;
        rest(200);
      }
      else if(selector==3){
        selected=load_bitmap( "images/selected.png", NULL);
        selector=0;
        rest(200);
      }
    }

    //Draws Menu Button
    menu.draw(buffer);

    //Checks for mouse press
    if(mouse_b&1){
      if(menu.get_hover()==true){
        cursor=load_bitmap("images/cursor.png", NULL);
        gameScreen = 1;
        highcolor_fade_out(16);
        highcolor_fade_in(main_menu, 16);
      }
    }
  }
  //Draws mouse cursor
  draw_sprite(buffer,cursor,mouse_x,mouse_y);

  //Draws buffer
  draw_sprite(screen,buffer,0,0);
}

// Program starts here
int main() {
  // Setup for first time
  setup( true);

  // Run game loop while game running
  while( !key[KEY_ESC] && gameRunning){
    game();
  }

  // Destroy images
  destroy_bitmap( buffer);
  destroy_bitmap( grid);
  destroy_bitmap( X);
  destroy_bitmap( O);
  destroy_bitmap( blank);
  destroy_bitmap( selected);
  destroy_bitmap( xwin);
  destroy_bitmap( owin);
  destroy_bitmap( catsgame);

  // Destroy sounds
  destroy_sample( win);
  destroy_sample( lose);
  destroy_sample( cat);
  destroy_sample( place);

  // End
  return 0;
}
END_OF_MAIN();

