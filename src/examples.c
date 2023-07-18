#include "joplin.c"
#define PI 3.14159265358979323846

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
        draw_circle(center, 30 + i / 100, palette[rand() % sizeof(palette) / sizeof(Color32)],1, true);
    }

    render_image(OUTPUT_FILE_PPM);
}

void concentric_rects() {
    time_t start = time(NULL);
    fill_image(BLUE_COLOR);
    srand(time(NULL));

    for (int i = 0; i < IMAGE_WIDTH; i++) {
        Point center = {.x = IMAGE_WIDTH / 2, .y = IMAGE_HEIGHT / 2};

        draw_rectangle(center, 16 * i, 9 * i, palette[rand() % (sizeof(palette) / sizeof(Color32))], 9, false);
    }

    render_image(OUTPUT_FILE_PPM);
    time_t end = time(NULL);
    printf("Time spent: %llds\n", end - start);
}