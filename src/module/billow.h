// billow.h
//
// Modified Work: Copyright (C) 2012 - 2018 Torsten Büschenfeld
// Original Work: Copyright (C) 2004 Jason Bevins
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

#ifndef NOISE_MODULE_BILLOW_H
#define NOISE_MODULE_BILLOW_H

#include "modulebase.h"

namespace noise
{

  namespace module
  {

    /// @addtogroup libnoise
    /// @{

    /// @addtogroup modules
    /// @{

    /// @addtogroup generatormodules
    /// @{

    /// Default frequency for the noise::module::Billow noise module.
    const double DEFAULT_BILLOW_FREQUENCY = 1.0;

    /// Default lacunarity for the the noise::module::Billow noise module.
    const double DEFAULT_BILLOW_LACUNARITY = 2.0;

    /// Default number of octaves for the the noise::module::Billow noise
    /// module.
    const int DEFAULT_BILLOW_OCTAVE_COUNT = 6;

    /// Default persistence value for the the noise::module::Billow noise
    /// module.
    const double DEFAULT_BILLOW_PERSISTENCE = 0.5;

    /// Default noise quality for the the noise::module::Billow noise module.
    const noise::NoiseQuality DEFAULT_BILLOW_QUALITY = QUALITY_STD;
    
    /// Default noise type for the noise::module::Billow noise module.
    const noise::NoiseType DEFAULT_BILLOW_TYPE = TYPE_GRADIENT;

    /// Default noise seed for the the noise::module::Billow noise module.
    const int DEFAULT_BILLOW_SEED = 0;

    /// Maximum number of octaves for the the noise::module::Billow noise
    /// module.
    const int BILLOW_MAX_OCTAVE = 30;

    /// Noise module that outputs three-dimensional "billowy" noise.
    ///
    /// @image html modulebillow.png
    ///
    /// This noise module generates "billowy" noise suitable for clouds and
    /// rocks.
    ///
    /// This noise module is nearly identical to noise::module::Perlin except
    /// this noise module modifies each octave with an absolute-value
    /// function.  See the documentation of noise::module::Perlin for more
    /// information.
    class Billow: public Module
    {

      public:

        /// Constructor.
        ///
        /// The default frequency is set to
        /// noise::module::DEFAULT_BILLOW_FREQUENCY.
        ///
        /// The default lacunarity is set to
        /// noise::module::DEFAULT_BILLOW_LACUNARITY.
        ///
        /// The default number of octaves is set to
        /// noise::module::DEFAULT_BILLOW_OCTAVE_COUNT.
        ///
        /// The default persistence value is set to
        /// noise::module::DEFAULT_BILLOW_PERSISTENCE.
        ///
        /// The default seed value is set to
        /// noise::module::DEFAULT_BILLOW_SEED.
        Billow ();

        /// Returns the frequency of the first octave.
        ///
        /// @returns The frequency of the first octave.
        double GetFrequency () const
        {
          return m_frequency;
        }

        /// Returns the lacunarity of the billowy noise.
        ///
        /// @returns The lacunarity of the billowy noise.
        /// 
        /// The lacunarity is the frequency multiplier between successive
        /// octaves.
        double GetLacunarity () const
        {
          return m_lacunarity;
        }

        /// Returns the quality of the billowy noise.
        ///
        /// @returns The quality of the billowy noise.
        ///
        /// See noise::NoiseQuality for definitions of the various
        /// coherent-noise qualities.
        noise::NoiseQuality GetNoiseQuality () const
        {
          return m_noiseQuality;
        }

        /// Returns the number of octaves that generate the billowy noise.
        ///
        /// @returns The number of octaves that generate the billowy noise.
        ///
        /// The number of octaves controls the amount of detail in the billowy
        /// noise.
        int GetOctaveCount () const
        {
          return m_octaveCount;
        }

        /// Returns the persistence value of the billowy noise.
        ///
        /// @returns The persistence value of the billowy noise.
        ///
        /// The persistence value controls the roughness of the billowy noise.
        double GetPersistence () const
        {
          return m_persistence;
        }

        /// Returns the seed value used by the billowy-noise function.
        ///
        /// @returns The seed value.
        int GetSeed () const
        {
          return m_seed;
        }

        virtual int GetSourceModuleCount () const
        {
          return 0;
        }

        virtual double GetValue (double x, double y) const;

        /// Sets the frequency of the first octave.
        ///
        /// @param frequency The frequency of the first octave.
        void SetFrequency (double frequency)
        {
          m_frequency = frequency;
        }

