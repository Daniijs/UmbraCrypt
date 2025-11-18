#include "menu.hpp"
#include "visuals.hpp"
#include "crypto.hpp"
#include "fileutils.hpp"
#include "metadata.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <limits>

namespace {

    void waitEnter() {
        std::cout << std::endl;
        std::cout << "Pulsa ENTER para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void flushLine() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

}

namespace menu {

    std::string askFilePath() {
        std::string path;
        std::cout << "Introduce la ruta del archivo: ";
        if (std::cin.peek() == '\n') {
            flushLine();
        }
        std::getline(std::cin, path);
        return path;
    }

    std::string askPassword() {
        std::string password;
        for (;;) {
            std::cout << "Introduce la clave (mínimo 8 caracteres): ";
            if (std::cin.peek() == '\n') {
                flushLine();
            }
            std::getline(std::cin, password);

            if (password.size() < 8) {
                std::cout << "[!] La clave es demasiado corta." << std::endl;
            } else {
                break;
            }
        }
        return password;
    }

    std::string askPasswordConfirm() {
        std::string password;
        std::cout << "Confirma la clave: ";
        if (std::cin.peek() == '\n') {
            flushLine();
        }
        std::getline(std::cin, password);
        return password;
    }

    bool askYesNo(const std::string &msg) {
        for (;;) {
            std::cout << msg << " (s/n): ";
            char c;
            std::cin >> c;
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            if (c == 's') {
                flushLine();
                return true;
            } else if (c == 'n') {
                flushLine();
                return false;
            } else {
                std::cout << "Respuesta no válida." << std::endl;
                flushLine();
            }
        }
    }

    void encryptMenu() {
        visuals::showBanner();
        visuals::showSubTitle("Encriptar archivo");

        std::string path = askFilePath();
        if (!fileutils::fileExists(path)) {
            std::cout << "[!] El archivo no existe o no se puede abrir." << std::endl;
            waitEnter();
            return;
        }

        std::vector<unsigned char> buffer;
        if (!fileutils::readBinaryFile(path, buffer)) {
            std::cout << "[!] No se pudo leer el archivo." << std::endl;
            waitEnter();
            return;
        }

        if (metadata::isEncrypted(buffer)) {
            int layers = metadata::getLayers(buffer);
            std::cout << "[i] Este archivo ya está encriptado por UmbraCrypt." << std::endl;
            std::cout << "    Capas actuales: " << layers << std::endl;
            if (!askYesNo("¿Deseas añadir una nueva capa de encriptación?")) {
                return;
            }
        }

        std::string password;
        for (;;) {
            password = askPassword();
            std::string confirm = askPasswordConfirm();
            if (password != confirm) {
                std::cout << "[!] Las claves no coinciden. Inténtalo de nuevo." << std::endl;
            } else {
                break;
            }
        }

        bool backup = askYesNo("¿Quieres crear una copia de seguridad ANTES de encriptar?");
        if (backup) {
            std::cout << "Introduce la ruta donde guardar la copia: ";
            std::string backupPath;
            if (std::cin.peek() == '\n') {
                flushLine();
            }
            std::getline(std::cin, backupPath);

            if (!fileutils::copyFile(path, backupPath)) {
                std::cout << "[!] No se pudo crear la copia de seguridad." << std::endl;
            } else {
                std::cout << "[i] Copia de seguridad creada correctamente." << std::endl;
            }
        }

        visuals::progressBar("Encriptando...", 25);

        if (!crypto::encryptFile(path, password)) {
            std::cout << std::endl << "[!] Error durante el proceso de encriptación." << std::endl;
        } else {
            std::cout << std::endl << "[OK] Archivo encriptado y sobrescrito correctamente." << std::endl;
        }

        waitEnter();
    }

