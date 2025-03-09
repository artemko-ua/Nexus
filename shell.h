#ifndef SHELL_H
#define SHELL_H

#include "filesystem.h"

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 16

typedef struct {
    char name[32];
    char description[64];
    int (*function)(int argc, char* argv[]);
} command_t;

void shell_init(filesystem_t* fs);
void shell_run();

// Shell built-in commands
int cmd_help(int argc, char* argv[]);
int cmd_clear(int argc, char* argv[]);
int cmd_echo(int argc, char* argv[]);
int cmd_ls(int argc, char* argv[]);
int cmd_cat(int argc, char* argv[]);
int cmd_touch(int argc, char* argv[]);
int cmd_rm(int argc, char* argv[]);
int cmd_write(int argc, char* argv[]);
int cmd_pwd(int argc, char* argv[]);

#endif // SHELL_H