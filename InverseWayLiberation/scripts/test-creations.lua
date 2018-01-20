-----------------------------
-- Création de plein d'objets
-----------------------------

-- Création de randf
if (randf == nil) then
    rand = math.random
    randf = function(min, max)
        return rand(min * 100000, max * 100000) / 100000
    end
    cprintln "Fonction randf creee."
end

function createObjects1()
    -- Boucle de création
    local texturesB = { "box", "box2", "way", "caisse", "tonneau" }
    local texturesC = { "ball", "circle" }
    local posRotB = b2Vec3(0, 100, 0)
    local x, xmax = 0, 2
    while x < xmax do
        -- Crée la Box
        local texture = texturesB[rand(1, 5)]
        EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture, 1)
        EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture)

        -- Crée le Circle
        local texture = texturesC[rand(1, 2)]
        EntityFactory.CreateCircle(posRotB, b2_dynamicBody, texture, 1)

        -- Crée le Poly
        local nb = rand(3, 8)
        local vec = vector_b2Vec2()
        for i = 0, nb do
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
    level.bckgcolor = Color(rand(0, 50), rand(0, 50), rand(0, 50))

    if (inputMgr:IsKeyPressed(Keyboard.O)) then
        level:Clear()
    else
        -- Charge les textures
        local textures = {
            "ball", "box", "box2", "caisse", "circle", "lampadere", "tonneau", "volcan", "wall", "way"
        }
        for texIndex = 1, #textures do
            local textureName = textures[texIndex]
            local texturePath = "tex/old_assets/" .. textureName .. ".png"
            resourceMgr:LoadTexture(textureName, texturePath)
        end

        -- Crée les variables
        local posRotB = b2Vec3(0, 10, 0)
        local posRotC = b2Vec3(0, 11, 0)
        local texturesB = { "box", "box2", "caisse", "tonneau", "way" }
        local texturesC = { "ball", "circle" }

        -- Boucle de création
        local xmin, xmax, y = -20, 20, 20
        local x = xmin
        while x < xmax do
            -- Met à your les coordonnées
            posRotB:Set(x, y - 0.5, 0)
            posRotC:Set(x, y + 0.5, 0)

            -- Crée des volcans à la nawak
            local posRotS = b2Vec3(randf(-20, 20), randf(-20, 20), randf(0, 360))
            EntityFactory.CreateDeco(posRotS, "volcan", 20)

            -- Crée des lampadaires à la nawak
            local posRotD = b2Vec3(randf(-20, 20), randf(0, 20), randf(0, 360))
            EntityFactory.CreateBox(posRotD, b2_staticBody, "lampadere", 1)

            -- Crée une Box
            local texture = texturesB[rand(1, 5)]
            EntityFactory.CreateBox(posRotB, b2_dynamicBody, texture, 1)

            -- Crée un Circle
            local texture = texturesC[rand(1, 2)]
            EntityFactory.CreateCircle(posRotC, b2_dynamicBody, texture, 1)

            -- Incrémente x
            x = x + 1
        end
    end
end