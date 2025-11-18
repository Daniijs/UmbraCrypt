#include "visuals.hpp"

#include <iostream>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(x) Sleep(x)
#else
#include <unistd.h>
#define SLEEP_MS(x) usleep((x) * 1000)
#endif

namespace {
    void printCentered(const std::string &text, int width) {
        int len = static_cast<int>(text.size());
        if (len >= width) {
            std::cout << text << std::endl;
            return;
        }
        int padding = (width - len) / 2;
        for (int i = 0; i < padding; i++) {
            std::cout << ' ';
        }
        std::cout << text << std::endl;
    }
}

namespace visuals {

    void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    }

    void showBanner() {
        clearScreen();
        const int width = 60;
        std::cout << std::endl;
        printCentered("============================================", width);
        printCentered("        U M B R A C R Y P T", width);
        printCentered("        \"Sombra en tus archivos\"", width);
        printCentered("============================================", width);
        std::cout << std::endl;
    }

    void showSubTitle(const std::string &text) {
        std::cout << std::endl;
        separator();
        std::cout << text << std::endl;
        separator();
        std::cout << std::endl;
    }

    void progressBar(const std::string &label, int totalSteps) {
        if (totalSteps <= 0) {
            return;
        }

        for (int step = 0; step <= totalSteps; step++) {
            int barWidth = 30;
            int pos = (barWidth * step) / totalSteps;

            std::cout << "\r[";
            for (int i = 0; i < barWidth; i++) {
                if (i < pos) {
                    std::cout << '#';
                } else if (i == pos) {
                    std::cout << '>';
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << "] " << label;

            std::cout.flush();
            SLEEP_MS(20);
        }
        std::cout << std::endl;
    }

    void separator() {
        std::cout << "--------------------------------------------" << std::endl;
    }

}
