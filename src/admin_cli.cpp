#include "admin_cli.h"
#include "job.h"
#include "priority.h"
#include <iostream>
#include <limits>

namespace AdminCLI {

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
        std::cout << "  GPU Cluster Queue -- Admin: " << username << "\n";
        std::cout << "==========================================\n";
        std::cout << "1) Approve / Reject a job\n";
        std::cout << "2) Show ALL cluster jobs\n";
        std::cout << "3) Manual Priority Boost\n";
        std::cout << "4) Exit\n";
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

            if (all_jobs.empty()) {
                std::cout << "\n[i] Queue is completely empty.\n";
                continue;
            }

            std::cout << "\n--- PENDING JOBS ---\n";
            std::cout << "ID\tUser\tPrio\tVRAM\tTime\tScript\n";
            std::cout << "--------------------------------------------------------\n";
            bool has_pending = false;
            for (const auto& j : all_jobs) {
                if (j.status == JobStatus::PENDING) {
                    std::cout << j.id << "\t" << j.user << "\t" << j.priority << "\t"
                              << j.vram_gb << "GB\t" << j.max_hours << "h\t" << j.script_path << "\n";
                    has_pending = true;
                }
            }

            if (!has_pending) {
                std::cout << "No pending jobs waiting for approval.\n";
                continue;
            }

            std::cout << "\nEnter Job-ID to update (0 to cancel): ";
            int target_id;
            if (!(std::cin >> target_id) || target_id == 0) continue;

            std::cout << "Action: [1] Approve  [2] Reject: ";
            int action;
            std::cin >> action;

            bool found = false;
            for (auto& j : all_jobs) {
                if (j.id == target_id) {
                    if (action == 1) j.status = JobStatus::APPROVED;
                    else if (action == 2) j.status = JobStatus::REJECTED;
                    found = true;
                    break;
                }
            }

            if (found) {
                PriorityEngine::sort_queue(all_jobs);
                if (write_jobs(all_jobs, db_path)) {
                    std::cout << "\n[SUCCESS] Job #" << target_id << " status updated.\n";
                } else {
                    std::cout << "\n[ERROR] Failed to update database.\n";
                }
            } else {
                std::cout << "\n[!] Job-ID not found.\n";
            }

        } else if (choice == 2) {
            auto all_jobs = read_jobs("", db_path);

            std::cout << "\n--- ALL CLUSTER JOBS (" << all_jobs.size() << ") ---\n";
            if (all_jobs.empty()) {
                std::cout << "Queue is empty.\n";
            } else {
                std::cout << "ID\tUser\tPrio\tVRAM\tTime\tStatus\t\tScript\n";
                std::cout << "--------------------------------------------------------------------\n";
                for (const auto& j : all_jobs) {
                    std::cout << j.id << "\t" 
                              << j.user << "\t"
                              << j.priority << "\t" 
                              << j.vram_gb << "GB\t" 
                              << j.max_hours << "h\t" 
                              << status_to_color_string(j.status) << "\t" 
                              << j.script_path << "\n";
                }
            }

        } else if (choice == 3) {
            auto all_jobs = read_jobs("", db_path);
            std::cout << "\nEnter Job-ID to give admin boost (+500 Prio): ";
            int target_id;
            if (std::cin >> target_id) {
                for (auto& j : all_jobs) {
                    if (j.id == target_id) {
                        j.priority += 500;
                        break;
                    }
                }
                PriorityEngine::sort_queue(all_jobs);
                write_jobs(all_jobs, db_path);
                std::cout << "\n[SUCCESS] Priority boosted for Job #" << target_id << "\n";
            }

        } else if (choice == 4) {
            std::cout << "\nExiting Admin Interface. Goodbye!\n";
            running = false;
        } else {
            std::cout << "\n[!] Invalid option, please choose [1], [2], [3] or [4]\n";
        }
    }
}

} // namespace AdminCLI