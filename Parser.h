/** @file Parser.h
 * Extract useful informations from an APT (Advanced Package Tool) log file.
 * @author Adrien RICCIARDI
 * @version 1.0 : 08/02/2015
 * @version 1.1 : 12/06/2015, the package architecture is now taken into account.
 */
#ifndef H_PARSER_H
#define H_PARSER_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The maximum allowed length for a command line. */
#define PARSER_MAXIMUM_COMMAND_LINE_LENGTH (1024 * 1024) // 1MB should be enough to store one line

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Open the APT log file to parse. This function must be called once prior any other parser function.
 * @param String_Log_File_Name The APT log file to open.
 * @return 0 if the file was successfully opened,
 * @return -1 if an error occured.
 */
int ParserOpenFile(char *String_Log_File_Name);

/** Close the previously opened log file. */
void ParserCloseFile(void);

/** Read the next logged command line.
 * @return a pointer on the command line in case of success,
 * @return NULL if the end of file is reached or if an error occured.
 */
char *ParserGetNextCommandLine(void);

/** Tell if the current command is installing something or not.
 * @return 1 if this is an install command,
 * @return 0 if not it is not an install command,
 * @return -1 if the end of file is reached too soon or if an error occured.
 */
int ParserIsInstallCommand(void);

/** Read the next package name.
 * @param On output, contain the package name.
 * @return 1 if the end of the packages list has been reached (in this case the string contains a valid package name),
 * @return 0 if the package name was successfully read,
 * @return -1 if the end of file is reached too soon or if an error occured.
 * @warning This function must be called only if the parser is currently reading a packages list or the result will be unpredictable.
 */
int ParserGetNextPackageName(char **Pointer_String_Package_Name);

#endif
