// Include the shell header file for necessary constants and function declarations
#include "shell.h"
extern char **environ;
int enable_color = 0;

// Function to read a command from the user input
void read_command(char **cmd)
{
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE)
    {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL)
  {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++)
  {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time)
  {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    system("clear"); // UNIX/Linux command to clear screen
#endif
    execute_cseshellrc();
    first_time = 0;
  }
  
  
  //get the current working directory to display in prompt
  char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return;
    }
  
  //get current time to display in prompt
  char time_str[100];
  time_t t = time(NULL);
  struct tm *tm_info = localtime(&t);
  strftime(time_str, sizeof(time_str), "[%H:%M %d/%m/%y]", tm_info);
  
  //get name of host
  char hostname[HOST_NAME_MAX];
  gethostname(hostname, sizeof(hostname));
  
  fflush(stdout); // Flush the output buffer
  if (enable_color)
  {
    printf("%s\033[31m%s\033[0m:\033[1m\033[34m~%s >> \033[0m\033[1m\033[3m\033[35mCSEShell\033[0m \n",
           time_str, hostname, cwd); // Print with colors
  }
  else
  {
    printf("%s:%s >> CSEShell \n", time_str, cwd); 
  }
  fflush(stdout);
  
}

int num_builtin_functions()
{
    return sizeof(builtin_commands) / sizeof(char *);
}

void toggle_prompt_color()
{
  enable_color = !enable_color; // Toggle the enable_color flag
}

void execute_cseshellrc()
{
  FILE *cseshellrc = fopen(".cseshellrc", "r");
  if (cseshellrc)
  {
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), cseshellrc))
    {
      // Execute each line in .cseshellrc using system()
      if (line[0] != '\n') // Skip empty lines
      {
        line[strcspn(line, "\n")] = 0; // Remove newline character
        int status = system(line);     // Execute the command using system()

        // Check the exit status to print command not found message
        if (WIFEXITED(status))
        {
          int exit_status = WEXITSTATUS(status);
          if (exit_status != 0)
          {
            fprintf(stderr, "cseshell: command not found: %s\n", line);
          }
        }
        else if (WIFSIGNALED(status))
        {
          fprintf(stderr, "cseshell: command not found: %s\n", line);
        }
        // Ignore other errors (e.g., from shell)
      }
    }
    fclose(cseshellrc);
  }
  else
  {
    printf("Failed to open .cseshellrc file\n");
    exit(1);
  }
}

void print_resource_usage(struct rusage usage) {
    printf("\n\033[1;34mResource Usage\n");
    printf("User CPU time used: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System CPU time used: %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    printf("Maximum resident set size (RSS): %ld KB\n", usage.ru_maxrss);
    printf("\033[0m");
}


// The main function where the shell's execution begins
int main(void)
{ 
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];
  int child_status;
  pid_t pid;
  
  char cwd[1024];
  
  if (getcwd(cwd, sizeof(cwd)) == NULL)
  {
      printf("Failed to get current working directory.");
      exit(1);
  }
  
  for (;;)
  { 
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user
    
    
    if (cmd[0] == NULL) {
      continue;
    } 
    
    int is_builtin = 0;
    for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
    {
     	if (strcmp(cmd[0], builtin_commands[command_index]) == 0)
        {
            is_builtin = 1;
            if (enable_color)
        	{
          		printf("\033[0;32m"); // Green text 	color
          		fflush(stdout);
        	}
            if ((*builtin_command_func[command_index])(cmd) == 0)
            {
                if (enable_color)
          	{
            	printf("\033[0;32m"); 
          	}
          	return 0;
            }
            
            if (enable_color)
        	{
          	    printf("\033[0;32m");
        	}
            break;
        }
    }

    if (is_builtin)
    	continue;
            
    pid = fork();
    
    if (pid < 0) {
    	printf("could not fork");
    	exit(1);
    }
    
    else if (pid == 0) {
      if (enable_color)
      {
        printf("\033[0;32m"); // Green text color
        fflush(stdout);
      }
      // Formulate the full path of the command to be executed
      char full_path[PATH_MAX];
      snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
      

      execv(full_path, cmd);

      // If execv returns, command execution has failed
      printf("Command %s not found\n", cmd[0]);
      exit(0);
      
      // Free the allocated memory for the command arguments before exiting
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
      }
      memset(cwd, '\0', sizeof(cwd)); // clear the cwd array 
      printf("\033[0m");
    }
   
    else if (pid > 0) {
    	printf("\033[0m");
    	struct rusage usage;
    	int status, child_exit_status;
   	waitpid(pid, &child_status, WUNTRACED);
        if (WIFEXITED(child_status))
        {
            child_exit_status = WEXITSTATUS(child_status);
            printf("Child process exited with status %d\n", child_exit_status);
        }
        
        if (getrusage(RUSAGE_CHILDREN, &usage) == -1) {
                perror("getrusage error");
            } else {
                print_resource_usage(usage);
            }
        if (enable_color)
      		{
        printf("\033[0;32m"); 
      		}
    }
  }
  
  return 0;
}

