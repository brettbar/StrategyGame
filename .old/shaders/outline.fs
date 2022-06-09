#version 330

uniform vec2 sprite_size;
uniform vec4 outline_color;
uniform bool include_corners;
uniform bool enabled;

float texelSizeX = 1.0 / sprite_size.x;
float texelSizeY = 1.0 / sprite_size.y;

vec4 effect( vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords )
{
	vec4 c = texture2D(texture, texture_coords);

    if(!enabled) {
        return c;
    }
    
    float weight = 
        texture2D(texture, vec2(texture_coords.x + texelSizeX  , texture_coords.y)).a *
        texture2D(texture, vec2(texture_coords.x               , texture_coords.y - texelSizeY)).a *
        texture2D(texture, vec2(texture_coords.x - texelSizeX  , texture_coords.y)).a *
        texture2D(texture, vec2(texture_coords.x               , texture_coords.y + texelSizeY)).a;

    if(include_corners) {
        weight *=
            texture2D(texture, vec2(texture_coords.x + texelSizeX  , texture_coords.y - texelSizeY)).a *
            texture2D(texture, vec2(texture_coords.x + texelSizeX  , texture_coords.y + texelSizeY)).a *
            texture2D(texture, vec2(texture_coords.x - texelSizeX  , texture_coords.y - texelSizeY)).a *
            texture2D(texture, vec2(texture_coords.x - texelSizeX  , texture_coords.y + texelSizeY)).a;
    }
    
    return lerp(outline_color, c, ceil(weight));
}
