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
#include "VCharacter.h"
#include "VTile.h"

Ccharacter::Ccharacter(void) {
    objectsroot = NULL;
    objectsthis = NULL;
    charactersroot = NULL;
    definitionsroot = NULL;
}

int Ccharacter::loadobjects(FILE *configurationfile, Cobjectslist& objects) {
    objectslist *list = NULL;
    int integer;
    char string[_DEFAULT_MAXD_LEN], *name = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/OBJECTS]")) != 0) && (!feof(configurationfile))) {
                if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[OBJECTS]")) != 0)) {
                    if (strings.vsscanf(string, '=', "sd", &name, &integer) > 0) LOG_ERROR("Unable to edit key %s", string);
                    if ((list = objects.getobject(integer)))
                        if (addobject(list->object) > 0) LOG_ERROR("Unable to load OBJECTS");
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

int Ccharacter::loadcharacters(FILE *configurationfile) {
    int integer, character;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/CHARACTERS]")) != 0) && (!feof(configurationfile))) {
                if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[CHARACTERS]")) != 0)) {
                    if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("Unable to edit key %s", string);
                    else if (strings.vsscanf(text, ':', "dd", &integer, &character) > 0) LOG_ERROR("Unable to load CHARACTERS");
                    else if (addcharacter(integer, (characterdefinition) character) > 0) LOG_ERROR("Unable to load CHARACTERS");
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                if (text) {
                    pvfree(text);
                    text = NULL;
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Ccharacter::saveobjects(FILE *configurationfile, Cobjectslist& objects) {
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

int Ccharacter::savecharacters(FILE *configurationfile) {
    characterslist *list = charactersroot;
    if (configurationfile) {
        fprintf(configurationfile, "[CHARACTERS]\n");
        while (list) {
            fprintf(configurationfile, "CHARACTER=%d:%d\n", list->characterID, (int) list->character);
            list = list->next;
        }
        fprintf(configurationfile, "[/CHARACTERS]");
    } else return 1;
    return 0;
}

int Ccharacter::savedefinitions(FILE *configurationfile) {
    definitionslist *list = definitionsroot;
    if (configurationfile) {
        while (list) {
            fprintf(configurationfile, "DEFINITION=%s:%s:%s:%d\n", list->completename, list->category, list->keyname, list->definitionID);
            list = list->next;
        }
    } else return 1;
    return 0;
}

int Ccharacter::addobject(Cobject *object) {
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
                } else return 1;
            } else if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                list->next->object = object;
                list->next->next = NULL;
                list->next->back = list;
            } else return 1;
        } else {
            if ((objectsroot = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                objectsroot->object = object;
                objectsroot->next = NULL;
                objectsroot->back = NULL;
            } else return 1;
        }
    } else return 1;
    return 0;
}

int Ccharacter::delobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if ((list = getobject(objectID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else objectsroot = list->next;
        pvfree(list);
    } else return 1;
    return 0;
}

int Ccharacter::delobject(const char *completename, unsigned int position) {
    objectslist *list = objectsroot;
    if ((list = getobject(completename, position))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else objectsroot = list->next;
        pvfree(list);
    } else return 1;
    return 0;
}

objectslist *Ccharacter::getobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while (((unsigned int) list->object->getID() != objectID) && (list->next)) list = list->next;
        if ((unsigned int) list->object->getID() != objectID) return NULL;
    } else return NULL;
    return list;
}

objectslist *Ccharacter::getobject(const char *completename, unsigned int position) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

int Ccharacter::getage(datainfos& present) {
    int years = 0;
    if ((birth->year < present.year) ||
            ((birth->year == present.year) && (birth->month < present.month)) ||
            ((birth->year == present.year) && (birth->month == present.month) && (birth->day < present.day))) {
        while (birth->year + years < present.year) years++;
        if ((birth->month > present.month) ||
                ((birth->month == present.month) && (birth->day >= present.month))) years++;
    } else return 0;
    return years;
}

