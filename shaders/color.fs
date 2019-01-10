#version 330 core

in vec3 Normal; 
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 ourColor;
uniform vec3 lightPos;

void main()
{
  float ambient = 0.1;
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  FragColor = vec4((ambient + diff) * ourColor, 1.0);    
}
