#!/bin/sh

if [ $# -lt 1 ]; then
    echo "Usage: $0 <username> [signal]"
    exit 1
fi

USERNAME=$1
SIGNAL=$2

PIDS=$(pgrep -u "$USERNAME" tmux)

if [ -z "$PIDS" ]; then
    echo "No tmux processes found for user $USERNAME"
    exit 0
fi

if [ -n "$SIGNAL" ]; then
    kill -s "$SIGNAL" $PIDS
else
    echo "tmux processes PIDs for user $USERNAME:"
    echo "$PIDS"
fi