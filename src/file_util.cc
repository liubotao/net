#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <string.h>
#include "file_util.h"

bool FileUtils::readFile(const char *path, std::string *content) {

    char newPath[1024] = {};
    if ('~' == path[0]) {
        snprintf(newPath, sizeof(newPath), "%s%s", getenv("HOME"), path + 1);
    } else {
        snprintf(newPath, sizeof(newPath), "%s", path);
    }

    bool ret = false;
    
    int fd = ::open(newPath, O_RDONLY);
    if (fd >= 0) {
        struct stat fileStat;
        if (0 == fstat(fd, &fileStat)) {
            content->resize(fileStat.st_size);
            
            if (read(fd, (char*) content->data(), fileStat.st_size) == fileStat.st_size) {
                ret = true;
            } else {
                printf("WARN: read(..., %llu) fail, errno %d, %s\n", (unsigned long long) fileStat.st_size, errno, strerror(errno));
            }
        } else {
            printf("WARN: stat %s faile, errno %d, %s\n", newPath, errno, strerror(errno));
        }

        close(fd);
    } else {
        printf("WARN: open %s fail, errno %d, %s\n", newPath, errno, strerror(errno));
    }

    return ret;
}


