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
#include "PVInterface.h"
#define INTRO_ALPHA_SLEEP_TIME_USEC 20000
#define INTRO_PGS_SLEEP_TIME_USEC 1000000
SDL_Texture *pgs = NULL;
SDL_Texture *tags = NULL;
SDL_Texture *backgroundS = NULL;
SDL_Texture *backgroundP = NULL;
SDL_Texture *vulture = NULL;
SDL_Texture *window = NULL;
SDL_Texture *warning = NULL;
SDL_Window *screen = NULL;
SDL_Renderer *render = NULL;
SDL_Surface *windowSurface = NULL;
int barvalue = 0;
int playervalue = -1;

SDL_Texture *SDL_interface_LoadBMP(const char *path) {
    SDL_Surface *_image;
    SDL_Texture *result = NULL;
    if ((_image = SDL_LoadBMP(path))) {
        result = SDL_CreateTextureFromSurface(render, _image);
        SDL_FreeSurface(_image);
    }
    return result;
}

SDL_Texture *SDL_interface_ColorKeyLoadBMP(const char *path, int R, int G, int B, SDL_Surface **image) {
    SDL_Texture *result = NULL;
    if (((*image) = SDL_LoadBMP(path))) {
        SDL_SetColorKey((*image), SDL_TRUE, SDL_MapRGB((*image)->format, R, G, B));
        result = SDL_CreateTextureFromSurface(render, (*image));
    }
    return result;
}

SDL_Texture *SDL_interface_SurfaceLoadBMP(const char *path) {

}

int SDL_interface_BlitSurface(SDL_Texture *surface, SDL_Rect *source, SDL_Window *screen, SDL_Rect *destination) {
    return SDL_RenderCopy(render, surface, source, destination);
}

void SDL_interface_FreeSurface(SDL_Texture *surface) {
    SDL_DestroyTexture(surface);
}

int SDL_interface_GetW(SDL_Texture *texture) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    return w;
}

int SDL_interface_GetH(SDL_Texture *texture) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    return h;
}

Uint32 Cfont::getpixel(SDL_Surface *surface, unsigned int posx, unsigned int posy) {
    Uint32 bpp;
    Uint8 *bits, red, green, blue;
    bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *) surface->pixels) + posy * surface->pitch + posx * bpp;
    switch (bpp) {
        case 1:
            return *((Uint8 *) surface->pixels + posy * surface->pitch + posx);
            break;
        case 2:
            return *((Uint16 *) surface->pixels + posy * surface->pitch / 2 + posx);
            break;
        case 3:
            red = *((bits) + (surface->format->Rshift / 8));
            green = *((bits) + (surface->format->Gshift / 8));
            blue = *((bits) + (surface->format->Bshift / 8));
            return SDL_MapRGB(surface->format, red, green, blue);
            break;
        case 4:
            return *((Uint32 *) surface->pixels + posy * surface->pitch / 4 + posx);
    }
    return 0;
}

int Cfont::initialize(const char *fileformat) {
    Uint32 pixel, pink;
    int posx = 0, index = 0, pitch;
    void *pixels = NULL;
    SDL_Surface *localSurface = NULL;
    if ((font = SDL_interface_ColorKeyLoadBMP(fileformat, 0, 0, 0, &localSurface))) {
        SDL_LockTexture(font, NULL, &pixels, &pitch);
        pink = SDL_MapRGB(localSurface->format, 255, 0, 255);
        while (posx < SDL_interface_GetW(font)) {
            if (this->getpixel(localSurface, posx, 0) == pink) {
                position[index++] = posx;
                while ((posx < SDL_interface_GetW(font)) && (this->getpixel(localSurface, posx, 0) == pink))
                    posx++;
                position[index++] = posx;
            }
            posx++;
        }
        maxposition = posx - 1;
        SDL_UnlockTexture(font);
        SDL_FreeSurface(localSurface);
    } else
        return 1;
    return 0;
}

