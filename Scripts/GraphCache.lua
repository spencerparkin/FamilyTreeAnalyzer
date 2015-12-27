-- GraphCache.lua

if not fta.amLoggedIn() and not fta.login() then
	error( 'Failed to login!' )
end

local personId = fta.whoAmI()
personId = 'KWZC-XN7'
fta.fillCache( personId, 100 )

fta.drawGraph( "graphviz", fta.getAllCachedPersonIds() )

-- GraphCache.lua