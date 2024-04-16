#!/usr/bin/env python

from PIL import Image

import colorsys


'''
## Definitions
- Archetype
	- Category of character, what sort of animations it can have
	- Ex: unarmed male, 1h spearman, spear cavalry
	- Ie: what kind of character?
- Character Reference
	- The still image of an actual character, usually including a left right view and separated by layers
	- Ex: roman_hastati, celtic_villager
	- Ie: the actual hand-drawn character in 2 or 4 tiles
- Archetype Silhouette
	- The still image of the greatest-extent flatcolor silhouette for an archetype, usually including a left right view and separated by layers
	- Ex: 1h_swordsman
	- Ie: the maximal silhouette of an archetype, big blobs with each limb/layer being a different flat color
- Archetype Reference
	- The archetype silhouette but processed so that each pixel on each layer is a unique color that can be used to map to the character reference during sprite generation
- Animation Reference
	- The archetype reference mapped into various animations, all of which belong to that given archetype
	- Ie: a bunch of animation frames made using the many pixel colored blobs
- Final Animations
	- The final animations for the character

## Process
1. Archetype Silhouette -> Archetype Reference -> Animations Reference (Once per new Archetype and/or new Animation)
2. Animations Reference + (Archetype Reference + Character Reference) -> Final Animations
'''
# Windows
# ASSET_PATH = "C:\\Users\\brett\\Nextcloud\\projects\\gameart\\Aseprite\\Units\\Generator\\"
GENERATOR_ROOT = "C:\\Projects\\game\\tools\\sprgen\\V2\\"

# Linux
# ASSET_PATH = "/home/brettbar/projects/gameart/Aseprite/Units/Generator/"

def main():
    ref = Image.open(GENERATOR_ROOT + 'Reference.png').convert('HSV')
    pass


def hue_in_degrees(hue):
    convert = int(hue / 255 * 360)
    if convert % 2 == 1: convert += 1
    return convert


if __name__ == "__main__":
   main() 


# def pain():
#     ref = Image.open(GENERATOR_ROOT + 'Reference.png').convert('HSV')

#     layer_hue_map = {
#         270: 'right_arm', 
#         180: 'right_equip', 
#         300: 'left_equip', 
#         0: 'head', 
#         60: 'torso', 
#         30: 'left_arm', 
#         120: 'right_leg', 
#         240: 'left_leg'
#     }

#     layer_hue_map_reverse = {
#         'right_arm': 270, 
#         'right_equip': 180, 
#         'left_equip': 300, 
#         'head': 0, 
#         'torso': 60, 
#         'left_arm': 30, 
#         'right_leg': 120, 
#         'left_leg': 240,
#     }

#     def hue_in_degrees(hue):
#         convert = int(hue / 255 * 360)
#         if convert % 2 == 1: convert += 1
#         return convert

#     layers = {
#         'right_arm': [],
#         'right_equip': [],
#         'left_equip': [],
#         'head': [],
#         'torso': [],
#         'left_arm': [],
#         'right_leg': [],
#         'left_leg': [],
#     }

#     layer_dims = {
#         'right_arm': (32, 32),
#         'right_equip': (32, 96),
#         'left_equip': (32, 64),
#         'head': (32, 32),
#         'torso': (32, 32),
#         'left_arm': (32, 32),
#         'right_leg': (32, 32),
#         'left_leg': (32, 32),
#     }

#     layer_color_bank = {
#         'right_arm': [],
#         'right_equip': [],
#         'left_equip': [],
#         'head': [],
#         'torso': [],
#         'left_arm': [],
#         'right_leg': [],
#         'left_leg': [],
#     }
  

#     # First, get all non-black pixels into the layer's array
#     # This is basically a 1d array of all of that layer's pixels
#     for x in range(0, ref.width):
#         for y in range(0, ref.height):
#             (h,s,v) = ref.getpixel((x, y))
#             if v <= 0: continue
#             layer = layer_hue_map[hue_in_degrees(h)]
#             layers[layer].append((x, y, h, s, v))

#     # next since we have our layers, we are going to define the gradient grid of hsv values
#     # that that layer can use. This is based off of the constant hue with varying sat and val
#     for layer in layers:
#         (layer_w, layer_h) = layer_dims[layer]

#         for s in range(254, (254 - layer_w), -2):
#             for v in range(254, (254 - layer_h), -2):
#                 layer_color_bank[layer].append((layer_hue_map_reverse[layer], s, v))
            

#     new_image = Image.new('HSV', ref.size)

#     for layer, pixels in layers.items():
#         print(layer)

#         colors = layer_color_bank[layer]

#         for i, pixel in enumerate(pixels):
#             (x,y,h,s,v) = pixel

#             new_image.putpixel((x, y), colors[i])
                
#     new_image = new_image.convert('RGBA')

#     check_unique = dict()

#     for x in range(0, new_image.width):
#         for y in range(0, new_image.height):
#             (r, g, b, a) = new_image.getpixel((x,y))

#             if a <= 0 or (r + g + b) == 0:
#                 new_image.putpixel((x, y), (0,0,0,0))
#                 continue

#             if ((r,g,b) in check_unique):
#                 print("Error duplicate pixel found", (r,g,b))
#                 # return

#             check_unique[(r,g,b)] = 1

            

#     new_image.save(GENERATOR_ROOT + 'Silhouette.png')


