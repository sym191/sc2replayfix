

#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <filesystem>

class Config {
public:
    static bool load();

    static std::string get(const std::string& key,const std::string& def = "");

    static int get_int(const std::string& key, int def = 0);

    static bool get_bool(const std::string& key, bool def = false);

    static std::filesystem::path default_config_path();

private:
    inline static std::unordered_map<std::string, std::string> data;
};


#endif //CONFIG_H
