#ifndef __STR_LOGGER_H__
#define __STR_LOGGER_H__

#include <fstream>
#include <string>
#include <sstream>

class StrLogger {
private:
    /* data */
    std::string   file_name_;
    std::ofstream file_;
public:
    StrLogger(const std::string file_name = "");
    ~StrLogger();

    void setLogFile(const std::string file_name); //设置文件名
    void writeMsg(const std::string &message); //写入数据

};



#endif /* __STR_LOGGER_H__ */
