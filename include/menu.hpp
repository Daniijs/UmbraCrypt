#ifndef MENU_HPP
#define MENU_HPP

#include <string>

namespace menu {
    void mainMenu();
    void encryptMenu();
    void decryptMenu();

    std::string askFilePath();
    std::string askPassword();
    std::string askPasswordConfirm();

    bool askYesNo(const std::string &msg);
}

#endif

