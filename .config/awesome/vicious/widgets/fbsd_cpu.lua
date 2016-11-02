---------------------------------------------------
-- Licensed under the GNU General Public License v2
--  * (c) 2011, Adrian C. <anrxc@sysphere.org>
--  * (c) 2009, Lucas de Vries <lucas@glacicle.com>
--  * (c) 2011, Jörg Thalheim <jthalheim@gmail.com>
---------------------------------------------------

-- {{{ Grab environment
local io = { popen = io.popen }
local setmetatable = setmetatable
local math = { floor = math.floor }
local string = { match = string.match }
-- }}}


-- Cpu: provides CPU usage for all available CPUs/cores
-- vicious.widgets.fbsd_cpu
local fbsd_cpu = {}


-- Initialize function tables
local cpu_usage  = 0
local cpu_total  = 0
local cpu_active = 0

-- {{{ CPU widget type
local function worker(format)
    -- Get CPU stats
    local f = io.popen('sysctl -n kern.cp_time')
    local user, nice, system, interrupt, idle = string.match(f:read(), "([^ ]+) ([^ ]+) ([^ ]+) ([^ ]+) ([^ ]+)")
    f:close()

    -- Calculate totals
    local total_new  = user + nice + system + idle
    local active_new = user + nice + system

    -- Calculate percentage
    local diff_total  = total_new - cpu_total
    local diff_active = active_new - cpu_active

    if diff_total == 0 then diff_total = 1E-6 end
    cpu_usage = math.floor((diff_active / diff_total) * 100)

    -- Store totals
    cpu_total   = total_new
    cpu_active  = active_new

    return { cpu_usage }
end
-- }}}

return setmetatable(fbsd_cpu, { __call = function(_, ...) return worker(...) end })
