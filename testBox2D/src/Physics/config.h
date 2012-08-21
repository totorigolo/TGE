#pragma once
#include <Box2D/Box2D.h>

// Constantes de convertion Metres <> Pixels
const float PIXELS_PER_METERS = 30.f;
const float PPM = PIXELS_PER_METERS;

const float METERS_PER_PIXELS = 1.f / PPM;
const float MPP = METERS_PER_PIXELS;

// Constantes de convertion Degrés <> Radians
const float DEGREES_PER_RADIANS = 180.f / b2_pi;
const float DPR = DEGREES_PER_RADIANS;

const float RADIANS_PER_DEGREES = 1.f / DPR;
const float RPD = RADIANS_PER_DEGREES;
