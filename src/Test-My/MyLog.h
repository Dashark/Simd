#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include <fstream>
#include <string>
#include <sstream>

#include "Simd/SimdLib.hpp"

class MyLog
{
private:
    /* data */
    std::ofstream _file;
public:
    MyLog(/* args */);
    ~MyLog();

    void setLogFile(std::string name);
    void writeMsg(const std::string &message);

};



#endif /* __MY_LOG_H__ */