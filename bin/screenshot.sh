#!/bin/sh

DIR=~/archive/shots
FILE="$(date "+%F-%H-%M-%S").png"
import -window root "$DIR/$FILE"
imp imgur "$DIR/$FILE" | xclip -selection clipboard &
# curl -F "upload=yes" -F "fileupload=~/archive/shots/screen.jpg" http://www.imageshack.us/upload_api.php | grep "image_html" | awk -F"="" '{print $2}' | cut -d "&" -f1 | tee -ai /var/log/screens.log | xsel -b -i
# aplay ~/bin/done.wav
