#include "GameApp.h"
#include <iostream>

int main() {
    try {
        GameApp::getInstance().run();
    } catch (const std::exception &e) {
        std::cerr << "\n[CRITICAL ERROR]: " << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cerr << "\n[UNKNOWN ERROR]\n";
        return -2;
    }
    return 0;
}