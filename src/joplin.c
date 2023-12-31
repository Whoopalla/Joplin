#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) ((x) > 0) ? (x) : ((~x) + 1)

#define RED_COLOR 0xFF0000FF
#define BLUEISH_COLOR 0xFF625b04
#define BLUE_COLOR 0xFFFF0000
#define WHITE_COLOR 0xFFFFFFFF

#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080
#define OUTPUT_FILE_PPM "output.ppm"

typedef uint32_t Color32;

typedef struct Point {
    int x, y;
} Point;

Color32 palette[] = {0x91c4f2, 0x8ca0d7, 0x9d79bc, 0xa14da0, 0x7e1f86, 0x231651, 0x4dccbd, 0x2374ab, 0xff8484};

Color32 image[IMAGE_HEIGHT][IMAGE_WIDTH];

int sqr_distance(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool is_within_image(int x, int y) {
    return y >= 0 && y < IMAGE_HEIGHT && x >= 0 && x < IMAGE_WIDTH;
}

void draw_if_within_image(int x, int y, Color32 color) {
    if (is_within_image(x, y)) {
        image[y][x] = color;
    }
}

static void plotline_low(Point a, Point b, Color32 color) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int d = 2 * dy - dx;
    int y = a.y;

    for (int x = a.x; x <= b.x; x++) {
        if (is_within_image(x, y)) {
            image[y][x] = color;
        }
        if (d > 0) {
            y += yi;
            d += 2 * (dy - dx);
        } else {
            d += 2 * dy;
        }
    }
}

static void plotline_high(Point a, Point b, Color32 color) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int xi = 1;
    if (dy < 0) {
        xi = -1;
        dx = -dx;
    }
    int d = 2 * dx - dy;
    int x = a.x;

    for (int y = a.y; y <= b.y; y++) {
        if (is_within_image(x, y)) {
            image[y][x] = color;
        }
        if (d > 0) {
            x += xi;
            d += 2 * (dx - dy);
        } else {
            d += 2 * dx;
        }
    }
}

void draw_line(Point a, Point b, Color32 color, size_t stroke) {
    if (a.x == b.x && a.y == b.y) return;
    for (size_t s = 0; s <= stroke; s++) {
        if (s > 0) {
            draw_line((Point){.x = a.x--, .y = a.y}, (Point){.x = b.x--, .y = b.y}, color, 0);
        }

        if (ABS(a.y - b.y) < ABS(a.x - b.x)) {
            if (a.x > b.x) {
                plotline_low(b, a, color);
            } else {
                plotline_low(a, b, color);
            }
        } else {
            if (a.y > b.y) {
                plotline_high(b, a, color);
            } else {
                plotline_high(a, b, color);
            }
        }
    }
}

void draw_circle(Point center, size_t r, Color32 color, size_t stroke, bool fill) {
    int ty = center.y - r;
    int by = center.y + r;
    int lx = center.x - r;
    int rx = center.x + r;

    for (int s = 0; s <= (int)stroke; s++) {
        for (int y = ty - s; y <= by + s; y++) {
            if (is_within_image(0, y)) {
                for (int x = lx - s; x <= rx + s; x++) {
                    if (fill) {
                        if ((size_t)sqr_distance(center.x, center.y, x, y) < ((r + s) * (r + s)) - s) {
                            if (is_within_image(x, y)) {
                                image[y][x] = color;
                            }
                        }
                    } else {
                        if ((size_t)sqr_distance(center.x, center.y, x, y) - ((r + s) * (r + s)) < 2 * (r + s) + 1) {
                            if (is_within_image(x, y)) {
                                image[y][x] = color;
                            }
                        }
                    }
                }
            }
        }
    }
}

void draw_rectangle(Point center, size_t width, size_t height, Color32 color, size_t stroke, bool fill) {
    int ty = center.y - height / 2 - height % 2 + 1;
    int by = center.y + height / 2;
    int lx = center.x - width / 2 - width % 2 + 1;
    int rx = center.x + width / 2;

    for (size_t s = 0; s <= stroke; s++) {
        for (int x = lx - (int)s; x <= rx + (int)s; x++) {
            if (is_within_image(x, ty - s)) {
                image[ty - s][x] = color;
            }
            if (is_within_image(x, by + s)) {
                image[by + s][x] = color;
            }
        }

        for (int y = ty - (int)s; y <= by + (int)s; y++) {
            if (is_within_image(lx - s, y)) {
                image[y][lx - s] = color;
            }
            if (is_within_image(rx + s, y)) {
                image[y][rx + s] = color;
            }
        }
    }

    if (fill) {
        for (int y = ty; y <= by; y++) {
            for (int x = lx; x <= rx; x++) {
                if (is_within_image(x, y)) {
                    image[y][x] = color;
                }
            }
        }
    }
}

void fill_image(Color32 color) {
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            image[y][x] = color;
        }
    }
}

void render_image(char *filepath) {
    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open the file. %s\n", strerror(errno));
        exit(1);
    }
    int wresult = fprintf(file, "P6 %d %d 255 ", IMAGE_WIDTH, IMAGE_HEIGHT);
    if (wresult <= 0) {
        fprintf(stderr, "Error while writing to file. %s\n", strerror(errno));
        exit(1);
    }

    int closed;
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            Color32 pixel = image[y][x];
            uint8_t color_bytes[3] = {
                (pixel & 0x0000FF) >> 8 * 0,
                (pixel & 0x00FF00) >> 8 * 1,
                (pixel & 0xFF0000) >> 8 * 2,
            };
            wresult = fwrite(color_bytes, sizeof(color_bytes), 1, file);
            if (wresult <= 0) {
                fprintf(stderr, "Error while writing to file. %s\n", strerror(errno));
                closed = fclose(file);
                exit(1);
            }
        }
    }

    closed = fclose(file);
    if (closed != 0) {
        fprintf(stderr, "Couldn't close the file. %s\n", strerror(errno));
        exit(1);
    }
}

int main(void) {
    time_t start = time(NULL);
    fill_image(BLUEISH_COLOR);
    srand(time(NULL));

    Point center = {.x = 0, .y = 0};
    Color32 color;

    

    for (int x = 240; x < IMAGE_WIDTH; x += 360) {
        for (int y = 180; y < IMAGE_HEIGHT; y += 360) {
            center.x = x;
            center.y = y;
            color = palette[rand() % sizeof(palette) / sizeof(Color32)];
            for (int j = 1; j < 20; j++) {
                draw_rectangle(center, j * 15, j * 15, color, 6, false);
            }
        }
    }

    render_image(OUTPUT_FILE_PPM);
    printf("Time spent: %llds\n", time(NULL) - start);
    return 0;
}