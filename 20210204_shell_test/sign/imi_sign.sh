#!/bin/bash

####################################################################
#                  Simple Template for Shell Script
# ==================================================================
# @Author:  lijiankai@imilab.com
# ------------------------------------------------------------------
# @Note:    To use "getopts" "let", we need bash.
# ------------------------------------------------------------------
# @History:
#20200911 first version
####################################################################

#########################
#### Global Settings ####
#########################

## debug settings
PATH=.:$PATH
set -e# -ex
## script version
VERSION=0.1.1
## script options
COMMANDS="usage sign_img"
COMMENTS="<sign_img> <ipc_project> <algo_type> <input_file> [options]"
OPTIONS=":o:"
EXAMPLE0=""
EXAMPLE1="sign_img ipc019a01 eddsa ota.img -o tf_recovery.img"

## Global varibables
SIGN_EXE=sign
SIGN_VERIFY_EXE=./sign_verify
IMI_KEYS_DIR=.
OUTPUT_FILE="tf_recovery.img"
## other variable
CONFIGS=

#########################
#### Function Region ####
#########################

### Private  Function ###

## hard code as Red
EchoError() {
  echo -e "\033[31m[Error ] $@\033[0m"
}

## hard code as LightBlue
EchoTitle() {
  echo -e "\033[36m$@\033[0m"
}

## hard code as Yellow
EchoSection() {
  echo -e "\033[33m$@\033[0m"
}

ShowTitle() {
  EchoTitle "Shell Script Template [Version: $VERSION]"
}

ShowUsage() {
  EchoSection "Usage:"
  echo "  $0 $COMMENTS"
}

ShowCommand() {
  EchoSection "Command:"
  echo "NULL for now"
}

ShowOptions() {
  EchoSection "Options:"
  echo "  -o   output file name (default: $OUTPUT_FILE)"
}

ShowExample() {
  EchoSection "Example:"
  echo "  $0 $EXAMPLE0"
  echo "  $0 $EXAMPLE1"
}

ShowNotes() {
  EchoSection "Note:"
  echo "1. ..."
}

## Error Code ##
## 101: miss/invalid parameter
## 102: open/close file fail
ErrorHandle() {
  #echo "[Debug ] Error Code: $1"
  case $1 in
    1)    ## command not defined
      EchoError "Unknown Command: $COMMAND";;
   # 127)  ## command not found
     # usage;;
    *)    ## do nothing
      ;;
  esac
}

#### Public Function ####

usage() {
  ShowTitle
  ShowUsage
  ShowCommand
  ShowOptions
  ShowExample
  #ShowNotes
}

#EXAMPLE1="sign_img ipc019a01 eddsa ota.img -o tf_recovery.img"
# sign -k "${SECURITY_DIR}/eddsa.skey" --aes-md5 "${SECURITY_DIR}/aes.key" "${tf_recovery_image_tmp}" "${tf_recovery_image}"
sign_img() {

	#SIGN_EXE -k "${IMI_KEYS_DIR}/${project_name}/eddsa.skey" --aes-md5 "{IMI_KEYS_DIR}/${project_name}/aes.key" "${input_file}" "${tf_recovery_image}"
	./sign -k "${IMI_KEYS_DIR}/${project_name}/eddsa.skey" --aes-md5 "${IMI_KEYS_DIR}/${project_name}/aes.key" "${input_file}" "${OUTPUT_FILE}"
}

test() {
  echo "[ Test ] Start: $@"
  echo "[ Test ] $MESSAGE"
  echo "[ Test ] Finish."
}

#########################
####  Main   Region  ####
#########################

## check inputs
if [ 0 -eq $# ]; then
  usage
  exit 0
fi

## get command
COMMAND="$1"
	project_name="$2"
	algo_type="$3"
	input_file="$4"
#echo "[Debug ] Command: $1"
shift

## parse option
while getopts $OPTIONS opt; do
  #echo "[Debug ] opt: $opt, arg: $OPTARG"
  case $opt in
    o)
      OUTPUT_FILE=$OPTARG
      echo "[Debug ] The output file is : $OUTPUT_FILE";;
    ?)
      EchoError "unknown option: $OPTARG"
      break
  esac
done
#echo "[Debug ] opt: $@, optind: $OPTIND"

## shift to remain options
shift $(($OPTIND - 1))
#echo "[Debug ] remain opt: $@"

## run command
([[ $COMMANDS =~ $COMMAND ]] && $COMMAND)

