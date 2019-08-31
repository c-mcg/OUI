#ifndef OUI_ARGUMENT_EXCEPTION_H
#define OUI_ARGUMENT_EXCEPTION_H

#include "OUI_Export.h"
#include "exception/OUI_Exception.h"

namespace oui {

    class OUI_API ArgumentException : public Exception {
        
        public:
            ArgumentException(std::string fileName, std::string functionName, std::string reason, std::string howToFix);
        
    };

}

#endif