#include <execution>
#include <format>
#include <iostream>
#include <memory>
#include <numbers>
#include <random>
#include <vector>

#include "curves/curves.h"

template <std::mt19937::result_type min, std::mt19937::result_type max>
double randint()
{
  static std::random_device dev;
  static std::mt19937 rng(dev());
  static std::uniform_int_distribution<std::mt19937::result_type> dist(
    min, max); // distribution in range [1, 6]

  return dist(rng);
}

template <std::mt19937::result_type min, std::mt19937::result_type max,
          double granularity>
double randdbl()
{
  static std::random_device dev;
  static std::mt19937 rng(dev());
  static std::uniform_int_distribution<std::mt19937::result_type> dist(
    min, max / granularity); // distribution in range [1, 6]

  return dist(rng) / (double)max * granularity;
}

void print(const curves::vec3 &vec)
{
  auto &[x, y, z] = vec;
  std::cout << std::format("({}, {}, {})", x, y, z);
}

void print(const curves::vec3 &vec1, const curves::vec3 &vec2)
{
  auto &[x1, y1, z1] = vec1;
  auto &[x2, y2, z2] = vec2;
  std::cout << std::format(
    "point ({}, {}, {}); derivative ({}, {}, {})", x1, y1, z1, x2, y2, z2);
}

int main()
{
  unsigned long long num_of_curves = 1 << 20;

  // Populate a container (e.g. vector or list) of objects of these types created in random manner with random parameters.
  // NOTE: using `std::pmr` would eliminate dynamic allocation but highly complicate the code
  std::vector<std::shared_ptr<curves::curve>> randcurves;
  randcurves.resize(num_of_curves);
  for (auto &curve : randcurves) {
    // NOTE: could be enum type
    int curvetype = randint<0, 2>();

    // NOTE: we set minimum to 1 to avoid degenerate curves
    switch (curvetype) {
      case 0:
        curve = std::make_shared<curves::circle>(randdbl<1, 1'000, 0.01>());
        break;
      case 1:
        curve = std::make_shared<curves::ellipses>(randdbl<1, 1'000, 0.01>(),
                                                   randdbl<1, 1'000, 0.01>());
        break;
      case 2:
        curve = std::make_shared<curves::helix>(randdbl<1, 1'000, 0.01>(),
                                                randdbl<1, 1'000, 0.01>());
        break;
    }
  }

  // Print coordinates of points and derivatives of all curves in the container at t=PI/4.
  double t = std::numbers::pi_v<double> / 4;
  for (const auto &curve : randcurves) {
    print(curve->point(t), curve->derivative(t));
    std::cout << std::endl;
  }

  std::vector<std::shared_ptr<curves::circle>> randcircles;
  randcircles.reserve(num_of_curves);
  for (const auto &curve : randcurves) {
    if (dynamic_cast<curves::circle *>(curve.get())) {
      randcircles.emplace_back(
        std::dynamic_pointer_cast<curves::circle, curves::curve>(curve));
    }
  }

  std::sort(randcircles.begin(),
            randcircles.end(),
            [](const auto &lhs, const auto &rhs) {
              return lhs->radius() < rhs->radius();
            });

  auto sum =
    std::transform_reduce(std::execution::par,
                          randcircles.begin(),
                          randcircles.end(),
                          0.0,
                          std::plus {},
                          [](const auto &lhs) { return lhs->radius(); });

  std::cout << std::format("sum of circle radii: {}", sum) << std::endl;
}
