#include "CsvData.h"
#include "cocos2d.h"
#include <iostream>

DECLARE_SINGLETON_MEMBER(ParseCsv);
USING_NS_CC;
ParseCsv::ParseCsv()
{
    _para = new CsvData();
}

ParseCsv::~ParseCsv()
{
    if (_para != nullptr) {
        delete _para;
    }
    _para = nullptr;
}

/*
*def：初始化 读入配置文件
*/
void Init()
{

}

/*
*def：按照分割符分割字符串
*@para1: 待分割的字符串
*@para2: 分割符号
*/
std::vector<std::string> ParseCsv::SplitString(const std::string &str, const std::string &splitStr)
{
    std::vector<std::string> vecStr;
    int startPos = 0;
    int splitLen = splitStr.length();
    int index;
    std::cout<<"split= "<<splitStr<<" len = "<<splitLen<<std::endl;
    while((index = str.find(splitStr, startPos)) != str.npos) {
        vecStr.push_back(str.substr(startPos, index - startPos));
        std::cout<<"startPos= "<<startPos<<" index= "<<index<<std::endl;
        startPos = index + splitLen;
    }
    std::string lastStr = str.substr(startPos);
    if (!lastStr.empty()) {
        vecStr.push_back(lastStr);
    }
    return vecStr;
}

/*
*def：加载CSV文件
*@para1: 文件名
*@para2: 文件相对路径
*/
bool ParseCsv::LoadCsv( std::string fileName,  std::string path)
{
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(path);
    size_t buffSize = 0;
    std::string buff = FileUtils::getInstance()->getStringFromFile(filePath);
    std::vector<std::string> vecTab = SplitString(buff, "\r\n");
    std::cout<<vecTab.size()<<std::endl;
    for (auto it = vecTab.begin(); it != vecTab.end(); it++) {
        std::cout<<"=========================="<<std::endl;
        std::vector<std::string> splitVec = SplitString(*it, "\t");
        std::cout<<*it<<std::endl;
        if (splitVec.size() <= 1) {
            continue;
        }
        _para->PushData(fileName, splitVec[0], splitVec[1]);
        std::cout<<splitVec[0]<<" ---  "<<splitVec[1]<<std::endl;
    }
    _para->Show(fileName);
    return true;
}

/*
*def：获取Csv文件对应文件名 的key的value
*@para1: 文件名
*@para2: csv文件中的key值
*/
bool ParseCsv::GetCsvData(std::string fileName, std::string head, OUT std::string &value)
{
    return _para->GetData(fileName, head, value);

}