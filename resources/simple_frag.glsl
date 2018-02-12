#version 330 core 
in vec3 fragNor;
out vec4 color;
uniform vec3 Color;
void main()
{
   if (Color.x==1.0f && Color.y==1.0f && Color.z==1.0f ){
	vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	vec3 Ncolor = 0.5*normal + 0.5;
	color = vec4(Ncolor, 1.0);
   }
   else{
      color = vec4(Color,1.0);
   }
}
