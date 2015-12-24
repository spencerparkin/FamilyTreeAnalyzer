-- GraphAncestors.lua

if not fta.amLoggedIn() and not fta.login() then
	error( 'Failed to login!' )
end

local query = { question = 'Graph how many generations back?', min = 0, max = 8, default = 4 }
if not fta.queryUserForInteger( query ) then
	return
end

local maxGen = query.integer

function CollectAncestors( personId, ancestors, curGen, maxGen )

	if curGen > maxGen then return end
	if personId == '' then return end
	
	fta.print( "Found person " .. personId .. " at generation " .. tostring( curGen ) )
	ancestors[ #ancestors + 1 ] = personId
	
	local person = fta.getPerson( personId )
	if person then
		CollectAncestors( person.fatherId, ancestors, curGen + 1, maxGen )
		CollectAncestors( person.motherId, ancestors, curGen + 1, maxGen )
	end
	
end

local ancestors = {}
local personId = fta.whoAmI()
personId = 'KWZC-XN7'
CollectAncestors( personId, ancestors, 0, maxGen )

fta.setGraph( "graphviz", ancestors )

-- GraphAncestors.lua