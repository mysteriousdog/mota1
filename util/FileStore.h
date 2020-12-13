#ifndef __FILESTORE_H__
#define __FILESTORE_H__

#include "Singleton.h"
#include "cocos2d.h"

class FileStore : public Singleton<FileStore>
{
public:
    
private:
    FileStore();
    ~FileStore();
    std::map<std::string, std::string> musicFileMap;
};

#endif /* __FILESTORE_H__ */