-- GraphAncestors.lua

if not fta.login( 'username' ) then
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
	
	ancestors[ #ancestors + 1 ] = personId
	
	local person = fta.getPerson( personId )
	if person then
		CollectAncestors( person.fatherId, curGen + 1, maxGen )
		CollectAncestors( person.motherId, curGen + 1, maxGen )
	end
	
end

local ancestors = {}
CollectAncestors( fta.whoAmI(), ancestors, 0, maxGen )

fta.setGraph( "default", ancestors )

-- GraphAncestors.lua