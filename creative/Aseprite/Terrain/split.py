from PIL import Image
import os

def split_image(input_path, output_folder, tile_size):
    # Open the image
    original_image = Image.open(input_path)

    # Get the size of the original image
    original_width, original_height = original_image.size

    # Calculate the number of tiles in both dimensions
    num_tiles_x = original_width // tile_size[0]
    num_tiles_y = original_height // tile_size[1]

    # Ensure the output folder exists
    os.makedirs(output_folder, exist_ok=True)

    # Iterate through each tile and save it as a new image
    for i in range(num_tiles_x):
        for j in range(num_tiles_y):
            # Calculate the cropping region for each tile
            left = i * tile_size[0]
            upper = j * tile_size[1]
            right = (i + 1) * tile_size[0]
            lower = (j + 1) * tile_size[1]

            # Crop the image to the specified region
            tile = original_image.crop((left, upper, right, lower))

            # Save the tile as a new image in the output folder
            output_path = os.path.join(output_folder, f"tile_{i}_{j}.png")
            tile.save(output_path)

if __name__ == "__main__":
    # Replace these values with your actual file path, output folder, and tile size
    input_image_path = "./terrain.png"
    output_folder_path = "./output"
    tile_size = (64, 64)  # Specify the size (width, height) of each tile

    # Call the function to split the image
    split_image(input_image_path, output_folder_path, tile_size)
    