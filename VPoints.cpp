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
#include "VPoints.h"

Cpoints::Cpoints(void) {
    pointsroot = NULL;
    pointsthis = NULL;
    if (!(information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) LOG_ERROR("Unable to allocate basicinfos");
}

int Cpoints::load(FILE *configurationfile, const char *block) {
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL, *blocktag = NULL;
    if (configurationfile) {
        if ((blocktag = (char *) pvmalloc(strlen(block) + 4))) {
            snprintf(blocktag, strlen(block) + 4, "[/%s]", block);
            if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
                while ((compare.vcmpcase(string, LSTRSIZE(blocktag)) != 0) && (!feof(configurationfile))) {
                    if (compare.vcmpcase(string, CSTRSIZE("NAME")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("Unable to edit key %s", string);
                        strncpy(information->completename, text, _DEFAULT_NAME_LEN);
                        information->completename[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("SMALL")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("Unable to edit key %s", string);
                        strncpy(information->smalldescription, text, _DEFAULT_MIND_LEN);
                        information->smalldescription[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("LARGE")) == 0) {
                        if (strings.vsscanf(string, '=', "ss", &name, &text) > 0) LOG_ERROR("Unable to edit key %s", string);
                        strncpy(information->largedescription, text, _DEFAULT_MAXD_LEN);
                        information->largedescription[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(string, CSTRSIZE("[")) == 0) {
                        if (loadcategory(configurationfile, text = strings.vget(string, '[', ']')) > 0) LOG_ERROR("Unable to edit key %s", string);
                    } else if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) LOG_ERROR("[Cpoints] %s (unknown command)", string);
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
            }
        } else return 1;
        if (blocktag) {
            pvfree(blocktag);
            block = NULL;
        };
    } else return 1;
    return 0;
}

int Cpoints::loadcategory(FILE *configurationfile, const char *category) {
    int integer;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *block = NULL;
    if (configurationfile) {
        if ((block = (char *) pvmalloc(strlen(category) + 4))) {
            snprintf(block, strlen(category) + 4, "[/%s]", category);
            if (addcategory(category) == 0) {
                if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
                    while ((compare.vcmpcase(string, LSTRSIZE(block)) != 0) && (!feof(configurationfile))) {
                        if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) {
                            if (strings.vsscanf(string, '=', "sd", &name, &integer) > 0) LOG_ERROR("Unable to edit key %s", string);
                            else if (addvalue(category, name, integer) > 0) LOG_ERROR("Unable to add %s->%s Logic", category, name);
                        }
                        if (name) {
                            pvfree(name);
                            name = NULL;
                        }
                        fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
                    }
                } else return 1;
            } else return 1;
        } else return 1;
        if (block) {
            pvfree(block);
            block = NULL;
        }
    } else return 1;
    return 0;
}

int Cpoints::save(FILE *configurationfile, const char *blockname) {
    pointslist *list = pointsroot;
    if (configurationfile) {
        fprintf(configurationfile, "[%s]\n", blockname);
        fprintf(configurationfile, "NAME=%s\nSMALL=%s\nLARGE=%s\n", information->completename, information->smalldescription, information->largedescription);
        while (list) {
            if (savecategory(configurationfile, list->completename) > 0) LOG_ERROR("Unable to save %s Category", list->completename);
            fputc('\n', configurationfile);
            list = list->next;
        }
        fprintf(configurationfile, "[/%s]", blockname);
    } else return 1;
    return 0;
}

int Cpoints::savecategory(FILE *configurationfile, const char *category) {
    pointslist *list = NULL;
    if (configurationfile) {
        if ((list = getcategory(category)))
            if (list->values->save(configurationfile, category) > 0) LOG_ERROR("Unable to save %s Category", category);
    } else return 1;
    return 0;
}

int Cpoints::copy(Cpoints &points) {
    datalist *datalist = NULL;
    pointslist *pointslist = NULL;
    points.resetvalue();
    if (information) {
        pvfree(information);
        information = NULL;
    }
    if ((information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) {
        strcpy(information->completename, points.information->completename);
        strcpy(information->smalldescription, points.information->smalldescription);
        strcpy(information->largedescription, points.information->largedescription);
        information->completename[strlen(points.information->completename)] = '\0';
        information->smalldescription[strlen(points.information->smalldescription)] = '\0';
        information->largedescription[strlen(points.information->largedescription)] = '\0';
    } else return 1;
    while ((pointslist = points.nextvalue())) {
        if (addcategory(pointslist->completename) == 0) {
            pointslist->values->resetvalue();
            while ((datalist = pointslist->values->nextvalue())) if (addvalue(pointslist->completename, datalist->completename, datalist->value) > 0) LOG_ERROR("Unable to copy %s->%s Logic", pointslist->completename, datalist->completename);
        } else LOG_ERROR("Unable to copy %s Category", pointslist->completename);
    }
    return 0;
}

int Cpoints::addcategory(const char *category) {
    pointslist *list = pointsroot;
    if (hascategory(category) != 0) {
        if (pointsroot) {
            while (list->next) list = list->next;
            if ((list->next = (pointslist *) pvmalloc(sizeof (pointslist)))) {
                if ((list->next->completename = (char *) pvmalloc(strlen(category) + 1))) {
                    strcpy(list->next->completename, category);
                    list->next->completename[strlen(category)] = '\0';
                    list->next->values = new (Cdatabank);
                    list->next->next = NULL;
                    list->next->back = list;
                } else return 1;
            } else return 1;
        } else {
            if ((pointsroot = (pointslist *) pvmalloc(sizeof (pointslist)))) {
                if ((pointsroot->completename = (char *) pvmalloc(strlen(category) + 1))) {
                    strcpy(pointsroot->completename, category);
                    pointsroot->completename[strlen(category)] = '\0';
                    pointsroot->values = new (Cdatabank);
                    pointsroot->next = NULL;
                    pointsroot->back = NULL;
                } else return 1;
            } else return 1;
        }
    }
    return 0;
}

int Cpoints::delcategory(const char *category) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else pointsroot = list->next;
        list->values->unload();
        delete(list->values);
        if (list->completename) {
            pvfree(list->completename);
            list->completename = NULL;
        }
        pvfree(list);
    } else return 1;
    return 0;
}

