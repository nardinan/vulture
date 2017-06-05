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
#include "VServer.h"

Cserver::Cserver(void) {
    socketID = 0;
    information.completename = NULL;
    information.largedescription = NULL;
    information.serverURL = NULL;
    information.adminname = NULL;
    information.adminmail = NULL;
    socketsroot = NULL;
}

Cserver::Cserver(FILE *configurationfile) {
    socketID = 0;
    information.completename = NULL;
    information.largedescription = NULL;
    information.serverURL = NULL;
    information.adminname = NULL;
    information.adminmail = NULL;
    socketsroot = NULL;
    if (configurationfile)
        if (load(configurationfile) > 0) LOG_ERROR("unable to load CONFIGURATION");
}

int Cserver::load(FILE *configurationfile) {
    char backupstring[_DEFAULT_MAXD_LEN], *name = NULL, *text = NULL;
    if (configurationfile) {
        information.port = Vport;
        if (fgets(backupstring, _DEFAULT_MAXD_LEN, configurationfile)) {
            while ((compare.vcmpcase(backupstring, CSTRSIZE("[/SERVER]")) != 0) && (!feof(configurationfile))) {
                if (compare.vcmpcase(backupstring, CSTRSIZE("[SERVER]")) != 0) {
                    if (compare.vcmpcase(backupstring, CSTRSIZE("NAME")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) == 0)
                            if ((information.completename = (char *) pvmalloc(strlen(text) + 1))) {
                                strcpy(information.completename, text);
                                information.completename[strlen(text) - 1] = '\0';
                            }
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("LARGE")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) == 0)
                            if ((information.largedescription = (char *) pvmalloc(strlen(text) + 1))) {
                                strcpy(information.largedescription, text);
                                information.largedescription[strlen(text) - 1] = '\0';
                            }
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("SURL")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) == 0)
                            if ((information.serverURL = (char *) pvmalloc(strlen(text) + 1))) {
                                strcpy(information.serverURL, text);
                                information.serverURL[strlen(text) - 1] = '\0';
                            }
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("ANAME")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) == 0)
                            if ((information.adminname = (char *) pvmalloc(strlen(text) + 1))) {
                                strcpy(information.adminname, text);
                                information.adminname[strlen(text) - 1] = '\0';
                            }
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("AMAIL")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "ss", &name, &text) == 0)
                            if ((information.adminmail = (char *) pvmalloc(strlen(text) + 1))) {
                                strcpy(information.adminmail, text);
                                information.adminmail[strlen(text) - 1] = '\0';
                            }
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("PORT")) == 0) {
                        if (strings.vsscanf(backupstring, '=', "sd", &name, &information.port) > 0) LOG_ERROR("unable to load PORT");
                    } else if (compare.vcmpcase(backupstring, CSTRSIZE("#")) != 0) LOG_ERROR("[Cserver] %s (unknown command)", backupstring);
                }
                if (name) {
                    pvfree(name);
                    name = NULL;
                }
                if (text) {
                    pvfree(text);
                    text = NULL;
                }
                fgets(backupstring, _DEFAULT_MAXD_LEN, configurationfile);
            }
        } else return 1;
    } else return 1;
    return 0;
}

int Cserver::save(FILE *configurationfile) {
    if (configurationfile) {
        fprintf(configurationfile, "[SERVER]\n");
        if (information.completename) fprintf(configurationfile, "NAME=%s\n", information.completename);
        if (information.largedescription) fprintf(configurationfile, "LARGE=%s\n", information.largedescription);
        if (information.serverURL) fprintf(configurationfile, "SURL=%s\n", information.serverURL);
        if (information.adminname) fprintf(configurationfile, "ANAME=%s\n", information.adminname);
        if (information.adminmail) fprintf(configurationfile, "AMAIL=%s\n", information.adminmail);
        fprintf(configurationfile, "PORT=%d\n", information.port);
        fprintf(configurationfile, "[/SERVER]");
    } else return 1;
    return 0;
}

