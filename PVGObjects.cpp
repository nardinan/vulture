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
#include "PVGObjects.h"
#include "PVulture.h"
struct shash objepositions[] = {
    {"FI", "dita"},
    {"RH", "mano destra"},
    {"RS", "spalla destra"},
    {"RA", "braccio destro"},
    {"RL", "gamba destra"},
    {"RF", "piede destro"},
    {"LH", "mano sinistra"},
    {"LS", "spalla sinistra"},
    {"LA", "braccio sinistro"},
    {"LL", "gamba sinistra"},
    {"LF", "piede sinistro"},
    {"TH", "torso"},
    {"HE", "testa"},
    {"NE", "collo"},
    {"WE", "addosso"},
    {"SH", "alle spalle"},
    {"IN", "inventario"},
    {"NULL", "NULL"}
};

Cobject *Cgobjects::get(char *name, Cplayer *player) {
    Cobject *result = NULL;
    objectslist *list = NULL;
    int value = 0, position = 0;
    char *backup = NULL, *objectname = NULL;
    if ((value = getname(name, &objectname)) > 0) {
        player->resetvalue();
        while ((!result) && (list = player->nextvalue())) {
            position = getvalue("SYSTEM", "Position", list->object->logics, 0);
            if ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(objectname)) == 0) ||
                    (compare.vcmpcase(list->object->descriptions.smalldescription(position), LSTRSIZE(objectname)) == 0)) {
                if (--value < 1) 
                    result = list->object;
            }
        }
    } else 
        return NULL;
    if (objectname) {
        pvfree(objectname);
        objectname = NULL;
    }
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return result;
}

Cobject *Cgobjects::get(char *name, Cobject *object) {
    Cobject *result = NULL;
    objectslist *list = NULL;
    int value = 0, position = 0;
    char *backup = NULL, *objectname = NULL;
    if (object->logics.hascategory("CONTAINER") == 0) {
        if ((value = getname(name, &objectname)) > 0) {
            object->resetvalue();
            while ((!result) && (list = object->nextvalue())) {
                position = getvalue("SYSTEM", "Position", list->object->logics, 0);
                if ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(objectname)) == 0) ||
                        (compare.vcmpcase(list->object->descriptions.smalldescription(position), LSTRSIZE(objectname)) == 0)) {
                    if (--value < 1) 
                        result = list->object;
                }
            }
        } else 
            return NULL;
    } else 
        return NULL;
    if (objectname) {
        pvfree(objectname);
        objectname = NULL;
    }
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return result;
}

Cobject *Cgobjects::get(char *name, Ctile *tile) {
    Cobject *result = NULL;
    objectslist *list = tile->getobjectsroot();
    int value = 0, position = 0;
    char *backup = NULL, *objectname = NULL;
    if ((value = getname(name, &objectname)) > 0) {
        while ((!result) && (list)) {
            position = getvalue("SYSTEM", "Position", list->object->logics, 0);
            if ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(objectname)) == 0) ||
                    (compare.vcmpcase(list->object->descriptions.smalldescription(position), LSTRSIZE(objectname)) == 0)) {
                if (--value < 1) 
                    result = list->object;
            }
            list = list->next;
        }
    } else 
        return NULL;
    if (objectname) {
        pvfree(objectname);
        objectname = NULL;
    }
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return result;
}

Cobject *Cgobjects::get(char *name, Cmob *mob) {
    Cobject *result = NULL;
    objectslist *list = NULL;
    int value = 0, position = 0;
    char *backup = NULL, *objectname = NULL;
    if ((value = getname(name, &objectname)) > 0) {
        mob->resetvalue();
        while ((!result) && (list = mob->nextvalue())) {
            position = getvalue("SYSTEM", "Position", list->object->logics, 0);
            if ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(objectname)) == 0) ||
                    (compare.vcmpcase(list->object->descriptions.smalldescription(position), LSTRSIZE(objectname)) == 0)) {
                if (--value < 1) 
                    result = list->object;
            }
        }
    } else 
        return NULL;
    if (objectname) {
        pvfree(objectname);
        objectname = NULL;
    }
    if (backup) {
        pvfree(backup);
        backup = NULL;
    }
    return result;
}