int shell_cd(char **args)
{
  if (args[1] == NULL)
  {
    printf("CSEShell: Navigating to home directory. \n");
    chdir(getenv("HOME"));
    return 1;
  }
  else
  {
    if (chdir(args[1]) != 0)
    { 
      perror("CSEShell:");
    }
  }
  
  struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        perror("getrusage error");
    } else {
        print_resource_usage(usage);
    }
    
  printf("\033[0m");
  return 1;
}

int shell_help(char **args)
{	
    printf("CSEShell Interface \n");
    printf("Usage: command arguments \n");
    printf("The following commands are implemented within the shell: \n");
    for (int i = 0; i < num_builtin_functions(); i++){
        printf("  %s\n", builtin_commands[i]);
    }
    
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        perror("getrusage error");
    } else {
        print_resource_usage(usage);
    }
    
    printf("\033[0m");
    return 1;
}

int shell_exit(char **args)
{   printf("\033[0m");
    return 0;
}

int shell_usage(char **args)
{
    struct Usage {
    char *command;
    char *info;
};

struct Usage commandArray[] = {
    {"cd", "to change the current working directory of the shell"},
    {"help", "for supported commands"},
    {"usage", "to print a brief description on given command"},
    {"env", "to list all registered environment variables"},
    {"setenv", "to set a new environment variable"},
    {"unsetenv", "to remove given environment variable from the list of environment variables"},
    {"exit", "to terminate the shell gracefully"},
    {"browse", "opens website on your default browser"},
    {"togglecolor", "toggles colour between normal and aid colourblind personnel"}
};

    if (args[1] == NULL) {
        printf("Usage: this command enables you to gather detailed information on the command you pass as the argument\n");
        printf("\033[0m");
        return 1;
    }

    for (int i = 0; i < num_builtin_functions(); i++) {
        if (strcmp(args[1], commandArray[i].command) == 0) {
            printf("Type: %s %s\n", commandArray[i].command, commandArray[i].info);
            printf("\033[0m");
            return 1;
        }
    }
    
    printf("\033[0m");
    printf("The command '%s' is not a built-in command of this shell.\n", args[1]);
    return 1;
}

int list_env(char **args)
{
    
    if (args[1] != NULL) 
    {
        fprintf(stderr, "Usage: env\n");
        return 1;
    }
    
    char **env = environ;
    
    while (*env) { 
        printf("%s\n", *env); 
        env++; 
    }
    
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        perror("getrusage error");
    } else {
        print_resource_usage(usage);
    }
    printf("\033[0m");
    return 1;   
    
}

int set_env_var(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: setenv KEY=VALUE\n");
        return 1;
    }

    char *equal_sign = strchr(args[1], '=');

    if (equal_sign == NULL) {
        fprintf(stderr, "Invalid format. Use: setenv KEY=VALUE\n");
        return 1;
    }

    *equal_sign = '\0'; 
    char *key = args[1];
    char *value = equal_sign + 1;

    if (setenv(key, value, 1) != 0) {
        perror("setenv");
        return 1;
    }
    
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        perror("getrusage error");
    } else {
        print_resource_usage(usage);
    }
    printf("\033[0m");
    return 1;
    
}

int unset_env_var(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: unset KEY \n");
        return 1;
    }
    else {
        if (unsetenv(args[1]) != 0) {
            perror("shell");
        }
    }
    
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        perror("getrusage error");
    } else {
        print_resource_usage(usage);
    }
    printf("\033[0m");
    return 1;
    
}

int shell_browse(char **args) 
{
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: browse URL (must enter full website URL including domain)\n");
        return 1;
    } else {
        char url[256];
        if (strncmp(args[1], "http://", 7) != 0 && strncmp(args[1], "https://", 8) != 0) {
            snprintf(url, sizeof(url), "http://%s", args[1]);
        } else {
            snprintf(url, sizeof(url), "%s", args[1]);
        }

        char command[512];
        snprintf(command, sizeof(command), "xdg-open \"%s\" >/dev/null 2>&1", url);
        system(command);
    }
    printf("\033[34mRedirecting ...\033[0m\n");
    
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == -1) {
        perror("getrusage error");
    } else {
        print_resource_usage(usage);
    }
    printf("\033[0m");
    return 1;
}


int toggle_color(char **args)
{
  enable_color = !enable_color; 
    
  struct rusage usage;
  if (getrusage(RUSAGE_SELF, &usage) == -1) {
    perror("getrusage error");
  } 
  else {
    print_resource_usage(usage);
  }
  printf("\033[0m");
  return 1;                                   
} 
