

#include <algorithm>
#include <fstream>

#include "Config.h"

std::string trim(std::string s) {
    auto is_space = [](unsigned char c){ return std::isspace(c); };

    const auto first = std::ranges::find_if_not(s, is_space);
    const auto last  = std::find_if_not(s.rbegin(), s.rend(), is_space).base();

    return (first < last) ? std::string(first, last) : "";
}

bool Config::load()
{
    std::ifstream ifs(default_config_path());
    if (!ifs) return false;

    data.clear();
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#') continue;

        const auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string value = line.substr(pos + 1);

        // 去掉首尾双引号
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }

        data[trim(line.substr(0, pos))] = trim(value);
    }
    return true;
}

std::string Config::get(const std::string& key, const std::string& def)
{
    const auto it = data.find(key);
    return it != data.end() ? it->second : def;
}

int Config::get_int(const std::string& key, int def)
{
    const auto it = data.find(key);
    if (it == data.end()) return def;
    try {
        return std::stoi(it->second);
    } catch (...) {
        return def;
    }
}

bool Config::get_bool(const std::string& key, bool def)
{
    const auto it = data.find(key);
    if (it == data.end()) return def;
    return it->second == "true" || it->second == "1";
}

std::filesystem::path Config::default_config_path()
{
    return std::filesystem::current_path() / "config.conf";
}
