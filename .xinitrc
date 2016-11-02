export LANG=ru_RU.UTF-8
export MM_CHARSET=UTF-8

setxkbmap -layout 'us,ru' -option 'grp:ctrl_shift_toggle'
xsetroot -cursor_name left_ptr

# disable bell
xset -b

# start console
#yeahconsole &

#xmobar &
#exec xmonad > ~/.xmonad/pipe
#feh --bg-fill reWalls.com-16593.1.jpg
#eval $(cat ~/.fehbg)
#eval $(ssh-agent -s)

#exec xmonad

exec awesome
