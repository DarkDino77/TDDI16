/*
 * fast < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cassert>
#include <thread>

#include "point.h"
#include "window.h"

using namespace std;

int main(int argc, const char *argv[])
{
    WindowPtr window = create_window(argc, argv);
    if (!window)
        return 1;

    // The array of points
    vector<Point> points;

    // Read tolerance from cin
    double tolerance{};
    cin >> tolerance;

    // Read points from cin
    int N{};
    cin >> N;

    for (int i{0}; i < N; ++i)
    {
        double x{}, y{};
        cin >> x >> y;
        points.push_back(Point{x, y});
    }

    // draw points to screen all at once
    window->draw_points(points);

    // Sort points by their natural order. Makes finding endpoints a bit easier.
    sort(points.begin(), points.end());

    auto begin = chrono::high_resolution_clock::now();

    /////////////////////////////////////////////////////////////////////////////
    // Draw any lines that you find in 'points' using the function 'window->draw_line'.
    /////////////////////////////////////////////////////////////////////////////
    for (size_t i{0}; i < points.size(); i++)
    {
        vector<Point> sorted_points = points;

        const Point origin = points[i];
        sort(sorted_points.begin(), sorted_points.end(), PolarSorter{origin});

        assert(sorted_points[0].x == origin.x && sorted_points[0].y == origin.y);

        // for (const auto p : sorted_points)
        // {
        //     // std::cout << origin.slopeTo(p) << ", ";
        // }
        // std::cout << endl;

        size_t point_count{0};
        for (size_t j{1}; j < sorted_points.size() - 1; j++)
        {
            if (origin.sameSlope(sorted_points[j], sorted_points[j + 1], tolerance))
            {
                // std::cout << origin.slopeTo(sorted_points[j]) << ", " << origin.slopeTo(sorted_points[j + 1]) << '\n';
                // std::cout << origin.sameSlope(sorted_points[j], sorted_points[j + 1], tolerance) << endl;
                if (point_count == 0)
                    point_count += 2;
                else
                    point_count++;
            }
            else
            {
                if (point_count >= 3)
                {
                    std::vector<Point> line_points{origin};
                    // std::cout << point_count <<"\n";

                    for (size_t k = 0; k < point_count; k++)
                    {
                        const Point p = sorted_points[j - point_count + k + 1];

                        line_points.push_back(p);
                    }

                    // for (auto p : line_points)
                    // {
                    //     // std::cout << origin.slopeTo(p) << ", ";
                    // }
                    // std::cout << '\n';
                    // std::cout << "Line points: " << line_points.size() << '\n';

                    window->draw_line(line_points);
                    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                }

                point_count = 0;
            }
        }

        if (point_count >= 3)
        {
            std::vector<Point> line_points{origin};
            // std::cout << point_count <<"\n";

            for (size_t k = 0; k < point_count; k++)
            {
                const Point p = sorted_points[sorted_points.size() - 1 - point_count + k + 1];

                line_points.push_back(p);
            }

            // for (auto p : line_points)
            // {
            //     // std::cout << origin.slopeTo(p) << ", ";
            // }
            // std::cout << '\n';
            // std::cout << "Line points: " << line_points.size() << '\n';

            window->draw_line(line_points);
            // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}