int Ccharacter::addcharacter(unsigned int characterID, characterdefinition character) {
    characterslist *list = charactersroot, *backup = NULL;
    if (!(getcharacter(characterID, character))) {
        if (charactersroot) {
            while ((list->next) && (list->characterID < characterID)) list = list->next;
            if (list->characterID < characterID) {
                backup = list->next;
                if ((list->next = (characterslist *) pvmalloc(sizeof (characterslist)))) {
                    list->next->characterID = characterID;
                    list->next->character = character;
                    list->next->next = backup;
                    list->next->back = list;
                    if (backup) backup->back = list->next;
                } else return 1;
            } else if (list->back) {
                backup = list->back;
                if ((backup->next = (characterslist *) pvmalloc(sizeof (characterslist)))) {
                    backup->next->characterID = characterID;
                    backup->next->character = character;
                    backup->next->next = list;
                    backup->next->back = backup;
                    list->back = backup->next;
                } else return 1;
            } else if ((charactersroot = (characterslist *) pvmalloc(sizeof (characterslist)))) {
                charactersroot->characterID = characterID;
                charactersroot->character = character;
                charactersroot->next = list;
                charactersroot->back = NULL;
                list->back = charactersroot;
            } else return 1;
        } else {
            if ((charactersroot = (characterslist *) pvmalloc(sizeof (characterslist)))) {
                charactersroot->characterID = characterID;
                charactersroot->character = character;
                charactersroot->back = NULL;
                charactersroot->next = NULL;
            } else return 1;
        }
    }
    return 0;
}

int Ccharacter::delcharacter(unsigned int characterID, characterdefinition character) {
    characterslist *list = NULL;
    if ((list = getcharacter(characterID, character))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else charactersroot = list->next;
        pvfree(list);
    } else return 1;
    return 0;
}

characterslist *Ccharacter::getcharacter(unsigned int characterID, characterdefinition character) {
    characterslist *list = charactersroot;
    if (charactersroot) {
        while ((list->characterID <= characterID) && (list->next)) {
            if (list->characterID == characterID) {
                if (list->character == character) break;
            }
            list = list->next;
        }
        if ((list->characterID != characterID) || (list->character != character)) return NULL;
    } else return NULL;
    return list;
}

int Ccharacter::adddefinition(const char *definition, const char *category, const char *keyname, unsigned int definitionID) {
    definitionslist *list = definitionsroot, *backup = NULL;
    if (!(getdefinition(definitionID))) {
        if (definitionsroot) {
            while ((list->next) && (list->definitionID < definitionID)) list = list->next;
            if (list->definitionID < definitionID) {
                backup = list->next;
                if ((list->next = (definitionslist *) pvmalloc(sizeof (definitionslist)))) {
                    list->next->definitionID = definitionID;
                    if (!(list->next->completename = (char *) pvmalloc(strlen(definition) + 1))) return 1;
                    if (!(list->next->category = (char *) pvmalloc(strlen(category) + 1))) return 1;
                    if (!(list->next->keyname = (char *) pvmalloc(strlen(keyname) + 1))) return 1;
                    strcpy(list->next->completename, definition);
                    strcpy(list->next->category, category);
                    strcpy(list->next->keyname, keyname);
                    list->next->completename[strlen(definition)] = '\0';
                    list->next->category[strlen(category)] = '\0';
                    list->next->keyname[strlen(keyname)] = '\0';
                    list->next->next = backup;
                    list->next->back = list;
                    if (backup) backup->back = list->next;
                } else return 1;
            } else if (list->back) {
                backup = list->back;
                if ((backup->next = (definitionslist *) pvmalloc(sizeof (definitionslist)))) {
                    backup->next->definitionID = definitionID;
                    if (!(backup->next->completename = (char *) pvmalloc(strlen(definition) + 1))) return 1;
                    if (!(backup->next->category = (char *) pvmalloc(strlen(category) + 1))) return 1;
                    if (!(backup->next->keyname = (char *) pvmalloc(strlen(keyname) + 1))) return 1;
                    strcpy(backup->next->completename, definition);
                    strcpy(backup->next->category, category);
                    strcpy(backup->next->keyname, keyname);
                    backup->next->completename[strlen(definition)] = '\0';
                    backup->next->category[strlen(category)] = '\0';
                    backup->next->keyname[strlen(keyname)] = '\0';
                    backup->next->next = list;
                    backup->next->back = backup;
                    list->back = backup->next;
                } else return 1;
            } else if ((definitionsroot = (definitionslist *) pvmalloc(sizeof (definitionslist)))) {
                definitionsroot->definitionID = definitionID;
                if (!(definitionsroot->completename = (char *) pvmalloc(strlen(definition) + 1))) return 1;
                if (!(definitionsroot->category = (char *) pvmalloc(strlen(category) + 1))) return 1;
                if (!(definitionsroot->keyname = (char *) pvmalloc(strlen(keyname) + 1))) return 1;
                strcpy(definitionsroot->completename, definition);
                strcpy(definitionsroot->category, category);
                strcpy(definitionsroot->keyname, keyname);
                definitionsroot->completename[strlen(definition)] = '\0';
                definitionsroot->category[strlen(category)] = '\0';
                definitionsroot->keyname[strlen(keyname)] = '\0';
                definitionsroot->next = list;
                definitionsroot->back = NULL;
                list->back = definitionsroot;
            } else return 1;
        } else {
            if ((definitionsroot = (definitionslist *) pvmalloc(sizeof (definitionslist)))) {
                definitionsroot->definitionID = definitionID;
                if (!(definitionsroot->completename = (char *) pvmalloc(strlen(definition) + 1))) return 1;
                if (!(definitionsroot->category = (char *) pvmalloc(strlen(category) + 1))) return 1;
                if (!(definitionsroot->keyname = (char *) pvmalloc(strlen(keyname) + 1))) return 1;
                strcpy(definitionsroot->completename, definition);
                strcpy(definitionsroot->category, category);
                strcpy(definitionsroot->keyname, keyname);
                definitionsroot->completename[strlen(definition)] = '\0';
                definitionsroot->category[strlen(category)] = '\0';
                definitionsroot->keyname[strlen(keyname)] = '\0';
                definitionsroot->back = NULL;
                definitionsroot->next = NULL;
            } else return 1;
        }
    }
    return 0;
}

