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
		EntityFactory.CreateDynamicBox(posRotB, texture, 1)
		
		-- Crée le Circle
		texture = texturesC[rand(1, 2)]
		EntityFactory.CreateDynamicCircle(posRotB, texture, 1)
		
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
			EntityFactory.CreateStaticBox(posRotD, "lampadere", 1)

			-- Crée la Box
			texture = texturesB[rand(1, 5)]
			EntityFactory.CreateDynamicBox(posRotB, texture, 1)
			
			-- Crée le Circle
			texture = texturesC[rand(1, 2)]
			EntityFactory.CreateDynamicCircle(posRotC, texture, 1)
			
			-- Incrémente x
			x = x + 1
		end
	end
end