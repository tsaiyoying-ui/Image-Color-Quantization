#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>
#include <algorithm>
using namespace std;

// A simple structure to store one RGB pixel
struct Pixel {
    int r;
    int g;
    int b;
};

// A structure used to store one palette color
struct PaletteColor {
    int r;
    int g;
    int b;
    int pixelCount;
};

// A simple image structure for PPM image data
struct Image {
    int width;
    int height;
    int maxColorValue;
    vector<Pixel> pixels;
};

// Octree node used to represent a region in RGB color space
class OctreeNode {
public:
    long long redSum;
    long long greenSum;
    long long blueSum;
    int pixelCount;
    bool isLeaf;
    OctreeNode* children[8];

    OctreeNode() {
        redSum = 0;
        greenSum = 0;
        blueSum = 0;
        pixelCount = 0;
        isLeaf = false;

        for (int i = 0; i < 8; i++) {
            children[i] = nullptr;
        }
    }

    ~OctreeNode() {
        for (int i = 0; i < 8; i++) {
            delete children[i];
        }
    }
};

// Octree class for inserting RGB colors and collecting color statistics
class Octree {
private:
    OctreeNode* root;
    const int maxDepth = 8;

    int getChildIndex(const Pixel& pixel, int level) {
        int shift = 7 - level;

        int rBit = (pixel.r >> shift) & 1;
        int gBit = (pixel.g >> shift) & 1;
        int bBit = (pixel.b >> shift) & 1;

        int index = (rBit << 2) | (gBit << 1) | bBit;
        return index;
    }

    void insertColor(OctreeNode* node, const Pixel& pixel, int level) {
        if (level == maxDepth) {
            node->isLeaf = true;
            node->redSum += pixel.r;
            node->greenSum += pixel.g;
            node->blueSum += pixel.b;
            node->pixelCount++;
            return;
        }

        int index = getChildIndex(pixel, level);

        if (node->children[index] == nullptr) {
            node->children[index] = new OctreeNode();
        }

        insertColor(node->children[index], pixel, level + 1);
    }

    void collectPalette(OctreeNode* node, vector<PaletteColor>& palette) {
        if (node == nullptr) {
            return;
        }

        if (node->isLeaf && node->pixelCount > 0) {
            PaletteColor color;
            color.r = static_cast<int>(node->redSum / node->pixelCount);
            color.g = static_cast<int>(node->greenSum / node->pixelCount);
            color.b = static_cast<int>(node->blueSum / node->pixelCount);
            color.pixelCount = node->pixelCount;

            palette.push_back(color);
            return;
        }

        for (int i = 0; i < 8; i++) {
            collectPalette(node->children[i], palette);
        }
    }

    int countLeafNodes(OctreeNode* node) {
        if (node == nullptr) {
            return 0;
        }

        if (node->isLeaf && node->pixelCount > 0) {
            return 1;
        }

        int total = 0;
        for (int i = 0; i < 8; i++) {
            total += countLeafNodes(node->children[i]);
        }

        return total;
    }

public:
    Octree() {
        root = new OctreeNode();
    }

    ~Octree() {
        delete root;
    }

    void insert(const Pixel& pixel) {
        insertColor(root, pixel, 0);
    }

    vector<PaletteColor> getPalette() {
        vector<PaletteColor> palette;
        collectPalette(root, palette);
        return palette;
    }

    int getLeafCount() {
        return countLeafNodes(root);
    }
};

// Read P3 PPM image
bool readPPM(const string& filename, Image& image) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open input file: " << filename << endl;
        return false;
    }

    string magicNumber;
    file >> magicNumber;

    if (magicNumber != "P3") {
        cout << "Error: Only P3 PPM format is supported." << endl;
        return false;
    }

    file >> image.width >> image.height;
    file >> image.maxColorValue;

    if (image.width <= 0 || image.height <= 0 || image.maxColorValue <= 0) {
        cout << "Error: Invalid PPM header." << endl;
        return false;
    }

    image.pixels.clear();

    int totalPixels = image.width * image.height;

    for (int i = 0; i < totalPixels; i++) {
        Pixel pixel;
        file >> pixel.r >> pixel.g >> pixel.b;

        if (file.fail()) {
            cout << "Error: Not enough pixel data in PPM file." << endl;
            return false;
        }

        image.pixels.push_back(pixel);
    }

    file.close();
    return true;
}

