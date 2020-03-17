#ifndef CONFIG_H
#define CONFIG_H
#include <dirent.h>

static const char globaldir[]= "/home/acewong/server/network";

static bool isExist(const char* dir){
    DIR* temp = nullptr;
    temp = opendir(dir);
    if(temp == nullptr){
        closedir(temp);
        return false;
    }else{
        closedir(temp);
        return true;
    }
}

#endif // CONFIG_H
