#version 330

in vec2 fragTexCoord;       // Fragment input attribute: texture coordinate
in vec4 fragColor;          // Fragment input attribute: color

varying vec2 vUv;

//uniform sampler2D texture0; // Fragment input texture (always required, could be a white pixel)
uniform vec4 colDiffuse;    // Fragment input color diffuse (multiplied by texture color)
uniform sampler2D texture0;   // FS: GL_TEXTURE0
uniform sampler2D texture1;   // FS: GL_TEXTURE1
uniform sampler2D texture2;   // FS: GL_TEXTURE2

uniform float uZoom;

// Output fragment color
out vec4 finalColor;        // Fragment output: color

void main()
{
    const float pixelSize = 4.00;
    float foo = 0.5;
    float scale = pixelSize + foo * (uZoom - 1.0) * pixelSize;
    const float w = 128.0;
    const float h = 128.0;

    vec2 uv = floor(vUv) + 0.5;
    uv += 1.0 - clamp((1.0 - fract(vUv)) * scale, 0.0, 1.0);

    vec2 texCoord = uv / vec2(w, h);
    finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;


//     vec2 texCoord = (floor(vUv) + x_) / vec2(w, h);
//     finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;
}