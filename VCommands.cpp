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
#include "VCommands.h"

Ccommands::Ccommands(void) {
    elements = 0;
    commandsroot = NULL;
}

int Ccommands::addcommand(const char *command, const char *syntax, commandfunction *function, int length) {
    commandslist *list = commandsroot, *backup = NULL;
    if (!getcommand(command)) {
        if (commandsroot) {
            while ((list->next) && (compare.vcmpcase(command, list->command) > 0)) list = list->next;
            if (compare.vcmpcase(command, list->command) > 0) {
                backup = list->next;
                if ((list->next = (commandslist *) pvmalloc(sizeof (commandslist)))) {
                    list->next->function = function;
                    list->next->length = length;
                    if (!(list->next->command = (char *) pvmalloc(strlen(command) + 1))) return 1;
                    if (!(list->next->syntax = (char *) pvmalloc(strlen(syntax) + 1))) return 1;
                    strcpy(list->next->command, command);
                    strcpy(list->next->syntax, syntax);
                    list->next->command[strlen(command)] = '\0';
                    list->next->syntax[strlen(syntax)] = '\0';
                    list->next->next = backup;
                    list->next->back = list;
                    if (backup) backup->back = list->next;
                    elements++;
                } else return 1;
            } else if (list->back) {
                backup = list->back;
                if ((backup->next = (commandslist *) pvmalloc(sizeof (commandslist)))) {
                    backup->next->function = function;
                    backup->next->length = length;
                    if (!(backup->next->command = (char *) pvmalloc(strlen(command) + 1))) return 1;
                    if (!(backup->next->syntax = (char *) pvmalloc(strlen(syntax) + 1))) return 1;
                    strcpy(backup->next->command, command);
                    strcpy(backup->next->syntax, syntax);
                    backup->next->command[strlen(command)] = '\0';
                    backup->next->syntax[strlen(syntax)] = '\0';
                    backup->next->next = list;
                    backup->next->back = backup;
                    list->back = backup->next;
                    elements++;
                } else return 1;
            } else if ((commandsroot = (commandslist *) pvmalloc(sizeof (commandslist)))) {
                commandsroot->function = function;
                commandsroot->length = length;
                if (!(commandsroot->command = (char *) pvmalloc(strlen(command) + 1))) return 1;
                if (!(commandsroot->syntax = (char *) pvmalloc(strlen(syntax) + 1))) return 1;
                strcpy(commandsroot->command, command);
                strcpy(commandsroot->syntax, syntax);
                commandsroot->command[strlen(command)] = '\0';
                commandsroot->syntax[strlen(syntax)] = '\0';
                commandsroot->next = list;
                commandsroot->back = NULL;
                list->back = commandsroot;
                elements++;
            } else return 1;
        } else {
            if ((commandsroot = (commandslist *) pvmalloc(sizeof (commandslist)))) {
                commandsroot->function = function;
                commandsroot->length = length;
                if (!(commandsroot->command = (char *) pvmalloc(strlen(command) + 1))) return 1;
                if (!(commandsroot->syntax = (char *) pvmalloc(strlen(syntax) + 1))) return 1;
                strcpy(commandsroot->command, command);
                strcpy(commandsroot->syntax, syntax);
                commandsroot->command[strlen(command)] = '\0';
                commandsroot->syntax[strlen(syntax)] = '\0';
                commandsroot->next = NULL;
                commandsroot->back = NULL;
                elements++;
            } else return 1;
        }
    } else return 1;
    return 0;
}

int Ccommands::delcommand(const char *command) {
    commandslist *list = commandsroot;
    if (commandsroot) {
        while ((list->next) && (compare.vcmpcase(command, list->command) > 0)) list = list->next;
        if (compare.vcmpcase(command, list->command) == 0) {
            if (list->next) list->next->back = list->back;
            if (list->back) list->back->next = list->next;
            else commandsroot = list->next;
            if (list->command) {
                pvfree(list->command);
                list->command = NULL;
            }
            if (list->syntax) {
                pvfree(list->syntax);
                list->syntax = NULL;
            }
            pvfree(list);
            elements--;
        } else return 1;
    } else return 1;
    return 0;
}

commandfunction *Ccommands::getcommand(const char *command) {
    commandslist *list = commandsroot;
    if (commandsroot) {
        while ((list->next) && (compare.vcmpcase(command, list->command) > 0)) list = list->next;
        if (compare.vcmpcase(command, list->command) != 0) return NULL;
    } else return NULL;
    return list->function;
}

int Ccommands::runcommand(const char *command) {
    commandslist *list = commandsroot;
    if (commandsroot) {
        while ((list->next) && (compare.vcmpcase(command, list->command, list->length) > 0)) list = list->next;
        if (compare.vcmpcase(command, list->command, list->length) == 0) {
            if (list->function() > 0) LOG_ERROR("unable to run %s Command", command);
        } else return 1;
    } else return 1;
    return 0;
}

void Ccommands::unload(void) {
    while (commandsroot) {
        if (delcommand(commandsroot->command) > 0) {
            LOG_ERROR("unable to unload %s Command", commandsroot->command);
            commandsroot = commandsroot->next;
        }
    }
}