Cobject *Cgobjects::getweapon(char *position, Ccharacter *character) {
    objectslist *list = NULL;
    char *target = NULL;
    character->resetvalue();
    while ((list = character->nextvalue())) {
        if ((list->object->logics.hascategory("WEAPON") == 0) &&
                (getvalue("USAGE", "Use", list->object->logics, 0) > 0)) {
            if (compare.vcmpcase(position, CSTRSIZE("*")) == 0) 
                return list->object;
            else {
                if ((target = list->object->logics.getvalue("USAGE", 4)) &&
                        (compare.vcmpcase(position, LSTRSIZE(target)) == 0)) 
                    return list->object;
            }
        }
    }
    return NULL;
}

Cobject *Cgobjects::getweapon(char *position, Cplayer *player) {
    objectslist *list = NULL;
    char *target = NULL;
    player->resetvalue();
    while ((list = player->nextvalue())) {
        if ((list->object->logics.hascategory("WEAPON") == 0) &&
                (getvalue("USAGE", "Use", list->object->logics, 0) > 0)) {
            if (compare.vcmpcase(position, CSTRSIZE("*")) == 0) 
                return list->object;
            else {
                if ((target = list->object->logics.getvalue("USAGE", 4)) &&
                        (compare.vcmpcase(position, LSTRSIZE(target)) == 0)) 
                    return list->object;
            }
        }
    }
    return NULL;
}

Cobject *Cgobjects::getweapon(char *position, Cmob *mob) {
    objectslist *list = NULL;
    char *target = NULL;
    mob->resetvalue();
    while ((list = mob->nextvalue())) {
        if ((list->object->logics.hascategory("WEAPON") == 0) &&
                (getvalue("USAGE", "Use", list->object->logics, 0) > 0)) {
            if (compare.vcmpcase(position, CSTRSIZE("*")) == 0) 
                return list->object;
            else {
                if ((target = list->object->logics.getvalue("USAGE", 4)) &&
                        (compare.vcmpcase(position, LSTRSIZE(target)) == 0)) 
                    return list->object;
            }
        }
    }
    return NULL;
}

Cobject *Cgobjects::getobject(char *position, Cplayer *player) {
    objectslist *list = NULL;
    char *target = NULL;
    player->resetvalue();
    while ((list = player->nextvalue())) {
        if (getvalue("USAGE", "Use", list->object->logics, 0) > 0) {
            if (compare.vcmpcase(position, CSTRSIZE("*")) == 0) 
                return list->object;
            else {
                if ((target = list->object->logics.getvalue("USAGE", 4)) &&
                        (compare.vcmpcase(position, LSTRSIZE(target)) == 0)) 
                    return list->object;
            }
        }
    }
    return NULL;
}

Cobject *Cgobjects::getobject(char *position, Cmob *mob) {
    objectslist *list = NULL;
    char *target = NULL;
    mob->resetvalue();
    while ((list = mob->nextvalue())) {
        if (getvalue("USAGE", "Use", list->object->logics, 0) > 0) {
            if (compare.vcmpcase(position, CSTRSIZE("*")) == 0) 
                return list->object;
            else {
                if ((target = list->object->logics.getvalue("USAGE", 4)) &&
                        (compare.vcmpcase(position, LSTRSIZE(target)) == 0))
                    return list->object;
            }
        }
    }
    return NULL;
}

int Cgobjects::getweight(Cplayer *player) {
    objectslist *list = NULL;
    int weight = 0;
    player->resetvalue();
    while ((list = player->nextvalue()))
        weight += getweight(list->object);
    return weight;
}

int Cgobjects::getweight(Cobject *object) {
    objectslist *list = NULL;
    int weight = getvalue("STATS", "Weight", object->logics, 1);
    if (object->logics.hascategory("CONTAINER") == 0) {
        object->resetvalue();
        while ((list = object->nextvalue())) {
            weight += getvalue("STATS", "Weight", list->object->logics, 1);
        }
    }
    return weight;
}

int Cgobjects::getweight(Ctile *tile) {
    objectslist *list = tile->getobjectsroot();
    int weight = 0;
    while (list) {
        weight += getvalue("STATS", "Weight", list->object->logics, 1);
        list = list->next;
    }
    return weight;
}

int Cgobjects::getweight(Cmob *mob) {
    objectslist *list = NULL;
    int weight = 0;
    mob->resetvalue();
    while ((list = mob->nextvalue())) {
        weight += getweight(list->object);
    }
    return weight;
}

