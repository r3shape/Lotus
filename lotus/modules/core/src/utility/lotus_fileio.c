#include "../include/utility/lotus_fileio.h"
#include "../include/memory/lotus_memory.h"

#include <stdio.h>

ubyte lotus_is_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return LOTUS_FALSE;
    fclose(file);
    return LOTUS_TRUE;
}

char* lotus_read_file(const char* path) {
    FILE* file = fopen(path, "rb");

    if (!file) {
        // error: failed to open file
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if (length < 0) {
        // error: failed to determine file size
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    char* content = (char*)lotus_memory_api->alloc(length + 1, 16);
    if (!content) {
        // error: failed to allocate memory for file content\n
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(content, sizeof(char), length, file);
    if (bytesRead != (size_t)length) {
        // error: failed to read the entire file\n
        lotus_memory_api->free(content);
        fclose(file);
        return NULL;
    }

    content[length] = '\0';
    fclose(file);
    return content;
}

ubyte lotus_delete_file(const char* path) {
    if (remove(path) != LOTUS_FALSE) {
        // error: failed to delotus_e file at path: %s\n", pat
        return LOTUS_FALSE;
    }
    return LOTUS_TRUE;
}

long lotus_get_file_size(const char* path) {
    FILE* file = fopen(path, "rb");

    if (!file) {
        // error: failed to open file at path: %s\n", pat
        return LOTUS_FALSE;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

ubyte lotus_copy_file(const char* dest, const char* src) {
    FILE* srcFile = fopen(src, "rb");
    if (!src) {
        // error: failed to open source file: %s\n", sr
        return LOTUS_FALSE;
    }

    FILE* destFile = fopen(dest, "wb");
    if (!dest) {
        // error: failed to open destination file: %s\n", des
        fclose(srcFile);
        return LOTUS_FALSE;
    }

    size_t bytesRead;
    char buffer[4096];
    do {
        fwrite(buffer, sizeof(char), bytesRead, destFile);
    } while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), srcFile)) > 0);

    fclose(srcFile);
    fclose(destFile);
    return LOTUS_TRUE;
}

ubyte lotus_append_file(const char* data, const char* path, ubyte newline) {
    if (newline) lotus_write_file("\n", path, 1);
    return lotus_write_file(data, path, 1);
}

ubyte lotus_write_file(const char* data, const char* path, ubyte preserve) {
    FILE* file = fopen(path, preserve ? "ab" : "wb");
    if (!file) {
        // error: failed to open file
        return LOTUS_FALSE;
    }

    size_t length = strlen(data);
    if (length <= 0) {
        // error: faield to compute length of write data\n
        fclose(file);
        return LOTUS_FALSE;
    }
    
    size_t bytesWritten = fwrite(data, sizeof(char), length, file);
    if (bytesWritten != length) {
        // error: failed to fully write data to file at path: %s\n", pat
        fclose(file);
        return LOTUS_FALSE;
    }

    fclose(file);
    return LOTUS_TRUE;
}

ubyte lotus_process_file(void (*processLine)(const char* line), const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        // error: failed to open file at path: %s\n", pat
        return LOTUS_FALSE;
    }

    char line[1024];
    do {
        processLine(line);
    } while(fgets(line, sizeof(line), file));

    fclose(file);
    return LOTUS_TRUE;
}
