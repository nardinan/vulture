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
#include "VDescriptions.h"

int Cdescriptions::load(FILE *configurationfile, const char *blockname) {
    int integer;
    char string[_DEFAULT_MAXD_LEN + _DEFAULT_NAME_LEN], *completename = NULL, *smalldescription = NULL, *largedescription = NULL, *block = NULL;
    if (configurationfile) {
        if ((block = (char *) pvmalloc(strlen(blockname) + 4))) {
            snprintf(block, strlen(blockname) + 4, "[/%s]", blockname);
            if (fgets(string, _DEFAULT_NAME_LEN + _DEFAULT_MAXD_LEN + 1, configurationfile)) {
                while ((compare.vcmpcase(string, LSTRSIZE(block)) != 0) && (!feof(configurationfile))) {
                    if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[DESCRIPTIONS]")) != 0)) {
                        if (strings.vsscanf(string, ':', "sssd", &completename, &smalldescription, &largedescription, &integer) == 0)
                            if (adddescription(integer, completename, smalldescription, largedescription) > 0) 
                                return 1;
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
                    fgets(string, _DEFAULT_MAXD_LEN + _DEFAULT_NAME_LEN, configurationfile);
                }
            } else 
                return 1;
        } else 
            return 1;
        if (block) {
            pvfree(block);
            block = NULL;
        };
    } else 
        return 1;
    return 0;
}

int Cdescriptions::save(FILE *configurationfile, const char *blockname) {
    informationlist *list = informationroot;
    if (configurationfile) {
        fprintf(configurationfile, "[%s]\n", blockname);
        while (list) {
            fprintf(configurationfile, "%s:%s:%s:%d\n", list->information->completename, list->information->smalldescription, list->information->largedescription, list->informationID);
            list = list->next;
        }
        fprintf(configurationfile, "[/%s]", blockname);
    } else return 1;
    return 0;
}

int Cdescriptions::copy(Cdescriptions &descriptions) {
    informationlist *list = NULL;
    if ((list = descriptions.getdescriptionsroot())) {
        while (list) {
            if (adddescription(list->informationID, descriptions.completename(list->informationID),
                    descriptions.smalldescription(list->informationID),
                    descriptions.largedescription(list->informationID)) > 0) {
                LOG_ERROR("unable to copy Description %d", list->informationID);
                return 1;
            }
            list = list->next;
        }
    }
    return 0;
}

int Cdescriptions::adddescription(unsigned int informationID, const char *completename, const char *smalldescription, const char *largedescription) {
    informationlist *list = NULL;
    if (getdescription(informationID))
        if (deldescription(informationID) > 0) LOG_ERROR("unable to delete Description %d", informationID);
    list = informationroot;
    if (informationroot) {
        while (list->next) list = list->next;
        if ((list->next = (informationlist *) pvmalloc(sizeof (informationlist)))) {
            if (!(list->next->information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) 
                return 1;
            strncpy(list->next->information->completename, completename, _DEFAULT_NAME_LEN);
            strncpy(list->next->information->smalldescription, smalldescription, _DEFAULT_MIND_LEN);
            strncpy(list->next->information->largedescription, largedescription, _DEFAULT_MAXD_LEN);
            list->next->information->completename[numbers.vmin(strlen(completename), (_DEFAULT_NAME_LEN - 1))] = '\0';
            list->next->information->smalldescription[numbers.vmin(strlen(smalldescription), (_DEFAULT_MIND_LEN - 1))] = '\0';
            list->next->information->largedescription[numbers.vmin(strlen(largedescription), (_DEFAULT_MAXD_LEN - 1))] = '\0';
            list->next->informationID = informationID;
            list->next->next = NULL;
            list->next->back = list;
        } else
            return 1;
    } else {
        if ((informationroot = (informationlist *) pvmalloc(sizeof (informationlist)))) {
            if (!(informationroot->information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) return 1;
            strncpy(informationroot->information->completename, completename, _DEFAULT_NAME_LEN);
            strncpy(informationroot->information->smalldescription, smalldescription, _DEFAULT_MIND_LEN);
            strncpy(informationroot->information->largedescription, largedescription, _DEFAULT_MAXD_LEN);
            informationroot->information->completename[strlen(completename)] = '\0';
            informationroot->information->smalldescription[strlen(smalldescription)] = '\0';
            informationroot->information->largedescription[strlen(largedescription)] = '\0';
            informationroot->information->completename[numbers.vmin(strlen(completename), (_DEFAULT_NAME_LEN - 1))] = '\0';
            informationroot->information->smalldescription[numbers.vmin(strlen(smalldescription), (_DEFAULT_MIND_LEN - 1))] = '\0';
            informationroot->information->largedescription[numbers.vmin(strlen(largedescription), (_DEFAULT_MAXD_LEN - 1))] = '\0';
            informationroot->informationID = informationID;
            informationroot->next = NULL;
            informationroot->back = NULL;
        } else 
            return 1;
    }
    return 0;
}

int Cdescriptions::deldescription(unsigned int informationID) {
    informationlist *list = NULL;
    if ((list = getdescription(informationID))) {
        if (list->next) 
            list->next->back = list->back;
        if (list->back) 
            list->back->next = list->next;
        else 
            informationroot = list->next;
        pvfree(list->information);
        pvfree(list);
    } else 
        return 1;
    return 0;
}

informationlist *Cdescriptions::getdescription(unsigned int informationID) {
    informationlist *list = informationroot;
    if (informationroot) {
        while ((list->informationID != informationID) && (list->next)) 
            list = list->next;
        if (list->informationID != informationID) 
            return NULL;
    } else 
        return NULL;
    return list;
}

char *Cdescriptions::completename(unsigned int informationID) {
    informationlist *list = NULL;
    if ((list = getdescription(informationID))) 
        return list->information->completename;
    return NULL;
}

char *Cdescriptions::smalldescription(unsigned int informationID) {
    informationlist *list = NULL;
    if ((list = getdescription(informationID))) 
        return list->information->smalldescription;
    return NULL;
}

char *Cdescriptions::largedescription(unsigned int informationID) {
    informationlist *list = NULL;
    if ((list = getdescription(informationID))) 
        return list->information->largedescription;
    return NULL;
}

void Cdescriptions::unload(void) {
    while (informationroot) {
        if (deldescription(informationroot->informationID) > 0) {
            LOG_ERROR("unable to unload Description %d", informationroot->informationID);
            informationroot = informationroot->next;
        }
    }
}
