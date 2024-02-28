#!/usr/bin/env python

from PIL import Image

import math



'''

End goal: FinishedAnimations

Template -> ColorMap (program)

ColorMap -> ColorAnimations (manual)

ColorAnimations + ColorMap + Reference -> FinishedAnimations

The ColorAnimations pixels color's map to pixels in the ColorMap.

he ColorMap's pixel positions map to the pixels in the Reference.

The FinalAnimations pixel will be the positions of ColorAnimations 
and the color of Reference.
'''
# Windows
# ASSET_PATH = "C:\\Users\\brett\\Nextcloud\\projects\\gameart\\Aseprite\\Units\\Generator\\"
GENERATOR_ROOT = "C:\\Projects\\game\\tools\\sprgen\\"

# Linux
# ASSET_PATH = "/home/brettbar/projects/gameart/Aseprite/Units/Generator/"

SOURCE_PATH = GENERATOR_ROOT + "Actors/"
FINAL_PATH = GENERATOR_ROOT + "FinalAnims/"
FINAL_ANIM_FILE = GENERATOR_ROOT + "Template.png"

def main():
    gen_color_maps() 
    gen_final_anims('Romans/colonist')
    gen_final_anims('Romans/hastati') 
    gen_final_anims('Greeks/colonist')
    gen_final_anims('Greeks/perioikoi')
    gen_final_anims('Celts/villager')
    gen_final_anims('Celts/warrior')
    gen_final_anims('Carthaginians/colonist')
    gen_final_anims('Carthaginians/hoplite')

