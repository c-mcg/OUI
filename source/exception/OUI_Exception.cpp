#include "exception/OUI_Exception.h"

oui::Exception::Exception(std::string fileName, std::string functionName, std::string reason, std::string howToFix): 
    fileName{fileName}, functionName{functionName}, reason{reason}, howToFix{howToFix},
    std::runtime_error(reason) {
    
}

std::string oui::Exception::getFileName() {
    return fileName;
}

std::string oui::Exception::getFunctionName() {
    return functionName;
}

std::string oui::Exception::getReason() {
    return reason;
}

std::string oui::Exception::getHowToFix() {
    return howToFix;
}