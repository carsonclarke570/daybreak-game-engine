#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <vector>
#include <fstream>

namespace daybreak {
    class Util {
    public:
        static std::vector<char> read_file(const std::string& filename);
    };
}


#endif
