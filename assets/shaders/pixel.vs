#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;


// Input uniform values
uniform mat4 mvp;             // VS: ModelViewProjection matrix

uniform mat4 matView;         // VS: View matrix
uniform mat4 matProjection;   // VS: Projection matrix
uniform mat4 matModel;        // VS: Model matrix
uniform mat4 matNormal;       // VS: Normal matrix

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;

// NOTE: Add here your custom variables
out vec2 vUv;

void main()
{
    const float w = 128.0;
    const float h = 128.0;

    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // Calculate final vertex position
    vUv = vertexTexCoord * vec2(w, h);
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}