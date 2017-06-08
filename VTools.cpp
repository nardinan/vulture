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
#include "VTools.h"

int Cstrings::vsscanf(const char *string, char divider, const char *format, ...) {
    va_list arguments;
    double *backupdouble;
    char integer[_DEFAULT_MIND_LEN], *position = NULL, **backupstring = NULL;
    int *backupinteger, index, length = strlen(format), numargs = 0;
    va_start(arguments, format);
    for (index = 0; index < length; index++) {
        switch (format[index]) {
            case 's':
                if (!(position = (char *) strchr(string, divider))) position = (char *) string + strlen(string);
                backupstring = va_arg(arguments, char**);
                if ((*backupstring = (char *) pvmalloc((position - string) + 1))) {
                    memcpy(*backupstring, string, position - string);
                    numargs += 1;
                } else return 1;
                break;
            case 'f':
                if (!(position = (char *) strchr(string, divider))) position = (char *) string + strlen(string);
                backupdouble = va_arg(arguments, double*);
                memcpy(integer, string, position - string);
                integer[position - string] = '\0';
                *backupdouble = atof(integer);
                numargs += 1;
                break;
            case 'd':
                if (!(position = (char *) strchr(string, divider))) position = (char *) string + strlen(string);
                backupinteger = va_arg(arguments, int*);
                memcpy(integer, string, position - string);
                integer[position - string] = '\0';
                *backupinteger = atoi(integer);
                numargs += 1;
        }
        if ((*position != '\0') && (*position != '\n')) string = position + 1;
        else break;
    }
    va_end(arguments);
    return (numargs != length) ? 1 : 0;
}

int Cstrings::voccurrence(const char *string, const char *word) {
    int index = 0;
    char *pointer = (char *) string;
    while ((pointer = strstr(pointer, word))) {
        index++;
        pointer += strlen(word);
    }
    return index;
}

int Cstrings::vpush(char **string, const char *word) {
    char *pointer;
    int stringlength = strlen(*string), wordlength = strlen(word);
    if (word) {
        if ((*string = (char *) pvrealloc(*string, stringlength + wordlength + 1))) {
            pointer = *string;
            memmove(pointer + wordlength, pointer, stringlength);
            memcpy(pointer, word, wordlength);
        } else return 1;
    } else return 1;
    return 0;
}

char *Cstrings::vpop(char **string) {
    char *pointer = *string, *word;
    int ssl, stringlength = 0, wordlength = 0, spaces = 0;
    if (*string) {
        while ((strlen(pointer) > 0) && (*++pointer != ' '));
        while ((strlen(pointer) > 0) && (*pointer == ' ')) {
            spaces++;
            pointer++;
        }
        stringlength = strlen(*string);
        wordlength = stringlength - strlen(pointer);
        ssl = strlen(pointer);
        if ((word = (char *) pvmalloc((wordlength - spaces) + 1))) {
            strncpy(word, *string, (wordlength - spaces));
            word[(wordlength - spaces)] = '\0';
        } else return NULL;
        pointer = *string;
        memmove(pointer, pointer + wordlength, (stringlength - wordlength) + 1);
        if ((ssl < stringlength) && (ssl > 0)) {
            if (!(*string = (char *) pvrealloc(*string, ssl + 1))) {
                throw "unable to realloc *string";
                return NULL;
            }
        } else if (ssl == 0) {
            pvfree(*string);
            *string = NULL;
        }
    } else return NULL;
    return word;
}

