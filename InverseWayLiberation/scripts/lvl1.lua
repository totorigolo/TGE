-----------------------
-- Création du niveau 1
-----------------------

-- Propriétés du monde
level.gravity = b2Vec2(0, -9.8)
level.PPM = 100
level.bckgcolor = Color(0, 0, 0, 255)
level.originview = b2Vec2(-1.7, 2)
level.defaultzoom = 5

-- Charge les textures
resourceMgr:LoadTexture("way",			"tex/old_assets/way.png")
resourceMgr:LoadTexture("box", 			"tex/old_assets/box.png")
resourceMgr:LoadTexture("box2",			"tex/old_assets/box2.png")
resourceMgr:LoadTexture("caisse", 		"tex/old_assets/caisse.png")
resourceMgr:LoadTexture("tonneau", 		"tex/old_assets/tonneau.png")
resourceMgr:LoadTexture("circle", 		"tex/old_assets/circle.png")
resourceMgr:LoadTexture("ball",			"tex/old_assets/ball.png")
resourceMgr:LoadTexture("lampadere",	"tex/old_assets/lampadere.png")
resourceMgr:LoadTexture("volcan", 		"tex/old_assets/volcan.png")

-- Crée la déco
EntityFactory.CreateDeco(b2Vec3(0, 5, 0), "volcan", 5)
EntityFactory.CreateDeco(b2Vec3(3, 1.25, 0), "lampadere", -1)

-- Crée les bodies simples
EntityFactory.CreateStaticBox(b2Vec3(0, -0.64, 0), "ground", -1)
EntityFactory.CreateStaticBox(b2Vec3(20.48, -0.64, 0), "way", -1)
EntityFactory.CreateStaticBox(b2Vec3(-20.48, -0.64, 0), "way", -1)
--[[
EntityFactory.CreateStaticBox("way", b2Vec3(-5, 5.5) rotation="90" osp="true", "-1")
EntityFactory.CreateDynBox("way", b2Vec3(-5, 6.5))

EntityFactory.CreateStaticBox("way", b2Vec3(-5, 0.5) rotation="90")
EntityFactory.CreateStaticBox("way", b2Vec3(-5, 1.5) rotation="90")
EntityFactory.CreateStaticBox("way", b2Vec3(-4.25, 1.75))
EntityFactory.CreateStaticBox("way", b2Vec3(-3.25, 1.75))
EntityFactory.CreateStaticBox("way", b2Vec3(-3, 1.5) rotation="90")

EntityFactory.CreateDynBox(id="1" "way", b2Vec3(-3, 0.5) rotation="90." density="1")
EntityFactory.CreateDynBox(id ="7" "tonneau", b2Vec3(-4.55, 0.25) density="0.2", "0.05")
EntityFactory.CreateDynCircle(id="8" "ball", b2Vec3(-4.55, 0.675), "20" bullet="true")

EntityFactory.CreateStaticBox(id="3" "way", b2Vec3(0, 2.5) osp="true", "-1")
EntityFactory.CreateStaticBox("way", b2Vec3(1.5, 2.5) osp="true", "-1")
EntityFactory.CreateDynBox("box", b2Vec3(0, 3))
EntityFactory.CreateDynBox(id="4" "box2", b2Vec3(1.5, 3))
EntityFactory.CreateDynBox(id="2" "caisse", b2Vec3(1.9, 3))

EntityFactory.CreateStaticBox(id="5" "caisse", b2Vec3(1.9, 4) restitution="1")
]]

-- Crée les Entities
EntityFactory.CreateRagdoll(b2Vec2(0, 1.5), 2)
--EntityFactory.CreatePlayer(b2Vec2(0, 5), "hero", 2)

-- Crée les joints
--JointFactory.CreatePulleyJoint(1, b2Vec2(0, 0), 2, b2Vec2(0, 0), b2Vec2(-2, 0.5), b2Vec2(1.9, 4), 1.5)
--JointFactory.CreateRopeJoint(3, b2Vec2(0, 0), 4, b2Vec2(0, 0), 1, true, (0, 255, 255))
























