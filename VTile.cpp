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
#include "VTile.h"

Ctile::Ctile(void) {
    archesroot = NULL;
    archesthis = NULL;
    mobsroot = NULL;
    playersroot = NULL;
    objectsroot = NULL;
}

int Ctile::load(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters) {
    char string[_DEFAULT_MAXD_LEN], *name = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/TILE]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[TILE]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("ID")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &tileID) > 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("NTILE")) == 0) 
                        tileID = NtileID++;
                    else if (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) == 0) {
                        if (descriptions.load(configurationfile) > 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[POINTS]")) == 0) {
                        if (logics.load(configurationfile) > 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[OBJECTS]")) == 0) {
                        if (loadobjects(configurationfile, objects) > 0)
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) 
                        LOG_ERROR("[Ctile] %s (unkown command)", string);
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

int Ctile::loadobjects(FILE *configurationfile, Cobjectslist& objects) {
    objectslist *list = NULL;
    int integer;
    char string[_DEFAULT_MAXD_LEN], *name = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/OBJECTS]")) != 0) && (!feof(configurationfile))) {
                if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[OBJECTS]")) != 0)) {
                    if (strings.vsscanf(string, '=', "sd", &name, &integer) > 0) 
                        LOG_ERROR("Unable to edit key %s", string);
                    if ((list = objects.getobject(integer)))
                        if (addobject(list->object) > 0) 
                            LOG_ERROR("Unable to load OBJECTS");
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else 
            return 1;
    } else 
        return 1;
    return 0;
}

int Ctile::save(FILE *configurationfile, Cobjectslist& objects, Ccharacterslist& characters) {
    if (configurationfile) {
        fprintf(configurationfile, "[TILE]\n");
        fprintf(configurationfile, "ID=%d\n", tileID);
        if (logics.save(configurationfile) > 0) 
            LOG_ERROR("Unable to save LOGICS of Tile %d", tileID);
        fputc('\n', configurationfile);
        if (descriptions.save(configurationfile) > 0) 
            LOG_ERROR("Unable to save DESCRIPTIONS of Tile %d", tileID);
        fputc('\n', configurationfile);
        if (saveobjects(configurationfile, objects) > 0) 
            LOG_ERROR("Unable to save OBJECTS of Tile %d", tileID);
        fprintf(configurationfile, "\n[/TILE]");
    } else 
        return 1;
    return 0;
}

int Ctile::saveobjects(FILE *configurationfile, Cobjectslist& objects) {
    objectslist *list = objectsroot;
    if (configurationfile) {
        fprintf(configurationfile, "[OBJECTS]\n");
        while (list) {
            if (objects.getobject(list->object->getID()))
                fprintf(configurationfile, "OBJECT=%d\n", list->object->getID());
            list = list->next;
        }
        fprintf(configurationfile, "[/OBJECTS]");
    } else return 1;
    return 0;
}

int Ctile::savearches(FILE *configurationfile) {
    archeslist *list = archesroot;
    if (configurationfile) {
        fprintf(configurationfile, "[ARCHES]\n");
        while (list) {
            if (list->tile) {
                fprintf(configurationfile, "%d:%d:%d:%d:%s:%s:%s:%d:%d:%d:%d\n",
                        zoneID, tileID, list->tile->getzoneID(), list->tile->getID(),
                        list->information->completename, list->information->smalldescription, list->information->largedescription,
                        ((list->door) ? ((list->close) ? 1 : 0) : -1), ((list->lock) ? 1 : 0), list->key, list->length);
            }
            list = list->next;
        }
        fprintf(configurationfile, "[/ARCHES]");
    } else return 1;
    return 0;
}