char *Cstrings::vpop(char **string, unsigned int length) {
    char *pointer = *string, *word;
    int ssl, stringlength = 0, wordlength = length;
    if (*string) {
        stringlength = strlen(*string);
        if (wordlength > stringlength) wordlength = strlen(*string);
        pointer += wordlength;
        ssl = strlen(pointer);
        if ((word = (char *) pvmalloc(wordlength + 1))) {
            strncpy(word, *string, (wordlength));
            word[wordlength] = '\0';
        } else return NULL;
        pointer = *string;
        memmove(pointer, pointer + wordlength, (stringlength - wordlength) + 1);
        if ((ssl < stringlength) && (ssl > 0)) {
            if (!(*string = (char *) pvrealloc(*string, ssl + 1))) {
                throw "unable to realloc *string";
                return NULL;
            }
        } else if (ssl == 0) {
            pvfree(*string);
            *string = NULL;
        }
    } else return NULL;
    return word;
}

char *Cstrings::vget(char *string, const char bchar, const char fchar) {
    char *init = NULL, *end = NULL, *result = NULL;
    if ((string) &&
            (init = strchr(string, bchar)) &&
            (end = strchr(++init, fchar))) {
        if ((result = (char *) pvmalloc((end - init) + 1))) {
            memcpy(result, init, (end - init));
            result[end - init] = '\0';
        } else return NULL;
    } else return NULL;
    return result;
}

bool Cstrings::vreplace(char **string, const char *old, const char *replace) {
    char *pointer;
    int index = 0, ssl, stringlength = strlen(*string), oldlength = strlen(old), replacelength = strlen(replace);
    for (index = 0, pointer = *string; (pointer = strstr(pointer, old)); index++, pointer++);
    if (index > 0) {
        pointer = *string;
        ssl = stringlength - (oldlength * index)+(replacelength * index);
        if (ssl > stringlength) if (!(*string = (char *) pvrealloc(*string, ssl + 1))) return false;
        if (replacelength == oldlength) {
            if (memcmp(old, replace, replacelength))
                while ((pointer = strstr(*string, old))) memcpy(pointer, replace, replacelength);
        } else {
            while ((pointer = strstr(*string, old))) {
                memmove(pointer + replacelength, pointer + oldlength, strlen(pointer + oldlength) + 1);
                memcpy(pointer, replace, replacelength);
                pointer += replacelength;
            }
        }
        if (ssl < stringlength) if (!(*string = (char *) pvrealloc(*string, ssl + 1))) return false;
    } else return false;
    return true;
}

bool Cstrings::vsreplace(char **string, const char *old, const char *replace) {
    char *pointer;
    int ssl, stringlength = strlen(*string), oldlength = strlen(old), replacelength = strlen(replace);
    if ((pointer = strstr(*string, old))) {
        pointer = *string;
        ssl = stringlength - oldlength + replacelength;
        if (ssl > stringlength) if (!(*string = (char *) pvrealloc(*string, ssl + 1))) return false;
        if (replacelength == oldlength) {
            if (memcmp(old, replace, replacelength))
                if ((pointer = strstr(*string, old))) memcpy(pointer, replace, replacelength);
        } else {
            if ((pointer = strstr(*string, old))) {
                memmove(pointer + replacelength, pointer + oldlength, strlen(pointer + oldlength) + 1);
                memcpy(pointer, replace, replacelength);
            }
        }
        if (ssl < stringlength) if (!(*string = (char *) pvrealloc(*string, ssl + 1))) return false;
    } else return false;
    return true;
}
Cstrings strings;

int Callocate::vstrlen(const char *format, va_list arguments) {
    bool found = false;
    int backupint = 0, rest = 0, length = strlen(format);
    char *pointer = NULL, *message = NULL, *backupstring = NULL;
    if ((message = (char *) pvmalloc(strlen(format) + 1))) {
        strcpy(message, format);
        pointer = message;
        while ((pointer = strchr(pointer, '%'))) {
            pointer++;
            if (*pointer == 's') {
                if ((backupstring = va_arg(arguments, char*))) {
                    length += strlen(backupstring) - 2;
                    found = true;
                } else return -1;
            } else if (*pointer == 'd') {
                backupint = va_arg(arguments, int);
                if (backupint < 0)
                    length++;
                rest = backupint % 10;
                backupint -= rest;
                length += (backupint / 10) - 1;
                found = true;
            } else if (*pointer == 'c') {
                length--;
                found = true;
            }
        }
        if (message) {
            pvfree(message);
            message = NULL;
        }
    }
    return length + 1;
}

