#ifndef __CSVDATA_H__
#define __CSVDATA_H__
#include "cocos2d.h"
#include "../util/Singleton.h"
#include "../Def.h"

typedef struct CsvData
{
    /* data */
    std::string fileName;
    std::map<std::string, std::map<std:string, std::string> > TabData;
    CsvData()
	{
		FileName = "";
		TabData.clear();
	}
 
	std::string GetData(std::string &key, std::string &head)
	{//获取数据
		return TabData[key][head];
	}
 
	void PushData(std::string key, std::map<std::string, std::string> &data )
	{//压入数据
		TabData[key] = data;//保存整行的数据
	}
 
	void SetFileName(std::string &fileName )
	{//设置文件名
		FileName = fileName;
	}

}csvdata;

class ParseCsv : public Singleton<ParseCsv>
{
public:
    ParseCsv();
    ~ParseCsv();
    bool LoadCsv(std::string fileName, std::string path);
    std::string GetCsvData(std::string fileName, std::string head);
    void SplitString(const std::string &str, OUT std::string &head, OUT std::string &data);

private:
    std::map<std::string, CsvData> *_fileCsvData;
};


#endif /* __CSVDATA_H__ */