#include "user_cli.hpp"
#include "job.hpp"
#include "priority.hpp"
#include <iostream>
#include <limits>

namespace UserCLI {

static std::string status_to_color_string(JobStatus status) {
    switch (status) {
        case JobStatus::APPROVED: return "[APPROVED]";
        case JobStatus::REJECTED: return "[REJECTED]";
        case JobStatus::PENDING:  return "[PENDING]";
    }
    return "[UNKNOWN]";
}

void run(const std::string& username, const std::string& db_path) {
    bool running = true;

    while (running) {
        std::cout << "\n==========================================\n";
        std::cout << "  GPU Cluster Queue -- User: " << username << "\n";
        std::cout << "==========================================\n";
        std::cout << "1) Queue a new job\n";
        std::cout << "2) Show my jobs\n";
        std::cout << "3) Exit\n";
        std::cout << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[!] Invalid input, please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            auto all_jobs = read_jobs("", db_path);

            Job new_job;
            new_job.id = all_jobs.empty() ? 1 : (all_jobs.back().id + 1);
            new_job.user = username;
            new_job.status = JobStatus::PENDING;

            std::cout << "\n--- Enter a new job ---\n";
            std::cout << "Scriptpath (z. B. /home/" << username << "/train.py): ";
            std::cin >> new_job.script_path;

            std::cout << "Used VRAM (GB): ";
            while (!(std::cin >> new_job.vram_gb) || new_job.vram_gb <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please enter a valid number of GB: ";
            }

            std::cout << "Max. Runtime (hours): ";
            while (!(std::cin >> new_job.max_hours) || new_job.max_hours <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please enter a valid number of hours: ";
            }

            new_job.priority = PriorityEngine::calculate_priority(new_job.vram_gb, new_job.max_hours);

            all_jobs.push_back(new_job);
            PriorityEngine::sort_queue(all_jobs);

            if (write_jobs(all_jobs, db_path)) {
                std::cout << "\n[SUCCESS] Job #" << new_job.id << " successfully initialized!\n";
                std::cout << "Calculate Priority: " << new_job.priority << "\n";
                std::cout << "Status: " << status_to_color_string(new_job.status) << "\n";
            } else {
                std::cout << "\n[ERROR] Encountered a unknown error while saving to databaes.\n";
            }

        } else if (choice == 2) {
            auto user_jobs = read_jobs(username, db_path);

            std::cout << "\n--- Meine Jobs (" << user_jobs.size() << ") ---\n";
            if (user_jobs.empty()) {
                std::cout << "Keine aktiven oder vergangenen Jobs gefunden.\n";
            } else {
                std::cout << "ID\tPrio\tVRAM\tTime\tStatus\t\tScript\n";
                std::cout << "----------------------------------------------------------------\n";
                for (const auto& j : user_jobs) {
                    std::cout << j.id << "\t" 
                              << j.priority << "\t" 
                              << j.vram_gb << "GB\t" 
                              << j.max_hours << "h\t" 
                              << status_to_color_string(j.status) << "\t" 
                              << j.script_path << "\n";
                }
            }

        } else if (choice == 3) {
            std::cout << "\nGood Bye!\n";
            running = false;
        } else {
            std::cout << "\n[!]Invalid option, please choose [1], [2] or [3]\n";
        }
    }
}

} // namespace UserCLI