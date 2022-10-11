#pragma once

#include <memory>
#include <unordered_map>

namespace ServiceLocator
{
    class ManagerLocator
    {
    public:
        ManagerLocator() : instances() {}
        ~ManagerLocator() { clear(); }

        void clear()
        {
            instances.clear();
        }

        template<typename T>
        void registerInstance(T* instance)
        {
            const size_t hash = typeid(T).hash_code();
            if (instances.find(hash) == instances.end())
                instances.emplace(hash, std::shared_ptr<void>(instance));
        }

        template<typename T>
        std::shared_ptr<T> resolve() const
        {
            const size_t hash = typeid(T).hash_code();
            auto itr1 = instances.find(hash);
            if (itr1 != instances.end())
                return std::static_pointer_cast<T>(itr1->second);

            return nullptr;
        }

    private:
        std::unordered_map<size_t, std::shared_ptr<void>> instances;
    };
}