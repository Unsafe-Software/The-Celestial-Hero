#pragma once
#include <algorithm>
#include <cmath>
#include <random>

namespace Engine {
    namespace World {
        class PerlinNoise {
           public:
            PerlinNoise();
            PerlinNoise(unsigned int seed);

            double noise(double x) const;

           private:
            double fade(double t) const;
            double lerp(double t, double a, double b) const;
            double grad(int hash, double x) const;

            int p[512];
        };
    }  // namespace World
}  // namespace Engine
