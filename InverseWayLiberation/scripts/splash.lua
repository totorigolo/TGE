-----------------------------
-- Cr�ation de plein d'objets
-----------------------------

-- Cr�ation de randf
if (randf == nil) then
	rand = math.random
	randf = function(min, max)
		return rand(min * 100000, max * 100000) / 100000
	end
	print "Fonction randf creee."
end

function createObjects1()
	-- Boucle de cr�ation
	texturesB = {"unknown"}
	posRotB = b2Vec3(0, 100, 0)
	x, xmax = 0, 2
	while x < xmax do
		-- Cr�e la Box
		texture = texturesB[1]--rand(1, 1)]
		EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture, 1)
		EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture)
		
		-- Incr�mente x
		x = x + 1
	end
end