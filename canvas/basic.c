#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <err.h>
#include <math.h>
#include <png.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

typedef struct canvas Canvas;
typedef union pixel Pixel;

union pixel {
  struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
  };
  uint32_t value;
};

#define ToPixel(val)                                                           \
  (Pixel) { .value = val }

const Pixel BLACK = ToPixel(0xff000000), RED = ToPixel(0xffff0000),
            YELLOW = ToPixel(0xffffff00), GREEN = ToPixel(0xff00ff00),
            CYAN = ToPixel(0xff00ffff), BLUE = ToPixel(0xff0000ff),
            MAGENTA = ToPixel(0xffff00ff), WHITE = ToPixel(0xffffffff);

struct canvas {
  Pixel *pixels;
  int width;
  int height;
};

struct vec {
  float x, y;
};

struct ball {
  float velocity, radius;
  struct vec direction, position;
};

typedef unsigned char uint8_t;

enum : uint8_t { OFF, RUNNING, PAUSED };

struct box {
  struct vec offset, dim;
  struct ball ball;
  uint8_t settings;
};

void simulate(struct box *box) {
  clock_t lapse = clock();
  float const mag = hypotf(box->ball.direction.x, box->ball.direction.y);
  box->ball.direction.x /= mag;
  box->ball.direction.y /= mag;
  float const min_x = box->offset.x + box->ball.radius,
              min_y = box->offset.y + box->ball.radius,
              max_x = box->offset.x + box->dim.x - box->ball.radius,
              max_y = box->offset.y + box->dim.y - box->ball.radius;
  while (box->settings & RUNNING) {
    float period = (clock() - lapse) / 1e4;
    box->ball.position.x += box->ball.direction.x * box->ball.velocity * period;
    box->ball.position.y += box->ball.direction.y * box->ball.velocity * period;
    while (box->settings & PAUSED)
      thrd_yield();
    lapse = clock();
    if (box->ball.position.x <= min_x) {
      box->ball.position.x = min_x;
      box->ball.direction.x = -box->ball.direction.x;
    } else if (box->ball.position.x >= max_x) {
      box->ball.position.x = max_x;
      box->ball.direction.x = -box->ball.direction.x;
    }
    if (box->ball.position.y <= min_y) {
      box->ball.position.y = min_y;
      box->ball.direction.y = -box->ball.direction.y;
    } else if (box->ball.position.y >= max_y) {
      box->ball.position.y = max_y;
      box->ball.direction.y = -box->ball.direction.y;
    }
  }
}

void draw(Pixel *old, Pixel new);

void clear(Canvas *canvas, Pixel color) {
  for (int x = 0; x < canvas->width; x++)
    for (int y = 0; y < canvas->height; y++)
      draw(&canvas->pixels[x * canvas->height + y], color);
}

void draw(Pixel *old, Pixel new) {
  float const alpha = new.alpha / 255.f, ialpha = 1 - alpha;
  old->red = round(old->red * ialpha + new.red *alpha);
  old->green = round(old->green * ialpha + new.green *alpha);
  old->blue = round(old->blue * ialpha + new.blue *alpha);
}

void draw_vertical(Canvas *canvas, int x, int y1, int y2, Pixel color) {
  if (y1 > y2) {
    int y = y1;
    y1 = y2;
    y2 = y;
  }
  for (; y1 <= y2; y1++)
    draw(&canvas->pixels[y1 * canvas->width + x], color);
}

void draw_horizontal(Canvas *canvas, int x1, int x2, int y, Pixel color) {
  if (x1 > x2) {
    int x = x1;
    x1 = x2;
    x2 = x;
  }
  for (; x1 <= x2; x1++)
    draw(&canvas->pixels[y * canvas->width + x1], color);
}

void draw_line(Canvas *canvas, int x1, int y1, int x2, int y2, Pixel color) {
  if (x1 == x2)
    return draw_vertical(canvas, x1, y1, y2, color);
  else if (y1 == y2)
    return draw_horizontal(canvas, x1, x2, y1, color);
  else if (x1 > x2) {
    int t = x1;
    x1 = x2;
    x2 = t;
    t = y1;
    y1 = y2;
    y2 = t;
  }
  float const m = (y2 - y1) / (float)(x2 - x1);
  for (int x = x1; x <= x2; x++) {
    float const y = m * (x - x1) + y1;
    draw(&canvas->pixels[(int)floor(y) * canvas->width + x], color);
    draw(&canvas->pixels[(int)ceil(y) * canvas->width + x], color);
  }
}

