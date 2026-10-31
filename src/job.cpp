#include "job.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

std::string status_to_string(JobStatus status) {
    switch (status) {
        case JobStatus::APPROVED: return "APPROVED";
        case JobStatus::REJECTED: return "REJECTED";
        case JobStatus::PENDING:  return "PENDING";
    }
    return "PENDING";
}

JobStatus string_to_status(const std::string& str) {
    if (str == "APPROVED") return JobStatus::APPROVED;
    if (str == "REJECTED") return JobStatus::REJECTED;
    return JobStatus::PENDING;
}

void read_jobs(const std::string& username, const std::string& DB_PATH) {
    std::ifstream file(DB_PATH);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open database file: " << DB_PATH << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> linesToUser;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;

        int i = 0;
        while (std::getline(iss, token, ';')) {
            if (i == 1) {
                if (token == username) {
                    linesToUser.push_back(line);
                }
                break;
            }
            i++;
        }
    }

    for (const auto l : linesToUser) {
        std::cout << l << std::endl;
    }
}

bool write_jobs(const std::vector<Job>& jobs, const std::string& db_path) {
    std::ofstream file(db_path, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open database for overwriting: " << db_path << std::endl;
        return false;
    }

    for (const auto& job : jobs) {
        file << job.id << ";"
             << job.user << ";"
             << job.script_path << ";"
             << job.vram_gb << ";"
             << job.max_hours << ";"
             << job.priority << ";"
             << status_to_string(job.status) << "\n";
    }

    return file.good();
}