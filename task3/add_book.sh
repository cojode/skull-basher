#!/bin/sh

# trap execution errors
set -e
trap 'echo "Error: Script execution failed" >&2; exit 1' ERR

# print usage message and exit 
usage() {
    for msg in "$@"; do
        printf "%s\n" "${0##*/}: $msg" >&2
    done
    echo "usage: ${0##*/} source name price AUTHOR ..." >&2
    exit 1
}

test $# -ge 4 || usage "not enough arguments"

ADD_BOOK_SOURCE=$1
ADD_BOOK_NAME=$2
ADD_BOOK_PRICE=$3

shift 3 # discard first three arguments

ADD_BOOK_AUTHORS="$@"

# Check for .library marker
if ! find . -maxdepth 1 -name ".library" | grep -q .; then
    echo "Error: Marker not found" >&2
    exit 1
fi

# test not failed - we can write a path of a directory to use later:
ADD_BOOK_LIBRARY_DIR=$(pwd)

ADD_BOOK_SUFFIX=""
ADD_BOOK_COUNTER=1

# create a unique name for a book file
while [ -e "$ADD_BOOK_LIBRARY_DIR/books/${ADD_BOOK_NAME}${ADD_BOOK_SUFFIX}" ]; do
    ADD_BOOK_SUFFIX=".$ADD_BOOK_COUNTER"
    ADD_BOOK_COUNTER=$((ADD_BOOK_COUNTER + 1))
done

ADD_BOOK_FILENAME="${ADD_BOOK_NAME}${ADD_BOOK_SUFFIX}"

# Ensure directories exist
mkdir -p "$ADD_BOOK_LIBRARY_DIR/.tmp"
mkdir -p "$ADD_BOOK_LIBRARY_DIR/books"
mkdir -p "$ADD_BOOK_LIBRARY_DIR/prices"
mkdir -p "$ADD_BOOK_LIBRARY_DIR/authors"

# copy book to a temporary directory
cp "$ADD_BOOK_SOURCE" "$ADD_BOOK_LIBRARY_DIR/.tmp/$(basename "$ADD_BOOK_FILENAME")"

# move book from tmp to a books directory
mv "$ADD_BOOK_LIBRARY_DIR/.tmp/$(basename "$ADD_BOOK_FILENAME")" "$ADD_BOOK_LIBRARY_DIR/books/$ADD_BOOK_FILENAME"
chmod 664 "$ADD_BOOK_LIBRARY_DIR/books/$ADD_BOOK_FILENAME"

# creates price file
if [ -n "$ADD_BOOK_PRICE" ] && [ "$ADD_BOOK_PRICE" != "-" ]; then
    echo "$ADD_BOOK_PRICE" > "$ADD_BOOK_LIBRARY_DIR/prices/${ADD_BOOK_FILENAME}.price"
    chmod 644 "$ADD_BOOK_LIBRARY_DIR/prices/${ADD_BOOK_FILENAME}.price"
fi

# author workflow
for AUTHOR in $ADD_BOOK_AUTHORS; do
    # author directory creation
    AUTHOR_DIR="$ADD_BOOK_LIBRARY_DIR/authors/$AUTHOR"
    if [ ! -d "$AUTHOR_DIR" ]; then
        mkdir "$AUTHOR_DIR"
        chmod 775 "$AUTHOR_DIR"
    fi
    
    # hard link creation
    ln "$ADD_BOOK_LIBRARY_DIR/books/$ADD_BOOK_FILENAME" "$AUTHOR_DIR/$ADD_BOOK_FILENAME"
done