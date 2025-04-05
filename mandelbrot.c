#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <time.h>

 int X_CENTER = 600;
const int Y_CENTER = 300;
const int WIDTH    = 1000;
const int HEIGHT   = 1000;

const int    NUM_POINTS        = 4;
const int    MAX_NUM_ITERATION = 256;
const int    MAX_RADIUS        = 100;
const double SCALE             = 0.004;
const double dx                = 0.004;
const double dy                = 0.004;

void calculating_points  (sf::VertexArray* points);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    sf::VertexArray points(sf::Points, WIDTH * HEIGHT);
    for(int i = 0; i < 20; i++)
    {
        clock_t time_start_calculating = 0;
        clock_t time_end_calculating = 0;

        time_start_calculating = clock();
        calculating_points(&points);
        time_end_calculating   = clock();
        double time_calculating = ((double) (time_end_calculating - time_start_calculating)) / CLOCKS_PER_SEC;
        printf("time calculating %f\n", time_calculating);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) window.close();
            }
            window.clear();
            window.draw(points);
            window.display();
        }
        X_CENTER += 100;
    }
    return 0;
}

void calculating_points(sf::VertexArray* points)
{
    for (int yi = 0; yi < HEIGHT; yi++)
    {
        double Y0 = dy*(yi - Y_CENTER);
        for (int xi = 0; xi < WIDTH; xi += NUM_POINTS)
        {
            double X0 = dx*(xi - X_CENTER);
            double x0[NUM_POINTS] = {X0, X0 + dx, X0 + dx*2, X0 + dx*3};
            double y0[NUM_POINTS] = {Y0,      Y0,        Y0,        Y0};

            double x[NUM_POINTS]             = {}; for(int i = 0; i < NUM_POINTS; i++)  x[i] = x0[i];
            double y[NUM_POINTS]             = {}; for(int i = 0; i < NUM_POINTS; i++)  y[i] = y0[i];

            int output_number[NUM_POINTS] = {};
            for (int counter = 0; counter < MAX_NUM_ITERATION; counter++)
            {
                double x2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  x2[i] = x[i]  *  x[i];
                double xy[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  xy[i] = x[i]  *  y[i];
                double y2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  y2[i] = y[i]  *  y[i];
                double r2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  r2[i] = x2[i] +  y2[i];

                int output_check[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++) if(r2[i] >= MAX_RADIUS) output_check[i] = 1;
                int summ = 0;
                for(int i = 0; i < NUM_POINTS; i++) summ += output_check[i];
                if (summ == NUM_POINTS) break;
                for(int i = 0; i < NUM_POINTS; i++) if(r2[i] <= MAX_RADIUS) output_number[i]++;

                for (int i = 0; i < NUM_POINTS; i++)
                {
                    if (output_check[i] == 0)
                    {
                        x[i] = x2[i] - y2[i] + x0[i];
                        y[i] = 2 * xy[i] + y0[i];
                    }
                }
            }
            for(int i = 0; i < 4; i++)
            {
                (*points)[yi * HEIGHT +xi +i].position = sf::Vector2f(xi + i, yi + i);
                (*points)[yi * HEIGHT +xi +i].color    = sf::Color((sf::Uint8)(21 + output_number[i]*20), 255, (sf::Uint8)(0 + output_number[i]*20));
            }
        }
    }
}

