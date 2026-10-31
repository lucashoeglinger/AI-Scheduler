#include "priority.hpp"
#include <algorithm>

namespace PriorityEngine {

int calculate_priority(int vram_gb, int max_hours, bool is_admin_boost) {
    int base_priority = 100;

    base_priority -= static_cast<int>(vram_gb * 1.25);
    base_priority -= max_hours * 2;

    if (is_admin_boost) {
        base_priority += 500;
    }

    return base_priority;
}

void sort_queue(std::vector<Job>& jobs) {
    std::sort(jobs.begin(), jobs.end(), 
        [](const Job& job_a, const Job& job_b) {
            if (job_a.priority != job_b.priority) {
                return job_a.priority > job_b.priority;
            }
            return job_a.id < job_b.id;
        });
}

} // namespace PriorityEngine