Cobject *Cgobjects::build(char *name) {
    Cobject *object = NULL;
    objectslist *copy = NULL;
    FILE *configurationfile = NULL;
    char path[_DEFAULT_MIND_LEN];
    snprintf(path, _DEFAULT_MIND_LEN, _PVFILES "objects/%s.do", name);
    if ((configurationfile = fopen(path, "r"))) {
        if ((object = new(Cobject)) && (object->load(configurationfile) == 0))
            if (!(copy = gameobjects.addobject(object))) 
                return NULL;
        fclose(configurationfile);
    } else 
        return NULL;
    return copy->object;
}

Cobject *Cgobjects::duplicate(Cobject *object) {
    objectslist *copy = NULL;
    if ((copy = gameobjects.addobject(object))) {
        if (!copy) 
            return NULL;
    } else 
        return NULL;
    return copy->object;
}

char *Cgobjects::getcompletename(Cobject *object) {
    char *buffer = NULL, string[_DEFAULT_MAXD_LEN];
    int position = getvalue("SYSTEM", "Position", object->logics, 0);
    snprintf(string, _DEFAULT_MAXD_LEN, "%s %s %s", 
        (object->getsex() != MALE) ? "una" : "uno", 
        object->descriptions.completename(position), 
        object->descriptions.smalldescription(position));
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
    } else 
        return NULL;
    return buffer;
}

char *Cgobjects::getsimplename(Cobject *object) {
    int position = getvalue("SYSTEM", "Position", object->logics, 0);
    return object->descriptions.completename(position);
}

char *Cgobjects::getadjective(Cobject *object) {
    int position = getvalue("SYSTEM", "Position", object->logics, 0);
    return object->descriptions.smalldescription(position);
}

char *Cgobjects::getdescription(Cobject *object) {
    int position = getvalue("SYSTEM", "Position", object->logics, 0);
    return object->descriptions.largedescription(position);
}

char *Cgobjects::getposition(Cobject *object) {
    int index = 0;
    char *buffer = NULL, *position = NULL;
    if ((position = object->logics.getvalue("USAGE", 4))) {
        while ((compare.vcmpcase(objepositions[index].representation, CSTRSIZE("NULL")) != 0) &&
                (compare.vcmpcase(objepositions[index].representation, LSTRSIZE(position)) != 0)) index++;
        if (compare.vcmpcase(objepositions[index].representation, LSTRSIZE(position)) == 0) {
            if ((buffer = (char *) pvmalloc(strlen(objepositions[index].string) + 1))) {
                strcpy(buffer, objepositions[index].string);
                buffer[strlen(objepositions[index].string)] = '\0';
            } else 
                return NULL;
        } else {
            if ((buffer = (char *) pvmalloc(sizeof ("da nessuna parte!") + 1))) {
                strcpy(buffer, "da nessuna parte!");
                buffer[strlen("da nessuna parte!")] = '\0';
            } else 
                return NULL;
        }
    } else 
        return NULL;
    return buffer;
}

char *Cgobjects::getinventory(Cplayer *player) {
    objectslist *list = NULL;
    int quantity = 1;
    bool equal = false;
    char *value = NULL, *backup = NULL, *buffer = NULL, *position = NULL, string[_DEFAULT_MIND_LEN];
    player->resetvalue();
    while ((list = player->nextvalue())) {
        if ((position = getposition(list->object))) {
            equal = false;
            if ((list->next) && (getvalue("SYSTEM", "Parent", list->next->object->logics, 0) == getvalue("SYSTEM", "Parent", list->object->logics, 0))) {
                if ((backup = getposition(list->next->object)) && (compare.vcmpcase(backup, position) == 0)) {
                    equal = true;
                    quantity++;
                }
            }
            if (backup) {
                pvfree(backup);
                backup = NULL;
            }
            if (!equal) {
                snprintf(string, _DEFAULT_MIND_LEN, "[%s] (%s) %s %s[n]",
                        (getvalue("USAGE", "Use", list->object->logics, 0) > 0) ? position : "Inventario",
                        value = funny.vdigit(quantity),
                        list->object->descriptions.completename(getvalue("SYSTEM", "Position", list->object->logics, 0)),
                        list->object->descriptions.smalldescription(getvalue("SYSTEM", "Position", list->object->logics, 0)));
                if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
                    string[_DEFAULT_MIND_LEN - 4] = '.';
                    string[_DEFAULT_MIND_LEN - 3] = '.';
                    string[_DEFAULT_MIND_LEN - 2] = '.';
                    string[_DEFAULT_MIND_LEN - 1] = '\0';
                }
                if (allocate.vadd(&buffer, string) > 0) 
                    return NULL;
                quantity = 1;
            }
        }
        if (value) {
            pvfree(value);
            value = NULL;
        }
        if (position) {
            pvfree(position);
            position = NULL;
        }
    }
    if (!buffer) {
        if ((buffer = (char *) pvmalloc(sizeof ("nulla![n]") + 1))) {
            strcpy(buffer, "nulla![n]");
            buffer[sizeof ("nulla![n]")] = '\0';
        } else 
            return NULL;
    }
    return buffer;
}