void fill_rect(Canvas *canvas, int x1, int y1, int x2, int y2, Pixel color) {
  int t;
  if (x1 > x2) {
    t = x1;
    x1 = x2;
    x2 = t;
  }
  if (y1 > y2) {
    t = y1;
    y1 = y2;
    y2 = t;
  }
  for (int t = x1; t <= x2; t++)
    draw_vertical(canvas, t, y1, y2, color);
}

void draw_rect(Canvas *canvas, int x1, int y1, int x2, int y2, Pixel color) {
  int t;
  if (x1 > x2) {
    t = x1;
    x1 = x2;
    x2 = t;
  }
  if (y1 > y2) {
    t = y1;
    y1 = y2;
    y2 = t;
  }
  for (int t = x1; t <= x2; t++) {
    draw(&canvas->pixels[y1 * canvas->width + t], color);
    draw(&canvas->pixels[y2 * canvas->width + t], color);
  }
  for (t = y1; t <= y2; t++) {
    draw(&canvas->pixels[t * canvas->width + x1], color);
    draw(&canvas->pixels[t * canvas->width + x2], color);
  }
}

void fill_circle(Canvas *const canvas, int const x, int const y, int const r,
                 Pixel const color) {
  for (int x0 = x - r; x0 <= x + r; x0++) {
    float _y = sqrt(r * r - (x - x0) * (x - x0));
    draw_vertical(canvas, x0, floor(y - _y), ceil(y + _y), color);
  }
}

void draw_circle(Canvas *const canvas, int const x, int const y, int const r,
                 Pixel const color) {
  int const R = r / M_SQRT2;
  for (int x0 = x - R; x0 <= x + R; x0++) {
    float _y = sqrt(r * r - (x - x0) * (x - x0));
    draw(&canvas->pixels[(int)floor(y + _y) * canvas->width + x0], color);
    draw(&canvas->pixels[(int)ceil(y - _y) * canvas->width + x0], color);
  }
  for (int y0 = y - R; y0 <= y + R; y0++) {
    float _x = sqrt(r * r - (y - y0) * (y - y0));
    draw(&canvas->pixels[y0 * canvas->width + (int)floor(x + _x)], color);
    draw(&canvas->pixels[y0 * canvas->width + (int)ceil(x - _x)], color);
  }
}

void fill_triangle(Canvas *canvas, int x1, int y1, int x2, int y2, int x3,
                   int y3, Pixel color) {
  int t;
  if (x2 > x3) {
    t = x2;
    x2 = x3;
    x3 = t;
    t = y2;
    y2 = y3;
    y3 = t;
  } else if (x2 == x3)
    draw_vertical(canvas, x2, y2, y3, color);
  if (x1 > x2) {
    t = x1;
    x1 = x2;
    x2 = t;
    t = y1;
    y1 = y2;
    y2 = t;
  } else if (x1 == x2)
    draw_vertical(canvas, x1, y1, y2, color);
  float const m12 = (y2 - y1) / (float)(x2 - x1),
              m13 = (y3 - y1) / (float)(x3 - x1),
              m23 = (y3 - y2) / (float)(x3 - x2);
  for (t = x1; t < x2; t++) {
    float const y12 = m12 * (t - x1) + y1, y13 = m13 * (t - x1) + y1;
    draw_vertical(canvas, t, y12, y13, color);
  }
  for (t = x2; t <= x3; t++) {
    float const y23 = m23 * (t - x2) + y2, y13 = m13 * (t - x2) + y1;
    draw_vertical(canvas, t, y23, y13, color);
  }
}

void draw_triangle(Canvas *canvas, int x1, int y1, int x2, int y2, int x3,
                   int y3, Pixel color) {
  draw_line(canvas, x1, y1, x2, y2, color);
  draw_line(canvas, x1, y1, x3, y3, color);
  draw_line(canvas, x2, y2, x3, y3, color);
}

int run_simulator(void *box) {
  simulate((struct box *)box);
  return 0;
}

