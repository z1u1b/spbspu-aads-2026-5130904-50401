#ifndef MY_SIPHASH_HPP
#define MY_SIPHASH_HPP

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstdint>
#include <string>

namespace zubarev
{
  class SipHash
  {
  private:
    boost::hash2::siphash_64 hasher_template_;

  public:
    // Конструктор с ключами
    SipHash(uint64_t k0 = 0x0706050403020100ULL, uint64_t k1 = 0x0F0E0D0C0B0A0908ULL)
    {
      uint64_t keys[2] = {k0, k1};
      hasher_template_ = boost::hash2::siphash_64(keys, sizeof(keys));
    }

    uint64_t operator()(const void* data, size_t len) const
    {
      auto h = hasher_template_;
      h.update(data, len);
      return boost::hash2::get_integral_result< uint64_t >(h);
    }

    uint64_t operator()(const std::string& s) const
    {
      return (*this)(s.data(), s.size());
    }
  };
}

#endif
