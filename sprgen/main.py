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
ASSET_PATH = "/home/brettbar/nextcloud/projects/gameart/Aseprite/Units/Generator/"

def main():
    gen_color_map()
    gen_finished_anims()

def gen_finished_anims():
    color_anims = Image.open(ASSET_PATH + 'MaleAnimations.png').convert('RGBA')
    color_map = Image.open(ASSET_PATH + 'MaleColorMap.png').convert('RGBA')
    reference = Image.open(ASSET_PATH + 'GreekVillager.png').convert('RGBA')

    final_anims = color_anims.copy()

    anims_width, anims_height = color_anims.size
    map_width, map_height =  color_map.size

    top_left_color_anims_pixels = []
    top_left_color_map_pixels = []


    # Pixel space
    # TODO only top left atm
    for anims_x in range(0, anims_width -1):
        for anims_y in range(0, anims_height -1):
            r, g, b, a = color_anims.getpixel((anims_x, anims_y))

            if a > 0: 
                # print("Adding pixel", (anims_x, anims_y), (r, g, b, a))
                top_left_color_anims_pixels.append((anims_x, anims_y))


    for map_x in range(0, math.floor(map_width /2) -1):
        for map_y in range(0, math.floor(map_height /2) -1):
            _, _, _, a = color_map.getpixel((map_x, map_y))

            if a > 0: top_left_color_map_pixels.append((map_x, map_y))
    


    for anims_pixel in top_left_color_anims_pixels:
        anims_color = color_anims.getpixel(anims_pixel)


        for map_pixel in top_left_color_map_pixels:
            map_color = color_map.getpixel(map_pixel)

            
            if map_color == anims_color:
                matching_pixel = map_pixel

                final_anims.putpixel(anims_pixel, reference.getpixel(matching_pixel))

            
    final_anims.save(ASSET_PATH + "MaleFinalAnimations.png")
            
                    


def gen_color_map():

    template_image = Image.open(ASSET_PATH + 'MaleTemplate.png')

    template_pic = template_image.convert('RGBA')
    r, _, _, _ = template_image.split()
    width, height = template_pic.size

    if not width == template_pic.size[0] or not height == template_pic.size[1]:
        print("Incompatible files")
        return

    # hues = {
    #     "right_arm": (250, 0, 0),
    #     "head": (225, 0, 0),
    #     "left_arm": (200, 0, 0),
    #     "torso": (175, 0, 0),
    #     "right_leg": (150, 0, 0),
    #     "left_leg": (125, 0, 0),
    #     "right_equip": (100, 0, 0),
    #     "left_equip": (75, 0, 0),
    #     "cape": (50, 0, 0),
    #     "mount": (25, 0, 0),
    #     #"shadow": (70, 0, 0),
    # }

    
    sprites = {
        "TopLeft": {
            "right_arm": [],
            "head": [],
            "left_arm": [],
            "torso": [],
            "right_leg": [],
            "left_leg": [],
            "right_equipped": [],
            "left_equip": [],
            "cape": [],
            "mount": [],
        },
        "TopRight": {
            "right_arm": [],
            "head": [],
            "left_arm": [],
            "torso": [],
            "right_leg": [],
            "left_leg": [],
            "right_equipped": [],
            "left_equip": [],
            "cape": [],
            "mount": [],
        }
    }


    
    for y in range(0, height -1):
        for x in range(0, width -1):
            r, g, b, a = template_pic.getpixel((x,y))

            if a == 0: continue

            direction = ""
            if x < 128:
                direction = "TopLeft"
            elif x >= 128:
                direction = "TopRight"

            if r == 255 and g == 255 and b == 255:
                sprites[direction]["right_arm"].append((x, y))
            elif r == 255 and g == 0 and b == 0:
                sprites[direction]["head"].append((x, y))
            elif r == 255 and g == 128 and b == 0:
                sprites[direction]["left_arm"].append((x, y))
            elif r == 255 and g == 255 and b == 0:
                sprites[direction]["torso"].append((x, y))
            elif r == 0 and g == 255 and b == 0:
                sprites[direction]["right_leg"].append((x, y))
            elif r == 0 and g == 0 and b == 255:
                sprites[direction]["left_leg"].append((x, y))
            elif r == 128 and g == 255 and b == 255:
                sprites[direction]["right_equip"].append((x, y))
            elif r == 255 and g == 128 and b == 255:
                sprites[direction]["left_equip"].append((x, y))
            elif r == 128 and g == 128 and b == 128:
                sprites[direction]["cape"].append((x, y))
            elif r == 255 and g == 128 and b == 128:
                sprites[direction]["mount"].append((x, y))

    for sprite in sprites.values():
        for body_part in sprite.values():

            if len(body_part) <= 0: continue


            prev_x, prev_y = body_part[0]

            for x,y in body_part:
                (pr, pg, pb, pa) = template_pic.getpixel((prev_x, prev_y))

                pr -= 2
                pg -= 2
                pb -= 2

                # if x > prev_x: # we moved a column over
                #     pr -= 1
                #     pg -= 2

                # if y > prev_y:  # we moved a row down
                #     pr -= 1
                #     pb -= 2
                    
                template_pic.putpixel((x, y), (pr, pg, pb, pa))

                prev_x = x
                prev_y = y
                 

    # picture.putalpha(alpha)
    template_pic.save(ASSET_PATH + "MaleColorMap.png")




if __name__ == "__main__":
   main() 
