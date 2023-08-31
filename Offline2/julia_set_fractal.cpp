#include <cmath>
#include "bitmap_image.hpp"

void julia_set_fractal(){
   bitmap_image fractal(600,400);

   fractal.clear();

   const unsigned int max_iterations = 300;

   const double cr = -0.70000;
   const double ci =  0.27015;

   double prevr, previ;

   for (unsigned int y = 0; y < fractal.height(); ++y)
   {
      for (unsigned int x = 0; x < fractal.width(); ++x)
      {
         double nextr = 1.5 * (2.0 * x / fractal.width () - 1.0);
         double nexti =       (2.0 * y / fractal.height() - 1.0);

         for (unsigned int i = 0; i < max_iterations; ++i)
         {
            prevr = nextr;
            previ = nexti;

            nextr =     prevr * prevr - previ * previ + cr;
            nexti = 2 * prevr * previ + ci;

            if (((nextr * nextr) + (nexti * nexti)) > 4)
            {
               rgb_t c = hsv_colormap[static_cast<int>((1000.0 * i) / max_iterations)];

               fractal.set_pixel(x, y, c);

               break;
            }
         }
      }
   }

   fractal.save_image("julia_set.bmp");
}


int main(){
    bitmap_image fractal(600,400);
    fractal.clear();

    rgb_t c = {255,0,0} ;

    for(unsigned int i =0;i<400;i++) 
        fractal.set_pixel(i,i,c) ;

/*
   for (unsigned int y = 0; y < fractal.height(); ++y){
      for (unsigned int x = 0; x < fractal.width(); ++x){
               rgb_t c ;
               c.red = 255; 
               c.blue = 0 ;
               c.green = 0 ;
               fractal.set_pixel(x, y, c);
               break;
         }
      } 
    */
   fractal.save_image("julia_set.bmp");
}