#include "Config.h"
#include "DirectoryWatcher.h"
#include "Logger.h"
#include "ReplayPatcher.h"
#include "Sc2Version.h"
#include "Timer.h"

// int fix_single_replay(const int argc, char** argv)
// {
//     const auto Sc2Version = parse_sc2_version("5.0.15.95740");
//     if (!Sc2Version) return 1;
//     const auto patcher = ReplayPatcher{(Sc2Version.value())};
//
//     // 拖入文件模式
//     for (int i = 1; i < argc; ++i) {
//         std::filesystem::path filePath((argv[i]));
//         if ( DirectoryWatcher::is_sc2_replay(filePath)) {
//             patcher.patch(filePath);
//         } else {
//             std::cerr << "Not an SC2Replay file: " << filePath.string() << "\n";
//         }
//     }
//     return 0;
// }

int main(const int argc, char** argv)
{
    // if (argc > 1) return fix_single_replay(argc, argv);

    if (!Config::load()) return 1;
    const auto version = Config::get("version", "5.0.15.95740");
    const auto Sc2Version = parse_sc2_version(version);
    if (!Sc2Version)
    {
        log("version err");
        return 1;
    }
    auto patcher = std::make_shared<ReplayPatcher>(Sc2Version.value());
    const auto watcher = DirectoryWatcher{std::filesystem::path(Config::get("path"))};
    watcher.start([patcher](const std::filesystem::path& path)
    {
        struct RetryContext : std::enable_shared_from_this<RetryContext>
        {
            RetryContext(std::filesystem::path path, std::shared_ptr<ReplayPatcher> p): path(std::move(path)),
                patcher(std::move(p)){}
            std::filesystem::path path;
            std::shared_ptr<ReplayPatcher> patcher;
            size_t times = 1;

            void schedule()
            {
                if (!patcher -> patch(path))
                {
                    if (times > Config::get_int("retry_times", 10))
                    {
                        log("retry give up");
                        return;
                    }
                    log("retry, times:{}, filename: {}", times, path.filename().string());
                    auto self = shared_from_this();
                    const auto delay = std::chrono::milliseconds(2000 * (1ULL << times));
                    Timer::instance().schedule(delay, [self]
                    {
                        ++self->times ;
                        self->schedule();
                    });
                }
            }
        };
        auto p = std::make_shared<RetryContext>(path, patcher);
        p -> schedule();
    });
    return 0;
}
