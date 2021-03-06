// perlin.cpp
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

#include "perlin.h"

using namespace noise::module;

Perlin::Perlin ():
  Module (GetSourceModuleCount ()),
  m_frequency       (DEFAULT_PERLIN_FREQUENCY   ),
  m_lacunarity      (DEFAULT_PERLIN_LACUNARITY  ),
  m_noiseQuality    (DEFAULT_PERLIN_QUALITY     ),
  m_noiseType       (DEFAULT_PERLIN_TYPE        ),
  m_octaveCount     (DEFAULT_PERLIN_OCTAVE_COUNT),
  m_octaveCountTmp  (DEFAULT_PERLIN_OCTAVE_COUNT),
  m_persistence     (DEFAULT_PERLIN_PERSISTENCE ),
  m_seed            (DEFAULT_PERLIN_SEED)
{
    // m_norm is a normalizing factor to keep the signal in [-1.0,1.0]
    // The maximum signal amplitude can be calculated, since it is a geometric series
    m_norm = 1.0;//(1.0 - m_persistence) / (1.0-pow(m_persistence,m_octaveCount+1));
}

double Perlin::GetValue (double x, double y) const
{
  double value = 0.0;
  double signal = 0.0;
  double curPersistence = 1.0;
  double nx, ny;
  int seed;

  x *= m_frequency;
  y *= m_frequency;
  
  switch (m_noiseType)
  {
    case noise::TYPE_GRADIENT:
    {
        for (int curOctave = 0; curOctave < m_octaveCount; curOctave++)
        {
            // Make sure that these floating-point values have the same range as a 32-
            // bit integer so that we can pass them to the coherent-noise functions.
            nx = MakeInt32Range (x);
            ny = MakeInt32Range (y);

            // Get the coherent-noise value from the input value and add it to the
            // final result.
            seed = (m_seed + curOctave) & 0xffffffff;
            signal = GradientCoherentNoise2D (nx, ny, seed, m_noiseQuality);
            value += signal * curPersistence;

            // Prepare the next octave.
            x *= m_lacunarity;
            y *= m_lacunarity;
            curPersistence *= m_persistence;
        }
        break;
    }
    case noise::TYPE_VALUE:
    {
        for (int curOctave = 0; curOctave < m_octaveCount; curOctave++)
        {
            // Make sure that these floating-point values have the same range as a 32-
            // bit integer so that we can pass them to the coherent-noise functions.
            nx = MakeInt32Range (x);
            ny = MakeInt32Range (y);

            // Get the coherent-noise value from the input value and add it to the
            // final result.
            seed = (m_seed + curOctave) & 0xffffffff;
            signal = ValueCoherentNoise2D (nx, ny, seed, m_noiseQuality);
            value += signal * curPersistence;

            // Prepare the next octave.
            x *= m_lacunarity;
            y *= m_lacunarity;
            curPersistence *= m_persistence;
        }
        break;
    }
  }

  
  
  return value*m_norm;
}
