#ifndef FS_EVENTMANAGER_H
#define FS_EVENTMANAGER_H

#include "luascript.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

struct EventListener {
  LuaScriptInterface *interface;
  int32_t scriptId;
  int32_t functionRef;
};

class EventManager {
public:
  static void registerEvent(const std::string &name,
                            LuaScriptInterface *interface, int32_t scriptId,
                            int32_t functionRef);
  static void emit(const std::string &name,
                   const std::function<int(lua_State *)> &pushFunc);
};

#endif // FS_EVENTMANAGER_H