char *Cgobjects::getinventory(Cmob *mob) {
    objectslist *list = NULL;
    int quantity = 1;
    bool equal = false;
    char *value = NULL, *backup = NULL, *buffer = NULL, *position = NULL, string[_DEFAULT_MIND_LEN];
    mob->resetvalue();
    while ((list = mob->nextvalue())) {
        if ((position = getposition(list->object))) {
            equal = false;
            if ((list->next) && (getvalue("SYSTEM", "Parent", list->next->object->logics, 0) == getvalue("SYSTEM", "Parent", list->object->logics, 0))) {
                if ((backup = getposition(list->next->object)) && (compare.vcmpcase(backup, position) == 0)) {
                    equal = true;
                    quantity++;
                }
            }
            if (backup) {
                pvfree(backup);
                backup = NULL;
            }
            if (!equal) {
                snprintf(string, _DEFAULT_MIND_LEN, "[%s] (%s) %s %s[n]",
                        (getvalue("USAGE", "Use", list->object->logics, 0) > 0) ? position : "Inventario",
                        value = funny.vdigit(quantity),
                        list->object->descriptions.completename(getvalue("SYSTEM", "Position", list->object->logics, 0)),
                        list->object->descriptions.smalldescription(getvalue("SYSTEM", "Position", list->object->logics, 0)));
                if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
                    string[_DEFAULT_MIND_LEN - 4] = '.';
                    string[_DEFAULT_MIND_LEN - 3] = '.';
                    string[_DEFAULT_MIND_LEN - 2] = '.';
                    string[_DEFAULT_MIND_LEN - 1] = '\0';
                }
                if (allocate.vadd(&buffer, string) > 0) 
                    return NULL;
                quantity = 1;
            }
        }
        if (value) {
            pvfree(value);
            value = NULL;
        }
        if (position) {
            pvfree(position);
            position = NULL;
        }
    }
    if (!buffer) {
        if ((buffer = (char *) pvmalloc(sizeof ("nulla![n]") + 1))) {
            strcpy(buffer, "nulla![n]");
            buffer[sizeof ("nulla![n]")] = '\0';
        } else 
            return NULL;
    }
    return buffer;
}

char *Cgobjects::getlook(Cplayer *player) {
    objectslist *list = NULL;
    int quantity = 1;
    bool equal = false;
    char *value = NULL, *backup = NULL, *buffer = NULL, *position = NULL, string[_DEFAULT_MIND_LEN];
    player->resetvalue();
    while ((list = player->nextvalue())) {
        if ((getvalue("USAGE", "Use", list->object->logics, 0) > 0) &&
                (position = getposition(list->object)))
            if ((compare.vcmpcase(position, CSTRSIZE("inventario")) != 0) &&
                    (compare.vcmpcase(position, CSTRSIZE("da nessuna parte")) != 0)) {
                equal = false;
                if ((list->next) && (getvalue("SYSTEM", "Parent", list->next->object->logics, 0) == getvalue("SYSTEM", "Parent", list->object->logics, 0))) {
                    if ((backup = getposition(list->next->object)) && (compare.vcmpcase(backup, position) == 0)) {
                        equal = true;
                        quantity++;
                    }
                }
                if (backup) {
                    pvfree(backup);
                    backup = NULL;
                }
                if (!equal) {
                    snprintf(string, _DEFAULT_MIND_LEN, "[%s] (%s) %s %s[n]", position,
                            value = funny.vdigit(quantity),
                            list->object->descriptions.completename(getvalue("SYSTEM", "Position", list->object->logics, 0)),
                            list->object->descriptions.smalldescription(getvalue("SYSTEM", "Position", list->object->logics, 0)));
                    if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
                        string[_DEFAULT_MIND_LEN - 4] = '.';
                        string[_DEFAULT_MIND_LEN - 3] = '.';
                        string[_DEFAULT_MIND_LEN - 2] = '.';
                        string[_DEFAULT_MIND_LEN - 1] = '\0';
                    }
                    if (allocate.vadd(&buffer, string) > 0) 
                        return NULL;
                    quantity = 1;
                }
            }
        if (value) {
            pvfree(value);
            value = NULL;
        }
        if (position) {
            pvfree(position);
            position = NULL;
        }
    }
    if (!buffer)
        buffer = allocate.vsalloc("sembra sia completamente nud%s![n]", (player->getsex() != MALE) ? "a" : "o");
    return buffer;
}

