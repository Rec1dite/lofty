#include <iostream>

// void heading(const std::string& msg, unsigned int newlinesBefore = 1, unsigned int newlinesAfter = 1) {
//     for (unsigned int i = 0; i < newlinesBefore; i++) std::cout << std::endl;
//     std::cout << "\033[96m========== " << msg << " ==========\033[0m";
//     for (unsigned int i = 0; i < newlinesAfter; i++) std::cout << std::endl;
// }

const char* HEADING_FORMAT = "\033[96m========== %s ==========\033[0m";
const char* SUBHEADING_FORMAT = "\033[96m%s:\033[0m";

void heading(const char* msg, unsigned int newlinesBefore = 0, unsigned int newlinesAfter = 1) {
    std::cout << std::string(newlinesBefore, '\n');
    printf(HEADING_FORMAT, msg);
    std::cout << std::string(newlinesAfter, '\n');
}

void subheading(const char* msg, unsigned int newlinesBefore = 0, unsigned int newlinesAfter = 0) {
    std::cout << std::string(newlinesBefore, '\n');
    printf(SUBHEADING_FORMAT, msg);
    std::cout << std::string(newlinesAfter, '\n');
}

template<class T>
void result(const T& res) {
    std::cout << "\033[93mResult: " << res << "\033[0m" << std::endl;
}