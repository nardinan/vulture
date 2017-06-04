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
#include "VLog.h"

void virtuallogger(const char *output, const char *file, const char *function, const char *format, ...) {
    va_list args;
    FILE *logfile = NULL;
    if (Vflog) {
        if ((logfile = fopen(output, "a")) || (logfile = fopen(output, "w"))) {
            fprintf(logfile, "[%s::%s] - ", file, function);
            va_start(args, format);
            vfprintf(logfile, format, args);
            va_end(args);
            fputc('\n', logfile);
            fclose(logfile);
        } else printf("Unable to build/open %s logfile\n", output);
    }
    if (Vplog) {
        printf("[%s::%s] - ", file, function);
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        fputc('\n', stdout);
    }
}

int Clogger::load(void) {
    time_t current;
    struct tm *local;
    char configurationfile[_DEFAULT_MIND_LEN];
    current = time(NULL);
    local = localtime(&current);
    strftime(configurationfile, _DEFAULT_MIND_LEN, "./logs/%d-%a-%B@%H.%M.%S.html", local);
    return load(configurationfile);
}

int Clogger::load(const char *configurationfile) {
    time_t current;
    struct tm *local;
    char timebuffer[_DEFAULT_MIND_LEN];
    if (logger) {
        unload();
    }
    if ((logger = fopen(configurationfile, "a"))) {
        current = time(NULL);
        local = localtime(&current);
        strftime(timebuffer, _DEFAULT_MIND_LEN, "%d %a %B", local);
        fprintf(logger, "<html>\n<head><title>LOG %s</title></head>\n<body>\n<font size=\"2px\">\n", timebuffer);
        fprintf(logger, "<center><b>VULTURE&copy; LOG SYSTEM in %s</b></center><br><br>\n", timebuffer);
    } else return 1;
    return 0;
}

int Clogger::log(const char *subject, const char *format, ...) {
    va_list args;
    time_t current;
    struct tm *local;
    int length = 0;
    char *message = NULL, *word = NULL, timebuffer[_DEFAULT_MIND_LEN], backup[_DEFAULT_MIND_LEN];
    if (logger) {
        va_start(args, format);
        length = allocate.vstrlen(format, args);
        va_end(args);
        if (length > 0) {
            if ((message = (char *) pvmalloc(length + 1))) {
                va_start(args, format);
                vsnprintf(message, length, format, args);
                va_end(args);
                if (vdecode) {
                    strings.vreplace(&message, "<", "&lt;");
                    strings.vreplace(&message, ">", "&gt;");
                    while ((word = strings.vget(message, '[', ']'))) {
                        if (compare.vcmpcase(word, "reset") == 0) {
                            snprintf(backup, _DEFAULT_MIND_LEN, "%s%s</font><font face=\"courier\" size=\"2px\" color=\"black\">", (isB) ? "</b>" : "", (isU) ? "</u>" : "");
                            strings.vsreplace(&message, "[reset]", backup);
                            isB = false;
                            isU = false;
                        } else if (compare.vcmpcase(word, "n") == 0) {
                            snprintf(backup, _DEFAULT_MIND_LEN, "%s%s<br>\n</font><font face=\"courier\" size=\"2px\" color=\"black\">", (isB) ? "</b>" : "", (isU) ? "</u>" : "");
                            strings.vsreplace(&message, "[n]", backup);
                            isB = false;
                            isU = false;
                        } else if (compare.vcmpcase(word, "bold") == 0) {
                            strings.vsreplace(&message, "[bold]", "<b>");
                            isB = true;
                        } else if (compare.vcmpcase(word, "under") == 0) {
                            strings.vsreplace(&message, "[under]", "<u>");
                            isU = true;
                        } else if (compare.vcmpcase(word, "red") == 0) strings.vsreplace(&message, "[red]", "</font><font face=\"courier\" size=\"2px\" color=\"red\">");
                        else if (compare.vcmpcase(word, "green") == 0) strings.vsreplace(&message, "[green]", "</font><font face=\"courier\" size=\"2px\" color=\"green\">");
                        else if (compare.vcmpcase(word, "yellow") == 0) strings.vsreplace(&message, "[yellow]", "</font><font face=\"courier\" size=\"2px\" color=\"yellow\">");
                        else if (compare.vcmpcase(word, "blue") == 0) strings.vsreplace(&message, "[blue]", "</font><font face=\"courier\" size=\"2px\" color=\"blue\">");
                        else {
                            snprintf(backup, _DEFAULT_MIND_LEN, "[%s]", word);
                            strings.vsreplace(&message, backup, "|?|");
                        }
                        if (word) {
                            pvfree(word);
                            word = NULL;
                        }
                    }
                    fprintf(logger, (const char *) message);
                } else {
                    current = time(NULL);
                    local = localtime(&current);
                    strftime(timebuffer, _DEFAULT_MIND_LEN, "%b %a %d at %H:%M", local);
                    fprintf(logger, "<font face=\"courier\" size=\"2px\" color=\"black\"><i>(%s)</i> [<b><u>%s</u></b>]</font> %s<br>\n", timebuffer, subject, message);
                }
                if (message) {
                    pvfree(message);
                    message = NULL;
                }
            } else return 1;
        } else return 1;
    } else return 1;
    return 0;
}
