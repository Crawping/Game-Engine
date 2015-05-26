--[[
	This file lists every asset to build
]]

return
{

	-- Textures
	{
		builder = "TextureBuilder.exe",
		extensions =
		{
			source = "png",
			target = "dds",
		},
		assets =
		{
			{
				name = "cubeTexture",
			},

			{
				name = "floorTexture",
			},

			{
				name = "soccerBallTexture",
			},

			{
				name = "cylinderTexture",
			},

			{
				name = "orangeSkull",
			},

			{
				name = "greenSkull",
			},
			{
				name = "greenBar",
			},
			{
				name = "optionsButton",
			},
			{
				name = "Player1Turn",
			},
			{
				name = "Player2Turn",
			},
			{
				name = "mainmenuButton",
			},

			{
				name = "numberSheet",
			},
			{
				name = "Texture_Tile",
			},
			{
				name = "cement_wall",
			},
			{
				name = "floor",
			},

			{
				name = "metal_brace",
			},
			{
				name = "railing",
			},
			{
				name = "wall",
			},

		},
	},
    
    -- Audio
	{
		builder = "GenericBuilder.exe",
		extensions =
		{
			source = "wav",
			target = "wav",
		},
		assets =
		{
			{
				name = "computer-talk",
			},	
            {
				name = "bonus-laughing",
			},
            {
				name = "buzzer-variants-3",
			},
            {
				name = "empty-2",
			},
            {
				name = "heavy-breathing-off-mic-loop",
			},
            {
				name = "jackpot8",
			},
            {
				name = "nuclear-alarm",
			},
            {
				name = "running-gravel-or-dry-leaves-loop",
			},
            {
				name = "running-hard-surface",
			},
            {
				name = "sweetalertsound5",
			},
            {
				name = "welcome-bonus",
			},
            {
				name = "winn-up",
			},
            {
				name = "ambient",
			},
             {
				name = "ambient-sound",
			},
            {
				name = "TitleMusic",
			},   
             {
				name = "background",
			},        
		},
	},
	-- Shader Programs
	{
		builder = "ShaderBuilder.exe",
		extensions =
		{
			source = "hlsl",
			target = "shd",
		},
		assets =
		{
			-- Vertex shader programs
			{
				name = "vertexShader",
				arguments = { "vertex" },
			},

			{
				name = "vertexShader_Sprite",
				arguments = { "vertex" },
			},

			{
				name = "vertexShader_DebugLines",
				arguments = { "vertex" },
			},

			{
				name = "vertexShader_Tiles",
				arguments = { "vertex" },
			},

			{
				name = "vertexShader_LineRenderer",
				arguments = { "vertex" },
			},

			-- Fragment shader programs
			{
				name = "fragmentShader",
				arguments = { "fragment" },
			},	

			{
				name = "fragmentShader_Sprite",
				arguments = { "fragment" },
			},

			{
				name = "fragmentShader_DebugLines",
				arguments = { "fragment" },
			},	
			
			{
				name = "fragmentShader_Tiles",
				arguments = { "fragment" },
			},	

			{
				name = "fragmentShader_TilesTransition",
				arguments = { "fragment" },
			},	
			{
				name = "fragmentShader_LineRenderer",
				arguments = { "fragment" },
			},		
		},
	},
	-- Generic Assets
	-- (That just get copied as-is rather than built)
	{
		builder = "GenericBuilder.exe",
		extensions =
		{
			source = "txt",
			target = "txt",
		},
		assets =
		{
			{
				name = "cubeMaterial",
			},
			{
				name = "Material_TileBlue",
			},
			{
				name = "Material_TileRed",
			},
			{
				name = "Material_TileWhite",
			},
			{
				name = "Material_TileTransition",
			},
			{
				name = "walls_m",
			},
			{
				name = "railing_m",
			},
			{
				name = "ceiling_m",
			},
			{
				name = "floor_m",
			},
			{
				name = "metal_m",
			},
			{
				name = "cement_m",
			},
			{
				name = "soccerBallMaterial",
			},
			{
				name = "cylinderMaterial",
			},
			{
				name = "spriteMaterial_OrangeSkull",
			},
			{
				name = "spriteMaterial_GreenSkull",
			},
			{
				name = "spriteMaterial_OptionsButton",
			},
			{
				name = "spriteMaterial_MainMenuButton",
			},
			{
				name = "spriteMaterial_NumberSheet",
			},
			{
				name = "spriteMaterial_RedBar",
			},
			{
				name = "spriteMaterial_BlueBar",
			},
			{
				name = "spriteMaterial_Player1TurnLabel",
			},
			{
				name = "spriteMaterial_Player2TurnLabel",
			},
			{
				name = "Material_DebugLines",
			},
			{
				name = "Material_Cannon",
			},
			{
				name = "Material_LineRenderer",
			},
			{
				name = "materialList",
			},
			{
				name = "meshList",
			},

			{
				name = "sceneFile",
			},
			{
				name = "spriteInformation",
			},
			{
				name = "lightingInfo",
			},
		},
	},

    -- Collision Meshes
	{
		builder = "CollisionMeshBuilder.exe",
		extensions =
		{
			source = "txt",
			target = "txt",
		},
		assets =
		{
			{
				name = "CollisionMesh",
			},
        },   
	},
}
