#!/usr/bin/env python

from PIL import Image

import math
import colorsys


# Windows
# ASSET_PATH = "C:\\Users\\brett\\Nextcloud\\projects\\gameart\\Aseprite\\Units\\Generator\\"
GENERATOR_ROOT = "C:\\Projects\\game\\tools\\sprgen\\V2\\"

# Linux
# ASSET_PATH = "/home/brettbar/projects/gameart/Aseprite/Units/Generator/"

def main():
	archetypes = ['1HSpearman']

	prefix = 'Archetypes\\'

	for archetype in archetypes:
		gen_archetype_map(prefix, archetype)
		

def gen_archetype_map(prefix, archetype):
		path = GENERATOR_ROOT + prefix + archetype + "\\output\\"
		silhouettes = {
			'Legs': Image.open(path + 'Silhouette-Legs.png').convert('RGBA'),
			'Torso': Image.open( path+ 'Silhouette-Torso.png').convert('RGBA'),
			'LeftArm': Image.open(path + 'Silhouette-LeftArm.png').convert('RGBA'),
			'Head': Image.open(path+ 'Silhouette-Head.png').convert('RGBA'),
			# 'LeftEquip': Image.open(path+ 'Silhouette-LeftEquip.png').convert('RGBA'),
			# 'RightEquip': Image.open(path+ 'Silhouette-RightEquip.png').convert('RGBA'),
			'RightArm': Image.open(path + 'Silhouette-RightArm.png').convert('RGBA'),
		}

		
		# archetype_map = Image.new('RGBA', [256, 128])
		hsv_map = Image.new('HSV', [256, 128])
		rgba_map = Image.new('RGBA', [256, 128])
		for body_part, silhouette in silhouettes.items():
			width = 128
			height = 128
			hsv_silh = silhouette.convert('HSV')
			rgba_silh = silhouette.convert('RGBA')

			opaque_pixels = [[]]
			for y in range(height):
				new_row = []
				for x in range(width):
					h,s,v = hsv_silh.getpixel((x,y))
					if v == 0: continue 
					new_row.append((x,y))
				if len(new_row) > 0:
					opaque_pixels.append(new_row)

			hue = 0

			if body_part == 'Legs':
				hue = 120
			elif body_part == 'Torso':
				hue = 60
			elif body_part == 'LeftArm':
				hue = 30
			elif body_part == 'Head':
				hue = 0
			elif body_part == 'LeftEquip':
				hue = 0
			elif body_part == 'RightEquip':
				hue = 0
			elif body_part == 'RightArm':
				hue = 200


			max_row_width = len(opaque_pixels[0])
			for j in range(len(opaque_pixels)):
				w = len(opaque_pixels[j])
				if w > max_row_width:
					max_row_width = w
			max_col_height = len(opaque_pixels)

			total_pixels = sum(len(row) for row in opaque_pixels)
			print(body_part, total_pixels)

			max_s = 255
			max_v = 255

			scaling_factor = max(int(255/ total_pixels), 1)
			width_gap = int((max_s / max_row_width) / scaling_factor)
			height_gap = int((max_v / max_col_height) / scaling_factor)

			for j in range(len(opaque_pixels)):
				for i in range(len(opaque_pixels[j])):
					(x,y) = opaque_pixels[j][i]
					ns = max_s - (j * width_gap)
					nv = max_v - (i * height_gap)
					hsv_map.putpixel([x,y], (hue,ns,nv))

		hsv_map.convert('RGBA').save(GENERATOR_ROOT+prefix+archetype+"\\output\\Map.png")
		# rgba_map.save(GENERATOR_ROOT+prefix+archetype+"\\output\\Map.png")
	

def hsv_to_rgb(h,s,v):
	r = h / (360/255)
	g = s / (100/255)
	b = v / (100/255)
	return (int(r),int(g),int(b))
def rgb_to_hsv(r, g, b):
	h = r * (360/255)
	s = g * (100/255)
	v = b * (100/255)
	return (h,s,v)

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


