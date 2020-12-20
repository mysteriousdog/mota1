#include "Music.h"
#include "../file/CsvData.h"
#include "../Def/FileDef.h"
#include "../Def/Def.h"
//#include <utility>
DECLARE_SINGLETON_MEMBER(Music);

Music::Music()
{
    musicMap = new std::map<std::string, int>();
}

Music::~Music()
{
    if (musicMap != nullptr) {
        delete musicMap;
    }
    musicMap = nullptr;
}

bool Music::getAudioId(std::string audioStr, OUT int& audioId)
{
    if (musicMap != nullptr && musicMap->find(audioStr) != musicMap->end()) {
        audioId = musicMap->at(audioStr);
        return true;
    }
    return false;
}

bool Music::SetMusicMap(std::string audioStr, int audioId)
{
    if (musicMap != nullptr) {
        musicMap->insert(std::pair<std::string, int>(audioStr, audioId));
        return true;
    }
    return false;
}

void Music::PLayMusic( std::string musicName) const
{
    std::string filePath;
	if (sParseCsv->GetCsvData(MUSIC_FILE_NAME, musicName, OUT filePath)) {
		std::cout<<"==========> "<<"filePath: "<<filePath<<std::endl;
		int audioID = cocos2d::AudioEngine::play2d(filePath, true);
		// 保存bgm对应的ID
		sMusic->SetMusicMap(musicName, audioID);
	}
}