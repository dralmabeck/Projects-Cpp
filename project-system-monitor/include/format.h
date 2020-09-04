#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <sstream>
#include <iomanip>

using std::string;
using std::stringstream;
using std::setw;
using std::setfill;

namespace Format {
    string ElapsedTime(long times);   // DONE - TODO: See src/format.cpp
};                                    // namespace Format

#endif