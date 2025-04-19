# **Mandelbrot set visualization**
Using C and SFML to visualize Mandelbrot using different optimizations

## Algorithm

1. The color of an image pixel depends on the number `N` (`output_number` in code).
2. The number `N` is the number of steps for which the **point** will go beyond the circle. If the point does not go beyond `N = 100` steps, then the calculating stops
3. The coordinates of the **point** are calculated using the formulas (`x[0]`, `y[0]` are the coordinates of the pixel being calculated):
* `x[i] = x[i - 1]^2 - y[i - 1]^2 + x[0]`
* `y[i] = 2 * x[i - 1] * y[i - 1] + y[0]`
4. SFML library calculates color in RGB format, my formula is:
  `(21 + output_number[i]*30), 0, (sf::Uint8)(0 + output_number[i]*30)`

## Compilation  
You need GCC compiler, make tool, SFML library

Example of installing (Linux) the SFML library (others are similar):
``` bash
sudo apt install libsfml-dev
```
Write ```make``` for create ```mandelbrot``` file

## Start
``` bash
./mandelbrot (options)
```
## Avaliable options:
* **base**

Uses just 1 pixel processing per loop
<details>
<summary> Code </summary>

```bash
./mandelbrot --mode fast --width 1920 --height 1080
```
</details> 

* **unroll**
  
Processes 4 pixels at a time in one iteration of the loop
* **intrinsics**
  
Uses AVX (Advanced Vector Extensions) intrinsics to calculate 8 pixels in parallel using 256-bit registers. 

| version                            |  FPS |
|------------------------------------|------|
| `Base`                             | 11   |                                       
| `Base`       -O3                   | 16   |  
| `Unroll`                           | 6    |                                       
| `Unroll`     -O3                   | 40   |
| `Intrinsics`                       | 30   |                                          
| `Intrinsics` -O3                   | 120  |

![Mandelbrot Picture1](mandelbrot_image/m1.png)

![Mandelbrot Picture2](mandelbrot_image/m2.png)

![Mandelbrot Picture3](mandelbrot_image/m3.png)
