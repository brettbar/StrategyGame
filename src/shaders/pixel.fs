#version 330

in vec2 fragTexCoord;       // Fragment input attribute: texture coordinate
in vec4 fragColor;          // Fragment input attribute: color

varying vec2 vUv;

//uniform sampler2D texture0; // Fragment input texture (always required, could be a white pixel)
uniform vec4 colDiffuse;    // Fragment input color diffuse (multiplied by texture color)
uniform sampler2D texture0;   // FS: GL_TEXTURE0
uniform sampler2D texture1;   // FS: GL_TEXTURE1
uniform sampler2D texture2;   // FS: GL_TEXTURE2

// Output fragment color
out vec4 finalColor;        // Fragment output: color

void main()
{
    const float pixelSize = 4.00;
    const float zoom = 8.00;
    float scale = pixelSize + (zoom - 1.0) * pixelSize;
    const float w = 128.0;
    const float h = 128.0;
//     vec2 locationWithinTexel = fract(vUv);
//     vec2 interpolationAmount = clamp(locationWithinTexel / texelsPerPixel, 0, 0.5)
//         + clamp((locationWithinTexel - 1) / texelsPerPixel + 0.5, 0, 0.5):
//
//     vec2 finalTextureCoords = (floor(vUv) + interpolationAmount) / vec2(w, h);
//
//     vec4 texelColor = texture(texture0, finalTextureCoords);

  // I chose this alpha because it looked nice in my demo
//   vec2 alpha = vec2(0.07);
//   vec2 x = fract(vUv);

  vec2 uv = floor(vUv) + 0.5;
//   vec2 x_ = clamp(0.5 / alpha * x, 0.0, 0.5) +
//             clamp(0.5 / alpha * (x - 1.0) + 0.5, 0.0, 0.5);
 uv += 1.0 - clamp((1.0 - fract(vUv)), 0.0, 1.0);

    vec2 texCoord = uv / vec2(w, h);
    finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;
//     vec2 texCoord = (floor(vUv) + x_) / vec2(w, h);
//     finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;

//     finalColor = texelColor*colDiffuse*fragColor;
}