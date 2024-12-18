#include <cmath>
#include <tuple>

namespace curves {
  using vec3 = std::tuple<double, double, double>;

  class curve {
    public:
      virtual vec3 point(double t) const noexcept = 0;
      virtual vec3 derivative(double t) const noexcept = 0;
  };

  class circle : public curve {
    double _radius = 1;

    public:
      circle(double radius) : _radius(radius) {}

      vec3 point(double t) const noexcept override final {
        return {_radius * std::cos(t), _radius * std::sin(t), 0};
      }
      vec3 derivative(double t) const noexcept override final {
        return {-_radius * std::sin(t), _radius * std::cos(t), 0};
      }

      double radius() const noexcept {
        return _radius;
      }
  };

  class ellipses : public curve {
    private:
      double _xradius = 1;
      double _yradius = 1;

    public:
      ellipses(double xradius, double yradius) : _xradius(xradius), _yradius(yradius) {}

      vec3 point(double t) const noexcept override final {
        return {_xradius * std::cos(t), _yradius * std::sin(t), 0};
      }
      vec3 derivative(double t) const noexcept override final {
        return {-_xradius * std::sin(t), _yradius * std::cos(t), 0};
      }

      double xradius() const noexcept {
        return _xradius;
      }
      double yradius() const noexcept {
        return _yradius;
      }
  };

  class helix : public curve {
    private:
      double _radius = 1;
      double _step = 1;

    public:
      helix(double radius, double step) : _radius(radius), _step(step) {}

      vec3 point(double t) const noexcept override final {
        return {_radius * std::cos(t), _radius * std::sin(t), _step * t};
      }
      vec3 derivative(double t) const noexcept override final {
        return {-_radius * std::sin(t), _radius * std::cos(t), _step};
      }

      double radius() const noexcept {
        return _radius;
      }
      double yradius() const noexcept {
        return _step;
      }
  };
}
