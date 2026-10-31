# GPU Queue Wrapper (C++17)

A lightweight, secure, and POSIX-integrated C++17 GPU job scheduling wrapper designed for shared academic multi-user servers. It manages computational job queues, calculates priorities based on resource requests (VRAM & Runtime), and enforces system-level access controls.

---

## Key Features

- **Resource-Aware Scheduling**: Dynamically calculates job priority based on requested VRAM (GB) and expected runtime (hours).
- **Security & Privilege Control**: Uses POSIX system calls (`geteuid()`) and SSH group checks (`ml-students`) to isolate student and administrator workflows.
- **Flat-File Persistence**: Stores structured job statuses and priority queues in a semicolon-delimited CSV database (`/var/log/gpu_queue.txt`).
- **Input Hardening**: Prevents terminal stream corruptions and invalid input crashes via input stream sanitization (`std::cin.ignore`).
- **Complete Build Pipeline**: Includes Makefile support with automated test checks (`make check`) and distribution packaging (`make distcheck`).

---

## System Architecture

gpu-queue-wrapper/
├── include/
│   ├── admin_cli.h       # Administrator interface declarations
│   ├── job.h             # Job struct, JobStatus enum & CSV I/O
│   ├── priority.h        # Priority engine algorithm & sorting
│   └── user_cli.h        # Student/User interface declarations
├── src/
│   ├── admin_cli.cpp     # Admin job approval & queue management
│   ├── job.cpp           # CSV file persistence logic
│   ├── main.cpp          # Entrypoint & POSIX privilege verification
│   ├── priority.cpp      # Resource priority calculation engine
│   └── user_cli.cpp      # Interactive user submission CLI
├── Makefile              # Build automation script
├── LICENSE               # PolyForm Noncommercial 1.0.0 (No-Malicious Clause)
└── README.md             # Project documentation

---

## Installation & Building

### Prerequisites

Ensure `g++` (supporting C++17) and `make` are installed on your Linux system:

# Ubuntu / Debian
sudo apt update && sudo apt install build-essential g++

# Arch Linux
sudo pacman -S base-devel gcc

# Fedora / RHEL
sudo dnf install gcc-c++ make

### Build Steps

1. **Clone the repository:**
   git clone https://github.com/your-username/gpu-queue-wrapper.git
   cd gpu-queue-wrapper

2. **Compile the binary:**
   make

3. **Run automated checks:**
   make check

4. **Verify release package build:**
   make distcheck

5. **Install system-wide:**
   sudo make install

---

## Setup & Database Permissions

The wrapper uses `/var/log/gpu_queue.txt` to store queue states. Create the database file and set group permissions for student access:

# Create the log file
sudo touch /var/log/gpu_queue.txt

# Option A: Restrict write access to the 'ml-students' group
sudo chown root:ml-students /var/log/gpu_queue.txt
sudo chmod 660 /var/log/gpu_queue.txt

# Option B: Grant write access to all local users
sudo chmod 666 /var/log/gpu_queue.txt

---

## Usage

### User Mode (Students)
Start the CLI as a standard user to submit or inspect GPU job requests:

queue-wrapper

### Admin Mode (Cluster Administrators)
Requires root privileges to approve, reject, or manually boost job priorities:

sudo queue-wrapper --admin

---

## License & Acceptable Use

Distributed under the **PolyForm Noncommercial License 1.0.0** with an explicit **No-Malicious-Use Clause**.

- **Permitted**: Non-commercial academic research, teaching, and administrative scheduling.
- **Prohibited**: Commercial exploitation, privilege escalation exploits, deployment of malware, or unauthorized network probing.

See [LICENSE](LICENSE) for full details.