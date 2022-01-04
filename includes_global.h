#pragma once

/*       DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                   Version 2, December 2004

Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

Everyone is permitted to copy and distribute verbatim or modified
copies of this license document, and changing it is allowed as long
as the name is changed.

           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

 0. You just DO WHAT THE FUCK YOU WANT TO.


    includes_global.h
    This file contains all the includes needed in the game.

 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <math.h>
#ifdef _WIN32
	#include <windows.h>
	#include <winsock.h>
	#include <sys/stat.h>
	#ifndef S_ISDIR
		#define S_ISDIR(a) ((a & _S_IFDIR) == _S_IFDIR)
	#endif
	#ifndef S_ISREG
		#define S_ISREG(a) ((a & _S_IFREG) == _S_IFREG)
	#endif
#else
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <dirent.h>

#endif

#include <SDL.h>
#include <SDL_image.h>

#define NR_OF_FREQ 70

#define WINDOW_SIZE_H 840 
#define WINDOW_SIZE_W 1600

