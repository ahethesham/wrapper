#!/bin/bash

HOME_DIR=$(pwd)
COMMAND_FILE=${HOME_DIR}/compile_commands.json

if [ ! -f ${COMMAND_FILE} ]; then
	touch ${COMMAND_FILE}
else
	truncate -s 0 ${COMMAND_FILE}
fi

# get all the include files first
ALL_INCLUDES="$(find ${HOME_DIR} -type f -name "*.h" 2>/dev/null)"
ALL_INCLUDES="$ALL_INCLUDES $(find "/opt/homebrew/include/openssl/" -type f -name "*.h" 2>/dev/null)"

echo $(find "/opt/homebrew/include/" -type f -name "*.h")

INCLUDES=""
DUPLICATES=""
function isDuplicate() {
	DIR=$(dirname "$1")
	for dir in $DUPLICATES; do
		if [[ "$DIR" == "$dir" ]]; then
			return 1
		fi
	done
	DUPLICATES="$DUPLICATES $DIR"
	return 0
}

for f in $ALL_INCLUDES; do
	isDuplicate $f
	if [ $? -eq 0 ]; then
		INCLUDES="$INCLUDES $f"
	fi
done
# get all the cxx files
CXX_FILES="$(find ${HOME_DIR} -type f -name "*.cxx" 2>/dev/null)"
CXX_FILES="$CXX_FILES $(find ${HOME_DIR} -type f -name "*.cc" 2>/dev/null)"

C_FILES=$(find ${HOME_DIR} -type f -name "*.c" 2>/dev/null)

printf "[\n" >>${COMMAND_FILE}

# for each .cxx file create a {"directory"}
for f in ${CXX_FILES}; do
	DIRECTORY=$(dirname $f)
	printf "\t{\n\t\t\"directory\": \"${DIRECTORY}\",\n\t\t" >>${COMMAND_FILE}
	printf "\"command\": \"g++ -std=c++23 " >>${COMMAND_FILE}

	for i in ${INCLUDES}; do
		printf " -I$(dirname $i)" >>${COMMAND_FILE}
	done

	printf " -o " >>${COMMAND_FILE}
	FILE_NAME=$(basename $f)
	FILE_LEN=${#FILE_NAME}

	for ((i = 0; i < $FILE_LEN; i++)); do
		if [ "${FILE_NAME:$i:1}" == "." ]; then
			break
		fi
		printf "${FILE_NAME:$i:1}" >>${COMMAND_FILE}
	done

	printf ".o -c ${FILE_NAME}\",\n\t\t\"file\": \"${FILE_NAME}\"\n\t},\n\n" >>${COMMAND_FILE}
done

for f in ${C_FILES}; do
	DIRECTORY=$(dirname $f)
	printf "\t{\n\t\t\"directory\": \"${DIRECTORY}\",\n\t\t" >>${COMMAND_FILE}
	printf "\"command\": \"gcc" >>${COMMAND_FILE}
	for i in ${INCLUDES}; do
		printf " -I$(dirname $i)" >>${COMMAND_FILE}
	done

	printf " -o " >>${COMMAND_FILE}
	FILE_NAME=$(basename $f)
	FILE_LEN=${#FILE_NAME}

	for ((i = 0; i < $FILE_LEN; i++)); do
		if [ "${FILE_NAME:$i:1}" == "." ]; then
			break
		fi
		printf "${FILE_NAME:$i:1}" >>${COMMAND_FILE}
	done

	printf ".o -c ${FILE_NAME}\",\n\t\t\"file\": \"${FILE_NAME}\"\n\t},\n\n" >>${COMMAND_FILE}
done

printf "\n]\n" >>${COMMAND_FILE}
