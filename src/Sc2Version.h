

#ifndef SC2VERSION_H
#define SC2VERSION_H
#include <optional>
#include <string_view>


struct Sc2Version {
    int major{};
    int minor{};
    int patch{};
    int build{};
};

std::optional<Sc2Version> parse_sc2_version(std::string_view s);

#endif //SC2VERSION_H