int Cfont::write(unsigned int posx, unsigned int posy, unsigned int length, const char *format, ...) {
    va_list args;
    SDL_Rect source, destination;
    char *buffer = NULL;
    int charoffset, basiclength;
    const char *character = NULL;
    source.y = 1;
    destination.y = posy;
    source.h = destination.h = SDL_interface_GetH(font) - 1;
    va_start(args, format);
    basiclength = allocate.vstrlen((char *) format, args);
    va_end(args);
    if (basiclength > 0) {
        if ((buffer = (char *) pvmalloc(basiclength + 1))) {
            va_start(args, format);
            vsnprintf(buffer, basiclength, format, args);
            va_end(args);
            if (strlen(buffer) > length)
                buffer[length] = '\0';
            for (character = buffer; *character != '\0' && posx <= SDL_interface_GetW(font); character++) {
                charoffset = ((int) (*character - 33)) * 2 - 1;
                if (*character == ' ' || charoffset < 0 || charoffset > maxposition)
                    posx += position[2] - position[1];
                else {
                    source.w = destination.w = (position[charoffset + 2] + position[charoffset + 1]) / 2 - (position[charoffset] + position[charoffset - 1]) / 2;
                    source.x = (position[charoffset] + position[charoffset - 1]) / 2;
                    destination.x = posx - (int) (position[charoffset] - position[charoffset - 1]) / 2;
                    SDL_RenderCopy(render, font, &source, &destination);
                    posx += position[charoffset + 1] - position[charoffset];
                }
            }
        } else return 1;
        if (buffer) {
            pvfree(buffer);
            buffer = NULL;
        }
    } else return 1;
    return 0;
}

int Cfont::write(unsigned int posx, unsigned int posy, const char *format, ...) {
    va_list args;
    SDL_Rect source, destination;
    char *buffer = NULL;
    int charoffset, basiclength;
    const char *character = NULL;
    source.y = 1;
    destination.y = posy;
    source.h = destination.h = SDL_interface_GetH(font) - 1;
    va_start(args, format);
    basiclength = allocate.vstrlen((char *) format, args);
    va_end(args);
    if (basiclength > 0) {
        if ((buffer = (char *) pvmalloc(basiclength + 1))) {
            va_start(args, format);
            vsnprintf(buffer, basiclength, format, args);
            va_end(args);
            for (character = buffer; *character != '\0' && posx <= SDL_interface_GetW(font); character++) {
                charoffset = ((int) (*character - 33)) * 2 - 1;
                if (*character == ' ' || charoffset < 0 || charoffset > maxposition) posx += position[2] - position[1];
                else {
                    source.w = destination.w = (position[charoffset + 2] + position[charoffset + 1]) / 2 - (position[charoffset] + position[charoffset - 1]) / 2;
                    source.x = (position[charoffset] + position[charoffset - 1]) / 2;
                    destination.x = posx - (int) (position[charoffset] - position[charoffset - 1]) / 2;
                    SDL_RenderCopy(render, font, &source, &destination);
                    posx += position[charoffset + 1] - position[charoffset];
                }
            }
        } else return 1;
        if (buffer) {
            pvfree(buffer);
            buffer = NULL;
        }
    } else return 1;
    return 0;
}

int Cfont::cwrite(unsigned int posy, const char *format, ...) {
    va_list args;
    char *buffer = NULL;
    const char *character = NULL;
    int charoffset = 0, width = 0, basiclength, value = 0;
    va_start(args, format);
    basiclength = allocate.vstrlen((char *) format, args);
    va_end(args);
    if (basiclength > 0) {
        if ((buffer = (char *) pvmalloc(basiclength + 1))) {
            va_start(args, format);
            vsnprintf(buffer, basiclength, format, args);
            va_end(args);
            for (character = buffer; *character != '\0'; character++) {
                if (*character != '\0') {
                    charoffset = ((int) *character - 33) * 2 + 1;
                    if (*character == ' ' || charoffset < 0 || charoffset > maxposition)
                        width += position[2] - position[1];
                    else
                        width += position[charoffset + 1] - position[charoffset];
                }
            }
            value = this->write(640 / 2 - width / 2, posy, buffer);
            if (buffer) {
                pvfree(buffer);
                buffer = NULL;
            }
        } else
            return 1;
    } else
        return 1;
    return value;
}
Cfont fontsystem;