// Write P3 PPM image
bool writePPM(const string& filename, const Image& image) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot write output file: " << filename << endl;
        return false;
    }

    file << "P3" << endl;
    file << image.width << " " << image.height << endl;
    file << image.maxColorValue << endl;

    for (int i = 0; i < image.pixels.size(); i++) {
        file << image.pixels[i].r << " "
             << image.pixels[i].g << " "
             << image.pixels[i].b << " ";

        if ((i + 1) % image.width == 0) {
            file << endl;
        }
    }

    file.close();
    return true;
}

// Calculate squared distance between a pixel and a palette color
int colorDistanceSquared(const Pixel& pixel, const PaletteColor& color) {
    int dr = pixel.r - color.r;
    int dg = pixel.g - color.g;
    int db = pixel.b - color.b;

    return dr * dr + dg * dg + db * db;
}

// Calculate squared distance between two palette colors
int paletteDistanceSquared(const PaletteColor& a, const PaletteColor& b) {
    int dr = a.r - b.r;
    int dg = a.g - b.g;
    int db = a.b - b.b;

    return dr * dr + dg * dg + db * db;
}

// Merge two palette colors using weighted average
PaletteColor mergeColors(const PaletteColor& a, const PaletteColor& b) {
    PaletteColor merged;
    int totalPixels = a.pixelCount + b.pixelCount;

    merged.r = (a.r * a.pixelCount + b.r * b.pixelCount) / totalPixels;
    merged.g = (a.g * a.pixelCount + b.g * b.pixelCount) / totalPixels;
    merged.b = (a.b * a.pixelCount + b.b * b.pixelCount) / totalPixels;
    merged.pixelCount = totalPixels;

    return merged;
}