int Callocate::vtelnet(char **string, const char *format, ...) {
    va_list args;
    int length = 0;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((*string = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(*string, length, format, args);
            va_end(args);
            try {
                if (strstr(*string, "[reset]")) {
                    strings.vreplace(string, "[reset]", "\xff\xfc\x1\x1B[0m");
                }
                if (strstr(*string, "[bold]")) {
                    strings.vreplace(string, "[bold]", "\x1B[1m");
                }
                if (strstr(*string, "[under]")) {
                    strings.vreplace(string, "[under]", "\x1B[4m");
                }
                if (strstr(*string, "[cls]")) {
                    strings.vreplace(string, "[cls]", "\x1B[2J");
                }
                if (strstr(*string, "[n]")) {
                    strings.vreplace(string, "[n]", "\n\r\x1B[0m");
                }
                if (strstr(*string, "[hide]")) {
                    strings.vreplace(string, "[hide]", "\xff\xfb\x1");
                }
                if (strstr(*string, "[red]")) {
                    strings.vreplace(string, "[red]", "\x1B[31m");
                }
                if (strstr(*string, "[green]")) {
                    strings.vreplace(string, "[green]", "\x1B[32m");
                }
                if (strstr(*string, "[yellow]")) {
                    strings.vreplace(string, "[yellow]", "\x1B[33m");
                }
                if (strstr(*string, "[blue]")) {
                    strings.vreplace(string, "[blue]", "\x1B[34m");
                }
            } catch (...) {
                return 1;
            }
        } else return 1;
    }
    return 0;
}

int Callocate::vclient(char **string, const char *format, ...) {
    va_list args;
    int length = 0;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((*string = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(*string, length, format, args);
            va_end(args);
            try {
                if (strstr(*string, "[reset]")) {
                    strings.vreplace(string, "[reset]", "%x");
                }
                if (strstr(*string, "[bold]")) {
                    strings.vreplace(string, "[bold]", "%w");
                }
                if (strstr(*string, "[under]")) {
                    strings.vreplace(string, "[under]", "%z");
                }
                if (strstr(*string, "[n]")) {
                    strings.vreplace(string, "[n]", "\n%x");
                }
                if (strstr(*string, "[cls]")) {
                    strings.vreplace(string, "[cls]", "%c");
                }
                if (strstr(*string, "[hide]")) {
                    strings.vreplace(string, "[hide]", "%h");
                }
                if (strstr(*string, "[red]")) {
                    strings.vreplace(string, "[red]", "%r");
                }
                if (strstr(*string, "[green]")) {
                    strings.vreplace(string, "[green]", "%g");
                }
                if (strstr(*string, "[yellow]")) {
                    strings.vreplace(string, "[yellow]", "%y");
                }
                if (strstr(*string, "[blue]")) {
                    strings.vreplace(string, "[blue]", "%b");
                }
            } catch (...) {
                return 1;
            }
        } else return 1;
    }
    return 0;
}

int Callocate::vadd(char **string, const char *format, ...) {
    va_list args;
    va_list copy;
    int length = 0, basiclength = 0;
    char *buffer = NULL, *pointer = NULL;
    va_start(args, format);
    basiclength = allocate.vstrlen(format, args);
    va_end(args);
    if (basiclength > 0) {
        if ((buffer = (char *) pvmalloc(basiclength + 1))) {
            va_start(args, format);
            vsnprintf(buffer, basiclength, format, args);
            va_end(args);
            if (*string) {
                length = strlen(*string);
                if ((*string = (char *) pvrealloc(*string, (length + strlen(buffer)) + 1))) {
                    pointer = (*string + length);
                    memcpy(pointer, buffer, strlen(buffer) + 1);
                    if (buffer) {
                        pvfree(buffer);
                        buffer = NULL;
                    }
                } else {
                    if (buffer) {
                        pvfree(buffer);
                        buffer = NULL;
                    }
                    return 1;
                }
            } else *string = buffer;
        } else 
            return 1;
    } else 
        return 1;
    return 0;
}

