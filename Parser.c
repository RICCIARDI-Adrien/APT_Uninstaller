/** @file Parser.c
 * @see Parser.h for description.
 * @author Adrien RICCIARDI
 */
#include <stdio.h>
#include <string.h>
#include "Parser.h"

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Enable or disable the debug messages. */
#ifdef DEBUG
	#define Debug(String_Format, ...) printf("[%s:%d] " String_Format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define Debug(String_Format, ...)
#endif

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The log file. */
static FILE *File_Log;

/** The internal line buffer. */
static char String_Line[PARSER_MAXIMUM_COMMAND_LINE_LENGTH];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int ParserOpenFile(char *String_Log_File_Name)
{
	File_Log = fopen(String_Log_File_Name, "r");
	if (File_Log == NULL) return -1;
	return 0;
}

void ParserCloseFile(void)
{
	fclose(File_Log);
}

char *ParserGetNextCommandLine(void)
{
	char *String_To_Find = "Commandline: ";
	int Length;

	Length = strlen(String_To_Find);

	// Find the first line starting with "Commandline: "
	while (1)
	{
		// Read the next full line
		if (fgets(String_Line, sizeof(String_Line), File_Log) == NULL) return NULL;
		Debug("Read line : %s\n", String_Line);

		// Bypass the initial "Commandline: " if the string was found
		if (strncmp(String_Line, String_To_Find, Length) == 0)
		{
			Debug("Command line found.\n");
			return &String_Line[Length];
		}
	}
}

int ParserIsInstallCommand(void)
{
	char *String_To_Find = "Install: ";
	int i, Length, Character;

	Length = strlen(String_To_Find);

	// Read the beginning of the next line to find "Install: "
	for (i = 0; i < Length; i++)
	{
		Character = fgetc(File_Log);
		Debug("Read character : %c\n", Character);
		if (Character == EOF) return -1;
		String_Line[i] = (char) Character;
	}
	String_Line[Length] = 0; // Append terminating zero

	if (strcmp(String_Line, String_To_Find) == 0)
	{
		Debug("Install command found.\n");
		return 1;
	}
	return 0;
}

int ParserGetNextPackageName(char **Pointer_String_Package_Name)
{
	int i = 0, Raw_Character;
	char Character;

	// Done here to allow exiting the function easily
	*Pointer_String_Package_Name = String_Line;

	// Read the package name up to the space preceding the first brace
	while (1)
	{
		// Read the next character
		Raw_Character = fgetc(File_Log);
		if (Raw_Character == EOF) return -1; // The package name is not complete
		Character = (char) Raw_Character;

		// Add the terminating zero to the package name string
		if (Character == ' ')
		{
			String_Line[i] = 0; // Terminate string
			break;
		}
		String_Line[i] = Character;
		i++;
	}

	// Read the remaining package characters until ')' is found
	while (1)
	{
		Raw_Character = fgetc(File_Log);
		if (Raw_Character == EOF) return -1; // The package description is not complete
		Character = (char) Raw_Character;

		if (Character == ')')
		{
			Raw_Character = fgetc(File_Log);
			if (Raw_Character == EOF) return -1; // The package description is not complete

			// The end of the list has been reached
			if ((char) Raw_Character == '\n') return 1;

			// Bypass the trailing space (',' has been bypassed by the last read)
			if (fgetc(File_Log) == EOF) return -1; // The package separator is not complete
			return 0;
		}
	}
}
