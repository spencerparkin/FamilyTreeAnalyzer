-- PrintAllAPIHelp.lua

-- This example shows how one can print help for every API function.
for k,v in pairs( fta ) do
	if type(v) == 'function' then
		fta.print( '------------------------------' )
		fta.help(v)
	end
end

-- PrintAllAPIHelp.lua