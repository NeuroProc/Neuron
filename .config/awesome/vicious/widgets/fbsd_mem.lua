---------------------------------------------------
-- Licensed under the GNU General Public License v2
--  * (c) 2010, Adrian C. <anrxc@sysphere.org>
--  * (c) 2009, Lucas de Vries <lucas@glacicle.com>
---------------------------------------------------

-- {{{ Grab environment
local io = { popen = io.popen }
local setmetatable = setmetatable
local math = { floor = math.floor }
-- }}}


-- Mem: provides RAM and Swap usage statistics
-- vicious.widgets.mem
local fbsd_mem = {}


-- {{{ Memory widget type
local function worker(format)
    local _mem = { buf = {}, swp = {} }

    -- Get MEM info
    local f

	f = io.popen( 'sysctl -n hw.pagesize' )
	local pagesize = f:read();
	f:close()

	f = io.popen( 'sysctl -n vm.stats.vm.v_page_count' )
	local total_pages = f:read();
	f:close()

	f = io.popen( 'sysctl -n vm.stats.vm.v_free_count' )
	local free_pages = f:read();
	f:close()

	f = io.popen( 'sysctl -n vm.stats.vm.v_inactive_count' )
	local inact_pages = f:read();
	f:close()

	-- Calculate memory percentage
	_mem.total = ( total_pages * pagesize )
	_mem.free  = ( free_pages + inact_pages ) * pagesize
	_mem.inuse = ( total_pages - free_pages - inact_pages ) * pagesize
	_mem.bcuse = _mem.total - _mem.free
	_mem.usep  = math.floor( _mem.inuse / _mem.total * 100 )

    -- Calculate swap percentage
    -- TODO
    _mem.swp.t     = 0
	_mem.swp.f     = 0
	_mem.swp.inuse = 0
	_mem.swp.usep  = 0

    return {_mem.usep,     _mem.inuse,     _mem.total, _mem.free,
            _mem.swp.usep, _mem.swp.inuse, _mem.swp.t, _mem.swp.f,
            _mem.bcuse }
end
-- }}}

return setmetatable(fbsd_mem, { __call = function(_, ...) return worker(...) end })
