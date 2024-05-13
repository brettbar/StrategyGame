#!/usr/bin/env python

from PIL import Image

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
			'LeftEquip': Image.open(path+ 'Silhouette-LeftEquip.png').convert('RGBA'),
			'RightEquip': Image.open(path+ 'Silhouette-RightEquip.png').convert('RGBA'),
			'RightArm': Image.open(path + 'Silhouette-RightArm.png').convert('RGBA'),
		}
		# archetype_map = Image.new('RGBA', [256, 128])
		hsv_map = Image.new('HSV', [256, 128])
		for body_part, silhouette in silhouettes.items():
			width = 128
			height = 128
			hsv_silh = silhouette.convert('HSV')

			num_pixels = 0
			for y in range(width):
				for x in range(height):
					h,s,v = hsv_silh.getpixel((x,y))
					if v == 0: continue 
					num_pixels+=1

					ns = s - (y*3)
					nv = v - (x*3)

					# @leftoff. Making progress, now we just need to make it work off of only
		 			# opaque pixels, this will save us a lot of color space so we dont
					# have to go so desatured/devalued so quickly
		 			# basically need a 2d array without the transparent pixels?

					# (nr, ng, nb) = hsv_to_rgb(h, ns, nv)
					hsv_map.putpixel([x,y], (h,ns,nv))

					# red = x % 256
					# grn = y % 256
					# # blu = (x + y) % 256
					# archetype_map.putpixel([x,y], (red,grn, 0,255))



		# for body_part, silhouette in silhouettes.items():
		# 	left_bank = {}
		# 	right_bank = {}
		# 	unique_color_map = {}

		# 	width = 128
		# 	height = 128
		# 	num_pixels = 0
		# 	for y in range(0, width -1):
		# 		for x in range(0, height-1):
		# 			r,g,b,a = silhouette.getpixel((x,y))
		# 			if a == 0: continue 
		# 			num_pixels += 1
		# 			h,s,v = rgb_to_hsv(r,g,b)
		# 			hue = h - 2*num_pixels

		# 			# archetype_map.putpixel([x, y], (r, g, b, 255))
		# 			left_bank[(x,y)] = (hue, s, v)

		# 	for x in range(width, (2*width)-1):
		# 		for y in range(0, height-1):
		# 			r,g,b,a = silhouette.getpixel((x,y))
		# 			if a == 0: continue 
		# 			h,s,v = rgb_to_hsv(r,g,b)
		# 			right_bank[(x,y)] = (h, s, v)

		# 	print(left_bank)
		# 	print(right_bank)
		# 	for pixel, color in left_bank.items():
		# 		(h, s, v) = color
		# 		r, g, b = hsv_to_rgb(h, s, v)
		# 		if (r,g,b) in unique_color_map:
		# 			print("ERROR: duplicate color found", (r,g,b))
		# 			# return
		# 		else:
		# 			unique_color_map[(r,g,b)] = 1

		# 		archetype_map.putpixel([pixel[0], pixel[1]], (r,g,b,255))

		# 			# archetype_map.putpixel([x, y], (r, g, b, 255))


		hsv_map.convert('RGBA').save(GENERATOR_ROOT+prefix+archetype+"\\output\\Map.png")
	
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


