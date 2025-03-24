# @leftoff Need to make the equip gradient colors consistent with what objects they reprent
# ie, the spear should always be purple and the shield always blue regardless ofthe direction

#!/usr/bin/env python

from PIL import Image, ImageDraw
import math
import sys

ARCHETYPES_DIR = './Archetypes/'
SPRITE_W = 128
SPRITE_H = 128
SUPPORTED_LAYERS = [
    'Cape',
    'LeftLeg',
    'RightLeg',
    'LeftArm',
    'Torso',
    'Head',
    'LeftEquip',
    'RightArm',
    'RightEquip',
]

LAYER_HUES_DEGREES = {
    'Cape': 160,
    'LeftLeg': 240,
    'RightLeg': 120,
    'LeftArm': 30,
    'Torso': 60,
    'Head': 0,
    'LeftEquip': 300,
    'RightArm': 270,
    'RightEquip': 190,
}

LAYER_MAX_DIMS = {
    'Cape': (32, 48),
    'LeftLeg': (32, 32),
    'RightLeg': (32, 32),
    'LeftArm': (16, 32),
    'Torso': (32, 32),
    'Head': (16, 32),
    'LeftEquip': (16, 32),
    'RightArm': (16, 32),
    'RightEquip': (16, 32),
}

def main():

    # @future eventually just read the folder directly
    archetypes = {
        '1HSpearman': [
            'Greek_Spartan_Perioikoi',
        ],
    }

    for archetype, sprites in archetypes.items():
        gen_archetype_gradient(archetype)
        gen_archetype_animations(archetype, sprites)


def gen_archetype_animations(archetype, sprites):

    sprites_dir = ARCHETYPES_DIR + archetype + '/7_Sprites/'
    gradient_layer_dir = ARCHETYPES_DIR + archetype + '/' + '4_GradientLayers/'


    for sprite in sprites:
        sprite_dir = sprites_dir + sprite

        right_color_map = build_color_map('right', gradient_layer_dir, sprite_dir, 0, 0)
        left_color_map = build_color_map('left', gradient_layer_dir, sprite_dir, SPRITE_W, 0)
        gen_animations(archetype, sprite, left_color_map, right_color_map)


def gen_animations(archetype, sprite, left_color_map, right_color_map):
    gradient_animations_dir= ARCHETYPES_DIR + archetype + '/6_GradientAnimations/'
    dims = Image.open(gradient_animations_dir + 'Cape.png').size

    final_animations_spritesheet = Image.new('RGBA', dims, (0, 0, 0, 0))

    for layer in SUPPORTED_LAYERS:
        apply_layer(layer, gradient_animations_dir, left_color_map, right_color_map, final_animations_spritesheet)

    final_animations_spritesheet_dir = ARCHETYPES_DIR + archetype + '/' + sprite + '_Animations.png'
    final_animations_spritesheet.save(final_animations_spritesheet_dir)

def apply_layer(layer, gradient_animations_dir, left_color_map, right_color_map, final_animations_spritesheet):
    gradient_layer = gradient_animations_dir + layer + '.png'
    gradient_layer_img = Image.open(gradient_layer)


    for y in range(gradient_layer_img.size[1]):
        for x in range(gradient_layer_img.size[0]):
            gradient_pixel = gradient_layer_img.getpixel((x,y))
            if gradient_pixel[3] < 255: continue

            color_map = left_color_map
            row = math.floor(y / SPRITE_H)
            if row % 2 == 0: 
                color_map = right_color_map
                
            if gradient_pixel in color_map:
                final_animations_spritesheet.putpixel((x,y), color_map[gradient_pixel])


def build_color_map(side, gradient_layer_dir, sprite_dir, start_x, start_y):
    color_map = {}
    for layer in SUPPORTED_LAYERS:
        gradient_layer = gradient_layer_dir + side + '_' + layer + '.png'
        sprite_layer = sprite_dir + '/SpriteLayers/' + layer + '.png'

        gradient_layer_img = Image.open(gradient_layer)
        sprite_layer_img = Image.open(sprite_layer)

        grad_size_x, grad_size_y = gradient_layer_img.size


        # maps a pixel in the sprite to a unique pixel in the gradient
        for y in range(start_y, grad_size_y):
            for x in range(start_x, grad_size_x):
                gradient_pixel = gradient_layer_img.getpixel((x,y))
                if gradient_pixel[3] < 255: continue
                
                sprite_pixel = sprite_layer_img.getpixel((x, y))
                if sprite_pixel[3] < 255: continue
                
                color_map[gradient_pixel] = sprite_pixel

    return color_map 

                

