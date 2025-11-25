#version 330 core

out vec4 FragColor;

in vec2 TexCoords; // Esto debe coincidir con el 'out' del Vertex Shader

// Asegúrate que tu clase C++ (MeshAnim) use este nombre de textura
uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    // Si la textura se ve mal (ej. fondo blanco), descomenta esto:
    // if (texColor.a < 0.1)
    //     discard;

    FragColor = texColor;
}
