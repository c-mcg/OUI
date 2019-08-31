#include "exception/OUI_ParseException.h"

oui::ParseException::ParseException(std::string fileName, std::string functionName, std::string reason, std::string howToFix): 
    Exception(fileName, functionName, reason, howToFix) {
    
}