---------------------------------------------------
-- Licensed under the GNU General Public License v2
--  * (c) 2010, Adrian C. <anrxc@sysphere.org>
--  * (c) 2013, NormalRa  <normalrawr@gmail.com>
---------------------------------------------------

-- {{{ Grab environment
local io = { popen = io.popen }
local tonumber = tonumber
local setmetatable = setmetatable
local string = { format = string.format }
local math = {
    min = math.min,
    floor = math.floor
}
-- }}}


-- Bat: provides state, charge, remaining time, and wear for a requested battery
-- vicious.widgets.fbsd_bat
local fbsd_bat = {}


-- {{{ Battery widget type
local function worker(format, warg)
    local battery_state = {}
    battery_state["0"] = ""  -- full
    battery_state["1"] = ""  -- unplugged/discharging
    battery_state["2"] = "↯" -- charging
    unknown = "⌁"

    local f = io.popen( 'sysctl -n hw.acpi.battery.life' )
    if not f then
        return { unknown, 0, "N/A", 0 }
    end
    local b_life = f:read()
    f:close()

    f = io.popen( 'sysctl -n hw.acpi.battery.time' )
    local b_time = f:read()
    f:close()

    f = io.popen( 'sysctl -n hw.acpi.battery.state' )
    local b_state = f:read()
    f:close()

    -- Get state information
    if b_time ~= "-1" then
        local time_h = math.floor( b_time / 60 )
        local time_m = b_time - 60 * time_h
        time = string.format("%02d:%02d", time_h, time_m)
    else
        time = "N/A"
    end

    local state = battery_state[b_state] or unknown

    return { state, tonumber(b_life), time, 0 }
end

return setmetatable(fbsd_bat, { __call = function(_, ...) return worker(...) end })
