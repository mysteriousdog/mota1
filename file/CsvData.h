#ifndef __CSVDATA_H__
#define __CSVDATA_H__
#include "cocos2d.h"
#include "../util/Singleton.h"
#include "../Def/Def.h"
#include <iostream>

typedef struct CsvData
{
    /* data */
    std::map<std::string, std::map<std::string, std::string> > TabData;
    CsvData()
	{
		TabData.clear();
	}
 
	bool GetData(std::string &filename, std::string &head, OUT std::string &value)
	{//获取数据
        if (IsExist(filename, head)) {
            value = TabData[filename][head];
            return true;
        }
        return false;
	}
 
	void PushData(std::string &key, std::map<std::string, std::string> &data )
	{//压入数据
		TabData[key] = data;//保存整行的数据
	}

    bool IsExistFile(const std::string &fileName)
    {// 判断文件是否存在
        if (TabData.find(fileName) != TabData.end()) {
            return true;
        }
        std::cout<<"in IsExistFile : dos no exists :"<<fileName<<std::endl;
        return false;
    }
    bool IsExist(std::string &fileName, std::string &head) 
    {//判断文件 + head是否存在
        if (IsExistFile(fileName) && TabData[fileName].find(head) != TabData[fileName].end()) {
            return true;
        }
        std::cout<<"in IsExist : dos no exists :"<<fileName<<"  "<<head<<std::endl;
        return false;
    }

    void PushData( std::string &fileName,  std::string &head,  std::string &value)
    {
        if (!IsExistFile(fileName)) {
            std::cout<<fileName<<" not exists in push:"<<std::endl;
            std::map<std::string, std::string> temp;
            TabData[fileName] = temp;
        }
        TabData[fileName][head] = value;
    }

}csvdata;

class ParseCsv : public Singleton<ParseCsv>
{
public:
    ParseCsv();
    ~ParseCsv();
    void Init();
    bool LoadCsv(std::string fileName, std::string path);
    bool GetCsvData(std::string fileName, std::string head, OUT std::string &value) const;
    std::vector<std::string> SplitString(const std::string &str, const std::string &splitStr);

private:
    CsvData* _para;
};

#define sParseCsv ParseCsv::instance()
#endif /* __CSVDATA_H__ */