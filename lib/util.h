// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2023 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// utility functions that don't belong elsewhere

#ifndef BOINC_UTIL_H
#define BOINC_UTIL_H

#include <stdlib.h>
#include <string>
#include <vector>
#ifdef _WIN32
#include "boinc_win.h"
#endif
extern double dtime();
extern double dday();
extern void boinc_sleep(double);
extern void push_unique(std::string, std::vector<std::string>&);

// NOTE: use #include <functional>   to get max,min

#define SECONDS_PER_DAY 86400
#define KILO (1024.)
#define MEGA (1024.*KILO)
#define GIGA (1024.*MEGA)
#define TERA (1024.*GIGA)

static inline double drand() {
    return (double)rand()/(double)RAND_MAX;
}
extern double rand_normal();

#ifdef _WIN32
#include "boinc_win.h"
#else
// setpriority(2) arg to run in background
//
static const int PROCESS_IDLE_PRIORITY = 19;
static const int PROCESS_MEDIUM_PRIORITY = 10;
static const int PROCESS_NORMAL_PRIORITY = 0;
static const int PROCESS_ABOVE_NORMAL_PRIORITY = -10;
static const int PROCESS_HIGH_PRIORITY = -15;
static const int PROCESS_REALTIME_PRIORITY = -20;
extern double linux_cpu_time(int pid);
#endif

extern void update_average(double, double, double, double, double&, double&);

extern int boinc_calling_thread_cpu_time(double&);

inline bool in_vector(int n, std::vector<int>& v) {
    for (unsigned int i=0; i<v.size(); i++) {
        if (n == v[i]) return true;
    }
    return false;
}

// fake a crash
//
extern void boinc_crash();

#ifdef _WIN32
extern int boinc_thread_cpu_time(HANDLE thread_handle, double& cpu);
extern int boinc_process_cpu_time(HANDLE process_handle, double& cpu);
extern int kill_process_with_status(int, int exit_code=0);
#endif

// define a type for reference to a process.
// Win also has an integer PID; that's not what we mean here
//
#ifdef _WIN32
#define PROCESS_REF  HANDLE
#else
#define PROCESS_REF  int
#endif

extern bool process_exists(PROCESS_REF);
extern int run_program(
    const char* dir,        // directory to run program in; NULL if current dir
    const char* file,       // path of executable
    int argc,
    char *const argv[],     // cmdline args, UNIX-style
    PROCESS_REF&             // ID of child process
);
extern int kill_process(PROCESS_REF);
extern int get_exit_status(PROCESS_REF, int& status, double dt);
    // get exit code of process
    // If dt is negative, wait indefinitely;
    // else wait for at most dt;
    // if process hasn't exited by then, return error
    //
    // Note: to see if a process has exited:
    // get_exit_status(pid, status, 0) == 0

extern int get_real_executable_path(char* path, size_t max_len);

#ifdef GCL_SIMULATOR
extern double simtime;
#define time(x) ((int)simtime)
#endif

#endif
