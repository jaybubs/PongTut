internal void render_background()
{
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = x * x + y * y;
		}
	}
}

internal void clear_screen(u32 color)
{
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);
	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void draw_arena_borders(float arena_x, float arena_y, u32 color)
  //drawing individual squares instead of overlaying one square on top of another is more machine efficient
{
  //transform x,y into actual pixel coords
  arena_x *= render_state.height * render_scale;
  arena_y *= render_state.height * render_scale;

  int x0 = (int)((float)render_state.width * .5f - arena_x);
  int y0 = (int)((float)render_state.height * .5f - arena_y);

  int x1 = (int)((float)render_state.width * .5f + arena_x);
  int y1 = (int)((float)render_state.height * .5f + arena_y);

  draw_rect_in_pixels(0, 0, render_state.width, y0, color);
  draw_rect_in_pixels(0, y1, x1, render_state.height, color);
  draw_rect_in_pixels(0, y0, x0, y1, color);
  draw_rect_in_pixels(x1, y0, render_state.width, render_state.height, color);


}

internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color)
{
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

  //make 0,0 the screen centre instead of bottom left

  x += render_state.width / 2.f;
  y += render_state.height / 2.f;


	//change to pixels and center
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;
	
	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

  //ensure that zero gets drawn, to effectively init the while loop
	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x, y + size * 2.f, half_size, half_size, color);
			draw_rect(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x + size, y + size, half_size, half_size, color);
			draw_rect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x - half_size, y, size, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - size, y + size, half_size, 1.5f * size, color);
			draw_rect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x - size, y + size, half_size, half_size, color);
			draw_rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x + half_size, y, size, half_size, color);
			draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x, y + size * 2.f, half_size, half_size, color);
			draw_rect(x, y - size * 2.f, half_size, half_size, color);
			draw_rect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x - half_size, y, size, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	}
}
