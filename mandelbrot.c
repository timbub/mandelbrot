#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <time.h>

const int X_CENTER = 600;
const int Y_CENTER = 400;
const int WIDTH    = 800;
const int HEIGHT   = 800;

const int    NUM_POINTS        = 4;
const int    MAX_NUM_ITERATION = 256;
const int    MAX_RADIUS        = 100;
const float SCALE              = 0.004;
const float dx                 = 0.004;
const float dy                 = 0.004;

void calculating_with_pipelining(sf::VertexArray* points);
void calculating_with_intrinsics(sf::VertexArray* points);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    sf::VertexArray points(sf::Points, WIDTH * HEIGHT);
    sf::Font font;
    if(!font.loadFromFile("arial.ttf"))
    {
        fprintf(stderr, "font is not load \n");
    }
    sf::Text fps;
    fps.setFont(font);
    fps.setCharacterSize(20);
    fps.setFillColor (sf::Color::Black);
    fps.setPosition (10, 10);
    sf::Clock fps_clock;

     while (window.isOpen())
     {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        fps_clock.restart();
        calculating_with_pipelining(&points);
        //calculating_with_intrinsics(&points);
        window.clear();
        window.draw(points);
        sf::Time iteration_time = fps_clock.getElapsedTime();
        float fps_value = 1.0 / iteration_time.asSeconds();

        fps.setString("FPS: " + std::to_string(fps_value));
        window.draw(fps);
        window.display();
    }
    return 0;
}

void calculating_with_intrinsics(sf::VertexArray* points)
{
    __m256 index    =  _mm256_setr_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
    __m256 dx_array =  _mm256_set1_ps(dx);
    __m256 offset   =  _mm256_mul_ps(dx_array, index);
    __m256 r2_max   =  _mm256_set1_ps(MAX_RADIUS*MAX_RADIUS);
    for (int yi = 0; yi < HEIGHT; yi++)
    {
        float Y0 = dy*(yi - Y_CENTER);
        __m256 y0 =  _mm256_set1_ps(Y0);
        //float y0[NUM_POINTS] = {Y0, Y0, Y0, Y0};
        for (int xi = 0; xi < WIDTH; xi += 8)
        {
            float X0 = dx*(xi - X_CENTER);
            __m256 X0_array = _mm256_set1_ps(X0);
            __m256 x0 = _mm256_add_ps(X0_array, offset);
            //float x0[NUM_POINTS] = {X0, X0 + dx, X0 + dx*2, X0 + dx*3};

            __m256 y = y0;
            __m256 x = x0;

            //float x[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  x[i] = x0[i];
            //float y[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  y[i] = y0[i];

           alignas(16) float output_number[8] = {};
            __m256 output_number_intrinsics = _mm256_setzero_ps();
            for (int counter = 0; counter < MAX_NUM_ITERATION; counter++)
            {
                //float x2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  x2[i] = x[i]  *  x[i];
                //float xy[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  xy[i] = x[i]  *  y[i];
                //float y2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  y2[i] = y[i]  *  y[i];
                //float r2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  r2[i] = x2[i] +  y2[i];

                __m256 x2 =  _mm256_mul_ps(x, x);
                __m256 xy =  _mm256_mul_ps(x, y);
                __m256 y2 =  _mm256_mul_ps(y, y);
                __m256 r2 =  _mm256_add_ps(x2, y2);

                __m256 cmp_mask   = _mm256_cmp_ps(r2, r2_max, _CMP_LT_OS);
                int check = _mm256_movemask_ps(cmp_mask);
                //printf("check = %d \n", check);
                if(check == 0) break;
                //__m256i ones_array = _mm256_set1_epi32(1);
               //__m256i result     = _mm256_and_si256(int_mask, ones_array);
               __m256 ones_array = _mm256_set1_ps(1);
               __m256 result = _mm256_and_ps(cmp_mask, ones_array);

                output_number_intrinsics = _mm256_add_ps(output_number_intrinsics, result);

                x = _mm256_sub_ps(x2, y2);
                x = _mm256_add_ps(x, x0);
                y = _mm256_add_ps(xy, xy);
                y = _mm256_add_ps(y, y0);

            }
            _mm256_store_ps(output_number, output_number_intrinsics);
            for(int i = 0; i < 8; i++)
            {
                //printf("number %f\n", output_number[i]);
                (*points)[yi * WIDTH +xi +i].position = sf::Vector2f(xi + i, yi);
                (*points)[yi * WIDTH +xi +i].color    = sf::Color((sf::Uint8)(21 + output_number[i]*20), 255, (sf::Uint8)(0 + output_number[i]*20));
            }
        }
    }
}

void calculating_with_pipelining(sf::VertexArray* points)
{
    for (int yi = 0; yi < HEIGHT; yi++)
    {
        float Y0 = dy*(yi - Y_CENTER);
        float y0[NUM_POINTS] = {Y0, Y0, Y0, Y0};
        for (int xi = 0; xi < WIDTH; xi += NUM_POINTS)
        {
            float X0 = dx*(xi - X_CENTER);
            float x0[NUM_POINTS] = {X0, X0 + dx, X0 + dx*2, X0 + dx*3};

            float x[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  x[i] = x0[i];
            float y[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  y[i] = y0[i];

            int output_number[NUM_POINTS] = {};
            for (int counter = 0; counter < MAX_NUM_ITERATION; counter++)
            {
                float x2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  x2[i] = x[i]  *  x[i];
                float xy[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  xy[i] = x[i]  *  y[i];
                float y2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  y2[i] = y[i]  *  y[i];
                float r2[NUM_POINTS] = {}; for(int i = 0; i < NUM_POINTS; i++)  r2[i] = x2[i] +  y2[i];

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


