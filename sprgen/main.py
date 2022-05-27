#!/usr/bin/env python

from PIL import Image


import os

def main():
    asset_path = "C:\\Users\\brett\\Nextcloud\\projects\\gameart\\Aseprite\\Units\\Generator\\"

    raw_image = Image.open(asset_path + 'MaleTemplate.png')
    picture = raw_image.convert('RGBA')
    r, g, b, alpha = raw_image.split()

    width, height = picture.size

    hues = {
        "right_arm": (250, 0, 0),
        "head": (225, 0, 0),
        "left_arm": (200, 0, 0),
        "torso": (175, 0, 0),
        "right_leg": (150, 0, 0),
        "left_leg": (125, 0, 0),
        "right_equipped": (100, 0, 0),
        "left_equip": (75, 0, 0),
        "cape": (50, 0, 0),
        "mount": (25, 0, 0),
        #"shadow": (70, 0, 0),
    }

    picture.putalpha(alpha)
    picture.save(asset_path + "new.png")


if __name__ == "__main__":
   main() 
