#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
  Display *display = XOpenDisplay(NULL);
  if (!display)
    err(2, "Error opening display.\n");

  int screen = DefaultScreen(display);

  int const W = 640, H = 480;

  Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0,
                                      0, W, H, 1, BlackPixel(display, screen),
                                      BlackPixel(display, screen));
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  int pixdat[W * H];
  XImage *image = XCreateImage(display, DefaultVisual(display, screen),
                               DefaultDepth(display, screen), ZPixmap, 0,
                               (char *)pixdat, W, H, 32, 0);

  if (!image)
    err(1, "Failed to create XImage.\n");

  srand(time(NULL));

  for (;;) {
    XEvent event;
    if (XPending(display) && XNextEvent(display, &event) == 0) {
      if (event.type == KeyPress)
        break;
    }

    for (int y = 0; y < H; y++) {
      for (int x = 0; x < W; x++) {
        unsigned long pixel_color = (rand() % 0xffffff) | 0xff000000;
        XPutPixel(image, x, y, pixel_color);
      }
    }

    GC gc = DefaultGC(display, screen);
    XPutImage(display, window, gc, image, 0, 0, 0, 0, W, H);
    usleep(30000);
  }
  image->data = NULL;
  XDestroyImage(image);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
