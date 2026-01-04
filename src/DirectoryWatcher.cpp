

#include "DirectoryWatcher.h"

#include <iostream>
#include <windows.h>
#include <fstream>
#include <thread>

#include "Config.h"
#include "Logger.h"

DirectoryWatcher::DirectoryWatcher(std::filesystem::path path): path_(std::move(path))
{
}

bool wait_until_file_ready(const std::filesystem::path& p) {
    /*
     *从Windows发出文件创建事件到立项文件完成写入整个过程需要一段时间，但是这里不太方便直接监听完成写入，暂时就用sleep简单处理了。
     */
    // for (int i = 0; i < 10; ++i) {
    //     std::ifstream f(p, std::ios::binary);
    //     if (f) return true;
    //     Sleep(200);
    // }
    // return false;
    std::this_thread::sleep_for(std::chrono::seconds(Config::get_int("wait_time", 3)));
    return true;
}

bool DirectoryWatcher::is_sc2_replay(const std::filesystem::path& p)
{
    std::cout << p.string() << std::endl;
    if (!p.has_extension()) return false;

    auto ext = p.extension().wstring();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);

    return ext == L".sc2replay";
}


void DirectoryWatcher::start(const Callback& cb) const
{
    const HANDLE hDir = CreateFileW(
        path_.wstring().c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        nullptr
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        log("open dict failed");
        return;
    }

    char buffer[4096];
    DWORD bytesReturned;

    log("Watching directory...");

    while (true) {
        if (!ReadDirectoryChangesW(
                hDir,
                buffer,
                sizeof(buffer),
                FALSE,
                FILE_NOTIFY_CHANGE_FILE_NAME,
                &bytesReturned,
                nullptr,
                nullptr
            )) {
            log("ReadDirectoryChangesW failed");
            break;
            }
        char* ptr = buffer;
        const char* end = buffer + bytesReturned;
        while (ptr < end) {
            auto* fni =
                reinterpret_cast<FILE_NOTIFY_INFORMATION*>(ptr);
            if (fni->Action == FILE_ACTION_ADDED) {
                std::wstring filename(
                    fni->FileName,
                    fni->FileNameLength / sizeof(wchar_t)
                );
                auto fullpath = path_ / filename;
                log("add file: " + fullpath.string());
                if (is_sc2_replay(fullpath)){
                    wait_until_file_ready(fullpath);
                    cb(fullpath);
                }
            }
            if (fni->NextEntryOffset == 0)
                break;
            ptr += fni->NextEntryOffset;
        }
    }

    CloseHandle(hDir);
}
