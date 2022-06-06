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

# Linux
# ASSET_PATH = "/home/brettbar/nextcloud/projects/gameart/Aseprite/Units/Generator/"

# WSL
ASSET_PATH = "/mnt/c/Users/brett/Nextcloud/projects/gameart/Aseprite/Units/Generator/"

def main():
    gen_color_maps() 
    gen_final_anims('Hastati') 
    gen_final_anims('SenoneWarrior')

def gen_final_anims(sprite_folder):
    # 128 x 128 Color Maps for Sprite
    color_maps = {
        # 'Cape': Image.open(ASSET_PATH + 'TemplateLayers/Cape.png'),
        'Legs': Image.open(ASSET_PATH + 'ColorMaps/Legs.png').convert('RGBA'),
        'Torso': Image.open(ASSET_PATH + 'ColorMaps/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(ASSET_PATH + 'ColorMaps/LeftArm.png').convert('RGBA'),
        'Head': Image.open(ASSET_PATH + 'ColorMaps/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(ASSET_PATH + 'ColorMaps/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(ASSET_PATH + 'ColorMaps/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(ASSET_PATH + 'ColorMaps/RightArm.png').convert('RGBA'),
    }

    # 128 x 128 Source Maps for Sprite
    source_maps = {
        'Legs': Image.open(ASSET_PATH + sprite_folder + '/Legs.png').convert('RGBA'),
        'Torso': Image.open(ASSET_PATH + sprite_folder + '/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(ASSET_PATH + sprite_folder + '/LeftArm.png').convert('RGBA'),
        'Head': Image.open(ASSET_PATH + sprite_folder + '/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(ASSET_PATH + sprite_folder + '/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(ASSET_PATH + sprite_folder + '/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(ASSET_PATH + sprite_folder + '/RightArm.png').convert('RGBA'),
    }

    # Entire Animation Spritesheet, separated by Layer
    color_anims = {
        'Legs': Image.open(ASSET_PATH + 'ColorAnims/Legs.png').convert('RGBA'),
        'Torso': Image.open(ASSET_PATH + 'ColorAnims/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(ASSET_PATH + 'ColorAnims/LeftArm.png').convert('RGBA'),
        'Head': Image.open(ASSET_PATH + 'ColorAnims/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(ASSET_PATH + 'ColorAnims/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(ASSET_PATH + 'ColorAnims/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(ASSET_PATH + 'ColorAnims/RightArm.png').convert('RGBA'),
    }


    # Entire Animation Spritesheet
    final_anims = Image.open(ASSET_PATH + 'Test.png').convert('RGBA')
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

                

            




                    
    final_anims.save(ASSET_PATH + sprite_folder + "/FinalAnims.png")




def gen_color_maps():
    color_template_maps = {
        # 'Cape': Image.open(ASSET_PATH + 'TemplateLayers/Cape.png'),
        'Legs': Image.open(ASSET_PATH + 'TemplateLayers/Legs.png').convert('RGBA'),
        'Torso': Image.open(ASSET_PATH + 'TemplateLayers/Torso.png').convert('RGBA'),
        'LeftArm': Image.open(ASSET_PATH + 'TemplateLayers/LeftArm.png').convert('RGBA'),
        'Head': Image.open(ASSET_PATH + 'TemplateLayers/Head.png').convert('RGBA'),
        'LeftEquip': Image.open(ASSET_PATH + 'TemplateLayers/LeftEquip.png').convert('RGBA'),
        'RightEquip': Image.open(ASSET_PATH + 'TemplateLayers/RightEquip.png').convert('RGBA'),
        'RightArm': Image.open(ASSET_PATH + 'TemplateLayers/RightArm.png').convert('RGBA'),
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


        new_color_map.save(ASSET_PATH + "ColorMaps/" + part + ".png")


if __name__ == "__main__":
   main() 