char *Cgobjects::getlook(Cobject *object) {
    objectslist *list = NULL;
    int quantity = 1;
    char *value = NULL, *buffer = NULL, *status = NULL, *contents = NULL, string[_DEFAULT_MAXD_LEN], backup[_DEFAULT_MIND_LEN];
    snprintf(string, _DEFAULT_MAXD_LEN, "Vedi %s %s %s %s[n]%s",
            (object->getsex() == MALE) ? "un" : "una", this->getsimplename(object),
            this->getadjective(object), status = getstatus(object->logics), this->getdescription(object));
    if (status) {
        pvfree(status);
        status = NULL;
    }
    if (object->logics.hascategory("CONTAINER") == 0) {
        if (getvalue("CONTAINER", "Open", object->logics, 0) == 0) {
            if (getvalue("CONTAINER", "Lock", object->logics, 0) == 0) 
                strncat(string, "[n]E' [red]chiuso[reset]![n]", _DEFAULT_MIND_LEN);
            else 
                strncat(string, "[n]E' [red]bloccato[reset]![n]", _DEFAULT_MIND_LEN);
        } else {
            strncat(string, "[n]E' [green]aperto[reset]! ", _DEFAULT_MAXD_LEN);
            object->resetvalue();
            while ((list = object->nextvalue())) {
                if ((list->next) && (getvalue("SYSTEM", "Parent", list->next->object->logics, 0) == getvalue("SYSTEM", "Parent", list->object->logics, 0))) 
                    quantity++;
                else {
                    snprintf(backup, _DEFAULT_MIND_LEN, "[n]\t- (%s) %s %s %s",
                            value = funny.vdigit(quantity), this->getsimplename(list->object),
                            this->getadjective(list->object), status = getstatus(list->object->logics));
                    if (value) {
                        pvfree(value);
                        value = NULL;
                    }
                    if (status) {
                        pvfree(status);
                        status = NULL;
                    }
                    if (strlen(backup) >= (_DEFAULT_MIND_LEN - 1)) {
                        string[_DEFAULT_MIND_LEN - 4] = '.';
                        string[_DEFAULT_MIND_LEN - 3] = '.';
                        string[_DEFAULT_MIND_LEN - 2] = '.';
                        string[_DEFAULT_MIND_LEN - 1] = '\0';
                    }
                    if (allocate.vadd(&contents, backup) > 0) 
                        return NULL;
                    quantity = 1;
                }
            }
            if (!contents) {
                if (allocate.vadd(&contents, "Ma dentro non c'e' nulla!") > 0) 
                    return NULL;
            }
        }
    }
    if ((buffer = (char *) pvmalloc(strlen(string) + 1))) {
        strcpy(buffer, string);
        buffer[strlen(string)] = '\0';
        if (contents) {
            if (allocate.vadd(&buffer, contents) > 0) 
                return NULL;
            pvfree(contents);
            contents = NULL;
        }
        if (allocate.vadd(&buffer, "[n]") > 0) 
            return NULL;
    } else 
        return NULL;
    return buffer;
}

