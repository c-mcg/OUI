#ifndef OUI_EXCEPTION_H
#define OUI_EXCEPTION_H

#include <stdexcept>

#include "OUI_Export.h"

namespace oui {

    class OUI_API Exception : public std::runtime_error {
        
        public:
            Exception(std::string fileName, std::string functionName, std::string reason, std::string howToFix);
            std::string getFileName();
            std::string getFunctionName();
            std::string getReason();
            std::string getHowToFix();

        
        private:
            std::string fileName;
            std::string functionName;
            std::string reason;
            std::string howToFix;
        
    };

}

#endif