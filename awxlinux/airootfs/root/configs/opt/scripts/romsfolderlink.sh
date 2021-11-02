#!/bin/bash

# Author: Awx

uname=$(whoami)
labl1=""
labl2=""

function ask() {
	echo -ne "$1"
	read -r inpt
}

echo -e "I take no responsibility for any data loss\n" \
	"This is a private script, it has been added for my convenience :)\n" \
	"You can use it tho, if you setup your emus like me\n\n" \
	"Supported applications:\n" \
	"retroarch, cemu, duckstation, pcem, pcsx2\n\n" \
	"You can point to the same disk if you have enough space.\n" \
	"If this is the first time you are using this script\n" \
	"and you want to prepare your disk, run this script as follow:\n" \
	"$ romsfolderlink setup\n\n"

if [ "$labl1" == "" ]; then
	ask "disk label of the disk for bios roms (q to exit): "
	if [ "$inpt" == "q" ]; then
		exit 0
	fi

	labl1="$inpt"
	
	ask "do you want me to remember the label for future runs? [y/n]: "
	if [ "$inpt" == "y" ] || [ "$inpt" == "Y" ]; then
		sed -i "s/labl1=\"\"/labl1=\"$labl1\"/" /opt/scripts/romsfolderlink.sh
	elif [ "$inpt" == "n" ] || [ "$inpt" == "N" ]; then
		:
	else
		echo -e "wrong answer\n"
	fi	
fi

if [ "$labl2" == "" ]; then
	ask "disk label of the disk for cemu games folder (q to exit): "
	if [ "$inpt" == "q" ]; then
		exit 0
	fi

	labl2="$inpt"
	
	ask "do you want me to remember the label for future runs? [y/n]: "
	if [ "$inpt" == "y" ] || [ "$inpt" == "Y" ]; then
		sed -i "s/labl2=\"\"/labl2=\"$labl2\"/" /opt/scripts/romsfolderlink.sh
	elif [ "$inpt" == "n" ] || [ "$inpt" == "N" ]; then
		:
	else
		echo -e "wrong answer\n"
	fi	
fi

if [ "$labl1" == "" ] || [ "$labl2" == "" ]; then
	echo -e "You must specify a label\n"
	exit 1
fi

folds=(
"/opt/Cemu/drive_c/Cemu/games"
"/run/media/$uname/$labl2/wiiu"

"/home/$uname/.config/PCSX2/bios"
"/run/media/$uname/$labl1/bios/pcsx2"

"/home/$uname/.pcem/roms"
"/run/media/$uname/$labl1/bios/pcem/roms"

"/home/$uname/.config/retroarch/system"
"/run/media/$uname/$labl1/bios/system"

"/home/$uname/.local/share/duckstation/bios"
"/run/media/$uname/$labl1/bios/system"
)

for ((i = 0; i < ${#folds[@]}; i += 2))
do
    let nx=(i+1)

    echo "${folds[$i]}"

    if [[ ! -e "${folds[$nx]}" ]]; then
	echo -e "no dir to link: ${folds[$nx]}\n"
	continue
    fi

    if [[ -e "${folds[$i]}" ]]; then
	if [[ ! -L "${folds[$i]}" ]]; then
	    rm -R "${folds[$i]}"
	    ln -s "${folds[$nx]}" "${folds[$i]}"
	    echo "linking.."
	else
	    echo "[ok]"
	fi
    else
       echo "not linked, try link.."
       ln -s "${folds[$nx]}" "${folds[$i]}" &>/dev/null
       
       if [ $? -eq 1 ]; then
       	echo -e "emulator is not set, please run it at least once\n"
       else
       	echo "linking.."
       fi
    fi

    echo ""
done

