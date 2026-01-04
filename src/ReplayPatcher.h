

#ifndef REPLAYPATCHER_H
#define REPLAYPATCHER_H

#include "Sc2Version.h"

#include <filesystem>

class ReplayPatcher {
public:
    explicit ReplayPatcher(Sc2Version target);
    [[nodiscard]] bool patch(const std::filesystem::path& replay) const;

private:
    static uint8_t to_vint(int);
    Sc2Version target_;
};



#endif //REPLAYPATCHER_H