def gen_archetype_gradient(archetype):
    silhouette_layers_dir = ARCHETYPES_DIR + archetype + '/' + '3_SilhouetteLayers/'

    # @future might replace this

    silhouettes = {}

    # get the silhouette layers into a dict
    for supported_layer in SUPPORTED_LAYERS:
        silhouettes[supported_layer] = Image.open(
	        silhouette_layers_dir + supported_layer + '.png'
	    ).convert('RGBA')

    start_x = 0
    end_x = SPRITE_W
    start_y = 0
    end_y = SPRITE_H


    # right
    generate_gradient_side("right", archetype, silhouettes, start_x=0, end_x=SPRITE_W, start_y=0, end_y=SPRITE_H)

    # left
    generate_gradient_side("left", archetype, silhouettes, start_x=SPRITE_W+1, end_x=SPRITE_W*2, start_y=0, end_y=SPRITE_H)


def generate_gradient_side(side, archetype, silhouettes, start_x, end_x, start_y, end_y):
    # gen gradient for each individual layer of the silhouette
    for silhouette, img in silhouettes.items():
        silhouette_arr = gen_silhouette_arr(img, start_x, end_x, start_y, end_y)

        gradient = gen_gradient_arr(silhouette)

        gradient_img = Image.new('HSV', (SPRITE_W*2, SPRITE_H), (0, 0, 0))
        gen_gradient_img(gradient_img, img, gradient, start_x, end_x, start_y, end_y)

        gradient_img = convert_hsv_to_rgba(gradient_img)

        if not only_unique_gradient_pixels(gradient_img, start_x, end_x, start_y, end_y):
            sys.exit(1)

        gradient_layer_output = ARCHETYPES_DIR + archetype + '/' + '4_GradientLayers/'
        gradient_img.save(gradient_layer_output + side + '_' + silhouette + '.png')



def convert_hsv_to_rgba(hsv_image):
    rgb_image = hsv_image.convert("RGB")

    rgba_image = Image.new("RGBA", rgb_image.size)

    for y in range(rgb_image.height):
        for x in range(rgb_image.width):
            hsv_pixel = hsv_image.getpixel((x,y))
            r,g,b = rgb_image.getpixel((x,y))

            if hsv_pixel == (0,0,0):
                rgba_image.putpixel((x,y), (0,0,0,0))
            else:
                rgba_image.putpixel((x,y), (r,g,b,255))

    return rgba_image

                
def only_unique_gradient_pixels(gradient_img, start_x, end_x, start_y, end_y):
    color_dict = {}
    for y in range(start_y, end_y):
        for x in range(start_x, end_x):
            color = gradient_img.getpixel((x,y))

            # dont care about transparent pixels
            if color[3] < 255: continue

            if color in color_dict:
                print('DUPLICATE GRADIENT PIXEL IDENTIFIED: ', color)
                return False
            else:
                color_dict[color] = 1
    return True


def gen_silhouette_arr(img, start_x, end_x, start_y, end_y):
    silhouette_arr = []
    for y in range(start_y, end_y):
        for x in range(start_x, end_x):
            pixel = img.getpixel((x, y))
            if pixel[3] == 255: 
                silhouette_arr.append(pixel)
    return silhouette_arr


def gen_gradient_arr(layer):
    gradient = []

    max_dimensions = LAYER_MAX_DIMS[layer]
    max_x, max_y = max_dimensions

    saturation_differential = 100 // max_x
    value_differential = 100 // max_y


    h_degrees = LAYER_HUES_DEGREES[layer]
    h_factor = h_degrees / 360
    h = int(255 * h_factor)

    for y in range(max_y):
        for x in range(max_x):
            s = 255 - (x * saturation_differential)
            v = 255 - (y * value_differential)
            gradient.append((h, s, v))

    return gradient

def gen_gradient_img(gradient_img, img, gradient, start_x, end_x, start_y, end_y):
    index = 0

    for y in range(start_y, end_y):
        for x in range(start_x, end_x):
            pixel = img.getpixel((x, y))
            if pixel[3] == 255: 
                gradient_img.putpixel((x,y), gradient[index])
                index += 1


if __name__ == "__main__":
   main() 
