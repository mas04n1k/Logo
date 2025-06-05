#define _USE_MATH_DEFINES
#include "CImg.h"
#include <cmath>
using namespace cimg_library;

int main() {
    const int imgW = 600, imgH = 600;
    CImg<unsigned char> canvas(imgW, imgH, 1, 3, 255);

    // Цвета
    unsigned char bg[] = { 18, 78, 135 };
    unsigned char pool[] = { 37, 103, 181 };
    unsigned char main_blue[] = { 86, 142, 200 };
    unsigned char alt_blue[] = { 85, 144, 201 };
    unsigned char deep_blue[] = { 0, 68, 130 };
    unsigned char accent_blue[] = { 0, 89, 156 };
    unsigned char white[] = { 255, 255, 255 };

    // Фон
    canvas.draw_rectangle(0, 0, imgW, imgH, bg);

    // Многоугольный blob
    CImg<float> shape(8, 2, 1, 1);
    float coords[][2] = {
        {0.10f, 0.25f}, {0.28f, 0.10f}, {0.65f, 0.08f}, {0.92f, 0.26f},
        {0.95f, 0.75f}, {0.78f, 0.97f}, {0.33f, 0.94f}, {0.14f, 0.78f}
    };
    for (int i = 0; i < 8; ++i) {
        shape(i, 0) = imgW * coords[i][0];
        shape(i, 1) = imgH * coords[i][1];
    }
    canvas.draw_polygon(shape, pool, 0.4f);

    // Угловые точки
    struct Circle {
        float x, y, r, a;
    } points[] = {
        {0.06f, 0.08f, 10, 0.4f}, {0.94f, 0.10f, 16, 0.35f},
        {0.10f, 0.94f, 23, 0.3f}, {0.92f, 0.88f, 32, 0.25f}
    };
    for (auto& c : points)
        canvas.draw_circle(imgW * c.x, imgH * c.y, c.r, alt_blue, c.a);

    // Шестиугольник
    int cx = imgW / 2, cy = imgH / 2, R = 180;
    float hex[6][2];
    for (int i = 0; i < 6; ++i) {
        float theta = 2 * M_PI * i / 6 + M_PI / 6;
        hex[i][0] = cx + R * std::cos(theta);
        hex[i][1] = cy + R * std::sin(theta);
    }

    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        const unsigned char* color = (i < 1) ? deep_blue :
            (i < 4) ? main_blue :
            (i == 5) ? accent_blue : main_blue;
        canvas.draw_triangle(cx, cy,
            (int)hex[i][0], (int)hex[i][1],
            (int)hex[next][0], (int)hex[next][1], color);
    }

    // Стрелки
    int offset = 80, gap = 20;
    int arrowLeftX = (int)(hex[2][0] - offset);
    canvas.draw_triangle(arrowLeftX, cy,
        arrowLeftX + gap, cy - 80,
        arrowLeftX + gap, cy + 80, alt_blue);
    canvas.draw_triangle(arrowLeftX + 15, cy,
        arrowLeftX + gap, cy - 60,
        arrowLeftX + gap, cy + 60, bg, 0.0f);

    int arrowRightX = (int)(hex[5][0] + offset);
    canvas.draw_triangle(arrowRightX, cy,
        arrowRightX - gap, cy - 80,
        arrowRightX - gap, cy + 80, alt_blue);
    canvas.draw_triangle(arrowRightX - 15, cy,
        arrowRightX - gap, cy - 60,
        arrowRightX - gap, cy + 60, bg, 0.0f);

    // Буква "C"
    int c_x = cx - 80, c_y = cy;
    int outer = 70, inner = 40;
    canvas.draw_circle(c_x, c_y, outer, white);
    canvas.draw_circle(c_x, c_y, inner, main_blue);
    int cutX = c_x - outer / 10 + 10;
    canvas.draw_rectangle(cutX, c_y - outer + 50, c_x + outer, c_y + outer - 50, main_blue);

    // Символы "++"
    int plSize = 35, plGap = 85, thick = 15;
    int p1x = c_x + outer + 50, p1y = c_y;
    int p2x = p1x + plGap;

    auto draw_plus = [&](int x, int y) {
        for (int i = -thick / 2; i <= thick / 2; ++i) {
            canvas.draw_line(x - plSize, y + i, x + plSize, y + i, white);
            canvas.draw_line(x + i, y - plSize, x + i, y + plSize, white);
        }
        };
    draw_plus(p1x, p1y);
    draw_plus(p2x, p1y);

    // Вывод
    CImgDisplay win(canvas, "C++ Logo");
    while (!win.is_closed()) {
        win.wait();
    }
    canvas.save("С++.jpg");
    return 0;
}
