

#ifndef REPLAYFILE_H
#define REPLAYFILE_H

#include <filesystem>
#include <fstream>
#include <span>
#include <vector>

class ReplayFile {
public:
    explicit ReplayFile(std::filesystem::path path);

    bool open();
    bool backup() const;
    bool write_bytes(std::size_t offset, std::span<uint8_t> data);

private:
    std::filesystem::path path_;
    std::fstream file_;
};



#endif //REPLAYFILE_H
