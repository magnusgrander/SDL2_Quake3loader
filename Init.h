#ifndef _INIT_H
#define _INIT_H

#include "Main.h"
#include "Quake3Bsp.h"

int VideoFlags = 0;                                        // stores info about how we would like our drawing surface to be
SDL_Window * MainWindow = NULL ;                          // this would eventually be our main drawing window
SDL_GLContext mainContext;

#endif
