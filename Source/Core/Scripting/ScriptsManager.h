﻿#ifndef IU_CORE_SCRIPTING_SCRIPTSMANAGER_H
#define IU_CORE_SCRIPTING_SCRIPTSMANAGER_H

#pragma once
#include <thread>
#include <mutex>
#include "Script.h"
#include "Core/Utils/CoreTypes.h"
#include "Core/Upload/ServerSync.h"

class ScriptsManager {
public:
    enum ScriptType {TypeUploadFilterScript};
    ScriptsManager();
    ~ScriptsManager();
    Script* getScript(std::string &fileName, ScriptType type);
    void unloadScripts();
    void clearThreadData();
    ServerSync* getServerSync(const std::string& fileName);
protected:
    std::map<std::thread::id, std::map<const std::string,Script*>> scripts_;
    std::mutex scriptsMutex_;
    std::string scriptsDirectory_;
    typedef std::string ServerSyncMapKey;
    std::map<ServerSyncMapKey, ServerSync*> serverSyncs_;
    std::mutex serverSyncsMutex_;
private:
    DISALLOW_COPY_AND_ASSIGN(ScriptsManager);
};


#endif