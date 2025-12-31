

#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <filesystem>
#include <functional>

class DirectoryWatcher {
public:
    using Callback = std::function<void(const std::filesystem::path&)>;

    explicit DirectoryWatcher(std::filesystem::path path);
    static bool is_sc2_replay(const std::filesystem::path& p);
    void start(const Callback& cb) const;

private:
    std::filesystem::path path_;
};



#endif //DIRECTORYWATCHER_H
