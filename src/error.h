#ifndef OT_ERROR_LOG_H
#define OT_ERROR_LOG_H

#include <string_view>

namespace ErrorLog {
void setup();
void log(std::string_view msg);
}

#endif // OT_ERROR_LOG_H
