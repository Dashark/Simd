#include "lbpSrc/StrLogger.h"

StrLogger::StrLogger(const std::string file_name) {
    file_name_ = file_name;
    if (!file_name_.empty()) {
        file_.open(file_name_);
    }
}

StrLogger::~StrLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void StrLogger::setLogFile(std::string file_name) {
    if (file_name_ != file_name && file_.is_open()) {
        file_.close();
        file_.open(file_name);
    }
}

void StrLogger::writeMsg(const std::string &message) {
    if (file_.is_open()) {
            file_ << message;
    }
}