char *Callocate::vsalloc(const char *format, ...) {
    va_list args;
    int length = 0;
    char *message = NULL;
    va_start(args, format);
    length = allocate.vstrlen(format, args);
    va_end(args);
    if (length > 0) {
        if ((message = (char *) pvmalloc(length + 1))) {
            va_start(args, format);
            vsnprintf(message, length, format, args);
            va_end(args);
        } else return NULL;
    }
    return message;
}

char *Callocate::vstring(unsigned int value) {
    char *string;
    unsigned int counter = 1, backupvalue = value;
    while (backupvalue > 0) {
        backupvalue -= (backupvalue % 10);
        backupvalue = (backupvalue / 10);
        counter++;
    }
    if (counter > 0) {
        if ((string = (char *) pvmalloc(counter + 1))) {
            snprintf(string, (counter + 1), "%d", value);
            string[counter + 1] = '\0';
        } else return NULL;
    } else return NULL;
    return string;
}
Callocate allocate;

int Ccompare::vcmpcase(const char *string, const char *compare, int length) {
    if ((!string) && (!compare)) return 0;
    else if ((!string) || (!compare)) return 1;
    return strncasecmp(string, compare, length);
}

int Ccompare::vcmpcase(const char *string, const char *compare) {
    if ((!string) && (!compare)) return 0;
    else if ((!string) || (!compare)) return 1;
    return strcasecmp(string, compare);
}

int Ccompare::vcompare(const char *string, const char *compare) {
    bool done = false;
    int index = 0, value = 0;
    while (!done) {
        if ((index < (int) strlen(string)) && (index < (int) strlen(compare))) {
            done = true;
            if ((int) string[index] > (int) compare[index]) value = 1;
            else if ((int) string[index] < (int) compare[index]) value = -1;
            else done = false;
            index++;
        } else {
            if (strlen(string) > strlen(compare)) value = 1;
            else value = -1;
            done = true;
        }
    }
    return value;
}

int Ccompare::vstep(const char *string, const char *compare) {
    char *pointer;
    if ((pointer = (char *) strstr(string, compare))) {
        if ((pointer != string) && (*(pointer - 1) != ' ')) return 1;
    } else return 1;
    return 0;
}
Ccompare compare;

int Cfunny::vrandom(char **string, unsigned int level) {
    int randomize = 0;
    char *pointer = *string;
    while ((pointer) && (*pointer != '\0')) {
        randomize = (rand() % (122 - 65)) + (level * 10) + 65;
        if ((randomize > 65) && (randomize < 122))
            *pointer = (char) randomize;
        pointer++;
    }
    return 0;
}

