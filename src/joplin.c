#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) (x) > 0 ? (x) : ((~x) + 1)

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

void draw_line(Point a, Point b, Color32 color) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int d = 2 * dy - dx;
    int y = a.y;

    for (int x = MIN(a.x, b.x); x <= MAX(a.x, b.x); x++) {
        if (is_within_image(x, y)) {
            image[y][x] = color;
        }
        if (d > 0) {
            y += yi;
            d += 2 * (dy - dx);
        }
        d += 2 * dy;
    }
}

void draw_circle(Point center, size_t r, Color32 color, size_t stroke, bool fill) {
    int ty = center.y - r;
    int by = center.y + r;
    int lx = center.x - r;
    int rx = center.x + r;

    for (int s = 0; s <= stroke; s++) {
        for (int y = ty - s; y <= by + s; y++) {
            if (is_within_image(0, y)) {
                for (int x = lx - s; x <= rx + s; x++) {
                    if (fill) {
                        if (sqr_distance(center.x, center.y, x, y) < ((r + s) * (r + s)) - s) {
                            if (is_within_image(x, y)) {
                                image[y][x] = color;
                            }
                        }
                    } else {
                        if (sqr_distance(center.x, center.y, x, y) - ((r + s) * (r + s)) < 2 * (r + s) + 1) {
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
        for (int x = lx - s + 1; x <= rx + s; x++) {
            if (is_within_image(x, ty - s) && is_within_image(x, by + s)) {
                image[ty - s][x] = color;
                image[by + s][x] = color;
            }
        }

        for (int y = ty - s + 1; y <= by + s; y++) {
            if (is_within_image(lx - s, y) && is_within_image(rx + s, y)) {
                image[y][lx - s] = color;
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
    FILE *file = fopen(OUTPUT_FILE_PPM, "wb");
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

int main(int argc, char **argv) {
    time_t start = time(NULL);
    fill_image(BLUEISH_COLOR);
    srand(time(NULL));

    Point center = {.x = IMAGE_WIDTH / 2, .y = IMAGE_HEIGHT / 2};
    Point b = {.x = 0, .y = IMAGE_HEIGHT};

    draw_line(center, b, RED_COLOR);

    render_image(OUTPUT_FILE_PPM);
    time_t end = time(NULL);
    printf("Time spent: %llds\n", end - start);
    return 0;
}