int Cserver::initializing(serverinfos information) {
    if (this->information.completename) {
        pvfree(this->information.completename);
        this->information.completename = NULL;
    }
    if ((this->information.completename = (char *) pvmalloc(strlen(information.completename) + 1)))
        strcpy(this->information.completename, information.completename);
    if (this->information.largedescription) {
        pvfree(this->information.largedescription);
        this->information.largedescription = NULL;
    }
    if ((this->information.largedescription = (char *) pvmalloc(strlen(information.largedescription) + 1)))
        strcpy(this->information.largedescription, information.largedescription);
    if (this->information.serverURL) {
        pvfree(this->information.serverURL);
        this->information.serverURL = NULL;
    }
    if ((this->information.serverURL = (char *) pvmalloc(strlen(information.serverURL) + 1)))
        strcpy(this->information.serverURL, information.serverURL);
    if (this->information.adminname) {
        pvfree(this->information.adminname);
        this->information.adminname = NULL;
    }
    if ((this->information.adminname = (char *) pvmalloc(strlen(information.adminname) + 1)))
        strcpy(this->information.adminname, information.adminname);
    if (this->information.adminmail) {
        pvfree(this->information.adminmail);
        this->information.adminmail = NULL;
    }
    if ((this->information.adminmail = (char *) pvmalloc(strlen(information.adminmail) + 1)))
        strcpy(this->information.adminmail, information.adminmail);
    this->information.port = information.port;
    return 0;
}

