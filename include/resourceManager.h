#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <unordered_map>
#include <string>

template<typename T>
class ResourceManager {
public:
    ResourceManager() {}
    ResourceManager& operator=(const ResourceManager&) = delete; 
    ResourceManager(const ResourceManager&) = delete;

    bool Remove(const std::string& fileName);
    T* Get(const std::string& fileName);
    ~ResourceManager();

private:
    bool Add(const std::string& fileName);
    std::pair<T*,  int>* Find(const std::string& fileName);
    void Free(const std::string& fileName);

    std::unordered_map<std::string, std::pair<T*, int>> resourceMap;
};

template<typename T>
std::pair<T*,  int>* ResourceManager<T>::Find(const std::string& fileName) {
    auto it = resourceMap.find(fileName);
    if (it != resourceMap.end()) {
        return &it->second;
    }
    else {
        return nullptr;
    }
}

template<typename T>
bool ResourceManager<T>::Add(const std::string& fileName) {
    auto res = Find(fileName);
    
    if (res) {
        ++res->second;
        return true;
    }

    T* resource = new T;
    resource->Load(fileName);

    if (!resource) {
        return false;
    }     
            
    resourceMap.emplace(fileName, std::make_pair(resource, 1));
    return true;
}

template<typename T>
bool ResourceManager<T>::Remove(const std::string& fileName) {
    auto res = Find(fileName);

    if (!res) {
        return false;
    }
        
    --res->second;
    if (!res->second) {
        delete res->first;
        resourceMap.erase(fileName);
    }  
        
    return true;
}

template<typename T>
T* ResourceManager<T>::Get(const std::string& fileName) {
    auto res = Find(fileName);
    if (!res) {
        bool ok = Add(fileName);
        if(!ok) {
            return nullptr;
        }
        res = Find(fileName);
    }
    return res->first;
}

template<typename T>
ResourceManager<T>::~ResourceManager() {
    while (resourceMap.begin() != resourceMap.end()) {
        delete resourceMap.begin()->second.first;
        resourceMap.erase(resourceMap.begin());
    }
}

#endif /* End of ResourceManager */