#include "MyLog.h"

MyLog::MyLog(/* args */) {
}

MyLog::~MyLog() {
    if (_file.is_open()) {
            _file.close();
    }
}

void MyLog::setLogFile(std::string name) {
    _file.open(name);
}

void MyLog::writeMsg(const std::string &message) {
    if (_file.is_open()) {
                _file << message;
    }
}