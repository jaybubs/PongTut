#define is_down(b) input->buttons[b].is_down
#define is_pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define is_released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

/* float player1_pos_x = 0.f; */
float player1_pos, player1_vel;
float player2_pos, player2_vel;
float player_half_size_x = 2.5, player_half_size_y = 8;
float arena_half_size_x = 85, arena_half_size_y = 45;
float ball_half_size = 1;
float ball_pos_x, ball_pos_y, ball_vel_x = 100, ball_vel_y;

int player1_score, player2_score;

internal void simulate_player(float *pos, float *vel, float *acc, float dt)
{
  *acc -= *vel * 10.f;
  *pos = *pos + *vel * dt + *acc * dt * dt * 0.5f;
  *vel = *vel + *acc * dt;

  if (*pos + player_half_size_y > arena_half_size_y) {
    *pos = arena_half_size_y - player_half_size_y;
    *vel *= -0; //0 will stop at the wall, -1 will reverse direction, -2 fling out, etc
  } else if (*pos - player_half_size_y < -arena_half_size_y) {
    *pos = -arena_half_size_y + player_half_size_y;
    *vel *= -0; //0 will stop at the wall, -1 will reverse direction, -2 fling out, etc
  }
}

//collision detection
internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y)
{
  return  (p1x + hs1x > p2x - hs2x &&
      p1x - hs1x < p2x + hs2x &&
      p1y + hs1y > p2y - hs2y &&
      p1y + hs1y < p2y + hs2y);

}

enum Gamemode {
  GM_MENU,
  GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void simulate_game(Input* input, float dt)
{
  //draw the arena
  draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);
  draw_arena_borders(arena_half_size_x, arena_half_size_y, 0xff5500);

  if (current_gamemode == GM_GAMEPLAY) {

    if (is_pressed(BUTTON_ESC)) {
      current_gamemode = GM_MENU;
    }
    float player1_acc = 0.f; //accn
    if (!enemy_is_ai) {
      if (is_down(BUTTON_UP)) player1_acc += 2000;
      if (is_down(BUTTON_DOWN)) player1_acc -= 2000;
    } else {
      player1_acc = (ball_pos_y - player1_pos)*500;
      if (player1_acc > 1600) {
        player1_acc = 1600;
      }
      if (player1_acc < -1600) {
        player1_acc = -1600;
      }
    }
    float player2_acc = 0.f; //accn
    if (is_down(BUTTON_1UP)) player2_acc += 2000;
    if (is_down(BUTTON_1DN)) player2_acc -= 2000;
    /* if (is_down(BUTTON_LEFT)) player1_pos_x -= speed * dt; */
    /* if (is_down(BUTTON_RIGHT)) player1_pos_x += speed * dt; */
    /* draw_rect(0.50, 0.50, .10, .10, 0x00ff22); */

    simulate_player(&player1_pos, &player1_vel, &player1_acc, dt);
    simulate_player(&player2_pos, &player2_vel, &player2_acc, dt);
    //simple 1D collision detection

    //simulate ball, no need to create an object
    {
      //ball physics, no acceleration to prevent erratic movements, collision physics slightly more involved
      ball_pos_x += ball_vel_x * dt;
      ball_pos_y += ball_vel_y * dt;

      //need collision condition for colliding with the actual player rectangle, not just x position
      if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 80, player1_pos, player_half_size_x, player_half_size_y)
         ) {
        ball_pos_x = 80 - player_half_size_x - ball_half_size;
        ball_vel_x *= -1;
        //change the y vel based on where it hit the player relative to players centre and players vertical vel
        ball_vel_y = (ball_pos_y - player1_pos) * 2 + player1_vel * .9f;
      } else if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, -80, player2_pos, player_half_size_x, player_half_size_y)
          ) {
        ball_pos_x = -80 + player_half_size_x + ball_half_size;
        ball_vel_x *= -1;
        ball_vel_y = (ball_pos_y - player2_pos) * 2 + player2_vel * .9f;
      }

      //collision with arena top and bottom
      if (ball_pos_y + ball_half_size > arena_half_size_y) {
        ball_pos_y = arena_half_size_y - ball_half_size;
        ball_vel_y *= -1;
      } else if (ball_pos_y - ball_half_size < -arena_half_size_y) {
        ball_pos_y = -arena_half_size_y + ball_half_size;
        ball_vel_y *= -1;
      }
      //collide with arena left and right and reset ball
      if (ball_pos_x + ball_half_size > arena_half_size_x) {
        ball_pos_x = 0;
        ball_pos_y = 0;
        ball_vel_x = -100;
        ball_vel_y = 0;
        player1_score++;
      } else if (ball_pos_x - ball_half_size < -arena_half_size_x) {
        ball_pos_x = 0;
        ball_pos_y = 0;
        ball_vel_x = 100;
        ball_vel_y = 0;
        player2_score++;
      }
    }

    draw_number(player1_score, -10, 40, 1.f, 0xbbffbb);
    draw_number(player2_score, 10, 40, 1.f, 0xbbffbb);

    draw_rect(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 0xffffff);

    draw_rect(80, player1_pos, player_half_size_x, player_half_size_y, 0xbb00bb);
    draw_rect(-80, player2_pos, player_half_size_x, player_half_size_y, 0x00bb00);
  } else { 
    //game menu
    /* if (is_pressed(BUTTON_ESC)) { */
    /*   DestroyWindow(hWnd); */
    /* } */
  if (is_pressed(BUTTON_ESC)) {
    running = false;
  }
    if (is_pressed(BUTTON_LEFT) || is_pressed(BUTTON_RIGHT)) {
      hot_button = !hot_button;
    }


    if (is_pressed(BUTTON_ENTER)) {
      current_gamemode = GM_GAMEPLAY;
      enemy_is_ai = hot_button ? 0 : 1;
    }

    draw_text("FUCKING BONG", -70, 40, 2, 0xff0000);


    if (hot_button == 0) {
      draw_text("SINGLE PLAYER", -80, -10, 1, 0xff0000);
      draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
    } else {
      draw_text("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
      draw_text("MULTIPLAYER", 20, -10, 1, 0xff0000);
    }
  }

}
