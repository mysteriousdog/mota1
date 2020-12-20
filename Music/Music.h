#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "MTGame.h"
//#include <jsoncpp/json/json.h>
USING_NS_CC;


class Music : public Singleton<Music>
{
public:
    Music();
    ~Music();
    bool getAudioId(std::string audioStr, OUT int& audioId);
    bool SetMusicMap(std::string audioStr, int audioId);
    void PLayMusic( std::string musicName, bool loop = false) const;
    void StopMusic(std::string musicname);
private:
    
    std::map<std::string, int> *musicMap;
};

#define sMusic Music::instance()
#endif