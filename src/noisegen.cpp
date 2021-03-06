// noisegen.cpp
//
// Modified Work: Copyright (C) 2012 - 2018 Torsten Büschenfeld
// Original Work: Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include "noisegen.h"
#include "interp.h"
#include "vectortable.h"
#include "xxhash.h"

#include <cmath>

using namespace noise;

const double SQRT1_2 = 1.0 / std::sqrt(2.0);

// Specifies the version of the coherent-noise functions to use.
// - Set to 2 to use the current version.
// - Set to 1 to use the flawed version from the original version of libnoise.
// If your application requires coherent-noise values that were generated by
// an earlier version of libnoise, change this constant to the appropriate
// value and recompile libnoise.
#define NOISE_VERSION 2

// These constants control certain parameters that all coherent-noise
// functions require.
#if (NOISE_VERSION == 1)
// Constants used by the original version of libnoise.
// Because X_NOISE_GEN is not relatively prime to the other values, and
// Z_NOISE_GEN is close to 256 (the number of random gradient vectors),
// patterns show up in high-frequency coherent noise.
const int X_NOISE_GEN = 1;
const int Y_NOISE_GEN = 31337;
const int SEED_NOISE_GEN = 1013;
const int SHIFT_NOISE_GEN = 13;
#else
// Constants used by the current version of libnoise.
const int X_NOISE_GEN = 1619;
const int Y_NOISE_GEN = 31337;
const int SEED_NOISE_GEN = 1013;
const int SHIFT_NOISE_GEN = 8;
#endif

double noise::GradientCoherentNoise2D (double x, double y, int seed,
  NoiseQuality noiseQuality)
{
  // Create a unit-length box aligned along an integer boundary.  This box
  // surrounds the input point.
  int x0 = (x > 0.0? (int)x: (int)x - 1);
  int x1 = x0 + 1;
  int y0 = (y > 0.0? (int)y: (int)y - 1);
  int y1 = y0 + 1;
  
  // Map the difference between the coordinates of the input value and the
  // coordinates of the box's outer-lower-left vertex onto an S-curve.
  double xs = 0, ys = 0;
  switch (noiseQuality) {
    case QUALITY_FAST:
      xs = (x - (double)x0);
      ys = (y - (double)y0);
      break;
    case QUALITY_STD:
      xs = SCurve3 (x - (double)x0);
      ys = SCurve3 (y - (double)y0);
      break;
    case QUALITY_BEST:
      xs = SCurve5 (x - (double)x0);
      ys = SCurve5 (y - (double)y0);
      break;
  }

  // Now calculate the noise values at each vertex of the box. To generate
  // the coherent-noise value at the input point, interpolate these four
  // noise values using the S-curve value as the interpolant (trilinear
  // interpolation.)
  double n0, n1, ix0, ix1;
  n0   = GradientNoise2D (x, y, x0, y0, seed);
  n1   = GradientNoise2D (x, y, x1, y0, seed);
  ix0  = LinearInterp (n0, n1, xs);
  n0   = GradientNoise2D (x, y, x0, y1, seed);
  n1   = GradientNoise2D (x, y, x1, y1, seed);
  ix1  = LinearInterp (n0, n1, xs);
  
  return LinearInterp (ix0, ix1, ys);
}

double noise::GradientNoise2D (double fx, double fy, int ix,
  int iy, int seed)
{
  // Randomly generate a gradient vector given the integer coordinates of the
  // input value.  This implementation generates a random number and uses it
  // as an index into a normalized-vector lookup table.
  int vectorIndex = (
      X_NOISE_GEN    * ix
    + Y_NOISE_GEN    * iy
    + SEED_NOISE_GEN * seed)
    & 0xffffffff;
  vectorIndex ^= (vectorIndex >> SHIFT_NOISE_GEN);
  vectorIndex &= 0xff;

  double xvGradient = g_randomVectors[(vectorIndex << 1)    ];
  double yvGradient = g_randomVectors[(vectorIndex << 1) + 1];

  // Set up us another vector equal to the distance between the two vectors
  // passed to this function.
  double xvPoint = (fx - (double)ix);
  double yvPoint = (fy - (double)iy);

  // Now compute the dot product of the gradient vector with the distance
  // vector.  The resulting value is gradient noise.  Apply a scaling value
  // so that this noise value ranges from -1.0 to 1.0.
  return ((xvGradient * xvPoint)
        + (yvGradient * yvPoint)) * SQRT1_2;
}

int noise::IntValueNoise2D (int x, int y, int seed)
{
  // All constants are primes and must remain prime in order for this noise
  // function to work correctly.
  int n = (
      X_NOISE_GEN    * x
    + Y_NOISE_GEN    * y
    + SEED_NOISE_GEN * seed)
    & 0x7fffffff;
  n = (n >> 13) ^ n;
  return (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
}

double noise::ValueCoherentNoise2D (double x, double y, int seed,
  NoiseQuality noiseQuality)
{
  // Create a unit-length box aligned along an integer boundary.  This box
  // surrounds the input point.
  int x0 = (x > 0.0? (int)x: (int)x - 1);
  int x1 = x0 + 1;
  int y0 = (y > 0.0? (int)y: (int)y - 1);
  int y1 = y0 + 1;

  // Map the difference between the coordinates of the input value and the
  // coordinates of the box's outer-lower-left vertex onto an S-curve.
  double xs = 0, ys = 0;
  switch (noiseQuality) {
    case QUALITY_FAST:
      xs = (x - (double)x0);
      ys = (y - (double)y0);
      break;
    case QUALITY_STD:
      xs = SCurve3 (x - (double)x0);
      ys = SCurve3 (y - (double)y0);
      break;
    case QUALITY_BEST:
      xs = SCurve5 (x - (double)x0);
      ys = SCurve5 (y - (double)y0);
      break;
  }

  // Now calculate the noise values at each vertex of the box. To generate
  // the coherent-noise value at the input point, interpolate these four
  // noise values using the S-curve value as the interpolant (trilinear
  // interpolation.)
  double n0, n1, ix0, ix1;
  n0   = ValueNoise2D (x0, y0, seed);
  n1   = ValueNoise2D (x1, y0, seed);
  ix0  = LinearInterp (n0, n1, xs);
  n0   = ValueNoise2D (x0, y1, seed);
  n1   = ValueNoise2D (x1, y1, seed);
  ix1  = LinearInterp (n0, n1, xs);
  return LinearInterp (ix0, ix1, ys);
}
#ifdef VALUE_NOISE_USE_XXHASH
    #include <cstdint>
#endif
double noise::ValueNoise2D (int x, int y, int seed)
{
    #ifdef VALUE_NOISE_USE_XXHASH
        constexpr auto FACTOR = 2.0 / 4294967296.0;
  
        std::uint64_t pos = 0u;
        pos = std::uint64_t(x) << 32 | std::uint32_t(y);
        auto hash = XXH32(&pos, 8, seed);
  
        return 1.0 - (double(hash) * FACTOR);
    #else
        return 1.0 - ((double)IntValueNoise2D (x, y,seed) / 1073741824.0);
    #endif
}

