//
// Created by 1 on 25-3-5.
//

#include <fstream>
#include <cassert>
#include "JsonParse.h"


std::shared_ptr<DBInfo> JsonParse::getDatabaseInfo(JsonParse::DBType type)
{
    std::string dbName = type == Mysql ? "mysql" : "redis";
    Json::Value node = m_root[dbName];
    DBInfo* info = new DBInfo;
    info->ip = node["ip"].asString();
    info->port = node["port"].asInt();
    if(type == Mysql)
    {
        info->user = node["user"].asString();
        info->passwd = node["password"].asString();
        info->dbname = node["dbname"].asString();
    }
    return std::shared_ptr<DBInfo>(info);
}


JsonParse::JsonParse(std::string fileName)
{
    // 打开文件
    std::ifstream ifs(fileName);
//    assert(ifs.is_open());
    ifs.is_open();
// 解析数据
    Json::Reader rd;
    rd.parse(ifs, m_root);
//    assert(m_root.isObject());
    m_root.isObject();
}


