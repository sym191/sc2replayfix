
#include "Sc2Version.h"

#include <charconv>

static bool parse_int(const std::string_view s, int& out) {
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), out);
    return ec == std::errc{} && ptr == s.data() + s.size();
}

std::optional<Sc2Version> parse_sc2_version(std::string_view s) {
    if (s.empty()) return std::nullopt;
    Sc2Version v{};
    size_t start = 0;
    size_t idx = 0;
    int* fields[4] = { &v.major, &v.minor, &v.patch, &v.build };

    for (int i = 0; i < 4; ++i) {
        idx = s.find('.', start);
        auto part = (i < 3)
            ? s.substr(start, idx - start)
            : s.substr(start);

        if (part.empty() || !parse_int(part, *fields[i]))
            return std::nullopt;

        if (i < 3) {
            if (idx == std::string_view::npos)
                return std::nullopt;
            start = idx + 1;
        }
    }
    return v;
}