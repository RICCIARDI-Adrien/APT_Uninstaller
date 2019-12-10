# APT Uninstaller

Allow to uninstall a Debian package and all of the dependencies installed in the same time.
The package must have been installed with `apt-get` or `apt`.

## Usage

The program parses the `/var/log/apt/history.log` file provided as argument (the file must be in plain text format, not gzipped) and prompts all "install" commands. If no command line argument is provided then `/var/log/apt/history.log` file is used.  
User can now choose whether to uninstall the displayed package and all of the dependencies that were installed in the same time.

**WARNING : this program is really stupid and uninstalls all the packages listed in an "install" command, so use it at your own risks !**

## Building

Type the following command to build the software :
```
make
```

Install the software to `/usr/bin` with the following command :
```
sudo make install
```

Uninstall the software with the following command :
```
sudo make uninstall
```
