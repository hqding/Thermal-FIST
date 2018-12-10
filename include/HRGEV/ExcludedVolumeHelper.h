/*
 * Thermal-FIST package
 * 
 * Copyright (c) 2014-2018 Volodymyr Vovchenko
 *
 * GNU General Public License (GPLv3 or later)
 */
#ifndef EXCLUDEDVOLUMEHELPER_H
#define EXCLUDEDVOLUMEHELPER_H

#include "HRGBase/xMath.h"
#include <cmath>

namespace thermalfist {

  namespace CuteHRGHelper {
    inline double vr(double r) { return (16. * xMath::Pi() / 3. * pow(r, 3)); }
    inline double rv(double v) { return pow(v * 3. / (16. * xMath::Pi()), 1. / 3.); }
    inline double brr(double r1, double r2) { return (2. * xMath::Pi() / 3.) * pow(r1 + r2, 3); }
  }

} // namespace thermalfist

#endif