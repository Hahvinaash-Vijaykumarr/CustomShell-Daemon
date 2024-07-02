#include "system_program.h"

char output_file_path[PATH_MAX];

static int daemon_work()
{
    // put your full PROJECT_DIR path here  
    strcpy(output_file_path, "/home/seed/Downloads/pa1/dspawn.log"); 

    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1)
    {

        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(output_file_path, "a");

        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.  \n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

int main(){

   pid_t pid = fork();
   
   if (pid < 0) {
   	perror("fork failed");
   	exit(1);
   }
   
   else if(pid ==0) {
   	setsid();
   	signal(SIGCHLD, SIG_IGN);
   	signal(SIGHUP, SIG_IGN);
   	
   	pid = fork();
   	
   	if (pid < 0) {
   		perror("fork failed");
   		exit(1);
   	}
   	
   	else if (pid == 0) {
   		umask(0);
   		chdir("/");
   		
   		int x;
   		int fd0, fd1, fd2;
		   for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
		   {
		       close (x);
		   }

		   fd0 = open("/dev/null", O_RDWR);
		   fd1 = dup(0);
		   fd2 = dup(0);
		   
		   if (daemon_work() != EXIT_SUCCESS) {
                   fprintf(stderr, "daemon_work failed\n");
                   exit(1);
    }
   	}
   	
   	if (pid > 0) {
   		exit(0);
   	}
   	
   }

   else {
   	exit(0);
   }
   
   return 0;
}
