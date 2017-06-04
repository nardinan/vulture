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
#include "VDatabank.h"

int Cdatabank::addvalue(const char *completename, int value) {
    datalist *list = dataroot;
    if (hasvalue(completename) != 0) {
        if (dataroot) {
            while (list->next != NULL) list = list->next;
            if ((list->next = (datalist *) pvmalloc(sizeof (datalist)))) {
                if (!(list->next->completename = (char *) pvmalloc(strlen(completename) + 1))) return 1;
                strcpy(list->next->completename, completename);
                list->next->completename[strlen(completename)] = '\0';
                list->next->value = value;
                list->next->next = NULL;
                list->next->back = list;
                elements++;
            } else return 1;
        } else {
            if ((dataroot = (datalist *) pvmalloc(sizeof (datalist)))) {
                if (!(dataroot->completename = (char *) pvmalloc(strlen(completename) + 1))) return 1;
                strcpy(dataroot->completename, completename);
                dataroot->completename[strlen(completename)] = '\0';
                dataroot->value = value;
                dataroot->next = NULL;
                dataroot->back = NULL;
                elements++;
            } else return 1;
        }
    } else {
        while ((compare.vcmpcase(list->completename, completename) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, completename) == 0) list->value = value;
        else return 1;
    }
    return 0;
}

int Cdatabank::setvalue(const char *completename, int value) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((compare.vcmpcase(list->completename, completename) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, completename) == 0) {
            list->value += value;
        } else return 1;
    } else return 1;
    return 0;
}

int Cdatabank::delvalue(const char *key, int value) {
    datalist *list = dataroot;
    if (dataroot) {
        while (((compare.vcmpcase(list->completename, key) != 0) || (list->value != value)) && (list->next)) list = list->next;
        if ((compare.vcmpcase(list->completename, key) == 0) && (list->value == value)) {
            if (list->next) list->next->back = list->back;
            if (list->back) list->back->next = list->next;
            else dataroot = list->next;
            pvfree(list->completename);
            pvfree(list);
            elements--;
        } else return 1;
    } else return 1;
    return 0;
}

int Cdatabank::delvalue(const char *completename) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((compare.vcmpcase(list->completename, completename) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, completename) == 0) {
            if (list->next) list->next->back = list->back;
            if (list->back) list->back->next = list->next;
            else dataroot = list->next;
            pvfree(list->completename);
            pvfree(list);
            elements--;
        } else return 1;
    } else return 1;
    return 0;
}

int Cdatabank::delvalue(int value) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((list->value != value) && (list->next)) list = list->next;
        if (list->value == value) {
            if (list->next) list->next->back = list->back;
            if (list->back) list->back->next = list->next;
            else dataroot = list->next;
            pvfree(list->completename);
            pvfree(list);
            elements--;
        } else return 1;
    } else return 1;
    return 0;
}

int Cdatabank::getvalue(const char *completename) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((compare.vcmpcase(list->completename, completename) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, completename) != 0) throw "Unable to find value";
    } else throw "Unable to find value";
    return list->value;
}

char *Cdatabank::getvalue(int value) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((list->value != value) && (list->next)) list = list->next;
        if (list->value != value) return NULL;
    } else return NULL;
    return list->completename;
}

int Cdatabank::hasvalue(const char *completename, int value) {
    datalist *list = dataroot;
    if (dataroot) {
        while (((compare.vcmpcase(list->completename, completename) != 0) || (list->value != value)) && (list->next)) list = list->next;
        if ((compare.vcmpcase(list->completename, completename) != 0) || (list->value != value)) return 1;
    } else return 1;
    return 0;
}

int Cdatabank::hasvalue(const char *completename) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((compare.vcmpcase(list->completename, completename) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->completename, completename) != 0) return 1;
    } else return 1;
    return 0;
}

int Cdatabank::hasvalue(int value) {
    datalist *list = dataroot;
    if (dataroot) {
        while ((list->value != value) && (list->next)) list = list->next;
        if (list->value != value) return 1;
    } else return 1;
    return 0;
}

int Cdatabank::load(FILE *configurationfile, const char *blockname) {
    int value;
    char string[_DEFAULT_MAXD_LEN], *name = NULL, *block = NULL;
    if (configurationfile) {
        if ((block = (char *) pvmalloc(strlen(blockname) + 4))) {
            snprintf(block, strlen(blockname) + 4, "[/%s]", blockname);
            if (fgets(string, _DEFAULT_MAXD_LEN, configurationfile)) {
                while ((compare.vcmpcase(string, LSTRSIZE(block)) != 0) && (!feof(configurationfile))) {
                    if (compare.vcmpcase(string, CSTRSIZE("#")) != 0) {
                        if (strings.vsscanf(string, '=', "sd", &name, &value) > 0) LOG_ERROR("Unable to edit key %s", string);
                        else this->addvalue(name, value);
                    }
                    if (name) {
                        pvfree(name);
                        name = NULL;
                    }
                    fgets(string, _DEFAULT_MAXD_LEN, configurationfile);
                }
            }
        } else return 1;
        if (block) {
            pvfree(block);
            block = NULL;
        }
    } else return 1;
    return 0;
}

int Cdatabank::save(FILE *configurationfile, const char *blockname) {
    datalist *list = dataroot;
    if (configurationfile) {
        fprintf(configurationfile, "[%s]\n", blockname);
        while (list) {
            fprintf(configurationfile, "%s=%d\n", list->completename, list->value);
            list = list->next;
        }
        fprintf(configurationfile, "[/%s]", blockname);
    } else return 1;
    return 0;
}

datalist *Cdatabank::nextvalue(void) {
    if (datathis) datathis = datathis->next;
    else datathis = dataroot;
    return datathis;
}

datalist *Cdatabank::backvalue(void) {
    if (datathis) datathis = datathis->back;
    else {
        datathis = dataroot;
        while (datathis->next) datathis = datathis->next;
    }
    return datathis;
}

bool Cdatabank::equal(Cdatabank *element) {
    datalist *list = dataroot;
    while (list) {
        if (element->hasvalue(list->completename, list->value) != 0) return false;
        list = list->next;
    }
    return true;
}

void Cdatabank::unload(void) {
    datathis = NULL;
    if (dataroot) {
        while (dataroot) {
            if (delvalue(dataroot->completename) > 0) {
                LOG_ERROR("Unable to unload %s Logic", dataroot->completename);
                dataroot = dataroot->next;
            }
        }
        elements = 0;
    }
}
