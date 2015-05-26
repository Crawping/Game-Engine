
// Constants
//==========
// Ambient Light Value
uniform float3 g_light_ambient = { 0.0, 0.0, 0.0 };

// Light Source Value
uniform float3 g_light_direction = { 0.0, -1.0, 0.0 };
uniform float3 g_light_color = { 1.0, 1.0, 1.0 };
// Per-Material
//-------------

uniform float g_time_count;


sampler2D g_color_sampler;

uniform float3 g_colorModifier = { 1.0, 1.0, 1.0 };


void main(in const float4 i_color_perVertex : COLOR0, in const float2 i_uv : TEXCOORD0, in const float3 i_world_normals : NORMAL,
	out float4 o_color : COLOR0)
{

	float3 normal_world = normalize(i_world_normals);
	// "Sample" the texture to get the color at the given texture coordinates
	float3 color_sample = tex2D(g_color_sampler, i_uv).rgb;
	// The output color is the texture color
	// modified by the interpolated per-vertex color
	// and the per-material constant
	float diffuseAmount = dot(i_world_normals, -g_light_direction);
	diffuseAmount = saturate(diffuseAmount);
	float3 diffuseLighting = diffuseAmount * g_light_color;
	float range = i_color_perVertex.a % 0.2;
	if (range<0.05 || range>0.15)
		o_color = float4(color_sample * g_colorModifier * i_color_perVertex*(g_light_ambient + diffuseLighting), 0.0);
	else
		o_color = float4(color_sample * g_colorModifier * i_color_perVertex*(g_light_ambient + diffuseLighting), i_color_perVertex.a* cos(g_time_count)+0.2);
}
