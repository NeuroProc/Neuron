#!/usr/local/bin/bash

DIR=~/archive/shots
FILE="$(date "+%F-%H-%M-%S").png"

activeWinLine=$(xprop -root | grep "_NET_ACTIVE_WINDOW(WINDOW)")
activeWinId=${activeWinLine:40}
import -window "$activeWinId" "$DIR/$FILE"

imp imgur "$DIR/$FILE" | xclip -selection clipboard &
