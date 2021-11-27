from PIL import Image, ImageDraw


picture = Image.open('temp.png')
width, height = picture.size


for x in range(0, width - 1):
    for y in range(0, height -1):
        r,g,b,a = picture.getpixel( (x,y) )
        if r==254 and g==254 and b==254:
            new_color = (225, 47, 47, 255)
            picture.putpixel( (x,y), new_color)
        if r==236 and g==236 and b==236:
            new_color = (190, 27, 27, 255)
            picture.putpixel( (x,y), new_color)
        if r==216 and g==216 and b==216:
            new_color = (152, 22, 22, 255)
            picture.putpixel( (x,y), new_color)

picture = picture.save("new.png")
