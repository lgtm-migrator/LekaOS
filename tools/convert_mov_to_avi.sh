#!/usr/bin/env zsh

# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Setup helpers
#

SCRIPT_DIR=$(dirname $0)

function try {
	. $SCRIPT_DIR'/helpers/try.sh' $@
}

#
# MARK: - Variables
#

MOV_INPUT_DIR="$SCRIPT_DIR/../fs/home/vid/raw"
AVI_OUTPUT_DIR="$SCRIPT_DIR/../fs/home/vid" # AVI output

#
# MARK: - Deal with args
#

HELP_TEXT="Example usage: $0 -i [DIRECTORY]

	-i 	{directory} input directory containing .mov files
	-h	Show this message
"

while getopts hi:qr: flag; do
	case ${flag} in
		i) MOV_INPUT_DIR="$OPTARG";;
		h) echo "${HELP_TEXT}"
		   exit 0;;
	esac
done

if [[ $(command -v ffmpeg) == "" ]]; then
   echo "ffmpeg is not installed, install with:"
   echo "\n\tbrew install ffmpeg\n"
   echo "${HELP_TEXT}"
   exit 1
fi

if [ -z "$MOV_INPUT_DIR" ]; then
   echo "Input directory is mandatory"
   echo "${HELP_TEXT}"
   exit 1
fi

if [ ! -d "$MOV_INPUT_DIR" ]; then
	echo "$MOV_INPUT_DIR DOES NOT exist \n"
	echo "${HELP_TEXT}"
	exit 1
fi

#
# MARK: - Main script
#

mkdir -p $AVI_OUTPUT_DIR

for file in "$MOV_INPUT_DIR"/*.mov; do

	filename=$file:t:r

	try ffmpeg -y -i $file            \
		-filter:v format=yuv420p      \
		-codec mjpeg                  \
		-qscale 0                     \
		$AVI_OUTPUT_DIR/$filename.avi

done

list_failed_commands
