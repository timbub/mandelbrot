#ifndef __MANDEL__
#define __MANDEL__
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <string.h>

int X_CENTER = 600;
int Y_CENTER = 400;
const int WIDTH    = 800;
const int HEIGHT   = 800;

const char* BASE_MODE   = "base";
const char* INTRIN_MODE = "intrinsics";
const char* UNROLL_MODE = "unroll";

const float  CPU_FREQ_GHz      = 2.4;

const int    NUM_POINTS        = 4;
const int    MAX_NUM_ITERATION = 100;
const int    MAX_RADIUS        = 100;
const float SCALE              = 0.004;
float dx                       = 0.004;
float dy                       = 0.004;

struct sfml_graphics
{
    sf::RenderWindow window;
    sf::Image        image;;
    sf::Texture      texture;
    sf::Sprite       sprite;
    sf::Font         font;
    sf::Text         fps;


};

void calculating_with_unroll(sf::Image* image);
void calculating_with_intrinsics(sf::Image* image);
void calculating_base(sf::Image* image);
void processing_window(struct sfml_graphics* sfml_info, int argc, char* argv[]);
void keyboard_processing(sf::Event* event, sf::RenderWindow* window);
void set_color(sf::Image* image, int x, int y, int n);
#endif
