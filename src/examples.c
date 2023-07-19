#include "joplin.c"
#include "math.h"

#define PI 3.14159265358979323846

#define SPIRAL_CIRCLES_OUTPUT "./examples_build/spiral_circles.ppm"
#define CONCENTRIC_RECTS_OUTPUT "./examples_build/concentric_rects.ppm"
#define PERPENDECULAR_LINES_OUTPUT "./examples_build/perpendecular_lines.ppm"

// In order to build following examples you need to
// remove or comment out the main method in joplin.c

void spiral_circles() {
    time_t start = time(NULL);
    fill_image(BLUE_COLOR);

    Point center = {.x = IMAGE_WIDTH / 2, .y = IMAGE_HEIGHT / 2};
    double a = 1.514;
    double b = 1.823;
    int x = IMAGE_WIDTH / 2, y = IMAGE_HEIGHT / 2;

    for (int i = 0; i <= 1000 * PI; i++) {
        center.x = x + (a + b * i) * cos(i);
        center.y = y + (a + b * i) * sin(i);
        draw_circle(center, 30 + i / 100, palette[rand() % sizeof(palette) / sizeof(Color32)], 1, true);
    }

    render_image(SPIRAL_CIRCLES_OUTPUT);
    printf("Time spent (%s): %llds\n", __func__, time(NULL) - start);
}


void concentric_rects() {
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

    render_image(CONCENTRIC_RECTS_OUTPUT);
    printf("Time spent (%s): %llds\n", __func__, time(NULL) - start);
}

void perpendecular_lines() {
    time_t start = time(NULL);
    fill_image(BLUEISH_COLOR);
    srand(time(NULL));

    Point a = {.x = IMAGE_WIDTH / 2, .y = IMAGE_HEIGHT / 2};
    Point b = {.x = 0, .y = IMAGE_HEIGHT / 2};
    int x = 0, y = 0, step = 60, stroke = 30;
    while (x < IMAGE_WIDTH * 4) {
        a.x = 0;
        a.y = y;
        b.x = x;
        b.y = 0;
        draw_line(a, b, palette[rand() % sizeof(palette) / sizeof(Color32)], stroke);

        x += step;
        y += step;
    }

    x = 0;
    y = IMAGE_HEIGHT;
    while (x < IMAGE_WIDTH * 4) {
        a.x = 0;
        a.y = y;
        b.x = x;
        b.y = IMAGE_HEIGHT;
        draw_line(a, b, palette[rand() % sizeof(palette) / sizeof(Color32)], stroke);

        x += step;
        y -= step;
    }

    render_image(PERPENDECULAR_LINES_OUTPUT);
    printf("Time spent (%s): %llds\n", __func__, time(NULL) - start);
}

int main(void) {
    spiral_circles();
    concentric_rects();
    perpendecular_lines();
}