[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/u16ttUuk)
# CSEShell
Table of Contents
Introduction
Building the Project
Running the CSEShell
Builtin Functions Supported
System Programs
Additional Features Supported
Sustainability and Inclusivity Considerations

## Introduction
This project implements a custom shell environment with additional system programs, focusing on enhancing shell functionality and system management tasks. The shell includes features like command execution, piping, redirection, and background process management. Additionally, it incorporates system programs (sys, dspawn, dcheck, backup) to provide system information retrieval, daemon process management, and backup automation, as well as details on additional features and considerations for sustainability and inclusivity.


## Building the Project

To build the CSEShell and system programs, navigate to the root directory and run:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

## Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

From there, you can execute built-in commands and any of the included system programs (e.g., `find`, `ld`, `ldr`).

## System Programs

The shell supports the following built-in commands:
`cd`: Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
`help`: Lists all builtin commands in the shell.
`exit`: Exits the shell.
`usage`: Provides a brief usage guide for the shell and its built-in commands.
`env`: Lists all the environment variables currently set in the shell.
`setenv`: Sets or modifies an environment variable for this shell session.
`unsetenv`: Removes an environment variable from the shell.

## System Programs
- find : Searches for files in a directory
- Ld : Lists the contents of the current directory
- ldr : Lists the contents of the current directory recursively 
- sys : Prints out basic information about the working operating system
- dspawn : Summons a daemon process and then terminates so that the shell can continue to display the next prompt
- dcheck : Prints out how many live daemon processes are currently alive
- backup : Automatically zips a directory whose name matches the value in the environment variable BACKUP_DIR, and stores the zipped contents in PROJECT_DIR/archive.

## Additional Features Supported
- `browse`: Launches browser with the specified URL.
- Prompt Decoration: The shell prompt displays useful information such as the current user, time and current working directory, in a user-friendly colour palette.
- Calendar Function: Displays a calender, showing the current date, month and year once the cseshell starts


## Sustainability and Inclusivity Considerations
In designing CSEShell, sustainability and inclusivity considerations were addressed through:
- Accessibility for users with Colour Vision Deficiencies: Option to change the shell command colours to white text on a black background. This high-contrast theme ensures better readability and ease of use for all users.
- Resource Usage Feedback: The shell prints out resource usage statistics upon execution of each command.


