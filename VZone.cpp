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
#include "VZone.h"

int Czone::load(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters) {
    Ctile *list = NULL;
    char string[_DEFAULT_MAXD_LEN], *name = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/ZONE]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[ZONE]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("ID")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &zoneID) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("NZONE")) == 0) zoneID = NzoneID++;
                    else if (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) == 0) {
                        if (descriptions.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[POINTS]")) == 0) {
                        if (logics.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[TILE]")) == 0) {
                        list = new(Ctile);
                        if (list->load(configurationfile, objects, characters) > 0) LOG_ERROR("Unable to load TILE");
                        else if (addtile(list) > 0) {
                            LOG_ERROR("Unable to load TILE");
                            delete(list);
                        } else list = NULL;
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Czone] %s (unkown command)", string);
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Czone::save(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters) {
    tileslist *list = tilesroot;
    if (configurationfile) {
        fprintf(configurationfile, "[ZONE]\n");
        fprintf(configurationfile, "ID=%d\n", zoneID);
        if (descriptions.save(configurationfile) > 0) LOG_ERROR("Unable to save DESCRIPTIONS of Zone %d)", zoneID);
        fputc('\n', configurationfile);
        if (logics.save(configurationfile) > 0) LOG_ERROR("Unable to save LOGICS of Zone %d)", zoneID);
        fputc('\n', configurationfile);
        while (list) {
            if (list->tile->save(configurationfile, objects, characters) > 0) LOG_ERROR("Unable to save Tile %d", list->tile->getID());
            else fputc('\n', configurationfile);
            list = list->next;
        }
        fprintf(configurationfile, "[/ZONE]");
    } else return 1;
    return 0;
}

int Czone::savearches(FILE *configurationfile) {
    tileslist *list = tilesroot;
    if (configurationfile) {
        while (list) {
            if (list->tile->savearches(configurationfile) > 0) LOG_ERROR("Unable to save ARCHES of Node %d", list->tile->getID());
            list = list->next;
            if (list) fputc('\n', configurationfile);
        }
    } else return 1;
    return 0;
}

int Czone::addtile(Ctile *tile) {
    tileslist *list = tilesroot;
    if (tilesroot) {
        while (list->next) list = list->next;
        if ((list->next = (tileslist *) pvmalloc(sizeof (tileslist)))) {
            tile->setzoneID(this->zoneID);
            list->next->tile = tile;
            list->next->next = NULL;
            list->next->back = list;
        } else return 1;
    } else {
        if ((tilesroot = (tileslist *) pvmalloc(sizeof (tileslist)))) {
            tile->setzoneID(this->zoneID);
            tilesroot->tile = tile;
            tilesroot->next = NULL;
            tilesroot->back = NULL;
        } else return 1;
    }
    return 0;
}

int Czone::deltile(unsigned int tileID) {
    tileslist *list = NULL;
    if ((list = gettile(tileID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else tilesroot = list->next;
        list->tile->unload();
        delete(list->tile);
        pvfree(list);
    } else return 1;
    return 0;
}

tileslist *Czone::gettile(unsigned int tileID) {
    tileslist *list = tilesroot;
    if (tilesroot) {
        while (((unsigned int) list->tile->getID() != tileID) && (list->next)) list = list->next;
        if ((unsigned int) list->tile->getID() != tileID) return NULL;
    } else return NULL;
    return list;
}

tileslist *Czone::nextvalue(void) {
    if (tilesthis) tilesthis = tilesthis->next;
    else tilesthis = tilesroot;
    return tilesthis;
}

tileslist *Czone::backvalue(void) {
    if (tilesthis) tilesthis = tilesthis->back;
    else {
        tilesthis = tilesroot;
        while (tilesthis->next) tilesthis = tilesthis->next;
    }
    return tilesthis;
}

int Czone::delcharacters(void) {
    tileslist *list = tilesroot;
    int errors = 0;
    while (list) {
        errors += list->tile->delplayers();
        errors += list->tile->delmobs();
        list = list->next;
    }
    return errors;
}

int Czone::addarch(basicinfos *information, unsigned int from, unsigned int to, bool door, bool close, bool lock, unsigned int key, unsigned int length) {
    tileslist *backupfrom = NULL, *backupto = NULL;
    if ((backupto = gettile(to)) && (backupfrom = gettile(from))) {
        if (backupfrom->tile->addarch(information, backupto->tile, door, close, lock, key, length) > 0) return 1;
    } else return 1;
    return 0;
}

int Czone::addarch(basicinfos *information, Ctile *from, Ctile *to, bool door, bool close, bool lock, unsigned int key, unsigned int length) {
    if (from->addarch(information, to, door, close, lock, key, length) > 0) return 1;
    return 0;
}

int Czone::delarch(unsigned int tileID, const char *completename) {
    tileslist *list = NULL;
    if ((list = gettile(tileID))) {
        if (list->tile->delarch(completename) > 0) return 1;
    } else return 1;
    return 0;
}

int Czone::pvsend(Cserver& server, const char *format, ...) {
    va_list args;
    tileslist *list = tilesroot;
    int length = 0;
    char *message = NULL;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((message = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(message, length, format, args);
            va_end(args);
            while (list) {
                if (list->tile->pvsend(server, message) > 0) return 1;
                list = list->next;
            }
        } else return 1;
        if (message) {
            pvfree(message);
            message = NULL;
        }
    }
    return 0;
}

int Czone::spvsend(Cserver& server, const char *format, ...) {
    va_list args;
    tileslist *list = tilesroot;
    int length = 0;
    char *message = NULL;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((message = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(message, length, format, args);
            va_end(args);
            while (list) {
                if (list->tile->spvsend(server, message) > 0) return 1;
                list = list->next;
            }
        } else return 1;
        if (message) {
            pvfree(message);
            message = NULL;
        }
    }
    return 0;
}

void Czone::unload(void) {
    descriptions.unload();
    logics.unload();
    while (tilesroot) {
        if (deltile(tilesroot->tile->getID()) > 0) {
            LOG_ERROR("Unable to unload Tile %d", tilesroot->tile->getID());
            tilesroot = tilesroot->next;
        }
    }
}
