#include <iostream>
#include <vector>
using namespace std;

// A simple structure to store one RGB pixel
struct Pixel {
    int r;
    int g;
    int b;
};

// A structure used to store the result of each leaf node
struct PaletteColor {
    int r;
    int g;
    int b;
    int pixelCount;
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

    vector<PaletteColor> getPalettePreview() {
        vector<PaletteColor> palette;
        collectPalette(root, palette);
        return palette;
    }

    int getLeafCount() {
        return countLeafNodes(root);
    }
};

int main() {
    cout << "Image Color Quantization Tool Prototype" << endl;
    cout << "---------------------------------------" << endl;

    // Basic RGB test data for the prototype stage
    // These pixels simulate image pixel information
    vector<Pixel> pixels = {
        {255, 0, 0},
        {250, 10, 10},
        {245, 5, 5},

        {0, 255, 0},
        {10, 250, 10},
        {5, 245, 5},

        {0, 0, 255},
        {10, 10, 250},
        {5, 5, 245},

        {255, 255, 0},
        {250, 250, 10},

        {0, 255, 255},
        {10, 250, 250},

        {128, 128, 128},
        {130, 130, 130}
    };

    Octree octree;

    for (const Pixel& pixel : pixels) {
        octree.insert(pixel);
    }

    vector<PaletteColor> palette = octree.getPalettePreview();

    cout << "Total input pixels: " << pixels.size() << endl;
    cout << "Octree built successfully." << endl;
    cout << "Leaf node count: " << octree.getLeafCount() << endl;
    cout << endl;

    cout << "Palette Preview:" << endl;

    for (int i = 0; i < palette.size(); i++) {
        cout << i + 1 << ". RGB("
             << palette[i].r << ", "
             << palette[i].g << ", "
             << palette[i].b << "), pixels: "
             << palette[i].pixelCount << endl;
    }

    cout << endl;
    cout << "Prototype verification completed." << endl;
    cout << "This prototype verifies RGB insertion, Octree construction,";
    cout << " leaf node statistics, and preliminary palette preview." << endl;

    return 0;
}