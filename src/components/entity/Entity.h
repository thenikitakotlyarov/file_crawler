#pragma once

struct Entity {
    unsigned long id;

    bool transient;


    bool operator<(const Entity &rhs) const {
        return id < rhs.id;
    }


    bool operator>(const Entity &rhs) const {
        return id > rhs.id;
    }

    bool operator==(const Entity &other) const {
        return id == other.id;
    }


};

namespace std {
    template <>
    struct hash<Entity> {
        std::size_t operator()(const Entity& entity) const {
            return std::hash<unsigned long>{}(entity.id);
        }
    };
}
