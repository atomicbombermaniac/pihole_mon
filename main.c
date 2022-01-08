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
    // fprintf(stderr, "Error SDL TTF_Init error : %s\n", TTF_GetError());
    fprintf(stderr,"Error SDL TTF_Init error : \n");
    return EXIT_FAILURE;
  }

  SDL_Window *window; // main window
  // SDL_Surface *surface;   //master surface to blit to
  SDL_Renderer *renderer; // master renderer, if rendering used

  window = SDL_CreateWindow("piHole stats",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WINDOW_SIZE_W,
                            WINDOW_SIZE_H,
                            SDL_WINDOW_SHOWN);
  if (0 == window)
  {
    fprintf(stderr,"ERROR! Failed to create window: %s\n", SDL_GetError());
    exit(-1);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (0 == renderer)
  {
    fprintf(stderr,"ERROR! Failed to create renderer: %s\n", SDL_GetError());
    exit(-1);
  }

  char summary_json[5000];
  char over10min_json[5000];
  int ret;

  ret = getSummary(summary_json);
  if (!ret)
  {
    fprintf(stderr,"failed to get summary! %d\n", ret);
  }

  ret = getOver10Mins(over10min_json);
  if (!ret)
  {
    fprintf(stderr,"failed to get over10min data! %d\n", ret);
  }

  drawSummary(window, renderer, summary_json);
  drawPlotOver10Mins(window, renderer, over10min_json);

  int iters = 0;
  while (1)
  {
    iters++;
    if (iters == 120) //query the server every 2 minutes
    {
      ret = getSummary(summary_json);
      if (0 > ret)
      {
        fprintf(stdout,"failed to get summary! %d\n", ret);
      }

      usleep(100000);

      ret = getOver10Mins(over10min_json);
      if (0 > ret)
      {
       fprintf(stdout,"failed to get over10min data! %d\n", ret);
      }
      iters = 0;
    }
    // printf("iters = %d\n", iters);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    drawSummary(window, renderer, summary_json);
    drawPlotOver10Mins(window, renderer, over10min_json);

    SDL_RenderPresent(renderer); // copy to screen
    updateInput();
    usleep(1000000);
  }

  return 0;
};
