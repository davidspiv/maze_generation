#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES // for VS

typedef std::array<u_int8_t, 3> Tristimulus;

namespace clrspc {

// forward declarations
class Ok_Lch_Ab;
class Ok_Lab;
class Rgb;

class Color {
protected:
  std::array<float, 3> m_values;

public:
  Color(float x, float y, float z) { m_values = {x, y, z}; }

  [[nodiscard]] std::array<float, 3> get_values() const { return m_values; }

  virtual void print() const = 0;

  [[nodiscard]] bool operator==(Color const &other) const {
    auto [x, y, z] = m_values;
    float error = 1;
    auto [other_x, other_y, other_z] = other.get_values();
    return (std::abs(x - other_x) < error) && (std::abs(y - other_y) < error) &&
           (std::abs(z - other_z) < error);
  }

  [[nodiscard]] bool operator!=(Color const &other) const {
    return !(*this == other);
  }
};

class Ok_Lab : public Color {
public:
  Ok_Lab(float l, float a, float b);

  float l() const { return m_values[0]; }
  float a() const { return m_values[1]; }
  float b() const { return m_values[2]; }

  [[nodiscard]] Ok_Lch_Ab to_ok_lch_ab() const;
  [[nodiscard]] Rgb to_rgb() const;

  void print() const override;
};

class Ok_Lch_Ab : public Color {
public:
  Ok_Lch_Ab(float l, float c, float h);

  [[nodiscard]] Ok_Lab to_ok_lab() const;

  void print() const override;
};

class Rgb : public Color {
public:
  Rgb(float r, float g, float b);
  Rgb(Tristimulus channels);

  float r() const { return m_values[0]; }
  float g() const { return m_values[1]; }
  float b() const { return m_values[2]; }

  [[nodiscard]] Ok_Lab to_ok_lab() const;

