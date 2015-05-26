/*
	This is an example of a vertex shader
*/

// Constants
//==========

// Per-View
//---------

uniform float4x4 g_transform_worldToView;
uniform float4x4 g_transform_viewToScreen;
// Per-Instance
//-------------

uniform float4x4 g_transform_modelToWorld;

// Entry Point
//============

void main( in const float3 i_position_model : POSITION, in const float3 i_color : COLOR0, in const float2 i_uvs:TEXCOORD0, in const float3 i_normal_model:NORMAL,
	out float4 o_position_screen : POSITION, out float4 o_color : COLOR0, out float2 o_uvs : TEXCOORD0, out float3 o_normal_world : NORMAL)
{
	float alpha = 0.0;
	// Calculate position
	{
		// The position stored in the vertex is in "model space",
		// meaning that it is relative to the center (or "origin", or "pivot")
		// of the model.
		// The graphics hardware needs the position of the vertex
		// in normalized device coordinates,
		// meaning where the position of the vertex should be drawn
		// on the screen.
		// This position that we need to output, then,
		// is the result of taking the original vertex in "model space"
		// and transforming it into "screen space".

		// Any matrix transformations that include translation
		// will operate on a float4 position,
		// which _must_ have 1 for the w value	
		if (i_position_model.x == 0.0f && i_position_model.y == 0.0f && i_position_model.z == 0.0f)
			alpha = 1.0;
		float4 position_world = mul( float4( i_position_model, 1.0 ), g_transform_modelToWorld );
		float4 position_view = mul( position_world, g_transform_worldToView );
		o_position_screen = mul( position_view, g_transform_viewToScreen );
	}

	// Calculate color
	{
		// Set the output color directly from the input color:
		o_color = float4(i_color,alpha);
	}

	//Calculate the uvs
	{
		//Set the output uvs without alteration from the input uvs
		o_uvs = i_uvs;
	}

	//Calculate normals in world space
	{
	 float3x3 g_rotation_modelToWorld =
	 {
	 g_transform_modelToWorld[0].xyz,
	 g_transform_modelToWorld[1].xyz,
	 g_transform_modelToWorld[2].xyz
	 };
	 o_normal_world = mul( i_normal_model, g_rotation_modelToWorld );
	}
}
