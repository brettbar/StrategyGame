#version 330

precision highp float;

in vec2 fragTexCoord;       // Fragment input attribute: texture coordinate
in vec4 fragColor;          // Fragment input attribute: color

//uniform sampler2D texture0; // Fragment input texture (always required, could be a white pixel)
uniform vec4 colDiffuse;    // Fragment input color diffuse (multiplied by texture color)
uniform sampler2D texture0;   // FS: GL_TEXTURE0
uniform sampler2D texture1;   // FS: GL_TEXTURE1
uniform sampler2D texture2;   // FS: GL_TEXTURE2

uniform float wt;
uniform float ht;

// Output fragment color
out vec4 finalColor;        // Fragment output: color

//https://www.youtube.com/watch?v=Yu8k7a1hQuU&t=4232s
void main()
{

  float u = fragTexCoord.x;
  float v = fragTexCoord.y;

  float tx = u * wt;
  float ty = v * ht;


  vec2 alpha = 0.7 * vec2(dFdx(tx), dFdy(ty));
  vec2 x = fract(vec2(tx, ty));
  vec2 x_ = clamp(0.5 / alpha * x, 0.0, 0.5) +
            clamp(0.5 / alpha * (x - 1.0) + 0.5, 0.0, 0.5);

  vec2 texCoord = (floor(vec2(tx, ty)) + x_) / vec2(wt, ht);
  finalColor = texture(texture0, texCoord)*colDiffuse*fragColor;
}





































































  // if (floor(xmin) == floor(xmax)) {
  //   // set u to be the floor
  //   u = floor(u * wt) / wt;
  // } 
  // else {
  //   float how_much_in_next = xmax - floor(xmax);
  //   // float how_much_in_prev = floor(xmax) - xmin;
  //   
  //   float blend = (floor(xmax) - xmin) / (xmax - xmin);

  //   float xs = floor(xmax) - (1 - blend);

  //   u = (xs + 0.5) / wt;
  // }

  // if (floor(ymin) == floor(ymax)) {
  //   // set v to be the floor
  //   v = floor(v * ht) / ht;
  // }
  // else {
  //   float how_much_in_next = ymax - floor(ymax);
  //   // float how_much_in_prev = floor(ymax) - ymin;
  //   
  //   float blend = (floor(ymax) - ymin) / (ymax - ymin);

  //   float ys = floor(ymax) - (1 - blend);

  //   v = (ys + 0.5) / ht;
  // }

  // const float wt = 128.0f;
  // const float ht = 128.0f;

  // // from casey:
  // // in pixel shader:
  // // if the nearest neighbor sample for a pixel on either side of the pixel
  // // would have landed in two different texels, then blend them, otherwise dont

  // // overview:
  // // 1. floor to the closest texel
  // // 2. find the offset uv = offset
  // // 3. compute blend coeffs
  // // 4. blend

  // // PRECONDITION YOUR UV COORDINATES IN THE PIXEL SHADER

  // float u = fragTexCoord.x;
  // float v = fragTexCoord.y;

  // // if pixel is entirely contained in one texel
  // // then floor myself, 
  // // adjust the u coordinate to land exactly
  // // at the texel center
  // // (texels + 0.5) * 1 / textureWidth
  //  
  // 
  // // if stepped one pixel forward/back, how much would my u value change
  // float xGrad = dFdx(u);
  // float yGrad = dFdy(v);

  // float umin = u - 0.5*xGrad - 0.5;
  // float umax = u + 0.5*xGrad - 0.5;

  // float vmin = v - 0.5*yGrad - 0.5;
  // float vmax = v + 0.5*yGrad - 0.5;

  // float xmin = umin * wt;
  // float xmax = umax * wt;
  // float ymin = vmin * ht;
  // float ymax = vmax * ht;

  // if (floor(xmin) == floor(xmax)) {
  //   xmin = 0;
  // } else {
  //   float how_much_in_next = xmax - floor(xmax);
  //   // float how_much_in_prev = floor(xmax) - xmin;
  //   
  //   float blend = (floor(xmax) - xmin) / (xmax - xmin);

  //   float xs = floor(xmax) - (1 - blend);

  //   u = (xs + 0.5) / wt;
  // }

  // if (floor(ymin) == floor(ymax)) {
  //   ymin = 0;
  // } else {
  //   float how_much_in_next = ymax - floor(ymax);
  //   // float how_much_in_prev = floor(ymax) - ymin;
  //   
  //   float blend = (floor(ymax) - ymin) / (ymax - ymin);

  //   float ys = floor(ymax) - (1 - blend);

  //   v = (ys + 0.5) / ht;
  // }

  // finalColor = texture(texture0, vec2(u,v))*colDiffuse*fragColor;
