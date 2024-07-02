#include "system_program.h"

int main() {
  FILE *fp;
    char path[1035];
    int num_daemons = 0;

    system("ps -efj | grep dspawn | grep -Ev 'tty|pts' | wc -l > temp.txt");
    
    fp = fopen("temp.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    if (fgets(path, sizeof(path) - 1, fp) != NULL) {
        num_daemons = atoi(path);
    }

    fclose(fp);
    remove("temp.txt");

    if (num_daemons > 0) {
        printf("Live daemons: %d\n", num_daemons);
    } else {
        printf("No daemon is alive right now.\n");
    }

    return EXIT_SUCCESS;
}