char *Cgobjects::getlook(Ctile *tile) {
    objectslist *list = tile->getobjectsroot();
    int lpoints = 1, quantity = 1;
    char *value = NULL, *buffer = NULL, string[_DEFAULT_MIND_LEN];
    while (list) {
        if ((list->next) && (getvalue("SYSTEM", "Parent", list->next->object->logics, 0) == getvalue("SYSTEM", "Parent", list->object->logics, 0))) quantity++;
        else {
            for (int index = 0; index < _DEFAULT_MIND_LEN; index++) 
                string[index] = '\0';
            lpoints = getvalue("STATS", "LPoints", list->object->logics, 0);
            snprintf(string, _DEFAULT_MIND_LEN, "[O] (%s) %s %s ", value = funny.vdigit(quantity), this->getsimplename(list->object), this->getadjective(list->object));
            if (value) {
                pvfree(value);
                value = NULL;
            }
            if (list->object->logics.hascategory("CONTAINER") == 0) {
                if (getvalue("CONTAINER", "Open", list->object->logics, 0) == 0) {
                    if (getvalue("CONTAINER", "Lock", list->object->logics, 0) == 0) 
                        strncat(string, "([red]chiuso[reset]) ", _DEFAULT_MIND_LEN);
                    else 
                        strncat(string, "([red]bloccato[reset]) ", _DEFAULT_MIND_LEN);
                } else {
                    strncat(string, "([green]aperto[reset]) ", _DEFAULT_MIND_LEN);
                }
            }
            if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
                string[_DEFAULT_MIND_LEN - 4] = '.';
                string[_DEFAULT_MIND_LEN - 3] = '.';
                string[_DEFAULT_MIND_LEN - 2] = '.';
                string[_DEFAULT_MIND_LEN - 1] = '\0';
            }
            if (allocate.vadd(&buffer, string) > 0) 
                return NULL;
            if (getvalue("RANK", "Immortal", list->object->logics, 0) == 1) {
                if (allocate.vadd(&buffer, "[[green]I[reset]][n]") > 0) 
                    return NULL;
            } else {
                if (lpoints < 3) {
                    if (allocate.vadd(&buffer, "[[red]P[reset]][n]") > 0) 
                        return NULL;
                } else if (lpoints < 6) {
                    if (allocate.vadd(&buffer, "[[yellow]D[reset]][n]") > 0) 
                        return NULL;
                } else {
                    if (allocate.vadd(&buffer, "[[green]O[reset]][n]") > 0) 
                        return NULL;
                }
            }
            quantity = 1;
        }
        list = list->next;
    }
    return buffer;
}

char *Cgobjects::getlook(Cmob *mob) {
    objectslist *list = NULL;
    int quantity = 1;
    bool equal = false;
    char *value = NULL, *backup = NULL, *buffer = NULL, *position = NULL, string[_DEFAULT_MIND_LEN];
    mob->resetvalue();
    while ((list = mob->nextvalue())) {
        if ((getvalue("USAGE", "Use", list->object->logics, 0) > 0) &&
                (position = getposition(list->object)))
            if ((compare.vcmpcase(position, CSTRSIZE("inventario")) != 0) &&
                    (compare.vcmpcase(position, CSTRSIZE("da nessuna parte")) != 0)) {
                equal = false;
                if ((list->next) && (getvalue("SYSTEM", "Parent", list->next->object->logics, 0) == getvalue("SYSTEM", "Parent", list->object->logics, 0))) {
                    if ((backup = getposition(list->next->object)) && (compare.vcmpcase(backup, position) == 0)) {
                        equal = true;
                        quantity++;
                    }
                }
                if (backup) {
                    pvfree(backup);
                    backup = NULL;
                }
                if (!equal) {
                    snprintf(string, _DEFAULT_MIND_LEN, "[%s] (%s) %s %s[n]", position,
                            value = funny.vdigit(quantity),
                            list->object->descriptions.completename(getvalue("SYSTEM", "Position", list->object->logics, 0)),
                            list->object->descriptions.smalldescription(getvalue("SYSTEM", "Position", list->object->logics, 0)));
                    if (strlen(string) >= (_DEFAULT_MIND_LEN - 1)) {
                        string[_DEFAULT_MIND_LEN - 4] = '.';
                        string[_DEFAULT_MIND_LEN - 3] = '.';
                        string[_DEFAULT_MIND_LEN - 2] = '.';
                        string[_DEFAULT_MIND_LEN - 1] = '\0';
                    }
                    if (allocate.vadd(&buffer, string) > 0) 
                        return NULL;
                    quantity = 1;
                }
            }
        if (value) {
            pvfree(value);
            value = NULL;
        }
        if (position) {
            pvfree(position);
            position = NULL;
        }
    }
    if (!buffer)
        buffer = allocate.vsalloc("sembra sia completamente nud%s![n]", (mob->getsex() != MALE) ? "a" : "o");
    return buffer;
}
