#!/bin/sh

# trap execution errors
set -e
trap 'echo "Error: Script execution failed" >&2; exit 1' ERR

# print usage message and exit 
usage() {
    for msg in "$@"; do printf "%s\n" "${0##*/}: $msg" >&2; done
    echo "usage: ${0##*/} source group" >&2
    exit 1
}

# check if there are exactly 2 arguments, throw usage if not
test $# -eq 2 || usage

# capture arguments
LIBRARY_DIR="$1"
GROUP_NAME="$2"

# create high-level directory
mkdir -p "$LIBRARY_DIR"
chmod 755 "$LIBRARY_DIR"

# create a marker
echo "library v1.0" > "$LIBRARY_DIR/.library"
chmod 644 "$LIBRARY_DIR/.library"

# create a library directories
mkdir -p "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"
chmod 755 "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"
chmod o-w "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"

# create a price directory
mkdir -p "$LIBRARY_DIR/prices"
chmod 750 "$LIBRARY_DIR/prices"

# delegate directory to a provided group
chown :"$GROUP_NAME" "$LIBRARY_DIR/prices"

# create a tmp symlink, targeting system /tmp with our custom subdirectories
mkdir -p "/tmp/$LIBRARY_DIR/.tmp"
ln -sf "/tmp/$LIBRARY_DIR/.tmp" "$LIBRARY_DIR"

# provide permissions for the actual tmp directories
chmod 1770 "/tmp/$LIBRARY_DIR/.tmp"
chown :"$GROUP_NAME" "/tmp/$LIBRARY_DIR/.tmp"