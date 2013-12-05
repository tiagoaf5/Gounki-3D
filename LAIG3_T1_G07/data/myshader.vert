uniform sampler2D heightmap;
uniform float scale;
uniform float time;

void main() {
			
	// Set the position of the current vertex 
	float r = texture2D(heightmap,vec2(gl_MultiTexCoord0.s, gl_MultiTexCoord0.t + time*0.2)).r;
	vec4 offset = vec4(0.0,0.0,0.0,0.0);

	offset.y = r*scale;
	offset.z = r*scale/15; 
	vec4 relative_pos = offset + gl_Vertex;
	
	gl_Position = gl_ModelViewProjectionMatrix * relative_pos;
	
	//pass texture coordinates from VS to FS.
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.s, gl_MultiTexCoord0.t + time*0.2,0,0);
}