int Ccharacter::deldefinition(unsigned int definitionID) {
    definitionslist *list = NULL;
    if ((list = getdefinition(definitionID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else definitionsroot = list->next;
        if (list->completename) {
            pvfree(list->completename);
            list->completename = NULL;
        }
        if (list->category) {
            pvfree(list->category);
            list->category = NULL;
        }
        if (list->keyname) {
            pvfree(list->keyname);
            list->keyname = NULL;
        }
        pvfree(list);
    } else return 1;
    return 0;
}

int Ccharacter::deldefinition(const char *completename) {
    definitionslist *list = NULL;
    if ((list = getdefinition(completename))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else definitionsroot = list->next;
        if (list->completename) {
            pvfree(list->completename);
            list->completename = NULL;
        }
        if (list->category) {
            pvfree(list->category);
            list->category = NULL;
        }
        if (list->keyname) {
            pvfree(list->keyname);
            list->keyname = NULL;
        }
        pvfree(list);
    } else return 1;
    return 0;
}

definitionslist *Ccharacter::getdefinition(unsigned int definitionID) {
    definitionslist *list = definitionsroot;
    if (definitionsroot) {
        while ((list->definitionID != definitionID) && (list->next)) list = list->next;
        if (list->definitionID != definitionID) return NULL;
    } else return NULL;
    return list;
}

definitionslist *Ccharacter::getdefinition(const char *completename) {
    definitionslist *list = definitionsroot;
    if (definitionsroot) {
        while ((compare.vcmpcase(list->completename, LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

objectslist *Ccharacter::nextvalue(void) {
    if (objectsthis) objectsthis = objectsthis->next;
    else objectsthis = objectsroot;
    return objectsthis;
}

objectslist *Ccharacter::backvalue(void) {
    if (objectsthis) objectsthis = objectsthis->back;
    else {
        objectsthis = objectsroot;
        while (objectsthis->next) objectsthis = objectsthis->next;
    }
    return objectsthis;
}

void Ccharacter::unload(void) {
    logics.unload();
    descriptions.unload();
    while (definitionsroot) {
        if (deldefinition(definitionsroot->completename) > 0) {
            LOG_ERROR("Unable to unload Definition %s", definitionsroot->completename);
            definitionsroot = definitionsroot->next;
        }
    }
    while (objectsroot) {
        if (delobject(objectsroot->object->getID()) > 0) {
            LOG_ERROR("Unable to unload Object %d", objectsroot->object->getID());
            objectsroot = objectsroot->next;
        }
    }
}
