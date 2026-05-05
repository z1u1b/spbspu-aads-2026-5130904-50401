#ifndef MY_SIPHASH_HPP
#define MY_SIPHASH_HPP

#include <cstdint>
#include <cstring>
#include <string>

namespace zubarev
{
  class SipHash
  {
  private:
    uint64_t k0_;
    uint64_t k1_;

    static uint64_t rotl(uint64_t x, int b)
    {
      return (x << b) | (x >> (64 - b));
    }

    static void sipround(uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3)
    {
      v0 += v1;
      v1 = rotl(v1, 13);
      v1 ^= v0;
      v0 = rotl(v0, 32);

      v2 += v3;
      v3 = rotl(v3, 16);
      v3 ^= v2;

      v0 += v3;
      v3 = rotl(v3, 21);
      v3 ^= v0;

      v2 += v1;
      v1 = rotl(v1, 17);
      v1 ^= v2;
      v2 = rotl(v2, 32);
    }

  public:
    SipHash(uint64_t k0 = 0x0706050403020100ULL,
            uint64_t k1 = 0x0F0E0D0C0B0A0908ULL)
      : k0_(k0), k1_(k1)
    {}

    uint64_t operator()(const void* data, size_t len) const
    {
      const uint8_t* bytes = static_cast<const uint8_t*>(data);

      uint64_t v0 = 0x736f6d6570736575ULL ^ k0_;
      uint64_t v1 = 0x646f72616e646f6dULL ^ k1_;
      uint64_t v2 = 0x6c7967656e657261ULL ^ k0_;
      uint64_t v3 = 0x7465646279746573ULL ^ k1_;

      uint64_t m = 0;
      size_t i = 0;

      // обработка блоков по 8 байт
      for (; i + 8 <= len; i += 8)
      {
        std::memcpy(&m, bytes + i, 8);

        v3 ^= m;
        sipround(v0, v1, v2, v3);
        sipround(v0, v1, v2, v3);
        v0 ^= m;
      }

      // хвост
      uint64_t last = ((uint64_t)len) << 56;

      switch (len - i)
      {
        case 7: last |= ((uint64_t)bytes[i + 6]) << 48;
        case 6: last |= ((uint64_t)bytes[i + 5]) << 40;
        case 5: last |= ((uint64_t)bytes[i + 4]) << 32;
        case 4: last |= ((uint64_t)bytes[i + 3]) << 24;
        case 3: last |= ((uint64_t)bytes[i + 2]) << 16;
        case 2: last |= ((uint64_t)bytes[i + 1]) << 8;
        case 1: last |= ((uint64_t)bytes[i]);
      }

      v3 ^= last;
      sipround(v0, v1, v2, v3);
      sipround(v0, v1, v2, v3);
      v0 ^= last;

      // финализация
      v2 ^= 0xff;
      sipround(v0, v1, v2, v3);
      sipround(v0, v1, v2, v3);
      sipround(v0, v1, v2, v3);
      sipround(v0, v1, v2, v3);

      return v0 ^ v1 ^ v2 ^ v3;
    }

    // удобная перегрузка для std::string
    uint64_t operator()(const std::string& s) const
    {
      return operator()(s.data(), s.size());
    }
  };

}

#endif
