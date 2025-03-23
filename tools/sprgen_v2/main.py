#!/usr/bin/env python

from PIL import Image, ImageDraw
import math
import sys

ARCHETYPES_DIR = './Archetypes/'
SPRITE_W = 128
SPRITE_H = 128
SUPPORTED_LAYERS = [
    'Cape',
    'Head',
    'LeftArm',
    'LeftEquip',
    'RightLeg',
    'LeftLeg',
    'RightArm',
    'RightEquip',
    'Torso',
]


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
    gradient_animations_spritesheet_dir = ARCHETYPES_DIR + archetype + '/6_GradientAnimations.png'
    gradient_animations_spritesheet = Image.open(gradient_animations_spritesheet_dir)

    final_animations_spritesheet = Image.new('RGBA', gradient_animations_spritesheet.size, (0, 0, 0, 0))
    final_animations_spritesheet_dir = ARCHETYPES_DIR + archetype + '/' + sprite + '_Animations.png'


    for y in range(gradient_animations_spritesheet.size[1]):
        for x in range(gradient_animations_spritesheet.size[0]):
            gradient_pixel = gradient_animations_spritesheet.getpixel((x,y))
            if gradient_pixel[3] < 255: continue

            color_map = left_color_map
            row = math.floor(y / SPRITE_H)
            if row % 2 == 1: 
                color_map = right_color_map
                
            if gradient_pixel in color_map:
                final_animations_spritesheet.putpixel((x,y), color_map[gradient_pixel])

    final_animations_spritesheet.save(final_animations_spritesheet_dir)

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
                if gradient_pixel[3] > 255: continue
                
                # print(x, y)
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

        original_color = silhouette_arr[0]

        num_pixels = len(silhouette_arr)
        gradient = gen_gradient_arr(original_color, num_pixels)

        gradient_img = Image.new('RGBA', (SPRITE_W*2, SPRITE_H), (0, 0, 0, 0))
        gen_gradient_img(gradient_img, img, gradient, start_x, end_x, start_y, end_y)
        gradient_layer_output = ARCHETYPES_DIR + archetype + '/' + '4_GradientLayers/'
        # if not only_unique_gradient_pixels(gradient_img, start_x, end_x, start_y, end_y):
        #     sys.exit(1)
        gradient_img.save(gradient_layer_output + side + '_' + silhouette + '.png')





                
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
    print(color_dict)
    return True


def gen_silhouette_arr(img, start_x, end_x, start_y, end_y):
    silhouette_arr = []
    for y in range(start_y, end_y):
        for x in range(start_x, end_x):
            pixel = img.getpixel((x, y))
            if pixel[3] == 255: 
                silhouette_arr.append(pixel)
    return silhouette_arr

def lerp(a, b, t):
    return a + ((b - a) * t)


def gen_gradient_arr(original_color, num_pixels):
    r, g, b, a = original_color

    # instead of interp with black, I use half the original color since
    # it makes the darker parts of each layer easier to distinguish from 
    # one another
    end_color = ((0), (0), (0), a)
    # end_color = (math.sqrt(r), math.sqrt(g), math.sqrt(b), a)
    end_r, end_g, end_b, _ = end_color

    gradient = []

    for i in range(num_pixels):
        factor = (num_pixels - i - 1) / (num_pixels - 1)

        new_r = int(lerp(r, end_r, factor))
        new_g = int(lerp(g, end_g, factor))
        new_b = int(lerp(b, end_b, factor))

        gradient.append((new_r, new_g, new_b, a))

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
