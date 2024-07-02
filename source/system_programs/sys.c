#include "system_program.h"
#include <sys/sysinfo.h>

void print_system_info() {
    struct utsname unameData;
    if (uname(&unameData) != 0) {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    struct sysinfo sys_info;
    if (sysinfo(&sys_info) != 0) {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }

    struct passwd *pw;
    uid_t uid = geteuid();
    pw = getpwuid(uid);
    if (pw == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }

    long total_ram = sys_info.totalram / 1024 / 1024; 

    char cpu_model[128];
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo != NULL) {
        while (fgets(cpu_model, sizeof(cpu_model), cpuinfo)) {
            if (strncmp(cpu_model, "model name", 10) == 0) {
                strtok(cpu_model, ":");
                strcpy(cpu_model, strtok(NULL, "\n"));
                break;
            }
        }
        fclose(cpuinfo);
    } else {
        strcpy(cpu_model, "Unknown");
    }

    printf("Simple System Information\n");
    printf("OS: %s\n", unameData.sysname);
    printf("Kernel: %s\n", unameData.release);
    printf("Hostname: %s\n", unameData.nodename);
    printf("Uptime: %ld days, %ld hours, %ld minutes\n",
           sys_info.uptime / 86400,
           (sys_info.uptime % 86400) / 3600,
           (sys_info.uptime % 3600) / 60);
    printf("User: %s\n", pw->pw_name);
    printf("CPU: %s\n", cpu_model);
    printf("Total RAM: %ld MB\n", total_ram);
}

int main() {
    print_system_info();
    return 0;
}
