#include "linked_list.h"


LinkedList *createLinkedList() {
    LinkedList *pReturn = NULL;

    pReturn = (LinkedList *) malloc(sizeof(LinkedList));

    if (pReturn != NULL) {
        memset(pReturn, 0, sizeof(LinkedList));
    } else {
        printf("ERROR: Memory allocation failed.\n");
        return NULL;
    }

    return pReturn;
}


FDLinkedList *createFDLinkedList() {
    FDLinkedList *pReturn = NULL;

    pReturn = (FDLinkedList *) malloc(sizeof(FDLinkedList));

    if (pReturn != NULL) {
        memset(pReturn, 0, sizeof(FDLinkedList));
    } else {
        printf("ERROR: Memory allocation failed.\n");
        return NULL;
    }

    return pReturn;
}


int addLLElement(LinkedList *pList, int position, ListNode element) {
    int ret = FALSE;
    int i = 0;
    ListNode *pPreNode = NULL;
    ListNode *pNewNode = NULL;

    if (pList != NULL) {
        if (position >= 0 && position <= pList->currentElementCount) {
            pNewNode = (ListNode *) malloc(sizeof(ListNode));
            if (pNewNode != NULL) {
                *pNewNode = element;
                pNewNode->pLink = NULL;

                pPreNode = &(pList->headerNode);
                for (i = 0; i < position; i++) {
                    pPreNode = pPreNode->pLink;
                }

                pNewNode->pLink = pPreNode->pLink;
                pPreNode->pLink = pNewNode;

                pList->currentElementCount++;

                ret = TRUE;
            } else {
                printf("ERROR: Memory allocation failed : addLLElement()\n");
                return ret;
            }
        } else {
            printf("ERROR: [%d] Index out of rage: addLLElement()\n", position);
        }
    }
    return ret;
}


int addLLFDElement(FDLinkedList *pList, int position, FDListNode element) {
    int ret = FALSE;
    int i = 0;
    FDListNode *pPreNode = NULL;
    FDListNode *pNewNode = NULL;

    if (pList != NULL) {
        if (position >= 0 && position <= pList->currentElementCount) {
            pNewNode = (FDListNode *) malloc(sizeof(FDListNode));
            if (pNewNode != NULL) {
                *pNewNode = element;
                pNewNode->pLinkFD = NULL;

                pPreNode = &(pList->headerNode);
                for (i = 0; i < position; i++) {
                    pPreNode = pPreNode->pLinkFD;
                }

                pNewNode->pLinkFD = pPreNode->pLinkFD;
                pPreNode->pLinkFD = pNewNode;

                pList->currentElementCount++;

                ret = TRUE;
            } else {
                printf("ERROR: Memory allocation failed : addLLFDElement()\n");
                return ret;
            }
        } else {
            printf("ERROR: [%d] Index out of rage: addLLFDElement()\n", position);
        }
    }
    return ret;
}


int removeLLElement(LinkedList *pList, int position) {
    int ret = FALSE;
    int arrayCount = 0;
    ListNode *pPreNode = NULL;
    ListNode *pDelNode = NULL;

    if (pList != NULL) {
        arrayCount = getLinkedListLength(pList);
        if (position >= 0 && position < arrayCount) {
            pPreNode = &(pList->headerNode);
            for (int i = 0; i < position; i++) {
                pPreNode = pPreNode->pLink;
            }

            pDelNode = pPreNode->pLink;
            pPreNode->pLink = pDelNode->pLink;
            free(pDelNode);

            pList->currentElementCount--;

            ret = TRUE;
        } else {
            printf("ERROR: [%d] Index out of rage: deleteLLElement()\n", position);
        }
    }

    return ret;
}

ListNode *getLLElement(LinkedList *pList, int position) {
    ListNode *pReturn = NULL;
    ListNode *pNode = NULL;

    if (pList != NULL) {
        if (position >= 0 && position < pList->currentElementCount) {
            pNode = &(pList->headerNode);
            for (int i = 0; i <= position; i++) {
                pNode = pNode->pLink;
            }
            pReturn = pNode;
        }
    }
    return pReturn;
}


FDListNode *getLLFDElement(FDLinkedList *pList, int position) {
    FDListNode *pReturn = NULL;
    FDListNode *pNode = NULL;

    if (pList != NULL) {
        if (position >= 0 && position < pList->currentElementCount) {
            pNode = &(pList->headerNode);
            for (int i = 0; i <= position; i++) {
                pNode = pNode->pLinkFD;
            }
            pReturn = pNode;
        }
    }
    return pReturn;
}


void deleteLinkedList(LinkedList *pList) {

    if (pList != NULL) {
        clearLinkedList(pList);
        free(pList);
    }
}

void clearLinkedList(LinkedList *pList) {
    if (pList != NULL) {
        if (pList->currentElementCount > 0) {
            removeLLElement(pList, 0);
        }
    }
}

int getLinkedListLength(LinkedList *pList) {
    int ret = 0;

    if (pList != NULL) {
        ret = pList->currentElementCount;
    }

    return ret;
}

int isEmpty(LinkedList *pList) {
    int ret = FALSE;

    if (pList != NULL) {
        if (pList->currentElementCount == 0) {
            ret = TRUE;
        }
    }

    return ret;
}