        /// Sets the lacunarity of the billowy noise.
        ///
        /// @param lacunarity The lacunarity of the billowy noise.
        /// 
        /// The lacunarity is the frequency multiplier between successive
        /// octaves.
        ///
        /// For best results, set the lacunarity to a number between 1.5 and
        /// 3.5.
        void SetLacunarity (double lacunarity)
        {
          m_lacunarity = lacunarity;
        }

        /// Sets the quality of the billowy noise.
        ///
        /// @param noiseQuality The quality of the billowy noise.
        ///
        /// See noise::NoiseQuality for definitions of the various
        /// coherent-noise qualities.
        void SetNoiseQuality (noise::NoiseQuality noiseQuality)
        {
          m_noiseQuality = noiseQuality;
        }
        
        /// Sets the type of the billowy noise.
        ///
        /// @param noiseType The type of the billowy noise.
        ///
        /// See noise::NoiseType for definitions of the
        /// coherent-noise types.
        void SetNoiseType (noise::NoiseType noiseType)
        {
          m_noiseType = noiseType;
        }

        /// Sets the number of octaves that generate the billowy noise.
        /// This is the original or maximum number of octaves. Norma-
        /// lisation is done with respect to this value. For subsampling,
        /// use SetOctaveCountTmp.
        ///
        /// @param octaveCount The number of octaves that generate the billowy
        /// noise.
        ///
        /// @pre The number of octaves ranges from 1 to
        /// noise::module::BILLOW_MAX_OCTAVE.
        ///
        /// @throw noise::ExceptionInvalidParam An invalid parameter was
        /// specified; see the preconditions for more information.
        ///
        /// The number of octaves controls the amount of detail in the billowy
        /// noise.
        ///
        /// The larger the number of octaves, the more time required to
        /// calculate the billowy-noise value.
        void SetOctaveCount (int octaveCount)
        {
          if (octaveCount < 1 || octaveCount > BILLOW_MAX_OCTAVE) {
            throw noise::ExceptionInvalidParam ();
          }
          m_octaveCount = octaveCount;
          // m_norm is a normalizing factor to keep the signal in [-1.0,1.0]
          // The maximum signal amplitude can be calculated, since it is a geometric series
          m_norm = 1.0 - m_persistence;
        }
        
        /// Sets the number of octaves that generate the billowy noise,
        /// but only temporary. This might help for temporary subsampling
        /// of when representing a very complex surface, but viewing it from
        /// a large distance. The maximum number of octaves without subsampling
        /// is given by SetOctaveCount.
        ///
        /// @param octaveCountTmp The number of octaves that generate the
        /// subsampled billowy noise.
        ///
        /// @pre The number of octaves ranges from 1 to
        /// noise::module::PERLIN_MAX_OCTAVE.
        ///
        /// @throw noise::ExceptionInvalidParam An invalid parameter was
        /// specified; see the preconditions for more information.
        ///
        /// The number of octaves controls the amount of detail in the Billow
        /// noise.
        ///
        /// The larger the number of octaves, the more time required to
        /// calculate the Billow-noise value. Therefore, this method
        /// can be used to temporarily reduce the number of octaves for
        /// subsampling.
        void SetOctaveCountTmp (int octaveCount)
        {
          if (octaveCount < 1 || octaveCount > BILLOW_MAX_OCTAVE) {
            throw noise::ExceptionInvalidParam ();
          }
          m_octaveCountTmp = octaveCount;
        }

        /// Sets the persistence value of the billowy noise.
        ///
        /// @param persistence The persistence value of the billowy noise.
        ///
        /// The persistence value controls the roughness of the billowy noise.
        ///
        /// For best results, set the persistence value to a number between
        /// 0.0 and 1.0.
        void SetPersistence (double persistence)
        {
          m_persistence = persistence;
        }

        /// Sets the seed value used by the billowy-noise function.
        ///
        /// @param seed The seed value.
        void SetSeed (int seed)
        {
          m_seed = seed;
        }

      protected:

        /// Frequency of the first octave.
        double m_frequency;

        /// Frequency multiplier between successive octaves.
        double m_lacunarity;
        
        /// Normalizing factor to keep output values in [-1.0,1.0]
        double m_norm;

        /// Quality of the billowy noise.
        noise::NoiseQuality m_noiseQuality;
        
        /// Type of the billowy noise.
        noise::NoiseType m_noiseType;

        /// Total number of octaves that generate the billowy noise.
        int m_octaveCount;
        
        /// Total number of octaves that generate for temporary generation
        /// of Billow noise. Can be used for subsampling.
        int m_octaveCountTmp;

        /// Persistence value of the billowy noise.
        double m_persistence;

        /// Seed value used by the billowy-noise function.
        int m_seed;

    };

    /// @}

    /// @}

    /// @}

  }

}

#endif
