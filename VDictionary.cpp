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
#include "VDictionary.h"

Cdictionary::Cdictionary(void) {
    wordsroot = NULL;
    if (!(information = (basicinfos *) pvmalloc(sizeof (basicinfos)))) LOG_ERROR("unable to allocate basicinfos");
}

int Cdictionary::load(FILE *configurationfile) {
    char backupstring[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL, *word = NULL, *definition = NULL;
    if (configurationfile) {
        if (fgets(backupstring, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(backupstring, CSTRSIZE("[/DICTIONARY]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(backupstring, CSTRSIZE("[DICTIONARY]")) != 0) {
                    if (compare.vcmpcase(backupstring, CSTRSIZE("ID")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "sd", &name, &dictionaryID) > 0) LOG_ERROR("unable to edit key %s", backupstring);
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("NAME")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", backupstring);
                        strncpy(information->completename, text, _DEFAULT_NAME_LEN);
                        information->completename[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("SMALL")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", backupstring);
                        strncpy(information->smalldescription, text, _DEFAULT_MIND_LEN);
                        information->smalldescription[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("LARGE")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", backupstring);
                        strncpy(information->largedescription, text, _DEFAULT_MAXD_LEN);
                        information->largedescription[strlen(text) - 1] = '\0';
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("WORD")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) > 0) LOG_ERROR("unable to edit key %s", backupstring);
                        else {
                            if (text[strlen(text) - 1] == '\n') text[strlen(text) - 1] = '\0';
                            if (strings.vsscanf(text, ':', "ss", &word, &definition) > 0) LOG_ERROR("unable to edit key %s", text);
                            else if (addword(word, definition) > 0) LOG_ERROR("unable to load WORD");
                        }
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("#")) != 0) LOG_ERROR("[Cobject] %s (unknown command)", backupstring);
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                if (text) {
                    pvfree(text);
                    text = NULL;
                }
                if (word) {
                    pvfree(word);
                    word = NULL;
                }
                if (definition) {
                    pvfree(definition);
                    definition = NULL;
                }
                fgets(backupstring, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cdictionary::save(FILE *configurationfile) {
    wordslist *list = wordsroot;
    if (configurationfile) {
        fprintf(configurationfile, "[DICTIONARY]\n");
        fprintf(configurationfile, "ID=%d\n", dictionaryID);
        fprintf(configurationfile, "NAME=%s\nSMALL=%s\nLARGE=%s\n", information->completename, information->smalldescription, information->largedescription);
        while (list) {
            fprintf(configurationfile, "WORD=%s:%s\n", list->word, list->substitute);
            list = list->next;
        }
        fprintf(configurationfile, "[/DICTIONARY]");
    } else return 1;
    return 0;
}

int Cdictionary::addword(const char *word, const char *substitute) {
    wordslist *list = wordsroot;
    if (!getword(word)) {
        if (wordsroot) {
            while (list->next) list = list->next;
            if ((list->next = (wordslist *) pvmalloc(sizeof (wordslist)))) {
                if (!(list->next->word = (char *) pvmalloc(strlen(word) + 1))) return 1;
                if (!(list->next->substitute = (char *) pvmalloc(strlen(substitute) + 1))) return 1;
                strcpy(list->next->word, word);
                strcpy(list->next->substitute, substitute);
                list->next->next = NULL;
                list->next->back = list;
            } else return 1;
        } else {
            if ((wordsroot = (wordslist *) pvmalloc(sizeof (wordslist)))) {
                if (!(wordsroot->word = (char *) pvmalloc(strlen(word) + 1))) return 1;
                if (!(wordsroot->substitute = (char *) pvmalloc(strlen(substitute) + 1))) return 1;
                strcpy(wordsroot->word, word);
                strcpy(wordsroot->substitute, substitute);
                wordsroot->next = NULL;
                wordsroot->back = NULL;
            } else return 1;
        }
    } else return 1;
    return 0;
}

int Cdictionary::delword(const char *word) {
    wordslist *list = NULL;
    if ((list = getword(word))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else wordsroot = list->next;
        pvfree(list->word);
        pvfree(list->substitute);
        pvfree(list);
    } else return 1;
    return 0;
}

wordslist *Cdictionary::getword(const char *word) {
    wordslist *list = wordsroot;
    if (wordsroot) {
        while ((compare.vcmpcase(list->word, LSTRSIZE(word)) != 0) && (list->next)) list = list->next;
        if (compare.vcmpcase(list->word, LSTRSIZE(word)) != 0) return NULL;
    } else return NULL;
    return list;
}

int Cdictionary::rebuild(char **string) {
    wordslist *list = wordsroot;
    char *pointer = NULL, *backup = NULL;
    int ssl, stringlength, oldlength, replacelength;
    if (wordsroot) {
        while (list) {
            backup = *string;
            while ((pointer = strstr(backup, list->word))) {
                if (((strlen(pointer) == strlen(*string)) || ((*pointer - 1) == ' ')) &&
                        ((((strlen(*string) - strlen(pointer)) + strlen(list->word)) <= strlen(*string)) ||
                        ((*pointer + strlen(list->word)) == ' '))) {
                    oldlength = strlen(list->word);
                    replacelength = strlen(list->substitute);
                    stringlength = strlen(*string);
                    ssl = stringlength - oldlength + replacelength;
                    if (ssl > stringlength) if (!(*string = (char *) pvrealloc(*string, ssl + 1))) return false;
                    if (replacelength == oldlength) {
                        memcpy(pointer, list->substitute, replacelength);
                    } else {
                        memmove(pointer + replacelength, pointer + oldlength, strlen(pointer + oldlength) + 1);
                        memcpy(pointer, list->substitute, replacelength);
                    }
                    if (ssl < stringlength) if (!(*string = (char *) pvrealloc(*string, ssl + 1))) return false;
                }
                backup++;
            }
            list = list->next;
        }
    } else return 1;
    return 0;
}

void Cdictionary::unload(void) {
    if (information) {
        pvfree(information);
        information = NULL;
    }
    while (wordsroot) {
        if (delword(wordsroot->word) > 0) {
            LOG_ERROR("unable to unload Word %s", wordsroot->word);
            wordsroot = wordsroot->next;
        }
    }
}