int Ctile::addarch(basicinfos *information, Ctile *tile, bool door, bool close, bool lock, unsigned int key, unsigned int length) {
    archeslist *list = archesroot;
    if (!getarch(information->completename)) {
        if (archesroot) {
            while (list->next) list = list->next;
            if ((list->next = (archeslist *) pvmalloc(sizeof (archeslist)))) {
                list->next->tile = tile;
                list->next->key = key;
                list->next->door = door;
                list->next->close = close;
                list->next->lock = lock;
                list->next->length = length;
                list->next->next = NULL;
                list->next->back = list;
                if ((list->next->information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) {
                    strcpy(list->next->information->completename, information->completename);
                    strcpy(list->next->information->smalldescription, information->smalldescription);
                    strcpy(list->next->information->largedescription, information->largedescription);
                } else
                    return 1;
            } else 
                return 1;
        } else {
            if ((archesroot = (archeslist *) pvmalloc(sizeof (archeslist)))) {
                archesroot->tile = tile;
                archesroot->key = key;
                archesroot->door = door;
                archesroot->close = close;
                archesroot->lock = lock;
                archesroot->length = length;
                archesroot->next = NULL;
                archesroot->back = NULL;
                if ((archesroot->information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) {
                    strcpy(archesroot->information->completename, information->completename);
                    strcpy(archesroot->information->smalldescription, information->smalldescription);
                    strcpy(archesroot->information->largedescription, information->largedescription);
                } else 
                    return 1;
            } else 
                return 1;
        }
    } else 
        return 1;
    return 0;
}

int Ctile::delarch(const char *completename) {
    archeslist *list = archesroot;
    if ((list = getarch(completename))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else archesroot = list->next;
        pvfree(list->information);
        pvfree(list);
    } else
        return 1;
    return 0;
}

archeslist *Ctile::getarch(const char *completename) {
    archeslist *list = archesroot;
    if (list) {
        while ((compare.vcmpcase(list->information->completename, completename) != 0) && (list->next)) 
            list = list->next;
        if (compare.vcmpcase(list->information->completename, completename) != 0) 
            return NULL;
    } else 
        return NULL;
    return list;
}

archeslist *Ctile::nextvalue(void) {
    if (archesthis) 
        archesthis = archesthis->next;
    else 
        archesthis = archesroot;
    return archesthis;
}

archeslist *Ctile::backvalue(void) {
    if (archesthis) 
        archesthis = archesthis->back;
    else {
        archesthis = archesroot;
        while (archesthis->next) 
            archesthis = archesthis->next;
    }
    return archesthis;
}

int Ctile::addplayer(Cplayer *player) {
    playerslist *list = playersroot, *backup = NULL;
    if (!getplayer(player->getID())) {
        if (playersroot) {
            while ((list->next) && (list->player->getID() < player->getID())) list = list->next;
            if (list->player->getID() < player->getID()) {
                backup = list->next;
                if ((list->next = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                    player->position = this;
                    list->next->player = player;
                    list->next->next = backup;
                    list->next->back = list;
                    if (backup) 
                        backup->back = list->next;
                } else 
                    return 1;
            } else if (list->back) {
                backup = list->back;
                if ((backup->next = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                    player->position = this;
                    backup->next->player = player;
                    backup->next->next = list;
                    backup->next->back = backup;
                    list->back = backup->next;
                } else 
                    return 1;
            } else if ((playersroot = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                player->position = this;
                playersroot->player = player;
                playersroot->next = list;
                playersroot->back = NULL;
                list->back = playersroot;
            } else 
                return 1;
        } else {
            if ((playersroot = (playerslist *) pvmalloc(sizeof (playerslist)))) {
                player->position = this;
                playersroot->player = player;
                playersroot->next = NULL;
                playersroot->back = NULL;
            } else 
                return 1;
        }
    } else return 1;
    return 0;
}

int Ctile::delplayer(unsigned int characterID) {
    playerslist *list = playersroot;
    if ((list = getplayer(characterID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else playersroot = list->next;
        pvfree(list);
    } else 
        return 1;
    return 0;
}

int Ctile::delplayer(const char *completename, unsigned int position) {
    playerslist *list = playersroot;
    if ((list = getplayer(completename, position))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else playersroot = list->next;
        pvfree(list);
    } else 
        return 1;
    return 0;
}

int Ctile::delplayers(void) {
    int errors = 0;
    while (playersroot) {
        if (delplayer(playersroot->player->getID()) > 0) {
            LOG_ERROR("Unable to unload Player %d", playersroot->player->getID());
            playersroot = playersroot->next;
            errors++;
        }
    }
    return errors;
}

int Ctile::getplayers(void) {
    playerslist *list = playersroot;
    int elements = 0;
    while (list) {
        list = list->next;
        elements++;
    }
    return elements;
}

playerslist *Ctile::getplayer(unsigned int characterID) {
    playerslist *list = playersroot;
    if (playersroot) {
        while (((unsigned int) list->player->getID() < characterID) && (list->next)) list = list->next;
        if ((unsigned int) list->player->getID() != characterID) return NULL;
    } else return NULL;
    return list;
}

playerslist *Ctile::getplayer(const char *completename) {
    playerslist *list = playersroot;
    int position = 0;
    if (playersroot) {
        position = ((list->player->logics.hasvalue("SYSTEM", "Position") != 0) ? 1 : list->player->logics.getvalue("SYSTEM", "Position"));
        while ((compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) {
            list = list->next;
            position = ((list->player->logics.hasvalue("SYSTEM", "Position") != 0) ? 1 : list->player->logics.getvalue("SYSTEM", "Position"));
        }
        if (compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

playerslist *Ctile::getplayer(const char *completename, unsigned int position) {
    playerslist *list = playersroot;
    if (playersroot) {
        while ((compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->player->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

int Ctile::addmob(Cmob *mob) {
    mobslist *list = mobsroot, *backup = NULL;
    if (!getmob(mob->getID())) {
        if (mobsroot) {
            while ((list->next) && (list->mob->getID() < mob->getID())) list = list->next;
            if (list->mob->getID() < mob->getID()) {
                backup = list->next;
                if ((list->next = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                    mob->position = this;
                    list->next->mob = mob;
                    list->next->next = backup;
                    list->next->back = list;
                    if (backup) backup->back = list->next;
                } else 
                    return 1;
            } else if (list->back) {
                backup = list->back;
                if ((backup->next = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                    mob->position = this;
                    backup->next->mob = mob;
                    backup->next->next = list;
                    backup->next->back = backup;
                    list->back = backup->next;
                } else 
                    return 1;
            } else if ((mobsroot = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                mob->position = this;
                mobsroot->mob = mob;
                mobsroot->next = list;
                mobsroot->back = NULL;
                list->back = mobsroot;
            } else 
                return 1;
        } else {
            if ((mobsroot = (mobslist *) pvmalloc(sizeof (mobslist)))) {
                mob->position = this;
                mobsroot->mob = mob;
                mobsroot->back = NULL;
                mobsroot->next = NULL;
            } else 
                return 1;
        }
    } else return 1;
    return 0;
}

int Ctile::delmob(unsigned int characterID) {
    mobslist *list = mobsroot;
    if ((list = getmob(characterID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else mobsroot = list->next;
        pvfree(list);
    } else 
        return 1;
    return 0;
}

int Ctile::delmob(const char *completename, unsigned int position) {
    mobslist *list = mobsroot;
    if ((list = getmob(completename, position))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else mobsroot = list->next;
        pvfree(list);
    } else 
        return 1;
    return 0;
}

int Ctile::delmobs(void) {
    int errors = 0;
    while (mobsroot) {
        if (delmob(mobsroot->mob->getID()) > 0) {
            LOG_ERROR("Unable to unload Mob %d", mobsroot->mob->getID());
            mobsroot = mobsroot->next;
            errors++;
        }
    }
    return errors;
}

mobslist *Ctile::getmob(unsigned int characterID) {
    mobslist *list = mobsroot;
    if (mobsroot) {
        while (((unsigned int) list->mob->getID() < characterID) && (list->next)) list = list->next;
        if ((unsigned int) list->mob->getID() != characterID) return NULL;
    } else 
        return NULL;
    return list;
}

mobslist *Ctile::getmob(const char *completename, unsigned int position) {
    mobslist *list = mobsroot;
    if (mobsroot) {
        while ((compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->mob->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else 
        return NULL;
    return list;
}

int Ctile::addobject(Cobject *object) {
    objectslist *list = objectsroot, *backup = NULL;
    if (!getobject(object->getID())) {
        if (objectsroot) {
            while ((list->next) && (list->object->logics.getvalue("SYSTEM", "Parent") != object->logics.getvalue("SYSTEM", "Parent"))) {
                list = list->next;
                if (list->object->logics.hasvalue("SYSTEM", "Parent") != 0) {
                    LOG_ERROR("Unable to find SYSTEM->Parent Logic");
                    break;
                }
            }
            if (list->next) {
                backup = list->next;
                if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                    list->next->object = object;
                    list->next->next = backup;
                    list->next->back = list;
                    backup->back = list->next;
                } else 
                    return 1;
            } else if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                list->next->object = object;
                list->next->next = NULL;
                list->next->back = list;
            } else 
                return 1;
        } else {
            if ((objectsroot = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                objectsroot->object = object;
                objectsroot->next = NULL;
                objectsroot->back = NULL;
            } else
                return 1;
        }
    } else 
        return 1;
    return 0;
}

int Ctile::delobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if ((list = getobject(objectID))) {
        if (list->next) 
            list->next->back = list->back;
        if (list->back) 
            list->back->next = list->next;
        else
            objectsroot = list->next;
        pvfree(list);
    } else 
        return 1;
    return 0;
}

int Ctile::delobject(const char *completename, unsigned int position) {
    objectslist *list = objectsroot;
    if ((list = getobject(completename, position))) {
        if (list->next) 
            list->next->back = list->back;
        if (list->back) 
            list->back->next = list->next;
        else 
            objectsroot = list->next;
        pvfree(list);
    } else 
        return 1;
    return 0;
}

objectslist *Ctile::getobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if (list) {
        while (((unsigned int) list->object->getID() != objectID) && (list->next)) 
            list = list->next;
        if ((unsigned int) list->object->getID() != objectID) 
            return NULL;
    } else 
        return NULL;
    return list;
}

objectslist *Ctile::getobject(const char *completename, unsigned int position) {
    objectslist *list = objectsroot;
    if (list) {
        while ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next))
            list = list->next;
        if (compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) 
            return NULL;
    } else
        return NULL;
    return list;
}

int Ctile::cpvsend(unsigned int distance, Ctile *node) {
    archeslist *list = NULL;
    if (distance > 0) {
        node->resetvalue();
        while ((list = node->nextvalue())) {
            if (cpvsend((distance - 1), list->tile) > 0) 
                return 1;
            list = list->next;
        }
    } else {
        if (node->logics.hascategory("CHECKED") == 0)
            if (node->logics.delcategory("CHECKED") > 0) 
                return 1;
    }
    return 0;
}

int Ctile::sppvsend(Cserver& server, unsigned int distance, Ctile *node, const char *format, va_list args) {
    va_list copy;
    archeslist *archeslist = NULL;
    playerslist *playerslist = node->getplayersroot();
    int length = 0, result;
    char *message = NULL;
    if (distance > 0) {
        node->resetvalue();
        while ((archeslist = node->nextvalue())) {
            va_copy(copy, args);
            result = pvsend(server, (distance - 1), archeslist->tile, format, copy);
            va_end(copy);
            if (result > 0) 
                return 1;
            archeslist = archeslist->next;
        }
    } else {
        if (node->logics.hascategory("CHECKED") != 0) {
            if (node->logics.addcategory("CHECKED") > 0) 
                return 1;
            va_copy(copy, args);
            length = allocate.vstrlen(format, copy);
            va_end(copy);
            if (length > 0) {
                if ((message = (char *) pvmalloc(length + 1))) {
                    va_copy(copy, args);
                    vsnprintf(message, length, format, copy);
                    va_end(copy);
                    while (playerslist) {
                        if (playerslist->player->pvsend(server, message) > 0) 
                            return 1;
                        playerslist = playerslist->next;
                    }
                } else return 1;
                if (message) {
                    pvfree(message);
                    message = NULL;
                }
            }
        }
    }
    return 0;
}

int Ctile::pvsend(Cserver& server, unsigned int distance, bool clean, const char *format, ...) {
    va_list args;
    int value;
    va_start(args, format);
    if ((value = sppvsend(server, distance, this, format, args)) > 0)
        LOG_ERROR("Unable to run SEND()");
    if (clean) if (cpvsend(distance, this) > 0) 
        LOG_ERROR("Unable to run CSEND()");
    va_end(args);
    return value;
}

int Ctile::pvsend(Cserver& server, const char *format, ...) {
    va_list args;
    int value;
    va_start(args, format);
    if ((value = sppvsend(server, 0, this, format, args)) > 0) 
        LOG_ERROR("Unable to run SEND()");
    if (cpvsend(0, this) > 0) 
        LOG_ERROR("Unable to run CSEND()");
    va_end(args);
    return value;
}

int Ctile::spspvsend(Cserver& server, unsigned int distance, Ctile *node, const char *format, va_list args) {
    va_list copy;
    archeslist *archeslist = NULL;
    playerslist *playerslist = node->getplayersroot();
    int length = 0, result;
    char *message = NULL;
    if (distance > 0) {
        node->resetvalue();
        while ((archeslist = node->nextvalue())) {
            va_copy(copy, args);
            result = spvsend(server, (distance - 1), archeslist->tile, format, copy);
            va_end(copy);
            if (result > 0) 
                return 1;
            archeslist = archeslist->next;
        }
    } else {
        if (node->logics.hascategory("CHECKED") != 0) {
            if (node->logics.addcategory("CHECKED") > 0) 
                return 1;
            va_copy(copy, args);
            length = allocate.vstrlen(format, copy);
            va_end(copy);
            if (length > 0) {
                if ((message = (char *) pvmalloc(length + 1))) {
                    va_copy(copy, args);
                    vsnprintf(message, length, format, copy);
                    va_end(copy);
                    while (playerslist) {
                        if (playerslist->player->spvsend(server, message) > 0)
                            return 1;
                        playerslist = playerslist->next;
                    }
                } else return 1;
                if (message) {
                    pvfree(message);
                    message = NULL;
                }
            }
        }
    }
    return 0;
}

int Ctile::spvsend(Cserver& server, unsigned int distance, bool clean, const char *format, ...) {
    va_list args;
    int value;
    va_start(args, format);
    if ((value = spspvsend(server, distance, this, format, args)) > 0)
        LOG_ERROR("Unable to run SSEND()");
    if (clean) if (cpvsend(distance, this) > 0) 
        LOG_ERROR("Unable to run CSEND()");
    va_end(args);
    return value;
}

int Ctile::spvsend(Cserver& server, const char *format, ...) {
    va_list args;
    int value;
    va_start(args, format);
    if ((value = spspvsend(server, 0, this, format, args)) > 0)
        LOG_ERROR("Unable to run SSEND()");
    if (cpvsend(0, this) > 0)
        LOG_ERROR("Unable to run CSEND()");
    va_end(args);
    return value;
}

char *Ctile::randomize(void) {
    archeslist *list = archesroot;
    while (list) {
        if ((rand() % 100) > 50) return list->information->completename;
        if (!list->next) return list->information->completename;
        else list = list->next;
    }
    return NULL;
}

void Ctile::unload(void) {
    logics.unload();
    descriptions.unload();
    while (archesroot) {
        if (delarch(archesroot->information->completename) > 0) {
            LOG_ERROR("Unable to unload Arch %s", archesroot->information->completename);
            archesroot = archesroot->next;
        }
    }
    while (objectsroot) {
        if (delobject(objectsroot->object->getID()) > 0) {
            LOG_ERROR("Unable to unload Object %d", objectsroot->object->getID());
            objectsroot = objectsroot->next;
        }
    }
}