int Cserver::initializing(void) {
    int enable = 1;
    memset(&socketaddress, 0, sizeof (socketaddress));
#ifdef _WIN32
    WSAStartup(MAKEWORD(1, 1), &datasocket);
#endif
    if ((serversocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1) {
        setsockopt(serversocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        socketaddress.sin_family = AF_INET;
        socketaddress.sin_port = htons(information.port);
        socketaddress.sin_addr.s_addr = htonl(INADDR_ANY);
        memset(&(socketaddress.sin_zero), 0, 8);
        if (bind(serversocket, (struct sockaddr *) &socketaddress, sizeof (socketaddress)) == 0) {
            if (listen(serversocket, 5) != 0) 
                return 1;
        } else 
            return 1;
    } else 
        return 1;
    return 0;
}

int Cserver::search(unsigned int sec, unsigned int usec) {
    fd_set rdset;
    int socket;
    socketslist *list = socketsroot;
    timeval timeout = {(int) sec, (int) usec};
    int backupsocketID = -1, size;
    FD_ZERO(&rdset);
    FD_SET(serversocket, &rdset);
    if (select(serversocket + 1, &rdset, NULL, NULL, &timeout) > 0) {
        if ((socket = accept(serversocket, NULL, NULL)) != -1) {
            if (!list) {
                if ((socketsroot = (socketslist *) pvmalloc(sizeof (socketslist) + 1))) {
                    socketsroot->back = NULL;
                    socketsroot->next = NULL;
                    socketsroot->socket = socket;
                    for (size = 0; size < (_DEFAULT_MAXD_LEN); size++) socketsroot->torcv[size] = '\0';
                    socketsroot->socketID = socketID++;
                    backupsocketID = socketsroot->socketID;
                }
            } else {
                while (list->next != NULL) list = list->next;
                if ((list->next = (socketslist *) pvmalloc(sizeof (socketslist) + 1))) {
                    list->next->back = list;
                    list->next->next = NULL;
                    list->next->socket = socket;
                    list = list->next;
                    for (size = 0; size < (_DEFAULT_MAXD_LEN); size++) list->torcv[size] = '\0';
                    list->socketID = socketID++;
                    backupsocketID = list->socketID;
                }
            }
        } else return -1;
    } else return -1;
    return backupsocketID;
}

socketslist *Cserver::getsocket(unsigned int socketID) {
    socketslist *list = socketsroot;
    if (socketsroot) {
        while ((list->socketID != socketID) && (list->next)) list = list->next;
        if (list->socketID != socketID) return NULL;
    } else return NULL;
    return list;
}

int Cserver::pvsend(unsigned int socketID, const char *format, ...) {
    va_list args;
    int length = 0;
    char *message = NULL, *telnet = NULL;
    socketslist *list = NULL;
    if ((list = (getsocket(socketID)))) {
        va_start(args, format);
        length = allocate.vstrlen(format, args);
        va_end(args);
        if (length > 0) {
            if ((message = (char *) pvmalloc(length + 1))) {
                va_start(args, format);
                vsnprintf(message, length, format, args);
                va_end(args);
                if (allocate.vtelnet(&telnet, message) > 0) return 1;
                //if (send(list->socket, telnet, strlen(telnet) + 1, 0) < 0) return -1; //WINDOWS
                if (write(list->socket, telnet, strlen(telnet) + 1) < 0) return -1; //UNIX
                if (message) {
                    pvfree(message);
                    message = NULL;
                }
                if (telnet) {
                    pvfree(telnet);
                    telnet = NULL;
                }
            } else return 1;
        }
    } else return 1;
    return 0;
}

int Cserver::pvrecv(unsigned int socketID, char **message, unsigned int sec, unsigned int usec) {
    fd_set rdset;
    ssize_t size;
    socketslist *list = socketsroot;
    timeval timeout = {(int) sec, (int) usec};
    int index;
    char string[_DEFAULT_MAXD_LEN], character;
    if ((list = (getsocket(socketID)))) {
        FD_ZERO(&rdset);
        FD_SET(list->socket, &rdset);
        if (select(list->socket + 1, &rdset, NULL, NULL, &timeout) > 0) {
            for (index = 0; index < _DEFAULT_MAXD_LEN; index++) string[index] = '\0';
            //if ((size = recv(list->socket, string, _DEFAULT_MAXD_LEN - 1, 0)) > 0) { //WINDOWS
            if ((size = read(list->socket, &string, _DEFAULT_MAXD_LEN - 1)) > 0) { //UNIX
                for (index = 0; (index < size) && (!*message); index++) {
                    character = string[index];
                    if ((character == '\n') || (character == '\r')) {
                        if (character == '\n') {
                            if ((*message = (char *) pvmalloc(strlen(list->torcv) + 1))) {
                                strcpy(*message, list->torcv);
                                (*message)[strlen(list->torcv)] = '\0';
                            } else return 1;
                            for (index = 0; index < (_DEFAULT_MAXD_LEN); index++) list->torcv[index] = '\0';
                        }
                    } else if (character == '\b') {
                        if (strlen(list->torcv) > 0) list->torcv[strlen(list->torcv) - 1] = '\0';
                    } else {
                        if (strlen(list->torcv) < _DEFAULT_MAXD_LEN) list->torcv[strlen(list->torcv)] = character;
                    }
                }
            } else return 1;
        }
    } else return 1;
    return 0;
}

int Cserver::unload(unsigned int socketID) {
    socketslist *list = socketsroot;
    if ((list = (getsocket(socketID)))) {
        if (list->next) list->next->back = list->back;
        if (list->back) list->back->next = list->next;
        else socketsroot = list->next;
        shutdown(list->socket, 2);
        closesocket(list->socket);
        pvfree(list);
    } else return 1;
    return 0;
}

void Cserver::unload(void) {
    socketslist *list = socketsroot;
    unsigned int socketID;
    while (list) {
        socketID = list->socketID;
        list = list->next;
        if (unload(socketID) > 0) 
            LOG_ERROR("unable to unload Key %d", socketID);
    }
    if (information.completename) {
        pvfree(information.completename);
        information.completename = NULL;
    }
    if (information.largedescription) {
        pvfree(information.largedescription);
        information.largedescription = NULL;
    }
    if (information.serverURL) {
        pvfree(information.serverURL);
        information.serverURL = NULL;
    }
    if (information.adminname) {
        pvfree(information.adminname);
        information.adminname = NULL;
    }
    if (information.adminmail) {
        pvfree(information.adminmail);
        information.adminmail = NULL;
    }
    shutdown(serversocket, 2);
    closesocket(serversocket);
#ifdef _WIN32
    WSACleanup();
#endif
}
