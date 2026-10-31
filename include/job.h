#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>

enum class JobStatus {
    APPROVED,
    REJECTED,
    PENDING
};

struct Job {
    int id;
    std::string user;
    std::string script_path;
    int vram_gb;
    int max_hours;
    int priority;
    JobStatus status;
};

std::string status_to_string(JobStatus status);
JobStatus string_to_status(const std::string& str);

std::vector<Job> read_jobs(const std::string& username, const std::string& db_path);
bool write_jobs(const std::vector<Job>& jobs, const std::string& db_path);

#endif // JOB_H