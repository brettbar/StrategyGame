#version 330

precision highp float;

in vec2 fragTexCoord;       // Fragment input attribute: texture coordinate
in vec4 fragColor;          // Fragment input attribute: color

in vec2 vUv;

//uniform sampler2D texture0; // Fragment input texture (always required, could be a white pixel)
uniform vec4 colDiffuse;    // Fragment input color diffuse (multiplied by texture color)
uniform sampler2D texture0;   // FS: GL_TEXTURE0
uniform sampler2D texture1;   // FS: GL_TEXTURE1
uniform sampler2D texture2;   // FS: GL_TEXTURE2

uniform float uZoom;

// Output fragment color
out vec4 finalColor;        // Fragment output: color

//https://www.youtube.com/watch?v=Yu8k7a1hQuU&t=4232s
void main()
{
    float normZoom = (1 - uZoom) / 1.0;
    float pixelSize = 1.00;
    float zoom = 8.00;
//     float scale = pixelSize + ((cos((time + 8.0) / 3.7) + 1.0) / 2.0) * (zoom - 1.0) * pixelSize;
    const float w = 128.0;
    const float h = 128.0;

    vec2 alpha = 0.8 * vec2(dFdx(vUv.x), dFdy(vUv.y));

    vec2 x = fract(vUv) + 0.5;
    vec2 x_ = clamp(0.5 / alpha * x, 0.0, 0.5) + clamp(0.5 /alpha * (x - 1.0) + 0.5, 0.0, 0.5);

    vec2 texCoord = (floor(vUv) + x_) / vec2(w, h);
    finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;



















    // from casey:
    // in pixel shader:
    // if the nearest neighbor sample for a pixel on either side of the pixel
    // would have landed in two different texels, then blend them, otherwise dont

    // overview:
    // PRECONDITION YOUR UV COORDINATES
    // 1. floor to the closest texel
    // 2. find the offset uv = offset
    // 3. compute blend coeffs
    // 4. blend

 // t -> floored offset in texture space


// where does this pixel fall in texture space


// if pixel is entirely contained in one texel, then floor it myself
// aka adjust my u coordinate to fall exactly at the texel center
// (texels + 0.5) * 1 / textureWidth
// else
//

//     vec2 uv = floor(vUv) + 0.5;
//     uv += 1.0 - clamp((1.0 - fract(vUv)) * scale, 0.0, 1.0);

//     float u = fragTexCoord.x;
//     float v = fragTexCoord.y;
//
//     // if stepped one pixel forward/back, how much would my u value change
//     float xGrad = dFdx(u);
//     float yGrad = dFdy(v);
//
//     float umin = u - 0.5*xGrad - 0.5;
//     float umax = u + 0.5*xGrad - 0.5;
//     float vmin = v - 0.5*yGrad - 0.5;
//     float vmax = v + 0.5*yGrad - 0.5;
//
//     float xmin = umin * wt;
//     float xmax = umax * wt;
//     float ymin = vmin * ht;
//     float ymax = vmax * ht;
//
//     float fXmax = floor(xmax);
//     float fYmax = floor(ymax);
//
//     if (floor(xmin) == fXmax) {
//         xmin = 0;
// //         u = floor(u * wt)/wt;
//     }
//
//
//     if (floor(ymin) == fYmax) {
//         ymin = 0;
// //         v = floor(v * ht)/ht;
//     }
//
//     u = fXmax - 1 + (fXmax) / xGrad;
//
//     v = fYmax - 1 + (fYmax) / yGrad;
//
//
//     u = floor(u * wt)/wt;
//     v = floor(v * ht)/ht;
//
//     // (1 - t)C0 + tC1
//
//     finalColor = texture(texture0, vec2(u, v));
//


//     vec2 uv = floor(vUv) + 0.5;
//     uv += 1.0 - clamp((1.0 - fract(vUv)) * scale, 0.0, 1.0);
//
//     vec2 texCoord = uv / vec2(w, h);
//     finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;


//     vec2 texCoord = (floor(vUv) + x_) / vec2(w, h);
//     finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;
}