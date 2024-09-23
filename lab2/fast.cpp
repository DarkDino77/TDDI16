#include "image.h"
#include "window.h"
#include "load.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;

/**
 * Class that stores a summary of an image.
 *
 * This summary is intended to contain a high-level representation of the
 * important parts of an image. I.e. it shall contain what a human eye would
 * find relevant, while ignoring things that the human eye would find
 * irrelevant.
 *
 * To approximate human perception, we store a series of booleans that indicate
 * if the brightness of the image has increased or not. We do this for all
 * horizontal lines and vertical lines in a downsampled version of the image.
 *
 * See the lab instructions for more details.
 *
 * Note: You will need to use this data structure as the key in a hash table. As
 * such, you will need to implement equality checks and a hash function for this
 * data structure.
 */
class Image_Summary {
public:
    // Horizontal increases in brightness.
    vector<bool> horizontal;

    // Vertical increases in brightness.
    vector<bool> vertical;

    bool operator==(const Image_Summary &rhs) const
    {
        return horizontal == rhs.horizontal && 
               vertical == rhs.vertical;
    }
};

namespace std {
    template <>
    struct hash<Image_Summary> {
        size_t operator()(const Image_Summary &img) const
        {
            // TODO: behöver man hantera om storleken överskrider antalet bits i size_t?
            size_t horizontal = 0;
            size_t vertical = 0;

            for (size_t i = 0; i < img.horizontal.size(); i++) {
                horizontal |= img.horizontal[i] << i;
            }

            for (size_t i = 0; i < img.vertical.size(); i++) {
                vertical |= img.vertical[i] << i;
            }

            return horizontal ^ vertical;
        }
    };
}

// Compute an Image_Summary from an image. This is described in detail in the
// lab instructions.

Image_Summary compute_summary(const Image &image) {
    const size_t summary_size = 8;
    Image_Summary result;

    const Image shrinked_image = image.shrink(summary_size + 1, summary_size + 1);

    result.horizontal.reserve((summary_size + 1) * summary_size);
    result.vertical.reserve((summary_size + 1) * summary_size);

    for (size_t y = 0; y <= summary_size ; y++) {
        for (size_t x = 0; x < summary_size; x++) {

            const Pixel &left = shrinked_image.pixel(x, y);
            const Pixel &right = shrinked_image.pixel(x + 1, y);

            const bool value = left.brightness() < right.brightness();
            result.horizontal.push_back(value);         

        }
    } 

    for (size_t x = 0; x <= summary_size; x++) {
        for (size_t y = 0; y < summary_size; y++) {
            const Pixel &upper = shrinked_image.pixel(x, y);
            const Pixel &lower = shrinked_image.pixel(x, y + 1);

            const bool value = upper.brightness() < lower.brightness();

            result.vertical.push_back(value);
        }
    }

    return result;
}

int main(int argc, const char *argv[]) {
    WindowPtr window = Window::create(argc, argv);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [--nopause] [--nowindow] <directory>" << endl;
        cerr << "Missing directory containing files!" << endl;
        return 1;
    }

    vector<string> files = list_files(argv[1], ".jpg");
    cout << "Found " << files.size() << " image files." << endl;

    if (files.size() <= 0) {
        cerr << "No files found! Make sure you entered a proper path!" << endl;
        return 1;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<Image_Summary, std::vector<std::string>> image_map;

    for (const auto &file : files) {
        const Image image = load_image(file);
        const Image_Summary summary = compute_summary(image);

        if (image_map.count(summary)) {
            image_map[summary].push_back(file);
        } else {
            image_map.insert({summary, {file}});
        }
    }


    for (const auto &[summary, files] : image_map) {
        if (files.size() > 1) {
            window->report_match(files);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "Total time: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return 0;
}
