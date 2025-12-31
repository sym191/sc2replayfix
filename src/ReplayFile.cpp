
#include "ReplayFile.h"

#include <span>

ReplayFile::ReplayFile(std::filesystem::path path):path_(std::move(path))
{
}

bool ReplayFile::open()
{
    file_.open(path_, std::ios::binary | std::ios::in | std::ios::out);
    return file_.is_open();
}

bool ReplayFile::backup() const
{
    try
    {
        const auto bak = std::filesystem::current_path() / "bak";
        std::filesystem::create_directories(bak);
        std::filesystem::copy_file(path_, bak/path_.filename(),
            std::filesystem::copy_options::overwrite_existing);
        return true;
    }catch (...)
    {
        return false;
    }
}

bool ReplayFile::write_bytes(std::size_t offset, std::span<uint8_t> data)
{
    file_.seekp(offset);
    if (!file_) return false;
    file_.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file_.good();
}


