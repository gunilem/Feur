#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexId;
layout(location = 4) in float a_tillingFactor;
				
uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexId;
out float v_tillingFactor;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexId = a_TexId;
	v_tillingFactor = a_tillingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}



#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;
				
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexId;
in float v_tillingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(v_TexId)], v_TexCoord * v_tillingFactor) * v_Color;
}