function Teleport.isTeleport(self)
	return true
end

function SimpleTeleport(from, destination, condition)
	local teleport = MoveEvent()

	function teleport.onStepIn(creature, item, position, fromPosition)
		local player = creature:getPlayer()
		if not player then
			return false
		end

		if condition and not condition(player, item, position, fromPosition) then
			return false
		end

		player:teleportTo(destination)
		player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
		return true
	end

	teleport:position(from)
	teleport:register()
	return teleport
end
