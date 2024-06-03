#include "p3helpers.h"

#include <cmath>
#include <random>
#include <chrono>

double p3d::toDegree(double a) {
  return a*180.0/M_PI;
}

double p3d::toRadian(double a) {
  return a*M_PI/180.0;
}


// from https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
static std::uniform_real_distribution<double> *unif;
static bool isRandomInit_{false};
static std::mt19937_64 rng;


static void initRandom01() {
  // initialize the random number generator with time-dependent seed
  uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
  rng.seed(ss);
  unif = new std::uniform_real_distribution<double>(0,1);
  isRandomInit_=true;

  // initialize a uniform distribution between 0 and 1

}
double p3d::random01() {
  // ready to generate random numbers
  if (!isRandomInit_) initRandom01();
  double currentRandomNumber = (*unif)(rng);
  return currentRandomNumber;

}

