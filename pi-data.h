#ifndef _PI_DATA_H_
#define _PI_DATA_H_

#include "includes_global.h"


int getSummary(char * resp);

int getOver10Mins(char *resp);

void drawPlotOver10Mins(SDL_Window *window, SDL_Renderer *renderer, char *);

void plotSummary(SDL_Window *window, SDL_Renderer *renderer, char *);

void drawSummary(SDL_Window *window, SDL_Renderer *renderer, char *summary_json);


#endif
