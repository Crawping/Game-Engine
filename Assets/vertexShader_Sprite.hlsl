/*
	This is an example of a vertex shader
*/

// Entry Point
//============

void main( in const float3 i_position_sprite : POSITION,  in const float2 i_uvs:TEXCOORD0, 
	out float4 o_position_screen : POSITION, out float2 o_uvs:TEXCOORD0)
{

	//Output the uvs as they are
	{
		//Set the output uvs without alteration from the input uvs
		o_uvs = i_uvs;
	}
	//Output the position as it is
	{
		//Set the output uvs without alteration from the input uvs
		o_position_screen = float4(i_position_sprite,1.0);
	}
	
}
