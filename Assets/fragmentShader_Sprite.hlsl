
// Textures
//=========
texture2D g_color_texture;

// Samplers
//=========

uniform float3 g_colorModifier = { 1.0, 1.0, 1.0 };
sampler2D g_color_sampler;

// Entry Point
//============

void main( in const float2 i_uv : TEXCOORD0,
	out float4 o_color : COLOR0 )
{
	// "Sample" the texture to get the color at the given texture coordinates
	float4 color_sample = tex2D(g_color_sampler, i_uv).rgba;
		// The output color is the texture color
		// modified by the interpolated per-vertex color
		// and the per-material constant
		o_color = color_sample*float4(g_colorModifier,1.0);
}
