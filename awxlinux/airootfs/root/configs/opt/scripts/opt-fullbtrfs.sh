#!/bin/bash
# Author: Awx

uname=$(whoami)

if [ ! -d "/home/$uname/.local/share/Steam/steamapps" ]; then
	echo "run steam at least once to create its directories"
else
	chattr +C -R "/home/$uname/.local/share/Steam/steamapps/downloading"
	chattr +C -R "/home/$uname/.local/share/Steam/steamapps/temp"
fi
