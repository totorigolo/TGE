--------------------------------------------
-- Casse un joint pour relacher la pierre --
--------------------------------------------

--function breakJoints()
	-- Rompts les joints
	if physicMgr:JointExists("ecrase_j2") == true then
		physicMgr:GetJoint("ecrase_j2"):Destroy()
	end
--end