void displayLinkedList(LinkedList *pList) {
    int j = 0;
    if (pList != NULL) {
        printf("Current number of process: %d\n", pList->currentElementCount);
        for (int i = 0; i < pList->currentElementCount; i++) {
            printf("\n===============================================\n");
            printf("Pid : %d\n", getLLElement(pList, i)->pid);
            printf("PPid: %d\n", getLLElement(pList, i)->ppid);
            printf("comm: %s\n", getLLElement(pList, i)->comm);
            printf("cmdline: %s\n", getLLElement(pList, i)->cmdline);
            while(getLLFDElement(getLLElement(pList, i)->fd_list, j)->pLinkFD != NULL) {
                printf("\tfd: %d\t->\t%s\n", getLLFDElement((FDLinkedList *) getLLElement(pList, i)->fd_list, j)->fd,
                       getLLFDElement((FDLinkedList *) getLLElement(pList, i)->fd_list, j)->fd_info);
                j++;
            }
            j = 0;
            printf("===============================================\n");
        }
    } else {
        printf("No element\n");
    }
}


void save_processes(LinkedList* proc_list) {
    FILE *fp = NULL;
    uid_t uid = getuid();
    char command[128] = {0};
    char pid_str[10] = {0};
    int i = 0;
    int pid = 0;


    sprintf(command, "sudo ps aux | grep -v PID | awk '{print $2}'", uid);
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("ERROR: saveUserProcesses() - Cannot execute command\n");
        exit(1);
    }

    while(fgets(pid_str , sizeof(pid_str), fp) != NULL) {
        save_pid_to_linked_list(proc_list, pid_str, i, pid);
        i++;
    }
    pclose(fp);
}


void save_pid_to_linked_list(LinkedList *proc_list, char *pid_str, int i, int pid) {
    ListNode proc = {0};
    pid = atoi(pid_str);
    proc.pid = pid;
    addLLElement(proc_list, i, proc);
    memset(pid_str, 0, 10);
}


void process_directory_processing(LinkedList *proc_list) {
    char path[64] = {0};
    for (int i = 0; i < proc_list->currentElementCount; i++) {
        sprintf(path, "/proc/%d/", getLLElement(proc_list, i)->pid);
        DIR* dir = opendir(path);
        if (dir == NULL) {
            fprintf(stderr, "Failed to open directory %s\n", path);
        }

        save_ppid_to_linked_list(path, proc_list, i);
        save_comm_to_linked_list(path, proc_list, i);
        save_cmdline_to_linked_list(path, proc_list, i);
        strcat(path, "fd/");
        save_fd_to_linked_list(path, proc_list, i);


        closedir(dir);
    }
}


void save_ppid_to_linked_list(char *path, LinkedList *proc_list, int index) {
    int result = 0;
    FILE *fp;
    char line[30];
    char *token;

    result = chdir(path);
    if (result == 0) {
        fp = popen("cat status | grep -E '^(PPid):'", "r");
        if (fp == NULL) {
            puts("save_ppid_to_linked_list() - Cannot execute: \"cat status\"");
            exit(1);
        }
        while (fgets(line, sizeof(line), fp) != NULL) {
            token = strtok(line, "PPid:\t");
            token[strlen(token)] = 0;
            getLLElement(proc_list, index)->ppid = atoi(token);
        }
        pclose(fp);
    }
    else {
        puts("save_ppid_to_linked_list() - Directory doesn't exist");
    }
}


void save_comm_to_linked_list(char *path, LinkedList *proc_list, int index) {
    int result = 0;
    FILE *fp;
    char line[256];

    result = chdir(path);
    if (result == 0) {
        fp = popen("cat comm", "r");
        if (fp == NULL) {
            puts("save_comm_to_linked_list() - Cannot execute: \"cat comm\"");
            exit(1);
        }
        while (fgets(line, sizeof(line), fp) != NULL) {
            line[strlen(line) - 1] = 0;
            strcpy(getLLElement(proc_list, index)->comm, line);
        }
        pclose(fp);
    }
    else {
        puts("show_comm_to_linked_list() - Directory doesn't exist");
    }
}


void save_cmdline_to_linked_list(char *path, LinkedList *proc_list, int index) {
    int result = 0;
    FILE *fp;
    char line[512];

    result = chdir(path);
    if (result == 0) {
        fp = popen("cat cmdline", "r");
        if (fp == NULL) {
            puts("save_cmdline_to_linked_list() - Cannot execute: \"cat cmdline\"");
            exit(1);
        }
        while (fgets(line, sizeof(line), fp) != NULL) {
            strcpy(getLLElement(proc_list, index)->cmdline, line);
        }
        pclose(fp);
    }
    else {
        printf("show_cmdline_to_linked_list() - Directory doesn't exist");
    }
}


void save_fd_to_linked_list(char *path, LinkedList *proc_list, int index) {
    int result = 0;
    FILE *fp;
    char line[256], buffer[256];
    int count = 0;
    int i = 0, j = 0;
    FDLinkedList *fd_list = NULL;

    result = chdir(path);
    if (result == 0) {
        fp = popen("ls -al", "r");
        if (fp == NULL) {
            puts("save_fd_to_linked_list() - Cannot execute: \"ls -al\"");
            exit(1);
        }
        fd_list = createFDLinkedList();
        while (fgets(line, sizeof(line), fp) != NULL) {
            line[strlen(line) - 1] = 0;
            FDListNode fd_node = {0,};
            if (count >= 3) {
//                printf("line = %s\n", line);
                char *token = strtok(line, " ");
                while (i < 10) {
                    token = strtok(NULL, " ");
                    if (i == 7) {
                        fd_node.fd = atoi(token);
//                        printf("    token = %s\n", token);
                    }
                    if (i == 9) {
//                        printf("    token = %s\n", token);
                        strcpy(fd_node.fd_info, token);
                        addLLFDElement(fd_list, j, fd_node);
                    }
                    i++;
                }
                getLLElement(proc_list, j)->fd_list = (struct LinkedList *) fd_list;
                j++;
                i = 0;
            }
            count++;
        }
        pclose(fp);
    }
}