// Reduce palette to the target number of colors
vector<PaletteColor> reducePalette(vector<PaletteColor> palette, int targetColors) {
    if (targetColors <= 0) {
        return palette;
    }

    if (palette.size() <= static_cast<size_t>(targetColors)) {
        return palette;
    }

    while (palette.size() > static_cast<size_t>(targetColors)) {
        int bestI = 0;
        int bestJ = 1;
        int bestDistance = paletteDistanceSquared(palette[0], palette[1]);

        for (int i = 0; i < palette.size(); i++) {
            for (int j = i + 1; j < palette.size(); j++) {
                int distance = paletteDistanceSquared(palette[i], palette[j]);

                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        PaletteColor merged = mergeColors(palette[bestI], palette[bestJ]);

        palette[bestI] = merged;
        palette.erase(palette.begin() + bestJ);
    }

    return palette;
}

// Find the nearest palette color for a pixel
PaletteColor findNearestColor(const Pixel& pixel, const vector<PaletteColor>& palette) {
    int bestIndex = 0;
    int bestDistance = colorDistanceSquared(pixel, palette[0]);

    for (int i = 1; i < palette.size(); i++) {
        int distance = colorDistanceSquared(pixel, palette[i]);

        if (distance < bestDistance) {
            bestDistance = distance;
            bestIndex = i;
        }
    }

    return palette[bestIndex];
}

// Quantize all pixels using the reduced palette
vector<Pixel> quantizePixels(const vector<Pixel>& pixels, const vector<PaletteColor>& palette) {
    vector<Pixel> result;

    for (const Pixel& pixel : pixels) {
        PaletteColor nearest = findNearestColor(pixel, palette);

        Pixel newPixel;
        newPixel.r = nearest.r;
        newPixel.g = nearest.g;
        newPixel.b = nearest.b;

        result.push_back(newPixel);
    }

    return result;
}

// Count unique colors in a pixel list
int countUniqueColors(const vector<Pixel>& pixels) {
    vector<Pixel> uniqueColors;

    for (const Pixel& pixel : pixels) {
        bool found = false;

        for (const Pixel& existing : uniqueColors) {
            if (pixel.r == existing.r &&
                pixel.g == existing.g &&
                pixel.b == existing.b) {
                found = true;
                break;
            }
        }

        if (!found) {
            uniqueColors.push_back(pixel);
        }
    }

    return uniqueColors.size();
}

// Print palette colors
void printPalette(const vector<PaletteColor>& palette) {
    for (int i = 0; i < palette.size(); i++) {
        cout << i + 1 << ". RGB("
             << palette[i].r << ", "
             << palette[i].g << ", "
             << palette[i].b << "), pixels: "
             << palette[i].pixelCount << endl;
    }
}

int main(int argc, char* argv[]) {
    cout << "Image Color Quantization Tool - PPM Version" << endl;
    cout << "-------------------------------------------" << endl;

    if (argc != 4) {
        cout << "Usage:" << endl;
        cout << "  " << argv[0] << " input.ppm output.ppm target_colors" << endl;
        cout << endl;
        cout << "Example:" << endl;
        cout << "  " << argv[0] << " input.ppm output.ppm 16" << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];
    int targetColors = stoi(argv[3]);

    if (targetColors <= 0) {
        cout << "Error: target_colors must be greater than 0." << endl;
        return 1;
    }

    Image inputImage;

    if (!readPPM(inputFilename, inputImage)) {
        return 1;
    }

    cout << "Input file: " << inputFilename << endl;
    cout << "Output file: " << outputFilename << endl;
    cout << "Image size: " << inputImage.width << " x " << inputImage.height << endl;
    cout << "Total pixels: " << inputImage.pixels.size() << endl;
    cout << "Target colors: " << targetColors << endl;
    cout << endl;

    auto start = chrono::high_resolution_clock::now();

    Octree octree;

    for (const Pixel& pixel : inputImage.pixels) {
        octree.insert(pixel);
    }

    vector<PaletteColor> originalPalette = octree.getPalette();
    vector<PaletteColor> reducedPalette = reducePalette(originalPalette, targetColors);
    vector<Pixel> quantizedPixels = quantizePixels(inputImage.pixels, reducedPalette);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed = end - start;

    Image outputImage;
    outputImage.width = inputImage.width;
    outputImage.height = inputImage.height;
    outputImage.maxColorValue = inputImage.maxColorValue;
    outputImage.pixels = quantizedPixels;

    if (!writePPM(outputFilename, outputImage)) {
        return 1;
    }

    int originalUniqueColors = countUniqueColors(inputImage.pixels);
    int quantizedUniqueColors = countUniqueColors(quantizedPixels);

    double reductionRatio = 0.0;
    if (originalUniqueColors > 0) {
        reductionRatio = 100.0 * (1.0 - static_cast<double>(quantizedUniqueColors) / originalUniqueColors);
    }

    cout << "Octree built successfully." << endl;
    cout << "Octree leaf node count: " << octree.getLeafCount() << endl;
    cout << "Original unique colors: " << originalUniqueColors << endl;
    cout << "Original palette size: " << originalPalette.size() << endl;
    cout << "Reduced palette size: " << reducedPalette.size() << endl;
    cout << "Quantized unique colors: " << quantizedUniqueColors << endl;
    cout << "Color reduction ratio: " << reductionRatio << "%" << endl;
    cout << "Processing time: " << elapsed.count() << " ms" << endl;
    cout << endl;

    cout << "Reduced Palette:" << endl;
    printPalette(reducedPalette);
    cout << endl;

    cout << "Output image saved successfully: " << outputFilename << endl;
    cout << "Final verification completed." << endl;

    return 0;
}