    void decryptMenu() {
        visuals::showBanner();
        visuals::showSubTitle("Desencriptar archivo (solo mostrar)");

        std::string path = askFilePath();
        if (!fileutils::fileExists(path)) {
            std::cout << "[!] El archivo no existe o no se puede abrir." << std::endl;
            waitEnter();
            return;
        }

        std::vector<unsigned char> buffer;
        if (!fileutils::readBinaryFile(path, buffer)) {
            std::cout << "[!] No se pudo leer el archivo." << std::endl;
            waitEnter();
            return;
        }

        if (!metadata::isEncrypted(buffer)) {
            std::cout << "[i] Este archivo no parece estar encriptado por UmbraCrypt." << std::endl;
            waitEnter();
            return;
        }

        int layers = metadata::getLayers(buffer);
        std::cout << "[i] Capas de encriptación detectadas: " << layers << std::endl;

        std::string password = askPassword();

        std::vector<unsigned char> currentData;
        if (!crypto::decryptOnce(path, password, currentData)) {
            std::cout << "[!] No se pudo desencriptar la primera capa." << std::endl;
            waitEnter();
            return;
        }

        visuals::separator();
        std::cout << "Contenido (tras 1 capa desencriptada):" << std::endl;
        visuals::separator();

        if (!currentData.empty()) {
            std::string asText(currentData.begin(), currentData.end());
            std::cout << asText << std::endl;
        } else {
            std::cout << "[i] El contenido está vacío tras la desencriptación." << std::endl;
        }

        int layerCount = 1;

        while (askYesNo("¿Deseas seguir desencriptando otra capa sobre este resultado?")) {
            layerCount = layerCount + 1;
            std::cout << "Introduce la clave para la capa " << layerCount << ": ";
            std::string nextPassword;
            if (std::cin.peek() == '\n') {
                flushLine();
            }
            std::getline(std::cin, nextPassword);

            std::vector<unsigned char> key;
            crypto::expandKey(nextPassword, key);
            crypto::xorBuffer(currentData, key);

            visuals::separator();
            std::cout << "Contenido (tras " << layerCount << " capas desencriptadas):" << std::endl;
            visuals::separator();

            if (!currentData.empty()) {
                std::string asText2(currentData.begin(), currentData.end());
                std::cout << asText2 << std::endl;
            } else {
                std::cout << "[i] El contenido está vacío tras la desencriptación." << std::endl;
            }
        }

        waitEnter();
    }

    void mainMenu() {
        bool running = true;

        while (running) {
            visuals::showBanner();
            std::cout << "1. Encriptar archivo" << std::endl;
            std::cout << "2. Desencriptar archivo (mostrar por consola)" << std::endl;
            std::cout << "3. Ver información de archivo" << std::endl;
            std::cout << "4. Salir" << std::endl;
            visuals::separator();
            std::cout << "Selecciona una opción: ";

            int option;
            std::cin >> option;
            if (std::cin.fail()) {
                std::cin.clear();
                flushLine();
                std::cout << "[!] Entrada no válida." << std::endl;
                waitEnter();
                continue;
            }

            flushLine();

            switch (option) {
                case 1:
                    encryptMenu();
                    break;
                case 2:
                    decryptMenu();
                    break;
                case 3: {
                    visuals::showBanner();
                    visuals::showSubTitle("Información de archivo");
                    std::string path = askFilePath();
                    if (!fileutils::fileExists(path)) {
                        std::cout << "[!] El archivo no existe o no se puede abrir." << std::endl;
                        waitEnter();
                        break;
                    }

                    std::vector<unsigned char> buffer;
                    if (!fileutils::readBinaryFile(path, buffer)) {
                        std::cout << "[!] No se pudo leer el archivo." << std::endl;
                        waitEnter();
                        break;
                    }

                    std::cout << std::endl;
                    if (metadata::isEncrypted(buffer)) {
                        int layers = metadata::getLayers(buffer);
                        std::cout << "[i] Archivo encriptado por UmbraCrypt." << std::endl;
                        std::cout << "    Capas de encriptación: " << layers << std::endl;
                        std::cout << "    Tamaño (bytes): " << buffer.size() << std::endl;
                    } else {
                        std::cout << "[i] El archivo NO está encriptado por UmbraCrypt." << std::endl;
                        std::cout << "    Tamaño (bytes): " << buffer.size() << std::endl;
                    }

                    waitEnter();
                    break;
                }
                case 4:
                    running = false;
                    break;
                default:
                    std::cout << "[!] Opción no válida." << std::endl;
                    waitEnter();
                    break;
            }
        }

        visuals::clearScreen();
        std::cout << "Saliendo de UmbraCrypt..." << std::endl;
    }

}
