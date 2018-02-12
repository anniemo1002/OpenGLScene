#version 330 core 
in vec3 fragNor;
out vec4 color;

void main()
{
        vec3 Ncolor;
	vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
        float dot_product = dot(normal,vec3(0,0,1));
        if (dot_product>0.4){
           discard;
         }
         else if(dot_product>=0){
	    Ncolor = vec3(0.3,0.3,1.0)*(1-dot_product);
	    color = vec4(Ncolor, 1.0);
         }
         else{
            Ncolor = (1+dot_product)*vec3(1.0,1.0,1.0);
            color = vec4(Ncolor, 1.0);
         }
	//vec3 Ncolor = 0.5*normal + 0.5;
	//color = vec4(Ncolor, 1.0);
}
