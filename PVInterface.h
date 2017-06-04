/* PSYCHO GAMES(C) STUDIOS - 2007 www.psychogames.net
 * Project: Vulture(c)
 * Author : Andrea Nardinocchi
 * eMail  : andrea@nardinan.it
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __PVINTERFACEHP__
#define __PVINTERFACEHP__
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>
#include "PVulture.h"
extern SDL_Texture *pgs;
extern SDL_Texture *tags;
extern SDL_Texture *backgroundS;
extern SDL_Texture *backgroundP;
extern SDL_Texture *vulture;
extern SDL_Texture *window;
extern SDL_Texture *warning;
extern SDL_Window *screen;
extern SDL_Renderer *render;
extern SDL_Surface *windowSurface;
extern int barvalue;
extern int playervalue;

enum ekey {
    KESCAPE, KMOUSER, KMOUSEL, KNONE
};

typedef struct skeypressed {
    ekey ckey;
    unsigned int posx;
    unsigned int posy;
} skeypressed;

class Cfont {
private:
    int position[256];
    unsigned int maxposition;
public:
    SDL_Texture *font;
    SDL_Surface *fontSurface;
    Uint32 getpixel(SDL_Surface *surface, unsigned int posx, unsigned int posy);
    int initialize(const char *fileformat);
    int write(unsigned int posx, unsigned int posy, unsigned int length, const char *format, ...);
    int write(unsigned int posx, unsigned int posy, const char *format, ...);
    int cwrite(unsigned int posy, const char *format, ...);

    inline void unload(void) {
        if (font) {
            SDL_DestroyTexture(font);
            font = NULL;
        }
    };
};
extern Cfont fontsystem;

class Cinterface {
public:
    int initialize(void);
    int intro(void);
    int informations(void);
    int pslist(void);
    int pstats(void);
    int showme(void);

    inline void update(void) {
        SDL_RenderPresent(render);
    }
    int loading(int value, const char *message, bool error);

    inline int loading(int value, const char *message) {
        return loading(value, message, false);
    };

    inline int unloading(int value, const char *message) {
        return loading(value * -1, message, false);
    };
    int rectangle(int posx, int posy, int width, int heigh, unsigned int red, unsigned int green, unsigned int blue);
    skeypressed getinput(void);
};
extern void sdlunload(void);
extern Cinterface pvinterface;
#endif
