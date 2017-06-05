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
#include "VLObjects.h"
#include "VObject.h"

Cobject::Cobject(void) {
    objectsroot = NULL;
    objectsthis = NULL;
}

int Cobject::load(FILE *configurationfile) {
    int integer;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        descriptions.deldescription(0);
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/OBJECT]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[OBJECT]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("ID")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &objectID) != 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) == 0) {
                        if (descriptions.load(configurationfile) > 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("SEX")) == 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &integer) > 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                        sex = (sexdefinition) integer;
                    } else if (compare.vcmpcase(string, CSTRSIZE("NOBJECT")) == 0)
                        objectID = NobjectID++;
                    else if (compare.vcmpcase(string, CSTRSIZE("[POINTS]")) == 0) {
                        if (logics.load(configurationfile) > 0) 
                            LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0)
                        LOG_ERROR("[Cobject] %s (unkown command)", string);
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
        } else 
            return 1;
    } else
        return 1;
    return 0;
}

int Cobject::loadskeleton(const char *configurationfile, unsigned int objectID) {
    FILE *skeleton = fopen(configurationfile, "r");
    char string[_DEFAULT_MAXD_LEN];
    if (skeleton) {
        if (fgets(string, _DEFAULT_MAXD_LEN, skeleton)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/OBJECT]")) != 0) && (!feof(skeleton))) {
                if (compare.vcmpcase(string, CSTRSIZE("[OBJECT]")) == 0) {
                    if (load(skeleton) > 0) 
                        LOG_ERROR("Unable to load OBJECT'S shell");
                } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) 
                    LOG_ERROR("[Cobject] %s (unknown command)", string);
                fgets(string, _DEFAULT_MAXD_LEN, skeleton);
            }
            this->objectID = objectID;
            if (descriptions.adddescription(0, "###", "###", "###") > 0) 
                return 1;
        } else 
            return 1;
        fclose(skeleton);
    } else 
        return 1;
    return 0;
}

int Cobject::save(FILE *configurationfile) {
    if (configurationfile) {
        fprintf(configurationfile, "[OBJECT]\n");
        fprintf(configurationfile, "ID=%d\n", objectID);
        if (descriptions.save(configurationfile) > 0) 
            LOG_ERROR("Unable to save DESCRIPTIONS of Object %d", objectID);
        fputc('\n', configurationfile);
        if (logics.save(configurationfile) > 0) 
            LOG_ERROR("Unable to save LOGICS of Object %d", objectID);
        fprintf(configurationfile, "\n[/OBJECT]");
    } else 
        return 1;
    return 0;
}

int Cobject::copy(Cobject *object) {
    this->objectID = NobjectID++;
    if (logics.copy(object->logics) > 0) 
        LOG_ERROR("Unable to copy LOGICS of Object %d", objectID);
    if (descriptions.copy(object->descriptions) > 0)
        LOG_ERROR("Unable to copy DESCRIPTIONS of Object %d", objectID);
    return 0;
}

int Cobject::addobject(Cobject *object) {
    objectslist *list = objectsroot, *backup = NULL;
    if (!getobject(object->getID())) {
        if (objectsroot) {
            if (list->object->logics.hasvalue("SYSTEM", "Parent") == 0) {
                while ((list->next) && (list->object->logics.getvalue("SYSTEM", "Parent") != object->logics.getvalue("SYSTEM", "Parent"))) {
                    list = list->next;
                    if (list->object->logics.hasvalue("SYSTEM", "Parent") != 0) {
                        LOG_ERROR("Unable to find SYSTEM->Parent Logic");
                        break;
                    }
                }
            } else
                LOG_ERROR("Unable to find SYSTEM->Parent Logic");
            if (list->next) {
                backup = list->next;
                if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                    list->next->object = object;
                    list->next->back = list;
                    list->next->next = backup;
                    backup->back = list->next;
                } else
                    return 1;
            } else if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                list->next->object = object;
                list->next->back = list;
                list->next->next = NULL;
            } else
                return 1;
        } else {
            if ((objectsroot = (objectslist *) pvmalloc(sizeof (objectslist)))) {
                objectsroot->object = object;
                objectsroot->back = NULL;
                objectsroot->next = NULL;
            } else
                return 1;
        }
    } else
        return 1;
    return 0;
}

int Cobject::delobject(unsigned int objectID) {
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

int Cobject::delobject(const char *completename, unsigned int position) {
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

objectslist *Cobject::getobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while (((unsigned int) list->object->getID() != objectID) && (list->next))
            list = list->next;
        if ((unsigned int) list->object->getID() != objectID) 
            return NULL;
    } else 
        return NULL;
    return list;
}

objectslist *Cobject::getobject(const char *completename, unsigned int position) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) 
            list = list->next;
        if (compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) 
            return NULL;
    } else 
        return NULL;
    return list;
}

void Cobject::deldependences(void) {
    while (objectsroot) {
        if (delobject(objectsroot->object->getID()) > 0) {
            LOG_ERROR("Unable to unload Object %d", objectsroot->object->getID());
            objectsroot = objectsroot->next;
        }
    }
}

objectslist *Cobject::nextvalue(void) {
    if (objectsthis) 
        objectsthis = objectsthis->next;
    else
        objectsthis = objectsroot;
    return objectsthis;
}

objectslist *Cobject::backvalue(void) {
    if (objectsthis) objectsthis = objectsthis->back;
    else {
        objectsthis = objectsroot;
        while (objectsthis->next) 
            objectsthis = objectsthis->next;
    }
    return objectsthis;
}

void Cobject::unload(void) {
    logics.unload();
    descriptions.unload();
    while (objectsroot) {
        if (delobject(objectsroot->object->getID()) > 0) {
            LOG_ERROR("Unable to unload Object %d", objectsroot->object->getID());
            objectsroot = objectsroot->next;
        }
    }
}
