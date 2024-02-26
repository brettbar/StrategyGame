#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;


// NOTE: Add here your custom variables

// the sacred texts
void main()
{
  // retrieves the texture coordinates from the fragment shader
  vec2 uv = fragTexCoord;

  // determines the size of the texture in pixels
  vec2 size = vec2(textureSize(texture0, 0));

  // calculates the size of a single pixel in the texture
  vec2 pixel = vec2(1.0) / size;

  // adjusts the texture coords to point to the center of the pixel
  uv -= pixel * vec2(0.5);
  
  // converts the normalized texture coords to pixel coords
  vec2 uv_pixels = uv * size;

  // calculates the offset from the center of the pixel in pixel coords
  vec2 delta_pixel = fract(uv_pixels) - vec2(0.5);

  // computes the partial derivative of the texture coords with respect to screen space (for antialiasing)
  vec2 ddxy = fwidth(uv_pixels);

  // calculates the mip level based on the partial derivative
  // commonly used in mipmapping to select the appropriate level of detail
  vec2 mip = log2(ddxy) - 0.5;

  // performs texture sampling with mipmapping and antialiasing
  // the textureLod function is used to specify the level of detail (mip level)
  // the sampling coords are adjusted based on the antialiasing calculations
  vec4 texelColor = textureLod(texture0, uv + (clamp(delta_pixel / ddxy, 0.0, 1.0) - delta_pixel) * pixel, min(mip.x, mip.y));

  // multiplies the sampled color with a diffuse color and assigns the final color
  finalColor = texelColor*colDiffuse;
}


// the sacred texts
// void main()
// {
//   // retrieves the texture coordinates from the fragment shader
//   vec2 uv = fragTexCoord;

//   // determines the size of the texture in pixels
//   vec2 size = vec2(textureSize(texture0, 0));

//   // calculates the size of a single pixel in the texture
//   vec2 pixel = vec2(1.0) / size;

//   // adjusts the texture coords to point to the center of the pixel
//   uv -= pixel * vec2(0.5);
  
//   // converts the normalized texture coords to pixel coords
//   vec2 uv_pixels = uv * size;

//   // calculates the offset from the center of the pixel in pixel coords
//   vec2 delta_pixel = fract(uv_pixels) - vec2(0.5);

//   // computes the partial derivative of the texture coords with respect to screen space (for antialiasing)
//   vec2 ddxy = fwidth(uv_pixels);

//   // calculates the mip level based on the partial derivative
//   // commonly used in mipmapping to select the appropriate level of detail
//   vec2 mip = log2(ddxy) - 0.5;

//   // performs texture sampling with mipmapping and antialiasing
//   // the textureLod function is used to specify the level of detail (mip level)
//   // the sampling coords are adjusted based on the antialiasing calculations
//   vec4 texelColor = textureLod(texture0, uv + (clamp(delta_pixel / ddxy, 0.0, 1.0) - delta_pixel) * pixel, min(mip.x, mip.y));

//   // multiplies the sampled color with a diffuse color and assigns the final color
//   finalColor = texelColor*colDiffuse;
// }


// vec4 texturePointSmooth(sampler2D tex, vec2 uv) {
// 	vec2 size = vec2(textureSize(tex, 0));
// 	vec2 pixel = vec2(1.0) / size;
// 	uv -= pixel * vec2(0.5);
// 	vec2 uv_pixels = uv * size;
// 	vec2 delta_pixel = fract(uv_pixels) - vec2(0.5);
// 	vec2 ddxy = fwidth(uv_pixels);
// 	vec2 mip = log2(ddxy) - 0.5;
// 	return textureLod(tex, uv + (clamp(delta_pixel / ddxy, 0.0, 1.0) - delta_pixel) * pixel, min(mip.x, mip.y));
// }
