#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float outlineSize;
uniform vec4 outlineColor;

// Output fragment color
out vec4 finalColor;

void main()
{
	vec2 size = vec2(textureSize(texture0, 0));
  vec2 pixel = vec2(1.0) / size;
  vec4 sprite_color = texture(texture0, fragTexCoord);
  float alpha = -4.0 * sprite_color.a;

  alpha += texture(texture0, fragTexCoord + vec2(pixel.x, 0)).a;
  alpha += texture(texture0, fragTexCoord + vec2(-pixel.x, 0)).a;
  alpha += texture(texture0, fragTexCoord + vec2(0, pixel.y)).a;
  alpha += texture(texture0, fragTexCoord + vec2(0, -pixel.y)).a;

  vec4 pen = mix(sprite_color, outlineColor, clamp(alpha, 0.0, 1.0));
  vec4 newColor = vec4(pen.rgb, clamp(abs(alpha) + sprite_color.a, 0.0, 1.0));

  // If we set finalColor to newColor, the outline works for a fuzzy bilinear case
  // Now we must make this outline work with the pixel-perfect shader


  vec2 uv = fragTexCoord;
  uv -= pixel * vec2(0.5);

  vec2 uv_pixels = uv * size;
	vec2 delta_pixel = fract(uv_pixels) - vec2(0.5);
	vec2 ddxy = fwidth(uv_pixels);
	vec2 mip = log2(ddxy) - 0.5;


  vec4 texelColor = textureLod(texture0, uv + (clamp(delta_pixel / ddxy, 0.0, 1.0) - delta_pixel) * pixel, min(mip.x, mip.y));

  vec4 correctColor = texelColor*colDiffuse;


  // This is almost done, just figure out how to fix the fuzziness of the outline itself

  finalColor = mix(newColor, correctColor, correctColor.a);

}



//void main()
//{
//    vec4 texel = texture(texture0, fragTexCoord);   // Get texel color
//    vec2 texelScale = vec2(0.0);
//    texelScale.x = outlineSize/textureSize.x;
//    texelScale.y = outlineSize/textureSize.y;
//
//    // We sample four corner texels, but only for the alpha channel (this is for the outline)
//    vec4 corners = vec4(0.0);
//    corners.x = texture(texture0, fragTexCoord + vec2(texelScale.x, texelScale.y)).a;
//    corners.y = texture(texture0, fragTexCoord + vec2(texelScale.x, -texelScale.y)).a;
//    corners.z = texture(texture0, fragTexCoord + vec2(-texelScale.x, texelScale.y)).a;
//    corners.w = texture(texture0, fragTexCoord + vec2(-texelScale.x, -texelScale.y)).a;
//
//    float outline = min(dot(corners, vec4(1.0)), 1.0);
//    vec4 color = mix(vec4(0.0), outlineColor, outline);
//    finalColor = mix(color, texel, texel.a);
//}