int Cinterface::initialize(void) {
    SDL_Surface *localSurface;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERROR("Unable to load SDL library");
        return 1;
    } else {
        if (!(screen = SDL_CreateWindow("PV(c) [v"_VERSION"] Psychog Games(c) Studios",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                640,
                480,
                SDL_WINDOW_OPENGL))) {
            LOG_ERROR("Unable to load SDL pvinterface (%dx%dx%d SDL_SWSURFACE)", 640, 480, 32);
            return 1;
        }
    }
    windowSurface = SDL_GetWindowSurface(screen);
    if (!(render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED))) {
        LOG_ERROR("Unable to create SDL renderere");
        return 1;
    }
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
    if (fontsystem.initialize("./arts/font.bmp") > 0)
        return 1;
    if (!backgroundS) {
        if (!(backgroundS = SDL_interface_LoadBMP("./arts/screen.bmp")))
            return 1;
    }
    if (!backgroundP) {
        if (!(backgroundP = SDL_interface_LoadBMP("./arts/player.bmp")))
            return 1;
    }
    if (!tags) {
        if (!(tags = SDL_interface_ColorKeyLoadBMP("./arts/tags.bmp", 0, 0, 0, &localSurface)))
            return 1;
        SDL_FreeSurface(localSurface);
    } else
        return 1;
    if (!(pgs = SDL_interface_LoadBMP("./arts/pgs.bmp")))
        LOG_ERROR("Unable to load PGS.bmp");
    if (!(vulture = SDL_interface_LoadBMP("./arts/vulture.bmp")))
        LOG_ERROR("Unable to load VULTURE.bmp");
    if (!(window = SDL_interface_LoadBMP("./arts/window.bmp")))
        LOG_ERROR("Unable to load WINDOW.bmp");
    if (!(warning = SDL_interface_LoadBMP("./arts/error.bmp")))
        LOG_ERROR("Unable to load ERROR.bmp");
    atexit(sdlunload);
    return 0;
}

