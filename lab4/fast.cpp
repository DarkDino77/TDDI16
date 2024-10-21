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

std::vector<Point> get_line_points(const std::vector<Point> &points, const Point origin, const size_t line_start, const size_t point_count)
{
    std::vector<Point> line_points{origin};
    line_points.reserve(point_count - 1);
    for (size_t k = 0; k < point_count; k++)
    {
        const Point p = points[line_start - point_count + k + 1];

        line_points.push_back(p);
    }

    return line_points;
}

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

        size_t point_count{0};
        Point slopePoint = origin;
        for (size_t j{1}; j < sorted_points.size() - 1; j++)
        {
            if (point_count == 0)
            {
                slopePoint = sorted_points[j];
            }

            if (origin.sameSlope(slopePoint, sorted_points[j + 1], tolerance))
            {
                if (point_count == 0)
                    point_count += 2;
                else
                    point_count++;
            }
            else
            {
                if (point_count >= 3)
                {
                    const std::vector<Point> line_points = get_line_points(sorted_points, origin, j, point_count);
                    window->draw_line(line_points);
                }

                point_count = 0;
            }
        }

        if (point_count >= 3)
        {
            const std::vector<Point> line_points = get_line_points(sorted_points, origin, points.size() - 1, point_count);

            window->draw_line(line_points);
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
