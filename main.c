/*       DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                   Version 2, December 2004


Everyone is permitted to copy and distribute verbatim or modified
copies of this license document, and changing it is allowed as long
as the name is changed.

           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

 0. You just DO WHAT THE FUCK YOU WANT TO.


    main.c

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "includes_global.h"
#include "keyboard.h"

#include "pi-data.h"

int main(int argc, char **argv)
{
  SDL_Init(SDL_INIT_VIDEO); // init video

  if (TTF_Init() == -1)
  {
    //fprintf(stderr, "Error SDL TTF_Init error : %s\n", TTF_GetError());
    printf("Error SDL TTF_Init error : \n");
    return EXIT_FAILURE;
  }

  SDL_Window *window; //main window
  //SDL_Surface *surface;   //master surface to blit to
  SDL_Renderer *renderer; //master renderer, if rendering used

  window = SDL_CreateWindow("piHole stats",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WINDOW_SIZE_W,
                            WINDOW_SIZE_H,
                            SDL_WINDOW_SHOWN);
  if (0 == window)
  {
    printf("ERROR! Failed to create window: %s\n", SDL_GetError());
    exit(-1);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (0 == renderer)
  {
    printf("ERROR! Failed to create renderer: %s\n", SDL_GetError());
    exit(-1);
  }

  char response[10000];
  //int res;
  //res = getSummary(response);
  //printf("%s \n %d\n", response, res);

  //res = getOver10Mins(response);
  //printf("%s \n %d\n", response, res);

  drawSummary(window, renderer);
  drawPlotOver10Mins(window, renderer);

  int iters = 0;
  while (1)
  {
    iters++;
    if (iters == 11)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      drawSummary(window, renderer);
      drawPlotOver10Mins(window, renderer);
      iters = 0;
    }
    //printf("iters = %d\n", iters);

    SDL_RenderPresent(renderer); // copy to screen
    updateInput();
    usleep(1000000);
  }

  return 0;
};
