#ifndef VISUALS_HPP
#define VISUALS_HPP

#include <string>

namespace visuals {
    void clearScreen();
    void showBanner();
    void showSubTitle(const std::string &text);
    void progressBar(const std::string &label, int totalSteps);
    void separator();
}

#endif

