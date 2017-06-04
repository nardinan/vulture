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
#include "VPages.h"

Cpages::Cpages(void) {
    elements = 0;
    pagesroot = NULL;
}

int Cpages::load(FILE *configurationfile) {
    char string[_DEFAULT_MIND_LEN + _DEFAULT_MAXD_LEN + 1], *completename = NULL, *pagedescription = NULL;
    if (fgets(string, _DEFAULT_MIND_LEN + _DEFAULT_MAXD_LEN, configurationfile)) {
        while ((compare.vcmpcase(string, CSTRSIZE("[/PAGES]")) != 0) && (!feof(configurationfile))) {
            string[strlen(string) - 1] = '\0';
            if ((compare.vcmpcase(string, CSTRSIZE("#")) != 0) && (compare.vcmpcase(string, CSTRSIZE("[PAGES]")) != 0)) {
                if (strings.vsscanf(string, '=', "ss", &completename, &pagedescription) == 0)
                    if (addpage(completename, pagedescription) > 0) return 1;
            }
            if (completename) {
                pvfree(completename);
                completename = NULL;
            }
            if (pagedescription) {
                pvfree(pagedescription);
                pagedescription = NULL;
            }
            fgets(string, _DEFAULT_MIND_LEN + _DEFAULT_MAXD_LEN, configurationfile);
        }
    } else return 1;
    return 0;
}

int Cpages::save(FILE *configurationfile) {
    pageslist *list = pagesroot;
    if (configurationfile) {
        fprintf(configurationfile, "[PAGES]\n");
        while (list) {
            fprintf(configurationfile, "%s=%s\n", list->completename, list->page);
            list = list->next;
        }
        fprintf(configurationfile, "[/PAGES]");
    } else return 1;
    return 0;
}

int Cpages::addpage(const char *completename, const char *page) {
    pageslist *list = pagesroot;
    if (pagesroot) {
        while (list->next) list = list->next;
        if ((list->next = (pageslist *) pvmalloc(sizeof (pageslist)))) {
            if (!(list->next->completename = (char *) pvmalloc(strlen(completename) + 1))) return 1;
            if (!(list->next->page = (char *) pvmalloc(strlen(page) + 1))) return 1;
            strcpy(list->next->completename, completename);
            strcpy(list->next->page, page);
            list->next->completename[strlen(completename)] = '\0';
            list->next->page[strlen(page)] = '\0';
            list->next->next = NULL;
            list->next->back = list;
            elements++;
        } else return 1;
    } else {
        if ((pagesroot = (pageslist *) pvmalloc(sizeof (pageslist)))) {
            if (!(pagesroot->completename = (char *) pvmalloc(strlen(completename) + 1))) return 1;
            if (!(pagesroot->page = (char *) pvmalloc(strlen(page) + 1))) return 1;
            strcpy(pagesroot->completename, completename);
            strcpy(pagesroot->page, page);
            pagesroot->completename[strlen(completename)] = '\0';
            pagesroot->page[strlen(page)] = '\0';
            pagesroot->next = NULL;
            pagesroot->back = NULL;
            elements++;
        } else return 1;
    }
    return 0;
}

int Cpages::delpage(void) {
    pageslist *list = pagesroot;
    if (pagesroot) {
        pagesroot = list->next;
        if (pagesroot) pagesroot->back = NULL;
        pvfree(list->completename);
        pvfree(list->page);
        pvfree(list);
        elements--;
    } else return 1;
    return 0;
}

char *Cpages::getpage(bool remove) {
    int length;
    char *string = NULL;
    if (pagesroot) {
        length = strlen(pagesroot->completename) + strlen(pagesroot->page) + strlen("[reset] ha pensato: [green]\"\"[reset][n]");
        if ((string = (char *) pvmalloc(length + 1))) {
            snprintf(string, length + 1, "[reset]%s ha pensato: [green]\"%s\"[reset][n]", pagesroot->completename, pagesroot->page);
            if (remove) {
                if (delpage() > 0) return NULL;
            }
        } else return NULL;
    } else {
        if ((string = (char *) pvmalloc((length = strlen("non ci sono pensieri in archivio![n]")) + 1))) {
            strcpy(string, "non ci sono pensieri in archivio![n]");
        } else return NULL;
    }
    string[length] = '\0';
    return string;
}

void Cpages::unload(void) {
    while (pagesroot) {
        if (delpage() > 0) {
            LOG_ERROR("unable to unload Page from %s", pagesroot->completename);
            pagesroot = pagesroot->next;
        }
    }
}
