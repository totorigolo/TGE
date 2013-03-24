
-- Crée les variables
posRot = b2Vec3(0, 10, 0)
posRot2 = b2Vec2(0, 12)
textures = {"box", "box2", "way", "caisse", "tonneau"}
layer = -1

-- Crée la box
v = -50
while v < 50 do
	posRot.x = v
	posRot2.x = v
	
	texture = textures[math.random(1, 5)]
	EntityFactory.CreateBox(posRot, texture, layer)
	
	EntityFactory.CreateRagdoll(posRot2, layer)
	
	v = v + 1
end
