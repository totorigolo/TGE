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
	texturesB = {"box", "box2", "way", "caisse", "tonneau"}
	texturesC = {"ball", "circle"}
	posRotB = b2Vec3(0, 100, 0)
	x, xmax = 0, 2
	while x < xmax do
		-- Crée la Box
		texture = texturesB[rand(1, 5)]
		EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture, 1)
		EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture)
		
		-- Crée le Circle
		texture = texturesC[rand(1, 2)]
		EntityFactory.CreateCircle(posRotB, b2_dynamicBody, texture, 1)
		
		-- Crée le Poly
		nb = rand(3, 8)
		vec = vector_b2Vec2()
		for i=0,nb do
			vec:push_back(b2Vec2(rand(-3, 3), rand(150, 153)))
		end
		--EntityFactory.CreatePoly(vec, b2_dynamicBody, "unknown", 1)
		EntityFactory.CreatePolyChain(vec, PolyChain.Loop, "unknown", 1)
		
		-- Incrémente x
		x = x + 1
	end
end

function testAll()
	-- Test des évènements
	if (eventCreated == nil) then
		inputMgr:AddSpyedKey(Keyboard.O)
		eventCreated = true
	end

	-- Mumuse avec le level
	level.bckgcolor = Color(rand(200, 255), rand(200, 255), rand(200, 255))

	if (inputMgr:KeyReleased(Keyboard.O)) then
		level:Clear()
	else
		-- Charge
		level:LoadFromFile("lvls/2.xvl")

		-- Charge une petite déco
		if (tex_loaded == nil) then
			LoadTexture("skyrim", "tex/skyrim.jpg")
			tex_loaded = true
			print "Texture Skyrim chargee."
		end

		-- Crée les variables
		posRotB = b2Vec3(0, 10, 0)
		posRotC = b2Vec3(0, 11, 0)
		texturesB = {"box", "box2", "way", "caisse", "tonneau"}
		texturesC = {"ball", "circle"}

		-- Boucle de création
		xmin, xmax, y = -20, 20, 20
		x = xmin
		while x < xmax do
			-- Met à your les coordonnées
			posRotB:Set(x, y - 0.5, 0)
			posRotC:Set(x, y + 0.5, 0)
			
			-- Crée des skyrim à la nawak
			posRotS = b2Vec3(randf(-20, 20), randf(-20, 20), randf(0, 360))
			--EntityFactory.CreateDeco(posRotS, "skyrim", 20)
			
			-- Crée des lampadaires à la nawak
			posRotD = b2Vec3(randf(-20, 20), randf(0, 20), randf(0, 360))
			--EntityFactory.CreateBox(posRotD, b2BodyType.b2_staticBody, "lampadere", 1)

			-- Crée la Box
			texture = texturesB[rand(1, 5)]
			--EntityFactory.CreateBox(posRotB, b2BodyType.b2_dynamicBody, texture, 1)
			
			-- Crée le Circle
			texture = texturesC[rand(1, 2)]
			--EntityFactory.CreateCircle(posRotC, b2BodyType.b2_dynamicBody, texture, 1)
			
			-- Incrémente x
			x = x + 1
		end
	end
end