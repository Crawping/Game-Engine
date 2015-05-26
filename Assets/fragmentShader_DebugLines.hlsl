
texture2D g_color_texture;

// Samplers
//=========

sampler2D g_color_sampler;

// Entry Point
//============

void main(in const float3 i_color_perVertex : COLOR0,
	out float4 o_color : COLOR0 )
{
	o_color = float4( i_color_perVertex,1.0);
}
