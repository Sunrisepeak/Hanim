#ifndef UTILS_HPP_HANIM
#define UTILS_HPP_HANIM

#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace hanim {

static std::string get_current_date() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_t);

    // YY-MM-DD
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%y-%m-%d");
    return oss.str();
}

}

#endif