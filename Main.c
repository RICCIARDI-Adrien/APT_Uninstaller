/** @file Main.c
 * APT_Uninstaller allows to fully uninstall a program and all its dependencies. The program must have been installed using Debian APT.
 * @author Adrien RICCIARDI
 * @version 1.0 : 08/02/2015
 * @version 1.1 : 17/06/2015, made some code improvement.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "Parser.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Ask a yes/no question to the user.
 * @param String_Question The question to ask.
 * @return 1 if the user answered yes,
 * @return 0 if the user answered no.
 */
static int AskQuestion(char *String_Question)
{
	char String_Answer[32];

	while (1)
	{
		printf("%s", String_Question); // Newer gcc force to use a format string for security purpose

		// Read a full string to absorb the new line character
		String_Answer[0] = 0; // Reset the eventual previous answer
		scanf("%s", String_Answer);

		if (String_Answer[1] != 0) continue; // The answer is not valid if there are more than 1 character
		if (String_Answer[0] == 'y') return 1;
		if (String_Answer[0] == 'n') return 0;
	}
}

/** Uninstall all packages listed in the current log record. */
static void UninstallPackages(void)
{
	int Result;
	char *String_Package_Name, String_Uninstall_Command[1024];

	while (1)
	{
		Result = ParserGetNextPackageName(&String_Package_Name);
		if (Result == -1)
		{
			printf("Error : could not read the full package name.\n");
			exit(EXIT_FAILURE);
		}

		// Create the uninstall command
		sprintf(String_Uninstall_Command, "apt-get -y remove --purge %s", String_Package_Name);

		// Uninstall the package
		printf("\033[33mUninstalling %s...\033[0m\n", String_Package_Name);
		system(String_Uninstall_Command);

		// The last package has been uninstalled
		if (Result == 1) return;
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	static char String_Command_Line[PARSER_MAXIMUM_COMMAND_LINE_LENGTH]; // Avoid allocating so much space on stack
	char *Pointer_String;

	// Only root can run this program
	if (getuid() != 0)
	{
		printf("Error : only root can run this program.\n");
		return EXIT_FAILURE;
	}

	// Check parameters
	if (argc > 2)
	{
		printf("Usage : %s [APT_Log_File]\n"
			"APT_Log_File must be in plain text format and not GZ compressed.\n"
			"If no APT_Log_File is specified, /var/log/apt/history.log is used by default.\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// Select default APT log file if none was specified
	if (argc == 1) Pointer_String = "/var/log/apt/history.log";
	else Pointer_String = argv[1];

	// Try to open the log file
	if (ParserOpenFile(Pointer_String) != 0)
	{
		printf("Error : could not open the log file.\n");
		return EXIT_FAILURE;
	}
	atexit(ParserCloseFile); // Automatically close the log file when the program exits

	// List all installed programs present in the log file
	while (1)
	{
		// Store the next command line
		Pointer_String = ParserGetNextCommandLine();
		if (Pointer_String == NULL) break;
		strncpy(String_Command_Line, Pointer_String, sizeof(String_Command_Line)); // Keep the command line for later

		// Get the command type to show only install-related ones
		switch (ParserIsInstallCommand())
		{
			case -1:
				printf("Error : ParserIsInstallCommand() could not read enough characters.\n");
				return EXIT_FAILURE;

			case 1:
				// Ask a double confirmation before destroying the system
				printf("Found installation command : %s", String_Command_Line);
				if (AskQuestion("Do you want to uninstall (y/n) ? ") == 0) continue;
				if (AskQuestion("Are you sure (y/n) ? ") == 0) continue;

				// Start uninstallation
				UninstallPackages();
				break;

			default:
				break;
		}
	}

	return EXIT_SUCCESS;
}
