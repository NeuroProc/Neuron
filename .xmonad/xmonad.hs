import XMonad
import XMonad.Hooks.DynamicLog
import XMonad.Hooks.ManageDocks
import XMonad.Util.Run
import XMonad.Layout.NoBorders
import XMonad.Layout.IM
import XMonad.Layout.Grid
--import XMonad.Layout.Spacing
import XMonad.Layout.PerWorkspace (onWorkspace)
import XMonad.Layout.SimplestFloat
import XMonad.Layout.Circle
-- import XMonad.Layout.ThreeColumns
import XMonad.Layout.Tabbed
import XMonad.Layout.Accordion
import Data.Ratio ((%))
import XMonad.Hooks.EwmhDesktops (ewmh)

import System.IO

import System.Exit

import qualified XMonad.StackSet as W
import qualified Data.Map        as M

myTerminal      = "evilvte"

myBorderWidth   = 1

myModMask       = mod1Mask

myWorkspaces    = ["1:scam","2:irc","3:im","4:bt","5","6:web","7:emacs","8:VB","9:rox"]

myNormalBorderColor  = "#dddddd"
myFocusedBorderColor = "#ff0000"

myKeys conf@(XConfig {XMonad.modMask = modMask}) = M.fromList $

    -- launch a terminal
    [ ((controlMask ,          xK_grave ), spawn $ XMonad.terminal conf)
    , ((mod4Mask    ,          xK_grave ), spawn "evilvte" )
    -- , ((mod4Mask    ,          xK_grave ), spawn "pavucontrol" )

    -- launch dmenu
    -- , ((mod4Mask,               xK_p     ), spawn "exe=`dmenu_path | dmenu` && eval \"exec $exe\"")
    , ((mod4Mask,		xK_p     ), spawn "dmenu_run")
    , ((mod4Mask,               xK_b     ), sendMessage ToggleStruts)
    , ((mod4Mask               , xK_Up       ), spawn "mixer vol +10")
    , ((mod4Mask               , xK_Down     ), spawn "mixer vol -10")

    -- my proggies
    -- , ((modMask,		xK_b     ), spawn "firefox")
    -- , ((modMask,		xK_e     ), spawn "emacs")
    -- , ((modMask,		xK_i     ), spawn "pidgin")
    -- , ((modMask,		xK_l     ), spawn "liferea")
    -- , ((modMask,		xK_t     ), spawn "tkabber")
    -- , ((modMask,		xK_m     ), spawn "claws-mail")
    -- , ((modMask,		xK_x     ), spawn "xchat")
    -- , ((modMask,		xK_s     ), spawn "skype")

    -- xmms2d
    -- , ((mod4Mask               , xK_Home     ), spawn "xmms2 prev")
    -- , ((mod4Mask               , xK_End      ), spawn "xmms2 next")
    -- , ((mod4Mask               , xK_Insert   ), spawn "xmms2 play")
    -- , ((mod4Mask               , xK_Delete   ), spawn "xmms2 pause")
    -- , ((mod4Mask               , xK_Prior    ), spawn "xmms2 seek +30")
    -- , ((mod4Mask               , xK_Next     ), spawn "xmms2 seek -30")
    -- , ((mod4Mask               , xK_Up       ), spawn "xmms2 seek +20")
    -- , ((mod4Mask               , xK_Down     ), spawn "xmms2 seek -30")
    -- , ((mod4Mask               , xK_Right    ), spawn "xmms2 seek +10")
    -- , ((mod4Mask               , xK_Left     ), spawn "xmms2 seek -10")
    -- xmms2d fun stuff
    -- , ((mod4Mask               , xK_equal       ), spawn "xmms2 server config vocoder.speed 100")
    -- , ((mod4Mask               , xK_KP_Add      ), spawn "xmms2 server config vocoder.speed $((`xmms2 server config vocoder.speed | cut -d' ' -f3` + 10))")
    -- , ((mod4Mask               , xK_KP_Subtract ), spawn "xmms2 server config vocoder.speed $((`xmms2 server config vocoder.speed | cut -d' ' -f3` - 10))")

    -- misc
    -- , ((mod4Mask                 , xK_F8     ), spawn "~/.rc.d/x11/xmonad/mousekbd")
    , ((mod4Mask                 , xK_F4     ), spawn "xrandr --output HDMI3 --auto --scale 1.17x1.17 --primary")
    -- , ((mod4Mask .|. controlMask , xK_F9     ), spawn "xrandr -s 1280x1024 --output VGA1 --auto --primary; ~/.rc.d/x11/xmonad/wallpaper")
    -- , ((mod4Mask                 , xK_F10    ), spawn "~/bin/s2ram.sh")
    -- , ((mod4Mask                 , xK_F11    ), spawn "xrdb ~/.Xdefaults")
    -- , ((mod4Mask                 , xK_F12    ), spawn "xrandr              --output LVDS1 --auto --primary; ~/.rc.d/x11/xmonad/wallpaper")
    -- , ((mod4Mask .|. controlMask , xK_F12    ), spawn "xrandr -s 1280x800  --output LVDS1 --auto --primary; ~/.rc.d/x11/xmonad/wallpaper")

    -- , ((mod3Mask                 , xK_e       ), spawn "set_kb_map 0") -- en_US
    -- , ((mod3Mask                 , xK_r       ), spawn "set_kb_map 1") -- ru
    -- , ((mod3Mask                 , xK_s       ), spawn "set_kb_map 2") -- se
    -- , ((mod3Mask                 , xK_n       ), spawn "set_kb_map 3") -- no
    -- , ((mod3Mask                 , xK_f       ), spawn "set_kb_map 4") -- fi
    -- , ((mod3Mask                 , xK_f       ), spawn "set_kb_map 5") -- de

    -- , ((0                        , xK_Scroll_Lock), spawn "xset dpms force standby")
    , ((0			, xK_Print),       spawn "~/bin/screenshot.sh")
    , ((mod1Mask                , xK_Print),       spawn "~/bin/screenshot-active.sh")

    -- launch gmrun
    -- , ((mod4Mask .|. shiftMask, xK_p     ), spawn "gmrun")

    -- close focused window 
    , ((mod4Mask		, xK_c     ), kill)

     -- Rotate through the available layout algorithms
    , ((mod4Mask,               xK_space ), sendMessage NextLayout)

    --  Reset the layouts on the current workspace to default
    , ((mod4Mask .|. shiftMask, xK_space ), setLayout $ XMonad.layoutHook conf)

    -- Resize viewed windows to the correct size
    , ((mod4Mask,               xK_n     ), refresh)

    -- Move focus to the next window
    , ((mod4Mask,               xK_Tab   ), windows W.focusDown)

    -- Move focus to the next window
    , ((mod4Mask,               xK_j     ), windows W.focusDown)

    -- Move focus to the previous window
    , ((mod4Mask,               xK_k     ), windows W.focusUp  )

    -- Move focus to the master window
    , ((mod4Mask,               xK_m     ), windows W.focusMaster  )

    -- Swap the focused window and the master window
    , ((mod4Mask,               xK_Return), windows W.swapMaster)

    -- Swap the focused window with the next window
    , ((mod4Mask .|. shiftMask, xK_j     ), windows W.swapDown  )

    -- Swap the focused window with the previous window
    , ((mod4Mask .|. shiftMask, xK_k     ), windows W.swapUp    )

    -- Shrink the master area
    , ((mod4Mask,               xK_h     ), sendMessage Shrink)

    -- Expand the master area
    , ((mod4Mask,               xK_l     ), sendMessage Expand)

    -- Push window back into tiling
    , ((mod4Mask,               xK_t     ), withFocused $ windows . W.sink)

    -- Increment the number of windows in the master area
    , ((mod4Mask              , xK_comma ), sendMessage (IncMasterN 1))

    -- Deincrement the number of windows in the master area
    , ((mod4Mask              , xK_period), sendMessage (IncMasterN (-1)))

    -- toggle the status bar gap
    -- TODO, update this binding with avoidStruts , ((modMask              , xK_b     ),

    -- Quit xmonad
    , ((mod4Mask .|. shiftMask, xK_q     ), io (exitWith ExitSuccess))

    -- Restart xmonad
    , ((mod4Mask             , xK_q     ),
          broadcastMessage ReleaseResources >> restart "xmonad" True)
    ]
    ++

    --
    -- control-[1..9], Switch to workspace N
    -- mod-shift-[1..9], Move client to workspace N
    --
    [((m , k), windows $ f i)
        | (i, k) <- zip (XMonad.workspaces conf) [xK_1 .. xK_9]
        , (f, m) <- [(W.greedyView, controlMask), (W.shift, shiftMask .|. modMask)]]
{-
    ++

    --
    -- mod-{w,e,r}, Switch to physical/Xinerama screens 1, 2, or 3
    -- mod-shift-{w,e,r}, Move client to screen 1, 2, or 3
    --
    [((m .|. modMask, key), screenWorkspace sc >>= flip whenJust (windows . f))
        | (key, sc) <- zip [xK_w, xK_e, xK_r] [0..]
        , (f, m) <- [(W.view, 0), (W.shift, shiftMask)]]
-}

myMouseBindings (XConfig {XMonad.modMask = modMask}) = M.fromList $
    [ ((modMask, button1), (\w -> focus w >> mouseMoveWindow w))
    , ((modMask, button2), (\w -> focus w >> windows W.swapMaster))
    , ((modMask, button3), (\w -> focus w >> mouseResizeWindow w))
    -- Mouse scroll wheel to raise/lower windows
    -- , ((modMask, button5), (\w -> windows W.swapDown))
    -- , ((modMask, button4), (\w -> windows W.swapUp))
    ]

--myLayout = smartBorders $ avoidStruts $ (Full ||| tiled ||| Mirror tiled ||| withIM (1%7) (ClassName "Skype") Grid)
--  where
--     tiled   = Tall nmaster delta ratio
--     nmaster = 1
--     ratio   = 4/5
--     delta   = 3/100

--myLayout = onWorkspace "3:im" skypeLayout $ onWorkspace "9:rox" Circle $ onWorkspace "8:VB" Circle $ defaultLayouts
myLayout = defaultLayouts

defaultLayouts = Full ||| tiled ||| Mirror tiled ||| Circle ||| simpleTabbed ||| Accordion ||| Grid ||| skypeLayout
  where
     -- default tiling algorithm partitions the screen into two panes
     tiled = Tall nmaster delta ratio
     -- The default number of windows in the master pane
     nmaster = 1
     nmaster2 = 2
     -- Default proportion of screen occupied by master pane
     ratio = 2/3
     -- Percent of screen to increment by when resizing panes
     delta = 3/100

skypeLayout = withIM (1%7) (ClassName "Skype") Grid



myManageHook = composeAll
    [ className =? "MPlayer"        --> doFloat
    , className =? "Gimp"           --> doFloat
    , className =? "VirtualBox"     --> doFloat
    -- , className =? "ROX-Filer"      --> doFloat
    , className =? "Linphone"       --> doFloat 
    , className =? "Transmission"   --> doFloat
    , className =? "Glade-3"        --> doFloat
    -- , className =? "GQview"         --> doFloat
    -- , className =? "Wine"           --> doFloat
    , className =? "Xchat"          --> doF (W.shift "2:irc")
    , className =? "linphone"       --> doF (W.shift "3:im")
    , className =? "Firefox"        --> doF (W.shift "6:web")
    , className =? "ROX-Filer"      --> doF (W.shift "9:rox")
    , resource  =? "desktop_window" --> doIgnore
    , resource  =? "kdesktop"       --> doIgnore ]

myFocusFollowsMouse :: Bool
myFocusFollowsMouse = True

myStartupHook = return ()

------------------------------------------------------------------------
xmobarCmdT = "xmobar"
--xmobarCmdT = "                                            xmobar -o -B '#122c80' -F '#adbadd' -f 'xft:Terminus-12' -t '%StdinReader%'"
--xmobarCmdB = "~/bin/player-status.sh | xmobar -b -B '#122c60' -F '#adbadd' -f 'xft:Terminus-10' -t '%StdinReader% }{ %battery% | %cpu% | %memory% ||| %wlan0% ||| <fc=cyan>%date%</fc>'"
--xmobarCmdB = "~/bin/player-status.sh | xmobar -b -B '#122c60' -F '#adbadd' -f 'xft:Terminus-10' -t '%StdinReader% }{ %cpu% | %memory% ||| %wlan0% ||| <fc=cyan>%date%</fc>'"
main = do 
          dinT <- spawnPipe xmobarCmdT
          -- dinB <- spawnPipe xmobarCmdB
          xmonad $ ewmh $ defaultConfig {
      -- simple stuff
      terminal           = myTerminal,
      focusFollowsMouse  = myFocusFollowsMouse,
      borderWidth        = myBorderWidth,
      modMask            = myModMask,
      workspaces         = myWorkspaces,
      normalBorderColor  = myNormalBorderColor,
      focusedBorderColor = myFocusedBorderColor,

      -- key bindings
      keys               = myKeys,
      mouseBindings      = myMouseBindings,

      -- hooks, layouts
      layoutHook         = smartBorders $ avoidStruts $ myLayout,
      manageHook         = myManageHook <+> manageDocks,
      logHook            = dynamicLogWithPP $ xmobarPP { ppOutput = hPutStrLn dinT },

      startupHook        = myStartupHook
}