int Cinterface::intro(void) {
    int pgsalpha = 0;
    if (screen) {
        SDL_SetTextureBlendMode(pgs, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(vulture, SDL_BLENDMODE_BLEND);
        for (pgsalpha = 0; pgsalpha < 255; pgsalpha += 5) {
            getinput();
            SDL_RenderClear(render);
            if (SDL_SetTextureAlphaMod(pgs, pgsalpha) != 0)
                return 1;
            SDL_RenderCopy(render, pgs, NULL, NULL);
            SDL_RenderPresent(render);
            usleep(INTRO_ALPHA_SLEEP_TIME_USEC);
        }
        usleep(INTRO_PGS_SLEEP_TIME_USEC);
        for (pgsalpha = 0; pgsalpha < 255; pgsalpha += 5) {
            getinput();
            SDL_RenderClear(render);
            if (SDL_SetTextureAlphaMod(vulture, pgsalpha) != 0)
                return 1;
            SDL_RenderCopy(render, pgs, NULL, NULL);
            SDL_RenderCopy(render, vulture, NULL, NULL);
            SDL_RenderPresent(render);
            usleep(INTRO_ALPHA_SLEEP_TIME_USEC);
        }
        usleep(INTRO_PGS_SLEEP_TIME_USEC);
        for (pgsalpha = 255; pgsalpha > 0; pgsalpha -= 2) {
            getinput();
            SDL_RenderClear(render);
            if (SDL_SetTextureAlphaMod(vulture, pgsalpha) != 0)
                return 1;
            SDL_RenderCopy(render, vulture, NULL, NULL);
            SDL_RenderPresent(render);
            usleep(INTRO_ALPHA_SLEEP_TIME_USEC);
        }
    } else
        return 1;
    return 0;
}

int Cinterface::informations(void) {
    int loopsecs = pvulture.stopwatch.getsecs(timeloop);
    if (screen) {
        if (playervalue < 0) {
            if (SDL_interface_BlitSurface(backgroundS, NULL, screen, NULL) != 0)
                return 1;
        } else {
            if (SDL_interface_BlitSurface(backgroundP, NULL, screen, NULL) != 0)
                return 1;
        }
        if (fontsystem.write(220, 5, "[ DOOR: %d ] %s",
                pvulture.server.information.port,
                (pvulture.server.information.completename) ? pvulture.server.information.completename : "unknown") > 0)
            return 1;
        if (fontsystem.write(310, 40, "%d/%d/%d %02d:%02d",
                pvulture.time.pdate.day,
                pvulture.time.pdate.month,
                pvulture.time.pdate.year,
                pvulture.time.ptime.hour,
                pvulture.time.ptime.minutes) > 0)
            return 1;
        if (fontsystem.write(310, 56, "(in secs) %d", loopsecs) > 0)
            return 1;
        if (fontsystem.write(550, 56, "[ %s ]",
                (loopsecs < 5) ? ((loopsecs < 3) ? ((loopsecs < 2) ? "good" : "warning") : "critical!") : "DANGER!") > 0)
            return 1;
        if (fontsystem.write(310, 72, "%d KBytes", numbers.vdiv(memory, 1024)) > 0)
            return 1;
        if (fontsystem.write(10, 440, "%d", pvulture.pages.elements) > 0)
            return 1;
    } else
        return 1;
    return 0;
}

int Cinterface::pslist(void) {
    SDL_Rect destination, source;
    playerslist *playerslist = pvulture.characters.gamecharacters.playersroot;
    int posy = 155, elements = 0;
    if (screen) {
        while ((playerslist) && (elements < 18)) {
            destination.h = source.h = 16;
            source.x = 0;
            if (++elements < 18) {
                if (playerslist->player->logics.hasvalue("STATUS", "Password") != 0) {
                    if (playerslist->player->logics.hasvalue("STATUS", "Account") != 0) {
                        if (fontsystem.write(120, posy, "- (online) [%d] %s", playerslist->player->getID(), pvulture.characters.getsimplename(playerslist->player)) > 0)
                            return 1;
                        if (fontsystem.write(480, posy, "|MINS : %d", pvulture.stopwatch.getmins(playerslist->player->logics.getvalue("STATUS", "Online"))) > 0)
                            return 1;
                        if (getkind(playerslist->player, ADMIN))
                            source.y = 16;
                        else if (getkind(playerslist->player, MODERATOR))
                            source.y = 32;
                        else
                            source.y = 0;
                        destination.y = posy;
                        destination.x = 15;
                        destination.w = source.w = SDL_interface_GetW(tags);
                        if (SDL_interface_BlitSurface(tags, &source, screen, &destination) != 0)
                            return 1;
                        if (playerslist->player->logics.hasvalue("STATUS", "Hide") == 0) {
                            destination.x = 580;
                            source.y = 48;
                            source.x = source.w = destination.w = 30;
                            if (SDL_interface_BlitSurface(tags, &source, screen, &destination) != 0)
                                return 1;
                        }
                        if (playerslist->player->logics.hascategory("FIGHT") == 0) {
                            destination.x = 610;
                            source.y = 48;
                            source.x = 0;
                            source.w = destination.w = 30;
                            if (SDL_interface_BlitSurface(tags, &source, screen, &destination) != 0)
                                return 1;
                        }
                    } else {
                        if (fontsystem.write(120, posy, "- (login) [??] someone") > 0)
                            return 1;
                    }
                } else {
                    if (fontsystem.write(120, posy, "- (passwd) [%d] %s", playerslist->player->getID(), pvulture.characters.getsimplename(playerslist->player)) > 0)
                        return 1;
                }
            } else if (fontsystem.write(40, posy, "[ others %d players ... ]") > 0)
                return 1;
            posy += 16;
            playerslist = playerslist->next;
        }
    } else
        return 1;
    return 0;
}

int Cinterface::pstats(void) {
    playerslist *currentplayer = pvulture.characters.gamecharacters.getplayer(playervalue);
    char *backup = NULL, *description = NULL, *index = NULL;
    unsigned int newline, life, stamina, arms, legs, rows = 0;
    if (currentplayer) {
        if (currentplayer->player->logics.hascategory("CHARS") == 0) {
            if (fontsystem.write(540, 137, "%d/100", currentplayer->player->logics.getvalue("CHARS", "ST")) != 0) return 1;
            else if (fontsystem.write(540, 157, "%d/100", currentplayer->player->logics.getvalue("CHARS", "DE")) != 0) return 1;
            else if (fontsystem.write(540, 177, "%d/100", currentplayer->player->logics.getvalue("CHARS", "IN")) != 0) return 1;
            else if (fontsystem.write(540, 197, "%d/100", currentplayer->player->logics.getvalue("CHARS", "CH")) != 0) return 1;
            else if (fontsystem.write(540, 217, "%d/100", currentplayer->player->logics.getvalue("CHARS", "CO")) != 0) return 1;
            else if (fontsystem.write(540, 237, "%d/100", currentplayer->player->logics.getvalue("CHARS", "AC")) != 0) return 1;
            else if (fontsystem.write(540, 257, "%d/100", currentplayer->player->logics.getvalue("CHARS", "SP")) != 0) return 1;
            else if (fontsystem.write(540, 277, "%d/100", currentplayer->player->logics.getvalue("CHARS", "EM")) != 0) return 1;
        } else return 1;
        if (fontsystem.write(80, 157, "%s (ID %d)", currentplayer->player->descriptions.completename(getvalue("SYSTEM", "Position", currentplayer->player->logics, 0)), currentplayer->player->getID()) != 0) return 1;
        if (fontsystem.write(80, 177, "%s", (currentplayer->player->getsex() != MALE) ? "Female" : "Male") != 0) return 1;
        if (fontsystem.write(80, 197, "%s", currentplayer->player->logics.getvalue("RACE", 0)) != 0) return 1;
        life = currentplayer->player->logics.getvalue("STATS", "LPoints");
        stamina = currentplayer->player->logics.getvalue("STATS", "SPoints");
        arms = currentplayer->player->logics.getvalue("STATS", "Arms");
        legs = currentplayer->player->logics.getvalue("STATS", "Legs");
        rectangle(109, 256, 102, 22, 250, 250, 250);
        rectangle(110, 257, 100, 20, 0, 0, 0);
        rectangle(110, 257, life, 20, (life <= 50) ? 255 : 0, (life >= 20) ? 255 : 0, 0);
        rectangle(109, 276, 102, 22, 250, 250, 250);
        rectangle(110, 277, 100, 20, 0, 0, 0);
        rectangle(110, 277, stamina, 20, (stamina <= 50) ? 255 : 0, (stamina >= 20) ? 255 : 0, 0);
        rectangle(274, 181, 57, 22, 250, 250, 250);
        rectangle(275, 182, 55, 20, 0, 0, 0);
        rectangle(275, 182, numbers.vdiv((arms * 55), 100), 20, (arms <= 50) ? 255 : 0, (arms >= 20) ? 255 : 0, 0);
        rectangle(274, 283, 57, 22, 250, 250, 250);
        rectangle(275, 284, 55, 20, 0, 0, 0);
        rectangle(275, 284, numbers.vdiv((legs * 55), 100), 20, (legs <= 50) ? 255 : 0, (legs >= 20) ? 255 : 0, 0);
        if ((description = currentplayer->player->descriptions.largedescription(getvalue("SYSTEM", "Position", currentplayer->player->logics, 0)))) {
            if ((backup = (char *) pvmalloc(strlen(description) + 1))) {
                strcpy(backup, description);
                index = backup;
                while (strlen(index) > 50) {
                    newline = 50;
                    while ((index[newline] != ' ') && (newline < 55)) newline++;
                    if (index[newline] != ' ') {
                        newline = 50;
                        while ((index[newline] != ' ') && (newline > 45)) newline--;
                    }
                    if (index[newline] != ' ') newline = 50;
                    if (fontsystem.write(22, (340 + (16 * rows++)), ++newline, index) != 0) return 1;
                    index += newline;
                }
                if (strlen(index) > 0) {
                    if (fontsystem.write(22, (340 + (16 * rows++)), index) != 0) return 1;
                }
                if (backup) {
                    pvfree(backup);
                    backup = NULL;
                }
            } else return 1;
        }
    } else playervalue = -1;
    return 0;
}

int Cinterface::showme(void) {
    skeypressed result;
    playerslist *currentplayer = NULL;
    int index, value = 0;
    if (informations() == 0) {
        if (playervalue < 0) {
            if (pvinterface.pslist() > 0) return 1;
        } else if (pvinterface.pstats() > 0) return 1;
        result = pvinterface.getinput();
        if (result.ckey == KESCAPE) {
            if (pvulture.characters.gamecharacters.pvsend(pvulture.server, "[reset][n][red]unload del server (richiesta da localhost)[n]") > 0) return 1;
            if (pvulture.characters.gamecharacters.spvsend(pvulture.server, sshell) > 0) return 1;
            gstop = true;
        } else if (result.ckey == KMOUSEL) {
            if (playervalue < 0) {
                if (((result.posx >= 15) && (result.posx <= 620)) && ((result.posy >= 155) && (result.posy <= 439))) {
                    currentplayer = pvulture.characters.gamecharacters.playersroot;
                    if (currentplayer) {
                        for (index = 0; (currentplayer->next) && (index < numbers.vdiv((result.posy - 155), 16)); index++) currentplayer = currentplayer->next;
                        if (currentplayer->player->getonline()) playervalue = currentplayer->player->getID();
                    }
                }
            } else {
                if ((currentplayer = pvulture.characters.gamecharacters.getplayer(playervalue))) {
                    if (((result.posx >= 215) && (result.posx <= 238)) &&
                            ((result.posy >= 257) && (result.posy <= 276))) {
                        value = currentplayer->player->logics.getvalue("STATS", "LPoints");
                        if (currentplayer->player->logics.addvalue("STATS", "LPoints", numbers.vmin(value + 5, 100)) > 0) LOG_ERROR("Unable to set STATS->LPoints");
                    } else if (((result.posx >= 241) && (result.posx <= 268)) &&
                            ((result.posy >= 257) && (result.posy <= 276))) {
                        value = currentplayer->player->logics.getvalue("STATS", "LPoints");
                        if (currentplayer->player->logics.addvalue("STATS", "LPoints", numbers.vmax(value - 5, 0)) > 0) LOG_ERROR("Unable to set STATS->LPoints");
                    } else if (((result.posx >= 215) && (result.posx <= 238)) &&
                            ((result.posy >= 279) && (result.posy <= 298))) {
                        value = currentplayer->player->logics.getvalue("STATS", "SPoints");
                        if (currentplayer->player->logics.addvalue("STATS", "SPoints", numbers.vmin(value + 5, 100)) > 0) LOG_ERROR("Unable to set STATS->SPoints");
                    } else if (((result.posx >= 241) && (result.posx <= 268)) &&
                            ((result.posy >= 279) && (result.posy <= 298))) {
                        value = currentplayer->player->logics.getvalue("STATS", "SPoints");
                        if (currentplayer->player->logics.addvalue("STATS", "SPoints", numbers.vmax(value - 5, 0)) > 0) LOG_ERROR("Unable to set STATS->SPoints");
                    } else if (((result.posx >= 9) && (result.posx <= 76)) &&
                            ((result.posy >= 413) && (result.posy <= 431))) playervalue = -1;
                } else playervalue = -1;
            }
        }
    } else return 1;
    return 0;
}

int Cinterface::loading(int value, const char *message, bool error) {
    SDL_Rect destination;
    if ((barvalue + value <= 100) && (barvalue + value >= 0)) barvalue += value;
    else if (barvalue + value > 100) barvalue = 100;
    else barvalue = 0;
    destination.x = 170;
    destination.y = 170;
    destination.w = SDL_interface_GetW(window);
    destination.h = SDL_interface_GetH(window);
    if (screen) {
        if (SDL_interface_BlitSurface(backgroundS, NULL, screen, NULL) != 0) return 1;
        if (error) {
            if (SDL_interface_BlitSurface(warning, NULL, screen, &destination) != 0) return 1;
        } else if (SDL_interface_BlitSurface(window, NULL, screen, &destination) != 0) return 1;
        if (rectangle(170, 280, (barvalue * 3), 19, 0, 0, 150) > 0) return 1;
        if (!error) {
            if (message) {
                if (fontsystem.write(180, 262, message) > 0) return 1;
            }
            if (value >= 0) {
                if (fontsystem.write(180, 200, "Loading Adventure") > 0)
                    return 1;
                if (fontsystem.write(180, 216, "[memory: %d]", memory) > 0)
                    return 1;
            } else {
                if (fontsystem.write(180, 200, "Unloading Adventure") > 0)
                    return 1;
                if (fontsystem.write(180, 216, "[memory: %d]", memory) > 0)
                    return 1;
            }
        }
        SDL_RenderPresent(render);
        SDL_Delay(50);
    } else return 1;
    if (error)
        while (getinput().ckey != KESCAPE);
    else
        getinput();
    return 0;
}

int Cinterface::rectangle(int posx, int posy, int width, int heigh, unsigned int red, unsigned int green, unsigned int blue) {
    SDL_Rect size = {posx, posy, width, heigh};
    SDL_SetRenderDrawColor(render, red, green, blue, 255);
    SDL_RenderFillRect(render, &size);
    return 0;
    //return SDL_FillRect(windowSurface, &size, SDL_MapRGB(windowSurface->format, red, green, blue));
}

skeypressed Cinterface::getinput(void) {
    SDL_Event event;
    skeypressed result;
    result.ckey = KNONE;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_MOUSEMOTION: break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) result.ckey = KMOUSEL;
            else if (event.button.button == SDL_BUTTON_RIGHT) result.ckey = KMOUSER;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) result.ckey = KESCAPE;
            break;
        default: break;
    }
    result.posx = event.motion.x;
    result.posy = event.motion.y;
    return result;
}

void sdlunload(void) {
    if (pgs) {
        SDL_interface_FreeSurface(pgs);
        pgs = NULL;
    }
    if (tags) {
        SDL_interface_FreeSurface(tags);
        tags = NULL;
    }
    if (vulture) {
        SDL_interface_FreeSurface(vulture);
        vulture = NULL;
    }
    if (backgroundS) {
        SDL_interface_FreeSurface(backgroundS);
        backgroundS = NULL;
    }
    if (backgroundP) {
        SDL_interface_FreeSurface(backgroundP);
        backgroundP = NULL;
    }
    if (window) {
        SDL_interface_FreeSurface(window);
        window = NULL;
    }
    if (warning) {
        SDL_interface_FreeSurface(warning);
        warning = NULL;
    }
    if (windowSurface) {
        SDL_FreeSurface(windowSurface);
        windowSurface = NULL;
    }
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(screen);
    fontsystem.unload();
    SDL_Quit();
}
Cinterface pvinterface;