int Cpoints::hascategory(const char *category) {
    pointslist *list = pointsroot;
    if (pointsroot) {
        while ((list->next) && (compare.vcmpcase(list->completename, LSTRSIZE(category)) != 0)) list = list->next;
        if (compare.vcmpcase(list->completename, LSTRSIZE(category)) != 0) return 1;
    } else return 1;
    return 0;
}

pointslist *Cpoints::getcategory(const char *category) {
    pointslist *list = pointsroot;
    if (pointsroot) {
        while ((list->next) && (compare.vcmpcase(list->completename, LSTRSIZE(category)) != 0)) list = list->next;
        if (compare.vcmpcase(list->completename, LSTRSIZE(category)) != 0) return NULL;
    } else return NULL;
    return list;
}

int Cpoints::addvalue(const char *category, const char *key, int value) {
    pointslist *list = NULL;
    if (!hascategory(category) != 0) if (addcategory(category) > 0) return 1;
    if ((list = getcategory(category))) {
        if (list->values->addvalue(key, value) > 0) return 1;
    } else return 1;
    return 0;
}

int Cpoints::setvalue(const char *category, const char *key, int value) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        if (list->values->setvalue(key, value) > 0) return 1;
    } else return 1;
    return 0;
}

int Cpoints::delvalue(const char *category, const char *key, int value) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        if (list->values->delvalue(key, value) > 0) return 1;
    } else return 1;
    return 0;
}

int Cpoints::delvalue(const char *category, const char *key) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        if (list->values->delvalue(key) > 0) return 1;
    } else return 1;
    return 0;
}

int Cpoints::delvalue(const char *category, int value) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        if (list->values->delvalue(value) > 0) return 1;
    } else return 1;
    return 0;
}

int Cpoints::hasvalue(const char *category, const char *key, int value) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        return list->values->hasvalue(key, value);
    } else return 1;
    return 0;
}

int Cpoints::hasvalue(const char *category, const char *key) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        return list->values->hasvalue(key);
    } else return 1;
    return 0;
}

int Cpoints::hasvalue(const char *category, int value) {
    pointslist *list = NULL;
    if ((list = getcategory(category))) {
        return list->values->hasvalue(value);
    } else return 1;
    return 0;
}

int Cpoints::getvalue(const char *category, const char *key) {
    pointslist *list = NULL;
    int value = 0;
    if ((list = getcategory(category))) {
        try {
            value = list->values->getvalue(key);
        } catch (...) {
            throw "no value";
        }
    } else throw "no category";
    return value;
}

char *Cpoints::getvalue(const char *category, int value) {
    pointslist *list = NULL;
    if ((list = getcategory(category)))
        return list->values->getvalue(value);
    return NULL;
}

pointslist *Cpoints::nextvalue(void) {
    if (pointsthis) pointsthis = pointsthis->next;
    else pointsthis = pointsroot;
    return pointsthis;
}

pointslist *Cpoints::backvalue(void) {
    if (pointsthis) pointsthis = pointsthis->back;
    else {
        pointsthis = pointsroot;
        while (pointsthis->next) pointsthis = pointsthis->next;
    }
    return pointsthis;
}

bool Cpoints::equal(Cpoints* element) {
    pointslist *category = NULL;
    pointslist *list = pointsroot;
    while (list) {
        if (Vexcategory.hasvalue(list->completename) != 0) {
            if ((category = element->getcategory(list->completename))) {
                if (!list->values->equal(category->values)) return false;
            } else return false;
        }
        list = list->next;
    }
    return true;
}

void Cpoints::unload(void) {
    if (information) {
        pvfree(information);
        information = NULL;
    }
    while (pointsroot) {
        if (delcategory(pointsroot->completename) > 0) {
            LOG_ERROR("Unable to unload %s Category", pointsroot->completename);
            pointsroot = pointsroot->next;
        }
    }
}
