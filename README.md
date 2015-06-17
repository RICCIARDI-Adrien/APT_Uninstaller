# APT Uninstaller
Allow to uninstall a Debian package and all of the dependencies installed in the same time.
The package must have been installed with apt-get.

The program parses the provided /var/log/apt/history.log file (the file must be in plain text format, not gzipped) and prompts all "install" commands.
The user can now choose to uninstall or not the corresponding package and all of the installed dependencies.

**CAUTION : this program is really stupid and uninstalls all the packages listed in an "install" command, so use it at your own risks !**