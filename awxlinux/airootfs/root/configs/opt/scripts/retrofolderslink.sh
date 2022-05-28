#!/bin/bash
# Author: Awx

uname=$(whoami)
labl1=""
labl2=""

function ask() {
	echo -ne "$1"
	read -r inpt
}

clear
echo -e "I take no responsibility for any data loss\n" \
	"This is a private script, it has been added for my convenience :)\n" \
	"You can use it tho, if you setup your emus like me\n\n" \
	"Supported applications:\n" \
	"dolphin, rpcs3, ppsspp, scummvm, citra, retroarch, ryujinx\n" \
	"yuzu, cemu, duckstation, flycast, pcem, pcsx2, pcsx2man\n\n" \
	"For yuzu, cemu, citra and ryujinx, the whole data dir is linked,\n" \
	"so make sure you have enough space\n\n" \
	"You can use the same disk if you have enough space.\n" \
	"If this is the first time you are using this script\n" \
	"and you want to setup your disk, run this script as follow:\n" \
	"$ savefolderlink setup\n\n"

if [ "$labl1" == "" ]; then
	ask "disk label of the disk for save files (q to exit): "
	if [ "$inpt" == "q" ]; then
		exit 0
	fi

	labl1="$inpt"

	ask "do you want me to remember the label for future runs? [y/n]: "
	if [ "$inpt" == "y" ] || [ "$inpt" == "Y" ]; then
		sed -i "s/labl1=\"\"/labl1=\"$labl1\"/" /opt/scripts/savefolderlink.sh
	elif [ "$inpt" == "n" ] || [ "$inpt" == "N" ]; then
		:
	else
		echo -e "wrong answer\n"
	fi
fi

if [ "$labl2" == "" ]; then
	ask "disk label of the disk for emus with nand (eg. Yuzu, citra) (q to exit): "
	if [ "$inpt" == "q" ]; then
		exit 0
	fi

	labl2="$inpt"

	ask "do you want me to remember the label for future runs? [y/n]: "
	if [ "$inpt" == "y" ] || [ "$inpt" == "Y" ]; then
		sed -i "s/labl2=\"\"/labl2=\"$labl2\"/" /opt/scripts/savefolderlink.sh
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
"/home/$uname/.local/share/dolphin-emu/GC"
"/run/media/$uname/$labl1/SAVES/dolphin/GC"

"/home/$uname/.local/share/dolphin-emu/Wii"
"/run/media/$uname/$labl1/SAVES/dolphin/Wii"

"/home/$uname/.local/share/dolphin-emu/StateSaves"
"/run/media/$uname/$labl1/SAVES/dolphin/StateSaves"

"/home/$uname/.local/share/dolphin-emu/GameSettings"
"/run/media/$uname/$labl1/SAVES/dolphin/GameSettings"

"/home/$uname/.local/share/dolphin-emu/GBA"
"/run/media/$uname/$labl1/SAVES/dolphin/GBA"

"/home/$uname/.config/dolphin-emu"
"/run/media/$uname/$labl1/configs/dolphin-emu"

"/home/$uname/.config/rpcs3/dev_hdd0/home"
"/run/media/$uname/$labl1/SAVES/ps3/home"

"/home/$uname/.config/rpcs3/dev_hdd0/savedata"
"/run/media/$uname/$labl1/SAVES/ps3/savedata"

"/home/$uname/.config/ppsspp/PSP/SAVEDATA"
"/run/media/$uname/$labl1/SAVES/psp/SAVEDATA"

"/home/$uname/.config/ppsspp/PSP/PPSSPP_STATE"
"/run/media/$uname/$labl1/SAVES/psp/PPSSPP_STATE"

"/home/$uname/.config/ppsspp/PSP/SYSTEM/CACHE"
"/home/$uname/.cache/ppsspp/CACHE"

"/home/$uname/.config/ppsspp/PSP/SYSTEM"
"/run/media/$uname/$labl1/configs/ppsspp/SYSTEM"

"/home/$uname/.local/share/scummvm/saves"
"/run/media/$uname/$labl1/SAVES/scumm/saves"

"/home/$uname/.config/scummvm"
"/run/media/$uname/$labl1/configs/scummvm"

"/home/$uname/.local/share/citra-emu/sdmc"
"/run/media/$uname/$labl1/SAVES/3dscitra/sdmc"

"/home/$uname/.local/share/citra-emu/cheats"
"/run/media/$uname/$labl2/citra/cheats"

"/home/$uname/.local/share/citra-emu/nand"
"/run/media/$uname/$labl2/citra/nand"

"/home/$uname/.local/share/citra-emu/sysdata"
"/run/media/$uname/$labl2/citra/sysdata"

"/home/$uname/.local/share/citra-emu/shaders"
"/home/$uname/.cache/citra/shaders"

"/home/$uname/.local/share/citra-emu/log"
"/home/$uname/.cache/citra/log"

"/home/$uname/.config/citra-emu"
"/run/media/$uname/$labl1/configs/citra-emu"

"/home/$uname/.pcsx2man"
"/run/media/$uname/$labl1/pcsx2cfgs/linuxcfg/.pcsx2man"

"/home/$uname/.config/retroarch/saves"
"/run/media/$uname/$labl1/SAVES/retroarch/saves"

"/home/$uname/.config/retroarch/states"
"/run/media/$uname/$labl1/SAVES/retroarch/states"

"/home/$uname/.config/retroarch/config"
"/run/media/$uname/$labl1/SAVES/retroarch/config"

