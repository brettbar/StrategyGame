#!/usr/bin/env python

from PIL import Image


import os

def main():
    asset_path = "/mnt/c/Users/brett/Nextcloud/projects/gameart/Aseprite/Units/Generator/"

    raw_image = Image.open(asset_path + 'MaleTemplate.png')
    picture = raw_image.convert('RGBA')
    width, height = picture.size

    '''
    body_directions {
        body_parts {
            head_pixels: [],
            right_arm_pixels: [],
            torso_pixels: [],
            left_arm_pixels: [],
            right_leg_pixels: [],
            left_leg_pixels: [],
        }
    }
    '''
    
    body_directions = dict()
    body_directions["DR"] = []
    body_directions["DL"] = []
    
    body_parts = dict()
    body_parts["head_pixels"] = []
    body_parts["right_arm_pixels"] = []
    body_parts["torso_pixels"] =  []
    body_parts["left_arm_pixels"] = []
    body_parts["right_leg_pixels"]= []
    body_parts["left_leg_pixels"] = []

    part_colors = dict()
    part_colors["head_pixels"] = (8, 71, 0, 255)
    part_colors["right_arm_pixels"] = (156, 67, 48, 255)
    part_colors["torso_pixels"] = (118, 0, 77, 255)
    part_colors["left_arm_pixels"] = (0, 122, 124, 255)
    part_colors["right_leg_pixels"] = (82, 23, 140, 255)
    part_colors["left_leg_pixels"] = (186, 0, 0, 255)



    

    # Add each pixel to the correct body part array
    for x in range(0, width - 1):
        for y in range(0, height -1):
            r,g,b,_ = picture.getpixel( (x,y) )
            # if r > 0 or b > 0 or g > 0:
            #     print(r, g, b)
            body_direction = ""
            if x < 64 and y < 64:
                body_direction = "DR"
            elif x >= 64 and y < 64:
                body_direction = "DL"
            
            if r == 8:
                body_directions[body_direction]["head_pixels"].append((x,y))
            if r == 156:
                body_directions[body_direction]["right_arm_pixels"].append((x,y))
            if r == 118:
                body_directions[body_direction]["torso_pixels"].append((x,y))
            if r == 0 and b > 1:
                body_directions[body_direction]["left_arm_pixels"].append((x,y))
            if r == 82:
                body_directions[body_direction]["right_leg_pixels"].append((x,y))
            if r == 186:
                body_directions[body_direction]["left_leg_pixels"].append((x,y))
                
            # if r==254 and g==254 and b==254:
            #     new_color = (225, 47, 47, 255)
            #     picture.putpixel( (x,y), new_color)
            # if r==236 and g==236 and b==236:
            #     new_color = (190, 27, 27, 255)
            #     picture.putpixel( (x,y), new_color)
            # if r==216 and g==216 and b==216:
            #     new_color = (152, 22, 22, 255)
            #     picture.putpixel( (x,y), new_color)

    # print(body_parts)

    for key, direction in body_directions.items():
        for key, arr in body_parts.items(): 
            xOff = 0
            yOff = 0

            for pixel in arr:
                r, g, b, a = part_colors[key]
                color = (r+xOff, g+xOff, b+xOff, a)
                picture.putpixel( pixel, color)
                xOff+=1
                yOff+=1


    picture = picture.save(asset_path + "new.png")


if __name__ == "__main__":
   main() 
