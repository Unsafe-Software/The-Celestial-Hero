#include "perlin_noise.hh"

namespace Engine {
    namespace World {
        PerlinNoise::PerlinNoise() { PerlinNoise(0); }

        PerlinNoise::PerlinNoise(unsigned int seed) {
            for (int i = 0; i < 256; ++i) p[i] = p[i + 256] = rand() % 256;
            std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));
        }

        double PerlinNoise::noise(double x) const {
            int X = static_cast<int>(std::floor(x)) & 255;
            x -= std::floor(x);
            double u = fade(x);
            int A = p[X];
            int B = p[X + 1];
            return lerp(u, grad(A, x), grad(B, x - 1));
        }

        double PerlinNoise::fade(double t) const { return t * t * t * (t * (t * 6 - 15) + 10); }

        double PerlinNoise::lerp(double t, double a, double b) const { return a + t * (b - a); }

        double PerlinNoise::grad(int hash, double x) const {
            int h = hash & 15;
            double u = h < 8 ? x : -x;
            return u;
        }
    }  // namespace World
}  // namespace Engine