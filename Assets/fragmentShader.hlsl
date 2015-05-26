/*
	This is an example of a fragment shader
*/

// Constants
//==========

// Per-Material
//-------------

uniform float3  g_colorModifier= { 1.0, 1.0, 1.0 };

// Ambient Light Value
uniform float3 g_light_ambient = { 0.0 , 0.0 , 0.0 };

// Light Source Value
uniform float3 g_light_direction = {0.0, -1.0, 0.0};
uniform float3 g_light_color = { 1.0, 1.0, 1.0 };
// Textures
//=========

// NOTE: These are actually not necessary to declare in Direct3D 9.
// I debated whether it would be more confusing to include the declarations or to leave them out,
// and finally decided it would be best to show them even though you're unnecessary.
// It is up to you to decide how to represent textures and samplers in your material file format.

texture2D g_color_texture;

// Samplers
//=========

sampler2D g_color_sampler;

// Entry Point
//============

void main( in const float2 i_uv : TEXCOORD0, in const float3 i_color_perVertex : COLOR0, in const float3 i_world_normals:NORMAL,
	out float4 o_color : COLOR0 )
{
	float3 normal_world = normalize( i_world_normals );
	// "Sample" the texture to get the color at the given texture coordinates
	float3 color_sample = tex2D( g_color_sampler, i_uv ).rgb;
	// The output color is the texture color
	// modified by the interpolated per-vertex color
	// and the per-material constant
	float diffuseAmount = dot( i_world_normals, -g_light_direction );
	diffuseAmount = saturate( diffuseAmount );
	float3 diffuseLighting= diffuseAmount * g_light_color;
	o_color = float4(color_sample * g_colorModifier * i_color_perVertex*(g_light_ambient + diffuseLighting),
		// For now the A value should _always_ be 1.0
		1.0 );
}
