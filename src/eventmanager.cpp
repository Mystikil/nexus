#include "eventmanager.h"
#include "luascript.h"
#include "otpch.h"

namespace {
std::unordered_map<std::string, std::vector<EventListener>> listeners;
}

void EventManager::registerEvent(const std::string &name,
                                 LuaScriptInterface *interface,
                                 int32_t scriptId, int32_t functionRef) {
  listeners[name].push_back({interface, scriptId, functionRef});
}

void EventManager::emit(const std::string &name,
                        const std::function<int(lua_State *)> &pushFunc) {
  auto it = listeners.find(name);
  if (it == listeners.end()) {
    return;
  }

  for (const EventListener &listener : it->second) {
    if (!lua::reserveScriptEnv()) {
      std::cout << "[Error - EventManager::emit] Call stack overflow"
                << std::endl;
      continue;
    }

    ScriptEnvironment *env = lua::getScriptEnv();
    env->setScriptId(listener.scriptId, listener.interface);

    lua_State *L = listener.interface->getLuaState();
    lua_rawgeti(L, LUA_REGISTRYINDEX, listener.functionRef);

    int argCount = 0;
    if (pushFunc) {
      argCount = pushFunc(L);
    }

    if (lua::protectedCall(L, argCount, 0) != 0) {
      lua::reportError(__FUNCTION__, lua::popString(L), L, true);
    }

    lua::resetScriptEnv();
  }
}
