#include "job.h"
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

std::vector<Job> read_jobs(const std::string& username, const std::string& db_path) {
    std::vector<Job> jobs;
    std::ifstream file(db_path);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open database file: " << db_path << std::endl;
        return jobs;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string token;
        Job j;

        if (std::getline(iss, token, ';')) j.id = std::stoi(token);
        if (std::getline(iss, token, ';')) j.user = token;
        if (std::getline(iss, token, ';')) j.script_path = token;
        if (std::getline(iss, token, ';')) j.vram_gb = std::stoi(token);
        if (std::getline(iss, token, ';')) j.max_hours = std::stoi(token);
        if (std::getline(iss, token, ';')) j.priority = std::stoi(token);
        if (std::getline(iss, token, ';')) j.status = string_to_status(token);

        if (username.empty() || j.user == username) {
            jobs.push_back(j);
        }
    }

    return jobs;
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