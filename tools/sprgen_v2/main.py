#!/usr/bin/env python

from PIL import Image, ImageDraw


def main():

    # @future eventually just read the folder directly
    archetypes = ['1HSpearman']

    for archetype in archetypes:
        gen_archetype_gradient(archetype)




def gen_archetype_gradient(archetype):
    sprite_w = 128
    sprite_h = 128
    archetypes_dir = './Archetypes/'
    silhouette_layers_dir = archetypes_dir + archetype + '/' + '2_SilhouetteLayers/'
    gradient_layer_output = archetypes_dir + archetype + '/' + '3_GradientLayers/'

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

    # gen gradient for each individual layer of the silhouette
    for silhouette, img in silhouettes.items():
        gradient_img = Image.new('RGBA', (sprite_w, sprite_h), (0, 0, 0, 0))
        silhouette_arr = []

        for y in range(sprite_h):
            for x in range(sprite_w):
                pixel = img.getpixel((x, y))
                if pixel[3] == 255: 
                    silhouette_arr.append(pixel)
        
        original_color = silhouette_arr[0]
        r, g, b, a = original_color

        # instead of interp with black, I use half the original color since
        # it makes the darker parts of each layer easier to distinguish from 
        # one another
        end_color = (r/2, g/2, b/2, a)
        r2, g2, b2, _ = end_color

        num_pixels = len(silhouette_arr)
        gradient = []

        for i in range(num_pixels):
            factor = (num_pixels - i - 1) / (num_pixels - 1)

            new_r = int(r + (r2 - r) * factor)
            new_g = int(g + (g2 - g) * factor)
            new_b = int(b + (b2 - b) * factor)

            gradient.append((new_r, new_g, new_b, a))


        index = 0
        for y in range(sprite_h):
            for x in range(sprite_w):
                pixel = img.getpixel((x, y))
                if pixel[3] == 255: 
                    gradient_img.putpixel((x,y), gradient[index])
                    index += 1

        gradient_img.save(gradient_layer_output + silhouette + '.png')



if __name__ == "__main__":
   main() 
