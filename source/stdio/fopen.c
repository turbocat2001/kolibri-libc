#include <stdio.h>
#include <stdlib.h>

static char *strdup(char *str) {
    char *buf = malloc(strlen(str) + 1);
    buf[strlen(str)] = 0;
    strcpy(buf, str);
    return buf;
}

FILE *fopen(const char *restrict _name, const char *restrict _mode) {
    BDFE_struct info;
    if (_ksys_file_get_info(_name, &info)) {
        return NULL;
    }

    FILE *out = malloc(sizeof(FILE));

    if (!out) {
        return NULL;
    }

    out->name = strdup(_name);
    out->position = 0;
    out->error = 0;
    out->eof = 0;
    out->kind = 0;
    out->orientation = 0;
    out->mode = 0;

    if (strchr(_mode, 'b')) { out->mode |= _STDIO_F_B; }
    if (strchr(_mode, 'x')) { out->mode |= _STDIO_F_X; }
    if (strchr(_mode, 'a')) { out->mode |= _STDIO_F_A; }
    if (strchr(_mode, 'r')) { out->mode |= _STDIO_F_R; }
    if (strchr(_mode, 'w')) { out->mode |= _STDIO_F_W; }
    if (strchr(_mode, '+')) { out->mode |= _STDIO_F_R | _STDIO_F_W; }

    if (out->mode & _STDIO_F_A) {
        out->position = info.size;
        out->append_offset = info.size;
    }

    return out;
}
