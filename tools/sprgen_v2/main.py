#!/usr/bin/env python

from PIL import Image, ImageDraw

archetypes_dir = './Archetypes/'


def main():

    # @future eventually just read the folder directly
    archetypes = ['1HSpearman']

    for archetype in archetypes:
        gen_archetype_gradient(archetype)


def gen_archetype_gradient(archetype):
    silhouette_layers_dir = archetypes_dir + archetype + '/' + '2_SilhouetteLayers/'

    # @future might replace this
    supported_layers= [
        'Cape',
        'Head',
        'LeftArm',
        'LeftEquip',
        'Legs',
        'RightArm',
        'RightEquip',
        'Torso',
    ]

    silhouettes = {}

    # get the silhouette layers into a dict
    for supported_layer in supported_layers:
        print(supported_layer)

        silhouettes[supported_layer] = Image.open(
	        silhouette_layers_dir + supported_layer + '.png'
	    ).convert('RGBA')

    sprite_w = 128
    sprite_h = 128
    start_x = 0
    end_x = sprite_w
    start_y = 0
    end_y = sprite_h

    # gen gradient for each individual layer of the silhouette
    for silhouette, img in silhouettes.items():
        silhouette_arr = gen_silhouette_arr(img, 0, sprite_w, 0, sprite_h)

        original_color = silhouette_arr[0]

        num_pixels = len(silhouette_arr)
        gradient = gen_gradient_arr(original_color, num_pixels)

        gradient_img = gen_gradient_img(img, gradient, start_x, end_x, start_y, end_y)
        gradient_layer_output = archetypes_dir + archetype + '/' + '3_GradientLayers/'
        gradient_img.save(gradient_layer_output + silhouette + '.png')

def gen_silhouette_arr(img, start_x, end_x, start_y, end_y):
    silhouette_arr = []
    for y in range(start_y, end_y):
        for x in range(start_x, end_x):
            pixel = img.getpixel((x, y))
            if pixel[3] == 255: 
                silhouette_arr.append(pixel)
    return silhouette_arr

def gen_gradient_arr(original_color, num_pixels):
    r, g, b, a = original_color

    # instead of interp with black, I use half the original color since
    # it makes the darker parts of each layer easier to distinguish from 
    # one another
    end_color = (r/2, g/2, b/2, a)
    r2, g2, b2, _ = end_color

    gradient = []

    for i in range(num_pixels):
        factor = (num_pixels - i - 1) / (num_pixels - 1)

        new_r = int(r + (r2 - r) * factor)
        new_g = int(g + (g2 - g) * factor)
        new_b = int(b + (b2 - b) * factor)

        gradient.append((new_r, new_g, new_b, a))

    return gradient


def gen_gradient_img(img, gradient, start_x, end_x, start_y, end_y):
    gradient_img = Image.new('RGBA', (end_x - start_x, end_y - start_y), (0, 0, 0, 0))
    index = 0
    for y in range(start_y, end_y):
        for x in range(start_x, end_x):
            pixel = img.getpixel((x, y))
            if pixel[3] == 255: 
                gradient_img.putpixel((x,y), gradient[index])
                index += 1
    return gradient_img


if __name__ == "__main__":
   main() 
