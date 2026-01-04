

#include "ReplayPatcher.h"

#include <iostream>

#include "Logger.h"
#include "ReplayFile.h"

// 版本号相关字段在立项文件偏移量是43
static constexpr std::size_t VERSION_OFFSET = 43;

ReplayPatcher::ReplayPatcher(const Sc2Version target): target_(target)
{
}

bool ReplayPatcher::patch(const std::filesystem::path& replay) const
{
    auto file = ReplayFile(replay);
    if (!file.open())
    {
        log("open failed");
        return false;
    }

    if(!file.backup())
    {
        log("backup failed");
        return false;
    }

    std::vector<uint8_t> pac = {
        0x02,0x05,0x0c,0x00,0x09,0x02,0x02,0x09,
        to_vint(target_.major),
        0x04,0x09,
        to_vint(target_.minor),
        0x06,0x09,
        to_vint(target_.patch)
    };

    if(!file.write_bytes(VERSION_OFFSET, pac))
    {
        log("write failed");
        return false;
    }

    log("fix succuss" + replay.string());

    return true;
}

uint8_t ReplayPatcher::to_vint(const int v)
{
    if (v < 0 || v > 0x3f)
        throw std::runtime_error("version overflow");
    return (v & 0x3f) << 1;
}

