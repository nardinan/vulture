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
#include "VAlloc.h"
unsigned int memory = 0;
unsigned int pointers = 0;
allocationslist *allocationsroot = NULL;

void *vmalloc(size_t size, const char *file, int line) {
    vcoords *coords = NULL;
    allocationslist *list = allocationsroot;
    int index;
    void *result = NULL;
    unsigned char *check = NULL;
    if ((result = malloc(size + sizeof (vcoords) + 20))) {
        pointers++;
        memory += size + sizeof (vcoords) + 20;
        check = (unsigned char *) ((char *) result + size);
        coords = (vcoords *) (check + 20);
        coords->file = file;
        coords->line = line;
        for (index = 0; index < size; index++) ((char *) result)[index] = '\0';
        *((unsigned int *) check) = 0xdeadbeef;
        if (allocationsroot) {
            while (list->next) list = list->next;
            if ((list->next = (allocationslist *) malloc(sizeof (allocationslist)))) {
                list->next->pointer = result;
                list->next->buffersize = size;
                list->next->next = NULL;
                list->next->back = list;
            } else LOG_ERROR("unable to allocate node of the list");
        } else {
            if ((allocationsroot = (allocationslist *) malloc(sizeof (allocationslist)))) {
                allocationsroot->pointer = result;
                allocationsroot->buffersize = size;
                allocationsroot->next = NULL;
                allocationsroot->back = NULL;
            } else LOG_ERROR("unable to allocate root of the list");
        }
    }
    return result;
}

void *vrealloc(void *pointer, size_t size) {
    allocationslist *list = allocationsroot;
    void *result = NULL;
    while ((list) && (list->pointer != pointer)) list = list->next;
    if (list->pointer == pointer) {
        if ((result = pvmalloc(size))) {
            memcpy(result, pointer, list->buffersize);
            pvfree(pointer);
        }
    }
    return result;
}

void vfree(void *pointer) {
    allocationslist *list = allocationsroot;
    while ((list) && (list->pointer != pointer)) list = list->next;
    if ((list) && (list->pointer == pointer)) {
        pointers--;
        memory -= list->buffersize + sizeof (vcoords) + 20;
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else allocationsroot = list->next;
        free(list);
        free(pointer);
    } else LOG_ERROR("unable to find reference on pointers' list (double free!)");
}

void vdeallocate(void) {
    vshow();
    while (allocationsroot)
        pvfree(allocationsroot->pointer);
}

int vshow(void) {
    vcoords *coords = NULL;
    allocationslist *list = allocationsroot;
    unsigned char *check = NULL;
    unsigned int correct = 0, errors = 0;
    while (list) {
        check = (unsigned char *) (((char *) list->pointer) + (size_t) list->buffersize);
        coords = (vcoords *) (check + 20);
        if (*((unsigned int *) check) == 0xdeadbeef) correct++;
        else errors++;
        printf("\n[POINTER] %p at %s (row %d) 0xDEADBEEF (contain: %s) {  %s  }!",
                list->pointer,
                coords->file,
                coords->line,
                list->pointer,
                (*((unsigned int *) check) == 0xdeadbeef) ? "OK" : "ERROR");
        list = list->next;
    }
    printf("\n[FINISH] I've found: %d correct pointers AND %d damaged pointers (%s)\n",
            correct,
            errors,
            ((errors == 0) ? "that's good for you!" : "uh-oh!"));
    return errors;
}
