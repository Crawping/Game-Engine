
texture2D g_color_texture;

// Samplers
//=========

sampler2D g_color_sampler;
uniform float g_time_count;
// Entry Point
//============

void main(in const float3 i_color_perVertex : COLOR0, in const float2 i_uvs : TEXCOORD0,
	out float4 o_color : COLOR0)
{
	float3 color = i_color_perVertex;
	float alpha = 0.0f;

	/*if (i_uvs.x < 0.4)
		alpha = i_uvs.x;

	if (i_uvs.x > 0.6)
		alpha = 1.0 - i_uvs.x;
	if (i_uvs.x > 0.4 && i_uvs.x < 0.6)
	{
		alpha = 1.0;
		color = float3(1.0, 0.0, 0.0);
	}*/

	if (i_uvs.x <= 0.5)
	{
		alpha = i_uvs*1.0;
	}
	else
	{
		alpha = (1 - i_uvs.x)*1.0;
	}

	float time = g_time_count%1.0;
	if (i_uvs.y< (time + 0.2) && i_uvs.y>(time - 0.2))
		color = float3(0.0, 1.0, 1.0);
	else
	{
		color = float3(1.0, 1.0, 1.0);

	}

	if (time > 0.8)
	{
		float tempTime = 0.2 - (1.0 - time);
		if (i_uvs.y < (tempTime))
		{
			color = float3(0.0, 1.0, 1.0);
		}
	}
	if (time < 0.2)
	{
		float tempTime = 0.8 + time;
		if (i_uvs.y>(tempTime))
		{
			color = float3(0.0, 1.0, 1.0);
		}
	}
	
	
	o_color = float4(color, alpha );
}
