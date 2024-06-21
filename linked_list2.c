#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <limits.h> 

struct slist {
    char data;
    struct slist *next;
};

struct slist *head = NULL;
typedef struct slist node;

struct dir_node {
    char dir[PATH_MAX];
    struct dir_node *next;
};

struct dir_node *top = NULL;

void pushd(const char *dir) {
    struct dir_node *new_node = (struct dir_node *)malloc(sizeof(struct dir_node));
    if (new_node == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    strncpy(new_node->dir, dir, PATH_MAX);
    new_node->next = top;
    top = new_node;
}

char* popd() {
    if (top == NULL) {
        return NULL;
    }
    struct dir_node *tmp = top;
    top = top->next;
    char *dir = strdup(tmp->dir);
    free(tmp);
    return dir;
}

void execute_pushd(const char *dir) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }
    pushd(cwd);
    if (chdir(dir) != 0) {
        perror("chdir() error");
        char *popped_dir = popd();
        free(popped_dir);
    } else {
        printf("Changed directory to %s\n", dir);
    }
}

void execute_popd() {
    char *dir = popd();
    if (dir == NULL) {
        printf("Directory stack is empty\n");
        return;
    }
    if (chdir(dir) != 0) {
        perror("chdir() error");
    } else {
        printf("Changed directory to %s\n", dir);
    }
    free(dir);
}

void reverseDisplay() {
    node *tmp = head;
    if (tmp == NULL) {
        printf("Empty list....\n");
        return;
    }
    char stack[100];
    int top = -1;
    while (tmp != NULL) {
        if (tmp->data == ' ' || tmp->next == NULL) {
            if (tmp->next == NULL && tmp->data != ' ') {
                stack[++top] = tmp->data;
            }
            while (top >= 0) {
                printf("%c", stack[top--]);
            }
            if (tmp->data == ' ') {
                printf(" ");
            }
        } else {
            stack[++top] = tmp->data;
        }
        tmp = tmp->next;
    }
    printf("\n");
}

void freeList() {
    node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void createList(const char *input) {
    node *tmp, *tmp1;
    for (size_t i = 0; i < strlen(input); i++) {
        tmp = (node *)malloc(sizeof(node));
        tmp->data = input[i];
        tmp->next = NULL;
        if (head == NULL) {
            head = tmp;
        } else {
            tmp1 = head;
            while (tmp1->next != NULL) {
                tmp1 = tmp1->next;
            }
            tmp1->next = tmp;
        }
    }
}

void print_Current_directory() {
    char cdir[PATH_MAX];
    if (getcwd(cdir, sizeof(cdir)) != NULL) {
        printf("Current working directory: %s\n", cdir);
    } else {
        perror("getcwd() error");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <pushd dir|popd|string>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "pushd") == 0) {
        if (argc != 3) {
            printf("Usage: %s pushd <directory>\n", argv[0]);
            return 1;
        }
        execute_pushd(argv[2]);
        print_Current_directory();
    } else if (strcmp(argv[1], "popd") == 0) {
        execute_popd();
        print_Current_directory();
    } else {
        createList(argv[1]);
        reverseDisplay();
        freeList();
    }

    return 0;
}