def gen_final_anims(sprite_folder):
    # 128 x 128 Color Maps for Sprite
    color_maps = {
        # 'Cape': Image.open(ASSET_PATH + 'TemplateLayers/Cape.png'),
        'Legs': Image.open(GENERATOR_ROOT + 'ColorMaps/Legs.png').convert('RGBA'),
        'Torso': Image.open(GENERATOR_ROOT + 'ColorMaps/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(GENERATOR_ROOT + 'ColorMaps/LeftArm.png').convert('RGBA'),
        'Head': Image.open(GENERATOR_ROOT + 'ColorMaps/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(GENERATOR_ROOT + 'ColorMaps/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(GENERATOR_ROOT + 'ColorMaps/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(GENERATOR_ROOT + 'ColorMaps/RightArm.png').convert('RGBA'),
    }

    # 128 x 128 Source Maps for Sprite
    source_maps = {
        'Legs': Image.open(SOURCE_PATH + sprite_folder + '/output/Legs.png').convert('RGBA'),
        'Torso': Image.open(SOURCE_PATH + sprite_folder + '/output/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(SOURCE_PATH + sprite_folder + '/output/LeftArm.png').convert('RGBA'),
        'Head': Image.open(SOURCE_PATH + sprite_folder + '/output/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(SOURCE_PATH + sprite_folder + '/output/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(SOURCE_PATH + sprite_folder + '/output/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(SOURCE_PATH + sprite_folder + '/output/RightArm.png').convert('RGBA'),
    }

    # Entire Animation Spritesheet, separated by Layer
    color_anims = {
        'Legs': Image.open(GENERATOR_ROOT + 'ColorAnims/Legs.png').convert('RGBA'),
        'Torso': Image.open(GENERATOR_ROOT + 'ColorAnims/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(GENERATOR_ROOT + 'ColorAnims/LeftArm.png').convert('RGBA'),
        'Head': Image.open(GENERATOR_ROOT + 'ColorAnims/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(GENERATOR_ROOT + 'ColorAnims/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(GENERATOR_ROOT + 'ColorAnims/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(GENERATOR_ROOT + 'ColorAnims/RightArm.png').convert('RGBA'),
    }

    # Entire Animation Spritesheet
    final_anims = Image.open(FINAL_ANIM_FILE).convert('RGBA')
    final_anims_dict = dict()

    print("Generating Sprite:", sprite_folder)

    # For each Animation Color Map Layer
    for body_part, color_anim in color_anims.items():
        print("Evaluating AnimMap:", body_part)
        width, height = color_anim.size

        # All the visible pixels for a given layer in the entire animation sheet
        anim_pixels = []
        for x in range(0, width -1):
            for y in range(0, height -1):
                _,_,_,a = color_anim.getpixel((x,y))
               
                if a == 0: continue

                anim_pixels.append((x, y))


        # All the visible pixels for a given color map
        color_map_pixels = []
        cm_width, cm_height = color_maps[body_part].size
        for x in range(0, cm_width -1):
            for y in range(0, cm_height -1):
                _,_,_,a = color_maps[body_part].getpixel((x,y))
               
                if a == 0: continue

                color_map_pixels.append((x, y))

        # All the visible pixels for a given source map
        source_map_pixels = []
        sm_width, sm_height = source_maps[body_part].size
        for x in range(0, sm_width -1):
            for y in range(0, sm_height -1):
                _,_,_,a = source_maps[body_part].getpixel((x,y))
               
                if a == 0: continue

                source_map_pixels.append((x, y))

                
        # For each pixel in the entire animation sheet for one layer
        for anim_pixel in anim_pixels:

            # Get the pixels color
            anim_color = color_anim.getpixel(anim_pixel)

            # Find the position of that color in the color_map 
            lookup_pixel = (-1, -1)
            for map_pixel in color_map_pixels:
                map_color = color_maps[body_part].getpixel(map_pixel)

                if anim_color == map_color:
                    lookup_pixel = map_pixel
                    break

            # if lookup_pixel == (-1, -1):
            #     print("ERROR IN COLOR MAP LOOKUP")

            # With the position in the color map, find the color of that pixel in the source map
            for source_pixel in source_map_pixels:
                if source_pixel == lookup_pixel:
                    final_anims_dict[anim_pixel] = True
                    final_anims.putpixel(anim_pixel, source_maps[body_part].getpixel(lookup_pixel))
                    break

            
    for x in range(0, final_anims.size[0]):
        for y in range(0, final_anims.size[1]):
            if (x, y) in final_anims_dict:
                continue
            else:
                final_anims.putpixel((x, y), (0, 0, 0, 0))

                

            




                    
    final_anims.save(FINAL_PATH + sprite_folder + ".png")




def gen_color_maps():
    color_template_maps = {
        # 'Cape': Image.open(ASSET_PATH + 'TemplateLayers/Cape.png'),
        'Legs': Image.open(GENERATOR_ROOT + 'TemplateLayers/Legs.png').convert('RGBA'),
        'Torso': Image.open(GENERATOR_ROOT + 'TemplateLayers/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(GENERATOR_ROOT + 'TemplateLayers/LeftArm.png').convert('RGBA'),
        'Head': Image.open(GENERATOR_ROOT + 'TemplateLayers/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(GENERATOR_ROOT + 'TemplateLayers/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(GENERATOR_ROOT + 'TemplateLayers/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(GENERATOR_ROOT + 'TemplateLayers/RightArm.png').convert('RGBA'),
    }

    for part, image in color_template_maps.items():
        new_color_map = image.copy()
        width, height = image.size

        pixels_arr = []
        for x in range(0, width -1):
            for y in range(0, height -1):
                _,_,_,a = image.getpixel((x,y))
               
                if a == 0: continue

                new_color_map.putpixel((x,y), (0, 0, 0, 255))

                pixels_arr.append((x, y))


        prev_pixel = pixels_arr[0]
        for pixel in pixels_arr:
            pr, pg, pb, a = new_color_map.getpixel(prev_pixel)

            if pr < 255:
                pr += 2
            elif pg < 255:
                pg += 2
            elif pb < 255:
                pb += 2
            else:
                print("Exceeded maximum color combinations")

            new_color_map.putpixel(pixel, (pr, pg, pb, a))

            prev_pixel = pixel


        new_color_map.save(GENERATOR_ROOT + "ColorMaps/" + part + ".png")


if __name__ == "__main__":
   main() 
