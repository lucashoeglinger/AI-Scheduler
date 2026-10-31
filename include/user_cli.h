#ifndef USERCLI_H
#define USERCLI_H
#include <string>
#include "job.h"

namespace UserCLI{
    static std::string status_to_color_string(JobStatus status);
    void run (const std::string& username, const std::string& db_path);
}
#endif