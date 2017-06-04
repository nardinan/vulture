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
#include "VWorld.h"

int Cworld::load(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters) {
    Czone *list = NULL;
    char string[_DEFAULT_MAXD_LEN], *name = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/WORLD]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[WORLD]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) == 0) {
                        if (descriptions.load(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[ARCHESLIST]")) == 0) {
                        if (loadarches(configurationfile) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[ZONE]")) == 0) {
                        list = new(Czone);
                        if (list->load(configurationfile, objects, characters) > 0) LOG_ERROR("Unable to load ZONE");
                        else if (addzone(list) > 0) {
                            LOG_ERROR("Unable to add loaded ZONE");
                            delete(list);
                        } else list = NULL;
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cworld] %s (unkown command)", string);
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

int Cworld::loadarches(FILE *configurationfile) {
    tileinfos to, from;
    basicinfos *information = NULL;
    int close;
    unsigned int lock, key, length;
    char string[_DEFAULT_MAXD_LEN], *completename = NULL, *smalldescription = NULL, *largedescription = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/ARCHESLIST]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[ARCHESLIST]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("[ARCHES]")) == 0) {
                        while ((compare.vcmpcase(string, CSTRSIZE("[/ARCHES]")) != 0) && (!feof(configurationfile))) {
                            if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, "[ARCHES]", strlen("[ARCHES]")) != 0)) {
                                if (strings.vsscanf(string, ':', "ddddsssdddd", &from.zoneID, &from.tileID, &to.zoneID, &to.tileID,
                                        &completename, &smalldescription, &largedescription, &close, &lock, &key, &length) == 0) {
                                    if ((information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) {
                                        strcpy(information->completename, completename);
                                        strcpy(information->smalldescription, smalldescription);
                                        strcpy(information->largedescription, largedescription);
                                        information->completename[strlen(completename)] = '\0';
                                        information->smalldescription[strlen(smalldescription)] = '\0';
                                        information->largedescription[strlen(largedescription)] = '\0';
                                        if (addarch(information, from, to, ((close > 0) ? true : false), ((close > 0) ? true : false), ((lock > 0) ? true : false), key, length) > 0) LOG_ERROR("Unable to connect ARCHES");
                                    } else return 1;
                                } else return 1;
                            }
                            if (information) {
                                pvfree(information);
                                information = NULL;
                            }
                            if (completename) {
                                pvfree(completename);
                                completename = NULL;
                            }
                            if (smalldescription) {
                                pvfree(smalldescription);
                                smalldescription = NULL;
                            }
                            if (largedescription) {
                                pvfree(largedescription);
                                largedescription = NULL;
                            }
                            fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
                        }
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cworld] %s (unkown command)", string);
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cworld::save(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters) {
    zoneslist *list = zonesroot;
    if (configurationfile) {
        fprintf(configurationfile, "[WORLD]\n");
        if (descriptions.save(configurationfile) > 0) LOG_ERROR("Unable to save Descriptions");
        fputc('\n', configurationfile);
        while (list) {
            if (list->zone->save(configurationfile, objects, characters) > 0) LOG_ERROR("Unable to save Zone %d", list->zone->getID());
            else fputc('\n', configurationfile);
            list = list->next;
        }
        fprintf(configurationfile, "[ARCHESLIST]\n");
        if (savearches(configurationfile) > 0) LOG_ERROR("Unable to save ARCHES Category");
        fprintf(configurationfile, "[/ARCHESLIST]\n[/WORLD]");
    } else return 1;
    return 0;
}

int Cworld::savearches(FILE *configurationfile) {
    zoneslist *list = zonesroot;
    if (configurationfile) {
        while (list) {
            if (list->zone->savearches(configurationfile) > 0) LOG_ERROR("Unable to save ARCHES of Zone %d", list->zone->getID());
            list = list->next;
            fputc('\n', configurationfile);
        }
    } else return 1;
    return 0;
}

int Cworld::addzone(Czone *zone) {
    zoneslist *list = zonesroot;
    if (zonesroot) {
        while (list->next) list = list->next;
        if ((list->next = (zoneslist *) pvmalloc(sizeof (zoneslist)))) {
            list->next->zone = zone;
            list->next->next = NULL;
            list->next->back = list;
        } else return 1;
    } else {
        if ((zonesroot = (zoneslist *) pvmalloc(sizeof (zoneslist)))) {
            zonesroot->zone = zone;
            zonesroot->next = NULL;
            zonesroot->back = NULL;
        } else return 1;
    }
    return 0;
}

