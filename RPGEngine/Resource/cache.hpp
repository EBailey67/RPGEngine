#pragma once

#include <map>
#include <string>

template <typename Resource, void (*Eraser)(Resource) = nullptr>
class cache
{

public:
    using size_type = std::size_t;
    using resource_type = Resource;
    using id_type = std::string;
    using eraser_type = void (*)(resource_type);

    cache() = default;

    ~cache()
    {
        reset();
    }

    const eraser_type get_eraser() const noexcept
    {
        return Eraser;
    }

    size_type size() const noexcept
    {
        m_resources.size();
    }

    bool empty() const noexcept
    {
        m_resources.empty();
    }

    void reset()
    {
        if constexpr (Eraser)
        {
            for (auto &[id, resource] : m_resources)
            {
                Eraser(resource);
            }
        }

        m_resources.clear();
    }

    resource_type load(const id_type id, resource_type instance)
    {
        if (has(id))
        {
            erase(id);
        }
        return m_resources.emplace(id, instance).first->second;
    }

   
    resource_type resource(const id_type id)
    {
        return m_resources.at(id);
    }

    bool has(const id_type id) const noexcept
    {
        return (m_resources.find(id) != m_resources.end());
    }

    void erase(id_type id)
    {
        if constexpr (Eraser)
        {
            auto resource = m_resources.at(id);
            Eraser(resource);
        }
        m_resources.erase(id);
    }

private:
    std::map<id_type, resource_type> m_resources;
};

