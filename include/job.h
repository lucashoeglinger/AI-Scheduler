#ifndef JOB_H
#define JOB_H
#include <vector>


enum class JOB_STATUS{
    APPROVED,
    REJECTED,
    PENDING
}

struct Job{
    int id,
    std::string user,
    std::string script_path,
    int vram_gb,
    int max_hours,
    int priority,
    JOB_STATUS status,
}

std::vector<Job> read_jobs(std::string& username, std::string& DB_PATH){}
bool write_jobs(std::vector<Job> jobs, std::string& DB_PATH){}

#endif