"/home/$uname/.config/retroarch/screenshots"
"/run/media/$uname/$labl1/SAVES/retroarch/screenshots"

"/home/$uname/.local/share/yuzu/dump"
"/run/media/$uname/$labl2/yuzu/dump"

"/home/$uname/.local/share/yuzu/keys"
"/run/media/$uname/$labl2/yuzu/keys"

"/home/$uname/.local/share/yuzu/nand"
"/run/media/$uname/$labl2/yuzu/nand"

"/home/$uname/.local/share/yuzu/sdmc"
"/run/media/$uname/$labl2/yuzu/sdmc"

"/home/$uname/.local/share/yuzu/sysdata"
"/run/media/$uname/$labl2/yuzu/sysdata"

"/home/$uname/.local/share/yuzu/load"
"/run/media/$uname/$labl2/yuzu/load"

"/home/$uname/.local/share/yuzu/shader"
"/home/$uname/.cache/yuzu/shader"

"/home/$uname/.local/share/yuzu/log"
"/home/$uname/.cache/yuzu/log"

"/home/$uname/.config/yuzu"
"/run/media/$uname/$labl1/configs/yuzu"

"/home/$uname/.config/Ryujinx/bis"
"/run/media/$uname/$labl2/Ryujinx/bis"

"/home/$uname/.config/Ryujinx/mods"
"/run/media/$uname/$labl2/Ryujinx/mods"

"/home/$uname/.config/Ryujinx/profiles"
"/run/media/$uname/$labl2/Ryujinx/profiles"

"/home/$uname/.config/Ryujinx/system"
"/run/media/$uname/$labl2/Ryujinx/system"

"/home/$uname/.config/Ryujinx/sdcard"
"/run/media/$uname/$labl2/Ryujinx/sdcard"

"/home/$uname/.config/Ryujinx/Config.json"
"/run/media/$uname/$labl2/Ryujinx/Config.json"

"/opt/Cemu/drive_c/Cemu/mlc01"
"/run/media/$uname/$labl2/cemu/mlc01"

"/home/$uname/.local/share/duckstation/memcards"
"/run/media/$uname/$labl1/SAVES/duckstation/memcards"

"/home/$uname/.local/share/duckstation/savestates"
"/run/media/$uname/$labl1/SAVES/duckstation/savestates"

"/home/$uname/.local/share/duckstation/shaders"
"/home/$uname/.cache/duckstation/shaders"

"/home/$uname/.local/share/flycast"
"/run/media/$uname/$labl1/SAVES/flycast"

"/home/$uname/.config/flycast"
"/run/media/$uname/$labl1/configs/flycast"

"/home/$uname/.config/PCSX2/memcards"
"/run/media/$uname/$labl1/SAVES/pcsx2/memcards"

"/home/$uname/.config/PCSX2/sstates"
"/run/media/$uname/$labl1/SAVES/pcsx2/sstates"

"/home/$uname/.config/PCSX2/cheats"
"/run/media/$uname/$labl1/SAVES/pcsx2/cheats"

"/home/$uname/.config/PCSX2/cache"
"/home/$uname/.cache/PCSX2/cache"

"/home/$uname/.config/PCSX2/logs"
"/home/$uname/.cache/PCSX2/logs"

"/home/$uname/.config/PCSX2/inis"
"/run/media/$uname/$labl1/configs/pcsx2/inis"

"/home/$uname/.local/share/Steam/steamapps/shadercache"
"/home/$uname/.cache/steam/shadercache"

"/home/$uname/.local/share/xemu/xemu/xemu.ini"
"/run/media/$uname/$labl1/configs/xemu/xemu.ini"

"/home/$uname/.config/melonDS"
"/run/media/$uname/$labl1/configs/melonDS"

"/home/$uname/.config/mgba"
"/run/media/$uname/$labl1/configs/mgba"
)

roms=(
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

"/home/$uname/.config/rpcs3/dev_hdd0/game"
"/run/media/$uname/$labl2/rpcs3/game"
)


if [ "$1" == "setup" ]; then
	for ((i = 0; i < ${#folds[@]}; i += 2))
	do
	    let nx=(i+1)

	    echo "${folds[$nx]}"

	    if [[ -e "${folds[$nx]}" ]]; then
		continue
	    fi

	    mkdir -p "${folds[$nx]}"
	    cp -a "${folds[$i]}/." "${folds[$nx]}"
	done

	echo -e "Your disk is now ready.\n" \
		"You can move your saves into the new folders and link them.\n" \
		"Run the script again with no setup.\n"

else
	paths=("${folds[@]}" "${roms[@]}")

	for ((i = 0; i < ${#paths[@]}; i += 2))
	do
	    let nx=(i+1)

	    echo "${paths[$i]}"

	    if [[ ! -e "${paths[$nx]}" ]]; then
		echo -e "no dir to link: ${paths[$nx]}\n"
		continue
	    fi

	    if [[ -e "${paths[$i]}" ]]; then
		if [[ ! -L "${paths[$i]}" ]]; then
		    rm -R "${paths[$i]}"
		    ln -s "${paths[$nx]}" "${paths[$i]}"
		    echo "linking.."
		else
		    echo "[ok]"
		fi
	    else
	       echo "not linked, try link.."
	       ln -s "${paths[$nx]}" "${paths[$i]}" &>/dev/null

	       if [ $? -eq 1 ]; then
	       	echo -e "emulator is not set, please run it at least once\n"
	       else
	       	echo "linking.."
	       fi
	    fi

	    echo ""
	done
fi