int Cworld::delzone(unsigned int zoneID) {
    zoneslist *list = NULL;
    if ((list = getzonelist(zoneID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else zonesroot = list->next;
        list->zone->unload();
        delete(list->zone);
        pvfree(list);
    } else return 1;
    return 0;
}

zoneslist *Cworld::getzone(unsigned int zoneID) {
    zoneslist *list = zonesroot;
    if (zonesroot) {
        while (((unsigned int) list->zone->getID() != zoneID) && (list->next)) list = list->next;
        if ((unsigned int) list->zone->getID() != zoneID) return NULL;
    } else return NULL;
    return list;
}

tileslist *Cworld::gettile(tileinfos Position) {
    zoneslist *list = NULL;
    if ((list = getzone(Position.zoneID))) return list->zone->gettile(Position.tileID);
    return NULL;
}

zoneslist *Cworld::getzonelist(unsigned int zoneID) {
    zoneslist *list = zonesroot;
    if (zonesroot) {
        while (((unsigned int) list->zone->getID() != zoneID) && (list->next)) list = list->next;
        if ((unsigned int) list->zone->getID() != zoneID) return NULL;
    } else return NULL;
    return list;
}

int Cworld::delcharacters(void) {
    zoneslist *list = zonesroot;
    int errors = 0;
    while (list) {
        errors += list->zone->delcharacters();
        list = list->next;
    }
    return errors;
}

zoneslist *Cworld::nextvalue(void) {
    if (zonesthis) zonesthis = zonesthis->next;
    else zonesthis = zonesroot;
    return zonesthis;
}

zoneslist *Cworld::backvalue(void) {
    if (zonesthis) zonesthis = zonesthis->back;
    else {
        zonesthis = zonesroot;
        while (zonesthis->next) zonesthis = zonesthis->next;
    }
    return zonesthis;
}

int Cworld::addarch(basicinfos *information, tileinfos from, tileinfos to, bool door, bool close, bool lock, unsigned int key, unsigned int length) {
    zoneslist *backupfrom = NULL, *backupto = NULL;
    tileslist *backupfromtile = NULL, *backuptotile = NULL;
    if (from.zoneID == to.zoneID) {
        if ((backupfrom = getzonelist(from.zoneID))) {
            if (backupfrom->zone->addarch(information, from.tileID, to.tileID, door, close, lock, key, length) > 0) LOG_ERROR("Unable to load ARCHES (zone %d) from tile %d to tile %d", from.zoneID, from.tileID, to.tileID);
        } else return 1;
    } else {
        if ((backupfrom = getzonelist(from.zoneID)) && (backupto = getzonelist(to.zoneID))) {
            if ((backupfromtile = backupfrom->zone->gettile(from.tileID)) && (backuptotile = backupto->zone->gettile(to.tileID))) {
                if (backupfrom->zone->addarch(information, backupfromtile->tile, backuptotile->tile, door, close, lock, key, length) > 0) LOG_ERROR("Unable to load ARCHES (zone %d) tile %d to (zone %d) tile %d", from.zoneID, from.tileID, to.zoneID, to.tileID);
            } else return 1;
        } else return 1;
    }
    return 0;
}

int Cworld::delarch(tileinfos tile, char *completename) {
    zoneslist *list = NULL;
    if ((list = getzonelist(tile.zoneID))) {
        if (list->zone->delarch(tile.tileID, completename) > 0) return 1;
    } else return 1;
    return 0;
}

int Cworld::pvsend(Cserver& server, const char *format, ...) {
    va_list args;
    zoneslist *list = zonesroot;
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
                if (list->zone->pvsend(server, message) > 0) return 1;
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

void Cworld::unload(void) {
    descriptions.unload();
    while (zonesroot) {
        if (delzone(zonesroot->zone->getID()) > 0) {
            LOG_ERROR("Unable to unload Zone %d", zonesroot->zone->getID());
            zonesroot = zonesroot->next;
        }
    }
}
