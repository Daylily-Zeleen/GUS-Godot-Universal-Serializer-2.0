#!/usr/bin/env bash

# This script ensures proper POSIX text file formatting and a few other things.
# This is supplementary to clang_format.sh and black_format.sh, but should be
# run before them.

# We need dos2unix and recode.
if [ ! -x "$(command -v dos2unix)" -o ! -x "$(command -v recode)" ]; then
    printf "Install 'dos2unix' and 'recode' to use this script.\n"
fi

set -uo pipefail
IFS=$'\n\t'

# Loops through all text files tracked by Git.
git grep -zIl '' |
while IFS= read -rd '' f; do
    # Ensure that files are UTF-8 formatted.
    recode UTF-8 "$f" 2> /dev/null
    # Ensure that files have LF line endings and do not contain a BOM.
    dos2unix "$f" 2> /dev/null
    # Remove trailing space characters and ensures that files end
    # with newline characters. -l option handles newlines conveniently.
    perl -i -ple 's/\s*$//g' "$f"
done

diff=$(git diff --color)

# If no patch has been generated all is OK, clean up, and exit.
if [ -z "$diff" ] ; then
    printf "Files in this commit comply with the formatting rules.\n"
    rm -f patch.patch
    exit 0
fi

# A patch has been created, notify the user, clean up, and exit.
printf "\n*** The following differences were found between the code "
printf "and the formatting rules:\n\n"
echo "$diff"
printf "\n*** Aborting, please fix your commit(s) with 'git commit --amend' or 'git rebase -i <hash>'\n"
exit 1
