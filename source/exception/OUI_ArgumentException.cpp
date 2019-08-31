#include "exception/OUI_ArgumentException.h"

oui::ArgumentException::ArgumentException(std::string fileName, std::string functionName, std::string reason, std::string howToFix): 
    Exception(fileName, functionName, reason, howToFix) {
    
}