char *Cfunny::vdigit(unsigned int value) {
    char backupstring[_DEFAULT_MIND_LEN] = {'\0'}, *charvalue = NULL;
    if (value == 0) {
        strcat(backupstring, "nessuno/a");
    } else if (value == 1) {
        strcat(backupstring, "uno/a");
    } else {
        while (value > 0) {
            if (value < 20) {
                switch (value) {
                    case 1: strcat(backupstring, "uno");
                        value -= 1;
                        break;
                    case 2: strcat(backupstring, "due");
                        value -= 2;
                        break;
                    case 3: strcat(backupstring, "tre");
                        value -= 3;
                        break;
                    case 4: strcat(backupstring, "quattro");
                        value -= 4;
                        break;
                    case 5: strcat(backupstring, "cinque");
                        value -= 5;
                        break;
                    case 6: strcat(backupstring, "sei");
                        value -= 6;
                        break;
                    case 7: strcat(backupstring, "sette");
                        value -= 7;
                        break;
                    case 8: strcat(backupstring, "otto");
                        value -= 8;
                        break;
                    case 9: strcat(backupstring, "nove");
                        value -= 9;
                        break;
                    case 10: strcat(backupstring, "dieci");
                        value -= 10;
                        break;
                    case 11: strcat(backupstring, "undici");
                        value -= 11;
                        break;
                    case 12: strcat(backupstring, "dodici");
                        value -= 12;
                        break;
                    case 13: strcat(backupstring, "tredici");
                        value -= 13;
                        break;
                    case 14: strcat(backupstring, "quattordici");
                        value -= 14;
                        break;
                    case 15: strcat(backupstring, "quindici");
                        value -= 15;
                        break;
                    case 16: strcat(backupstring, "sedici");
                        value -= 16;
                        break;
                    case 17: strcat(backupstring, "diciasette");
                        value -= 17;
                        break;
                    case 18: strcat(backupstring, "diciotto");
                        value -= 18;
                        break;
                    case 19: strcat(backupstring, "diciannove");
                        value -= 19;
                        break;
                }
            } else if (value < 30) {
                value -= 20;
                strcat(backupstring, "vent");
                if ((value != 1) && (value != 8)) strcat(backupstring, "i");
            } else if (value < 40) {
                value -= 30;
                strcat(backupstring, "trent");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else if (value < 50) {
                value -= 40;
                strcat(backupstring, "quarant");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else if (value < 60) {
                value -= 50;
                strcat(backupstring, "cinquant");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else if (value < 70) {
                value -= 60;
                strcat(backupstring, "sessant");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else if (value < 80) {
                value -= 70;
                strcat(backupstring, "settant");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else if (value < 90) {
                value -= 80;
                strcat(backupstring, "ottant");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else if (value < 100) {
                value -= 90;
                strcat(backupstring, "novant");
                if ((value != 1) && (value != 8)) strcat(backupstring, "a");
            } else {
                snprintf(backupstring, _DEFAULT_MAXD_LEN, "%d", value);
                value = 0;
            }
        }
    }
    if (strlen(backupstring) > 0) {
        if ((charvalue = (char *) pvmalloc(strlen(backupstring) + 1))) strcpy(charvalue, backupstring);
        else return NULL;
        charvalue[strlen(backupstring)] = '\0';
    } else return NULL;
    return charvalue;
}

char *Cfunny::vbar(unsigned int value, unsigned int maximum) {
    char *buffer = NULL;
    int final = 0, index = 0;
    final = (value * 10);
    final -= (final % maximum);
    final /= maximum;
    if ((buffer = (char *) pvmalloc(sizeof ("| [red]===[yellow]====[green]====[reset] |") + 1))) {
        strcpy(buffer, "| [red]===[yellow]====[green]====[reset] |");
        buffer[sizeof ("| [red]===[yellow]====[green]====[reset] |")] = '\0';
        while ((final >= 0) && (buffer[index] != '\0')) {
            if (buffer[index++] == '=') final--;
        }
        if (final < 0) buffer[index - 1] = '#';
    } else return NULL;
    return buffer;
}
Cfunny funny;

int Cnumbers::vdiv(int divider, int dividend) {
    int rest = 0;
    if (dividend < 1) dividend = 1;
    else if (divider < dividend) return 0;
    rest = divider % dividend;
    divider -= rest;
    return (int) (divider / dividend);
}

int Cnumbers::vmax(int valuea, int valueb, int valuec) {
    if (valuea > valueb) {
        if (valuea > valuec) return valuea;
        else return valuec;
    } else {
        if (valueb > valuec) return valueb;
        else return valuec;
    }
    return 0;
}

int Cnumbers::vmin(int valuea, int valueb, int valuec) {
    if (valuea < valueb) {
        if (valuea < valuec) return valuea;
        else return valuec;
    } else {
        if (valueb < valuec) return valueb;
        else return valuec;
    }
    return 0;
}
Cnumbers numbers;
