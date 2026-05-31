# Image Color Quantization Tool

## Proposal Report


### Motivation and Goal
High-resolution 24-bit True Color images ($16.7$ million colors) consume significant memory and bandwidth. The goal of this project is to implement the Octree algorithm to compress image colors to a target number (e.g., 256). This approach significantly reduces data size while maintaining as much visual fidelity as possible through efficient color partitioning.


### Competitive Analysis
There are several existing image processing tools and libraries that support color quantization, such as Photoshop, ImageMagick, Pillow, and OpenCV. These tools are powerful and highly optimized, but most of them provide color reduction as a built-in function, so the internal data structure and algorithm details are usually hidden from users.

Compared with these existing tools, this project focuses on manually implementing the core color quantization process using the Octree data structure. Instead of relying on library functions, the program builds the tree, inserts RGB colors, prunes nodes, and generates the reduced color palette step by step.

The main goal of this project is not to outperform professional image processing software, but to demonstrate how data structures can be applied to a real image processing problem. By implementing the Octree algorithm directly, this project can show the relationship between tree construction, recursive traversal, pruning strategy, and color compression performance.


### Expected Functions
Image Parsing: Read and parse RGB data from standard lossless formats such as BMP or PPM.

Dynamic Octree Construction: Build an 8-level deep Octree based on the unique color distribution of the input image.

Color Pruning: Implement a pruning algorithm to automatically merge leaf nodes with the lowest frequencies.

Indexed Image Output: Generate the quantized image file along with its corresponding Color Palette.

Customization: Allow users to define the target number of colors (e.g., 16, 64, or 256 colors).


### Technologies Used
Language: C++ (chosen to demonstrate manual memory management and pointer manipulation).

Tools: VS Code, GCC, Git/GitHub (for version control and PR interaction).

Image Processing: Custom-written file I/O for header and pixel data parsing, avoiding heavy third-party libraries to focus on core logic.


### Prototype Verification
The prototype stage is expected to verify the feasibility of the core data structure and algorithm design of this project. Instead of focusing on a fully completed image processing tool, the prototype will mainly demonstrate whether the Octree-based color quantization process can work correctly on basic RGB pixel data.

At this stage, the program is expected to read image pixel information, insert RGB colors into an Octree, and record basic color statistics such as pixel counts and accumulated RGB values in leaf nodes. These results will be used to confirm that the tree structure can correctly represent different regions of the RGB color space.

The prototype will also provide a foundation for later features, including node pruning, reduced color palette generation, and quantized image output. By completing this stage, the project can verify that the main data structure is suitable for further development and performance analysis.



---

## Prototype Report


### Current Progress
The current prototype has completed the basic Octree construction workflow using predefined RGB pixel data. Instead of processing a full image file at this stage, the program uses a small set of RGB pixels to simulate image input and verify the core data structure.

The program can insert RGB colors into an Octree according to their bit values. Each leaf node records the pixel count and accumulated RGB values, which are then used to generate a preliminary palette preview. This verifies that the Octree structure can represent different regions of the RGB color space and collect basic color statistics correctly.

The prototype is currently implemented in C++ in `src/main.cpp`. It demonstrates the basic feasibility of RGB insertion, recursive tree traversal, leaf node statistics, and palette preview generation.


### Challenges
One difficulty at this stage is deciding the proper scope of the prototype. Since complete image file parsing, pruning, and output image generation can be complex, the current version focuses on verifying the Octree data structure first.

Another challenge is calculating the correct child index for each RGB color. The program needs to use bit-level information from the red, green, and blue channels to decide which of the eight child nodes a color should be inserted into.

Memory management is also an important issue because the Octree nodes are dynamically allocated in C++. The prototype includes recursive deletion of child nodes to reduce the risk of memory leaks.


### Next Steps
The next step is to extend the prototype from predefined RGB test data to actual image pixel input. A simple image format such as PPM will be considered first because it is easier to parse than more complex formats.

After image input is added, the project will implement Octree pruning so that the number of leaf nodes can be reduced to a target number of colors, such as 16, 64, or 256. This will allow the program to generate a reduced color palette.

Finally, the program will map original pixels to the generated palette and produce a quantized output image. Performance testing will also be added to compare different target color counts and pruning strategies.

---

## Final Report


### Project Overview
This project implements an Image Color Quantization Tool in C++. The program reads a P3 PPM image, builds an Octree from RGB pixel data, generates a color palette, reduces the palette to a target number of colors, and outputs quantized PPM images.

The final implementation is in `src/main.cpp`. Earlier development versions are kept as references in `src/main_prototype.cpp` and `src/main_ppm_basic.cpp`.


### How to Run
Compile the program:

```powershell
g++ src/main.cpp -o quantizer
```

Run the program:

```powershell
.\quantizer.exe input.ppm output 8
```

Arguments:

```text
input.ppm    input P3 PPM image
output       output folder
8            target number of colors
```

Example commands:

```powershell
.\quantizer.exe input.ppm output 4
.\quantizer.exe input.ppm output 8
.\quantizer.exe input.ppm output 16
```

The quantized images will be generated in the `output/` folder.


### Completed Features
- Read and write P3 PPM image files
- Build an Octree from RGB pixel data
- Generate a color palette from Octree leaf nodes
- Reduce the image to a target number of colors
- Map each pixel to the nearest palette color
- Compare two palette reduction algorithms
- Output quantized images and performance statistics

  
### Algorithm Comparison and Performance Analysis
The main data structure used in this project is an Octree. Each node has up to eight children, and each level uses one bit from the red, green, and blue channels to determine the child index. Leaf nodes store accumulated RGB values and pixel counts, which are used to generate average palette colors.

This project compares two palette reduction algorithms:

1. **Closest-Pair Merge**  
   Repeatedly finds the two closest palette colors and merges them using weighted average.

2. **Frequency Top-K**  
   Sorts palette colors by pixel frequency and keeps the most frequent `K` colors.

Experimental results:

| Target Colors | Algorithm | Reduction Ratio | Average Color Error | Processing Time |
|---:|---|---:|---:|---:|
| 4 | Closest-Pair Merge | 90% | 89.5074 | 0.1761 ms |
| 4 | Frequency Top-K | 90% | 182.966 | 0.0184 ms |
| 8 | Closest-Pair Merge | 80% | 25.905 | 0.2804 ms |
| 8 | Frequency Top-K | 80% | 139.198 | 0.027 ms |
| 16 | Closest-Pair Merge | 60% | 9.06943 | 0.1438 ms |
| 16 | Frequency Top-K | 60% | 66.213 | 0.0277 ms |

The results show that Closest-Pair Merge produces lower average color error, which means it preserves color similarity better. However, it takes more processing time because it repeatedly searches for the closest pair of colors.

Frequency Top-K is faster because it only selects the most frequent colors, but it produces higher average color error because frequent colors are not always the best representatives of the entire color space.


### Conclusion
This project demonstrates how the Octree data structure can be applied to image color quantization. By comparing two palette reduction algorithms, the project shows the trade-off between output quality and processing speed.

Closest-Pair Merge provides better color quality, while Frequency Top-K provides faster execution.



