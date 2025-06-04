#!/bin/bash

set -e
trap 'echo "Error: Script execution failed" >&2; exit 1' ERR

usage() {
    for msg in "$@"; do printf "%s\n" "${0##*/}: $msg" >&2; done
    echo "usage: ${0##*/} [-v] source group" >&2
    exit 1
}

test $# -eq 2 || usage

LIBRARY_DIR="$1"
GROUP_NAME="$2"

mkdir -p "$LIBRARY_DIR"
chmod 755 "$LIBRARY_DIR"

echo "library v1.0" > "$LIBRARY_DIR/.library"
chmod 644 "$LIBRARY_DIR/.library"

mkdir -p "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"
chmod 755 "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"
chmod 755 "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"

chmod o-w "$LIBRARY_DIR/books" "$LIBRARY_DIR/authors"

mkdir -p "$LIBRARY_DIR/prices"
chmod 750 "$LIBRARY_DIR/prices"
sudo chown :"$GROUP_NAME" "$LIBRARY_DIR/prices"

mkdir -p "$LIBRARY_DIR/.tmp"
chmod 770 "$LIBRARY_DIR/.tmp"
sudo chown :"$GROUP_NAME" "$LIBRARY_DIR/.tmp"