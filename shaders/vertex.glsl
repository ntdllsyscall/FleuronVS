#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
  
out vec3 color; // output a color to the fragment shader

uniform mat4 rotation;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * rotation * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aColor; // set ourColor to the input color we got from the vertex data
}   