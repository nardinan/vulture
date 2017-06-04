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

int Cobjectslist::load(FILE *configurationfile) {
    char string[_DEFAULT_MAXD_LEN];
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/OBJECTS]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("[OBJECTS]")) != 0) {
                    if (compare.vcmpcase(string, CSTRSIZE("[OBJECT]")) == 0) {
                        if (!addobject(configurationfile)) LOG_ERROR("Unable to load an OBJECT");
                    } else if (compare.vcmpcase(string, CSTRSIZE("[DEPENDENCES]")) == 0) {
                        if (loaddependences(configurationfile) > 0) LOG_ERROR("Unable to load DEPENDENCES");
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cobjectslist] %s (unknown command)", string);
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cobjectslist::loaddependences(FILE *configurationfile) {
    objectslist *objectcontainer = NULL, *objectcontained = NULL;
    int container, contained;
    char string[_DEFAULT_MAXD_LEN];
    if (configurationfile) {
        if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(string, CSTRSIZE("[/DEPENDENCES]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) {
                    if (strings.vsscanf(string, ':', "dd", &container, &contained) == 0) {
                        if ((objectcontainer = getobject(container))) {
                            if ((objectcontained = getobject(contained))) {
                                if (objectcontainer->object->addobject(objectcontained->object) != 0) LOG_ERROR("Unable to add OBJECT %d into OBJECT %d", contained, container);
                            } else LOG_ERROR("Unable to find Object %d", contained);
                        } else LOG_ERROR("Unable to find Object %d", container);
                    } else return 1;
                }
                fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cobjectslist::save(FILE *configurationfile) {
    objectslist *list = objectsroot;
    if (configurationfile) {
        fprintf(configurationfile, "[OBJECTS]\n");
        while (list) {
            if (compare.vcmpcase(list->object->descriptions.completename(0), CSTRSIZE("###")) != 0) {
                if (list->object->save(configurationfile) > 0) LOG_ERROR("Unable to save Object %d", list->object->getID());
                else fputc('\n', configurationfile);
            }
            list = list->next;
        }
        if (savedependences(configurationfile) > 0) return 1;
        fprintf(configurationfile, "\n[/OBJECTS]");
    } else return 1;
    return 0;
}

int Cobjectslist::savedependences(FILE *configurationfile) {
    objectslist *objectlist = objectsroot, *containedlist = NULL;
    if (configurationfile) {
        fprintf(configurationfile, "[DEPENDENCES]\n");
        while (objectlist) {
            if (compare.vcmpcase(objectlist->object->descriptions.completename(0), CSTRSIZE("###")) != 0) {
                objectlist->object->resetvalue();
                while ((containedlist = objectlist->object->nextvalue())) {
                    fprintf(configurationfile, "%d:%d\n", objectlist->object->getID(), containedlist->object->getID());
                    containedlist = containedlist->next;
                }
            }
            objectlist = objectlist->next;
        }
        fprintf(configurationfile, "[/DEPENDENCES]");
    } else return 1;
    return 0;
}

objectslist * Cobjectslist::addobject(FILE *configurationfile) {
    objectslist *list = objectsroot, *present = NULL;
    if (objectsroot) {
        while (list->next) list = list->next;
        if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
            list->next->object = new (Cobject);
            list->next->back = list;
            list->next->next = NULL;
            if (list->next->object->load(configurationfile) > 0) return NULL;
            present = list->next;
        } else return NULL;
    } else {
        if ((objectsroot = (objectslist *) pvmalloc(sizeof (objectslist)))) {
            objectsroot->object = new (Cobject);
            objectsroot->back = NULL;
            objectsroot->next = NULL;
            if (objectsroot->object->load(configurationfile) > 0) return NULL;
            present = objectsroot;
        } else return NULL;
    }
    return present;
}

int Cobjectslist::addobject(const char *configurationfile, unsigned int objectID) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while (list->next) list = list->next;
        if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
            list->next->object = new (Cobject);
            list->next->back = list;
            list->next->next = NULL;
            if (list->next->object->loadskeleton(configurationfile, objectID) > 0) return 1;
        } else return 1;
    } else {
        if ((objectsroot = (objectslist *) pvmalloc(sizeof (objectslist)))) {
            objectsroot->object = new (Cobject);
            objectsroot->back = NULL;
            objectsroot->next = NULL;
            if (objectsroot->object->loadskeleton(configurationfile, objectID) > 0) return 1;
        } else return 1;
    }
    return 0;
}

objectslist *Cobjectslist::addobject(Cobject *object) {
    objectslist *list = objectsroot, *backuplist = NULL, *present = NULL;
    if (objectsroot) {
        while (list->next) list = list->next;
        if ((list->next = (objectslist *) pvmalloc(sizeof (objectslist)))) {
            list->next->object = new (Cobject);
            list->next->object->setID(NobjectID++);
            list->next->object->setsex(object->getsex());
            list->next->next = NULL;
            list->next->back = list;
            if (list->next->object->logics.copy(object->logics) > 0) LOG_ERROR("Unable to copy Logics %s", object->logics.information->completename);
            if (list->next->object->descriptions.copy(object->descriptions) > 0) LOG_ERROR("Unable to copy Descriptions");
            object->resetvalue();
            while ((backuplist = object->nextvalue())) if (!list->next->object->addobject(backuplist->object)) LOG_ERROR("Unable to add an OBJECT into OBJECT %d", backuplist->object->getID());
            present = list->next;
        } else return NULL;
    } else {
        if ((objectsroot = (objectslist *) pvmalloc(sizeof (objectslist)))) {
            objectsroot->object = new (Cobject);
            objectsroot->object->setID(NobjectID++);
            objectsroot->object->setsex(object->getsex());
            objectsroot->next = NULL;
            objectsroot->back = NULL;
            if (objectsroot->object->logics.copy(object->logics) > 0) LOG_ERROR("Unable to copy Logics %s", object->logics.information->completename);
            if (objectsroot->object->descriptions.copy(object->descriptions) > 0) LOG_ERROR("Unable to copy Descriptions");
            object->resetvalue();
            while ((backuplist = object->nextvalue())) if (!objectsroot->object->addobject(backuplist->object)) LOG_ERROR("Unable to add an OBJECT into OBJECT %d", backuplist->object->getID());
            present = objectsroot;
        } else return NULL;
    }
    return present;
}

int Cobjectslist::delobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if ((list = getobject(objectID))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else objectsroot = list->next;
        list->object->unload();
        delete(list->object);
        pvfree(list);
    } else return 1;
    return 0;
}

objectslist *Cobjectslist::getobject(unsigned int objectID) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while (((unsigned int) list->object->getID() != objectID) && (list->next)) list = list->next;
        if ((unsigned int) list->object->getID() != objectID) return NULL;
    } else return NULL;
    return list;
}

objectslist *Cobjectslist::getobject(const char *completename, unsigned int position) {
    objectslist *list = objectsroot;
    if (objectsroot) {
        while ((compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->object->descriptions.completename(position), LSTRSIZE(completename)) != 0) return NULL;
    } else return NULL;
    return list;
}

void Cobjectslist::unload(void) {
    objectslist *list = objectsroot;
    while (list) {
        list->object->deldependences();
        list = list->next;
    }
    while (objectsroot) {
        if (delobject(objectsroot->object->getID()) > 0) {
            LOG_ERROR("Unable to unload Object %d", objectsroot->object->getID());
            objectsroot = objectsroot->next;
        }
    }
}
