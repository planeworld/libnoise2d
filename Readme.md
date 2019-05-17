[![Build Status](https://travis-ci.org/planeworld/libnoise2d.svg?branch=master)](https://travis-ci.org/planeworld/libnoise2d)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/d5f2af8c2e4e42dd9c3491f8ba13ab45)](https://www.codacy.com/app/bfeldpw/libnoise2d?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=planeworld/libnoise2d&amp;utm_campaign=Badge_Grade)
[![AUR package](https://repology.org/badge/version-for-repo/aur/libnoise2d.svg)](https://repology.org/metapackage/libnoise2d)

libnoise2d
==========
2D adaption and extension of libnoise by Jason Bevins, a C++ library that is used to generate coherent noise.  
Main modifications include:
*  Move from 3D to 2D to reduce number of calculations
*  Correct calculation of maximum amplitudes
  * Given octaves and lacunarity, most generator modules can be forumlated as a geometric series
  * Normalisation to (-1, 1)

*  Add temporary setting of octaves to allow for correct subsampling without aliasing
*  Add possibility to switch between value and gradient noise
*  Add option to use xxhash as an alternative hash function for value noise

Installation
------------

    > git clone https://github.com/planeworld/libnoise2d.git
    > mkdir libnoise/build
    > cd libnoise/build
    > cmake ../
    > ccmake .
    (To configure compilation and installation details)
    > make install
