-----------------------------
-- Création de plein d'objets
-----------------------------

-- Création de randf
if (randf == nil) then
	rand = math.random
	randf = function(min, max)
		return rand(min * 100000, max * 100000) / 100000
	end
	print "Fonction randf creee."
end

function createObjects1()
	-- Boucle de création
	texturesB = {"unknown"}
	posRotB = b2Vec3(0, 100, 0)
	x, xmax = 0, 2
	while x < xmax do
		-- Crée la Box
		texture = texturesB[1]--rand(1, 1)]
		EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture, 1)
		EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture)
		
		-- Incrémente x
		x = x + 1
	end
end