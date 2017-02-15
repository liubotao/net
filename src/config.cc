#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "file_util.h"

Config::Config() {}

Config::~Config() {}

bool Config::initConfig(const char *path) {
    bool ret = FileUtils::readFile(path, &content_);
    if (ret) {
        content_.insert(0, "\n");
    }

    return ret;
}

void Config::setContent(const std::string &content) {
    content_.clear();
    content_.append("\n").append(content);
}

bool Config::readItem(const char *section, const char *key, int *value) {
    char tmp[128] = {};
    bool ret = readItem(section, key, tmp, sizeof(tmp));
    if (ret) {
        *value = atoi(tmp);
    }
    return ret;
}

bool Config::readItem(const char *section, const char *key, int *value, const int defaultValue) {
    char tmp[128] = {};
    bool ret = readItem(section, key, tmp, sizeof(tmp));
    if (ret) {
        *value = atoi(tmp);
    } else {
        *value = defaultValue;
    }

    return ret;
}

bool Config::readItem(const char *section, const char *key, char *value, size_t size, char *defaultValue) {
    bool ret = readItem(section, key, value, size);
    if (!ret) {
        snprintf(value, size, "%s",  defaultValue); 
    }

    return ret;
}

bool Config::readItem(const char *section, const char *key, char *value, size_t size) {
    bool ret = false;

    char tmpSection[128] = {};
    snprintf(tmpSection, sizeof(tmpSection), "\n[%s]", section);

    char tmpKey[128] = {};
    snprintf(tmpKey, sizeof(tmpKey), "\n%s", key);

    const char *endPos = NULL;
    const char *pos = strstr(content_.c_str(), tmpSection);
    //printf("tmpSection:%s", tmpSection);
    //printf("pos:%s\n", pos);
    //printf("content:%s\n", content_.c_str());

    if (NULL != pos) {
        pos = strchr(pos + 1, '\n');
        if (NULL == pos) {
            pos = strchr(content_.c_str(), '\0');
        }

        endPos = strstr(pos, "\n[");
        if (NULL == endPos) {
            endPos = strchr(pos, '\0');
        }
    }


    for (; NULL != pos && pos < endPos;) {
        pos = strstr(pos, tmpKey);
        
        if (NULL == pos || pos > endPos) {
            break;
        }

        const char *tmpPos = pos + strlen(tmpKey);
        if ((!isspace(*tmpPos)) && ('=' != *tmpPos)) {
            continue;
        }
   
        pos++;
        const char *eol = strchr(pos, '\n');
        
        if (NULL == eol) {
            eol = strchr(pos, '\0');
        }

        tmpPos = strchr(pos, '=');
        if (NULL != tmpPos && tmpPos < eol) {
            ret = true;
        
            for (tmpPos++; tmpPos <= eol && isspace(*tmpPos);) {
                tmpPos++;
            }
  
            //printf("tmpPos:%d\n", tmpPos);
            //printf("eolPos:%d\n", eol);
 
            for (size_t i = 0; tmpPos <= eol &&  i < (size - 1); i++) {
                if (isspace(*tmpPos)) {
                    break;
                }
              //printf("i:%d\n",i); 
                //printf("tmpPos:%c\n", *(tmpPos++));             
                *(value++) = *(tmpPos++);

                //printf("value:%s\n", value);
            }
            
            *value = '\0';

            break;
        }
    }

    return ret;
}