  void print() const override;
};

// =========== Utils ==========

inline float to_radians(float const degrees) {
  return degrees * (M_PI / 180.f);
}

inline float to_degrees(float const radians) {
  return radians * (180.f / M_PI);
}

inline float normalize_degrees(float x) {
  return x - std::floor(x / 360.0f) * 360.0f;
}

inline std::array<float, 3>
cartesian_to_polar(std::array<float, 3> const &cartesian_color_space) {
  auto [l, a, b] = cartesian_color_space;
  float c = std::sqrt(a * a + b * b);
  float h_component = to_degrees(std::atan2(b, a));
  float h = (h_component >= 0) ? h_component : h_component + 360.0;

  return {l, c, h};
}

inline std::array<float, 3>
polar_to_cartesian(std::array<float, 3> const &polar_color_space) {
  auto [l, c, h_deg] = polar_color_space;
  float h_rad = to_radians(h_deg);

  float a = c * std::cos(h_rad);
  float b = c * std::sin(h_rad);

  return {l, a, b};
}

// =========== okOK_LAB Space ==========

inline Ok_Lab::Ok_Lab(float l, float a, float b) : Color(l, a, b) {}

inline Ok_Lch_Ab Ok_Lab::to_ok_lch_ab() const {
  auto [l, c, h] = cartesian_to_polar(m_values);

  return Ok_Lch_Ab(l, c, h);
}

inline Rgb Ok_Lab::to_rgb() const

{
  auto gamma = [](float c) {
    c = std::fmax(0.f, c);
    float encoded =
        (c <= 0.0031308f)
            ? 12.92f * c
            : 1.055f * std::exp2f(std::log2f(c) * 0.41666f) - 0.055f;
    return encoded;
  };

  auto [L, a, b] = m_values;

  float l_ = L + 0.3963377774f * a + 0.2158037573f * b;
  float m_ = L - 0.1055613458f * a - 0.0638541728f * b;
  float s_ = L - 0.0894841775f * a - 1.2914855480f * b;

  float l = l_ * l_ * l_;
  float m = m_ * m_ * m_;
  float s = s_ * s_ * s_;

  float r1 = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
  float g1 = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
  float b1 = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

  return {gamma(r1), gamma(g1), gamma(b1)};
}

inline void Ok_Lab::print() const {
  std::cout << "[Ok_Lab]" << "\nL: " << m_values[0] << "\na: " << m_values[1]
            << "\nb: " << m_values[2] << "\n\n";
}

// =========== okLCH Space ==========

inline Ok_Lch_Ab::Ok_Lch_Ab(float l, float c, float h) : Color(l, c, h) {}

inline Ok_Lab Ok_Lch_Ab::to_ok_lab() const {
  auto [l, a, b] = polar_to_cartesian(m_values);

  return Ok_Lab(l, a, b);
}

inline void Ok_Lch_Ab::print() const {
  std::cout << "[LCHab]" << "\nL: " << m_values[0] << "\nc: " << m_values[1]
            << "\nh: " << m_values[2] << "\n\n";
}

// ========== sRGB Space ==========

inline Rgb::Rgb(float r, float g, float b) : Color(r, g, b) {}

inline Rgb::Rgb(Tristimulus channels)
    : Color(channels[0], channels[1], channels[2]) {}

inline Ok_Lab Rgb::to_ok_lab() const {
  auto linear = [](float c) {
    c = std::fmax(0.f, c); // normalize and avoid negatives
    return (c <= 0.04045f)
               ? c / 12.92f
               : std::exp2f(std::log2f((c + 0.055f) / 1.055f) * 2.4f);
  };

  float r_lin = linear(r());
  float g_lin = linear(g());
  float b_lin = linear(b());

  float l =
      0.4122214708f * r_lin + 0.5363325363f * g_lin + 0.0514459929f * b_lin;
  float m =
      0.2119034982f * r_lin + 0.6806995451f * g_lin + 0.1073969566f * b_lin;
  float s =
      0.0883024619f * r_lin + 0.2817188376f * g_lin + 0.6299787005f * b_lin;

  float l_ = cbrtf(l);
  float m_ = cbrtf(m);
  float s_ = cbrtf(s);

  return {
      0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
      1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
      0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
  };
}

inline void Rgb::print() const {
  std::cout << "[Rgb]" << "\nr: " << (int)m_values[0]
            << "\ng: " << (int)m_values[1] << "\nb: " << (int)m_values[2]
            << std::endl;
}

auto clamp255 = [](float x) {
  return static_cast<uint8_t>(std::max(0.f, std::min(1.f, x)) * 255);
};

/**
 * @brief Generates a smooth rainbow of colors starting from a specified color.
 *
 * @param sample_count The number of colors to generate. Must be at least 2.
 * @param start_color The color where the rainbow starts. Defaults to red.
 * @param rainbow_percent The portion of the color wheel to cover, as a
 * percentage (100 = full rainbow, 50 = half rainbow, etc.). Defaults to 100.
 *
 * @return std::vector<sf::Color> A list of colors smoothly transitioning across
 * the specified portion of the rainbow.
 */
inline std::vector<Tristimulus>
get_rainbow_colors(int sample_count, Tristimulus start_color = {255, 0, 0},
                   float rainbow_percent = 33.f) {
  if (sample_count < 2) {
    throw std::domain_error(
        "sample count must be >= 2 for correct interpolation.");
  }

  float const LIGHTNESS = .7f;
  float const CHROMA = .18f;
  float const sample_degrees = (360.0f * rainbow_percent) / 100.0f;

  float start_hue =
      clrspc::Rgb(start_color).to_ok_lab().to_ok_lch_ab().get_values()[2];

  start_hue -= 20.f; // offset to match perceived color

  std::vector<Tristimulus> colors;
  colors.reserve(sample_count);

  for (int i = 0; i < sample_count; ++i) {

    float const hue = clrspc::normalize_degrees(
        start_hue + (sample_degrees * i) / (sample_count - 1));

    clrspc::Ok_Lch_Ab const ok_lch_ab(LIGHTNESS, CHROMA, hue);

    auto const [r, g, b] = ok_lch_ab.to_ok_lab().to_rgb().get_values();

    colors.push_back({clamp255(r), clamp255(g), clamp255(b)});
  }

  return colors;
}

} // namespace clrspc
