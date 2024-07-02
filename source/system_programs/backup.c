#include "system_program.h"

void get_current_datetime(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, buffer_size, "%Y%m%d%H%M%S", t);
}

int main() {
    const char *backup_dir = getenv("BACKUP_DIR");
    if (backup_dir == NULL) {
        fprintf(stderr, "Error: BACKUP_DIR environment variable is not set.\n");
        return 1;
    }

    const char *archive_dir = "archive";

    struct stat st = {0};
    if (stat(archive_dir, &st) == -1) {
        if (mkdir(archive_dir, 0700) != 0) {
            perror("Error creating archive directory");
            return 1;
        }
    }

    char datetime[20];
    get_current_datetime(datetime, sizeof(datetime));

    char zip_filename[512];
    snprintf(zip_filename, sizeof(zip_filename), "%s/backup-%s.zip", archive_dir, datetime);

    char command[1024];
    snprintf(command, sizeof(command), "zip -r %s %s", zip_filename, backup_dir);

    printf("Creating backup of '%s' at '%s'\n", backup_dir, zip_filename);
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error creating backup.\n");
        return 1;
    }

    printf("Backup created successfully.\n");
    return 0;
}