void draw_oncanvas(Canvas *canvas, struct box *box) {
  clear(canvas, ToPixel(0x15000000));
  /*clear(canvas, BLACK);*/
  draw_rect(canvas, box->offset.x, box->offset.y, box->offset.x + box->dim.x,
            box->offset.y + box->dim.y, GREEN);
  draw_rect(canvas, box->ball.position.x - box->ball.radius,
            box->ball.position.y - box->ball.radius,
            box->ball.position.x + box->ball.radius,
            box->ball.position.y + box->ball.radius, WHITE);
  /*fill_circle(canvas, box->ball.position.x, box->ball.position.y,*/
  /*            box->ball.radius, WHITE);*/
  float const radius = box->ball.radius * sqrtf(0.75f),
              height = box->ball.radius / 2;
  fill_triangle(
      canvas, box->ball.position.x, box->ball.position.y - box->ball.radius,
      box->ball.position.x - radius, box->ball.position.y + height,
      box->ball.position.x + radius, box->ball.position.y + height, WHITE);
  draw_line(canvas, box->ball.position.x, box->ball.position.y,
            box->ball.position.x, box->ball.position.y - box->ball.radius,
            GREEN);
  draw_line(canvas, box->ball.position.x, box->ball.position.y,
            box->ball.position.x - radius, box->ball.position.y + height,
            GREEN);
  draw_line(canvas, box->ball.position.x, box->ball.position.y,
            box->ball.position.x + radius, box->ball.position.y + height,
            GREEN);
  draw_circle(canvas, box->ball.position.x, box->ball.position.y,
              box->ball.radius, RED);
  /*fill_triangle(canvas, box->ball.position.x - box->ball.radius,*/
  /*              box->ball.position.y + box->ball.radius,*/
  /*              box->ball.position.x - box->ball.radius,*/
  /*              box->ball.position.y - box->ball.radius,*/
  /*              box->ball.position.x + box->ball.radius,*/
  /*              box->ball.position.y + box->ball.radius, BLUE);*/
  /*int const center = (int)box->ball.position.y * canvas->width +*/
  /*                      (int)box->ball.position.x;*/
  /*draw(&canvas->pixels[center], RED);*/
}

int main() {
  Display *display = XOpenDisplay(NULL);
  if (!display)
    err(1, "Error opening display\n");
  int screen = DefaultScreen(display);
  int const W = 512, H = 512;
  Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0,
                                      0, W, H, 1, BlackPixel(display, screen),
                                      BlackPixel(display, screen));
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  Pixel pixbuffer[W * H];
  Canvas canvas = {.pixels = pixbuffer, .height = H, .width = W};

  XImage *image = XCreateImage(display, DefaultVisual(display, screen),
                               DefaultDepth(display, screen), ZPixmap, 0,
                               (char *)pixbuffer, W, H, 32, 0);
  if (!image)
    err(2, "Failed to create image\n");

  struct box box = {.dim = {.x = W - 50, .y = H - 50},
                    .offset = {.x = 25, .y = 25},
                    .ball = {.position = {.x = 65, .y = 65},
                             .direction = {.x = 2, .y = 3},
                             .velocity = 10,
                             .radius = 60},
                    .settings = RUNNING};
  thrd_t simulator_thread;
  if (thrd_create(&simulator_thread, run_simulator, &box))
    err(3, "Failed lauching simulator\n");

  for (;;) {
    XEvent event;
    if (XPending(display) && !XNextEvent(display, &event)) {
      if (event.type == KeyPress)
        switch (event.xkey.keycode) {
        case 24:
          goto outside;
        case 33:
          printf("State: box(ball})\n");
          break;
        case 65:
          box.settings = box.settings & PAUSED ? box.settings & ~PAUSED
                                               : box.settings | PAUSED;
          break;
        default:
          printf("Received: %d\n", event.xkey.keycode);
        }
    }
    draw_oncanvas(&canvas, &box);
    GC gc = DefaultGC(display, screen);
    XPutImage(display, window, gc, image, 0, 0, 0, 0, W, H);
    usleep(16e3);
  }
outside:
  box.settings = OFF;
  thrd_join(simulator_thread, NULL);
  image->data = NULL;

  XDestroyImage(image);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  /*
  int const W = 500, H = 350;
  Pixel pixels[W * H];
  Canvas canvas = {.pixels = pixels, .width = W, .height = H};
  clear(&canvas, BLACK);
  draw_rect(&canvas, 25, 25, W - 25, H - 25, WHITE);
  draw_circle(&canvas, 150, 75, 50, WHITE);
  png_image image = {.height = H,
                     .width = W,
                     .format = PNG_FORMAT_RGBA,
                     .version = PNG_IMAGE_VERSION};
  if (png_image_write_to_file(&image, "canvas.png", 0, pixels, 0, NULL) == 0)
    printf("Error writing canvas: %s\n", image.message);
*/
}
