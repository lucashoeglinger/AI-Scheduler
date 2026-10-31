#include "admin_cli.h"
#include "user_cli.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <string>

const std::string DB_PATH = "/var/log/gpu_queue.txt";

bool is_admin() {
    return geteuid() == 0;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--admin") {
        if (!is_admin()) {
            std::cerr << "[SECURITY ERROR] Unauthorized access attempt. Root privileges required for admin mode!\n";
            return 1;
        }

        const char* env_user = std::getenv("USER");
        std::string admin_user = env_user ? env_user : "root";

        AdminCLI::run(admin_user, DB_PATH);
        return 0;
    }

    const char* env_user = std::getenv("USER");
    if (!env_user || std::string(env_user).empty()) {
        std::cerr << "[SECURITY ERROR] Invalid session: USER environment variable missing.\n";
        return 1;
    }

    UserCLI::run(std::string(env_user), DB_PATH);
    return 0;
}