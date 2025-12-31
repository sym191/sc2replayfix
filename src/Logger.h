

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <format>


inline std::string now_string()
{
    using namespace std::chrono;

    const auto now = system_clock::now();
    std::chrono::zoned_time local_time{ current_zone(), now };
    return std::format("{:%Y-%m-%d %H:%M:%S}", local_time);
}


inline void append_to_file(const std::filesystem::path& path,
                           std::string_view text)
{
    std::ofstream ofs(path,
        std::ios::binary | std::ios::app); // app = 追加到末尾

    if (!ofs) {
        throw std::runtime_error("open file failed");
    }

    ofs.write(text.data(), text.size());
}

inline void log(std::string_view msg)
{
    static const auto log_file =
        std::filesystem::current_path() / "patch.log";
    std::cout << msg << std::endl;
    append_to_file(log_file,
        now_string() + " " + std::string(msg) + "\n");
}


#endif //LOGGER_H
