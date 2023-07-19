# Image Processing Program

This is an image processing program implemented in C++. It provides various filters and operations to manipulate images. The program reads input images, applies filters, and generates output images based on the specified operations.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Supported Operations](#supported-operations)
- [Examples](#examples)

## Features

- Supports reading and writing TGA image files.
- Provides various image filtering and manipulation operations.
- Performs tests to verify the correctness of the applied filters.

## Installation

1. Clone the repository:

```
git clone https://github.com/your-username/image-processing-program.git
```

2. Compile the program using a C++ compiler.

## Usage

To use the image processing program, follow these steps:

1. Prepare your input images in the TGA format and place them in the `input` directory.

2. Open the `main.cpp` file and modify the code in the `main` function to specify the desired image operations. You can uncomment or comment out different operations as needed.

3. Compile and run the program.

4. The resulting output images will be generated in the `output` directory.

## Supported Operations

The image processing program supports the following operations:

- Multiply: Multiplies two images pixel by pixel.
- Subtract: Subtracts one image from another pixel by pixel.
- Screen: Performs screen blending on two images.
- Overlay: Overlays two images using different blending formulas.
- IncreaseGreenChannel: Increases the green channel of an image by a specified amount.
- MultiplyRedBlueChannel: Multiplies the red and blue channels of an image by specified amounts.
- SeparatingChannels: Separates a specific channel (red, green, or blue) from an image.
- Combine: Combines three separate channels into a single image.
- Rotate: Rotates an image 180 degrees.

## Examples

The `examples` directory contains sample input and output images for each supported operation. These can be used to compare the results of the program with the expected output.
