#include <rawvideo/422/v210.h>

#include <cassert>

using namespace rawvideo;

v210::v210(size_t w, size_t h)
  : imgbuf_t(w,h)
{
  par.bitdepth=10;
  par.plane_cnt=1;
  par.chroma={4,2,2};
  // https://samples.mplayerhq.hu/V-codecs/R210/bmd_pixel_formats.pdf
  par.row_size = {((w+47)/48)*128,0,0};
  par.plane_offset = {0,0,0};
  par.plane_size = {par.row_size[0]*h,0,0};
}

uint16_t w10(uint32_t w, size_t s)
{
  return static_cast<uint16_t>((w >> s) & 0b1111111111);
};

v210::group_t v210::read_group(size_t x, size_t y) const
{
  // V210 is a repeating pattern of 128-bits (16 bytes)
  // Each of this pattern is composed of 4 32-bits blocks and represent a total of 6 pixels
  const uint32_t* v210_ptr = reinterpret_cast<const uint32_t*>(data + (y*par.row_size[0]) + ((x/6)*16));

  // https://developer.apple.com/library/archive/technotes/tn2162/_index.html#//apple_ref/doc/uid/DTS40013070-CH1-TNTAG8-V210__4_2_2_COMPRESSION_TYPE
  // resulting pattern is built as follows:
  // bits    | component | word | shift
  // ----------------------------------
  // 0-9     | U0+1      |      | >> 0
  // 10-19   | Y0        | wd0  | >> 10
  // 20-29   | V0+1      |      | >> 20
  //         |           |      |
  // 32-41   | Y1        |      | >> 0
  // 42-51   | U2+3      | wd1  | >> 10
  // 52-61   | Y2        |      | >> 20
  //         |           |      |
  // 64-73   | V2+3      |      | >> 0
  // 74-83   | Y3        | wd2  | >> 10
  // 84-93   | U4+5      |      | >> 20
  //         |           |      |
  // 96-105  | Y4        |      | >> 0
  // 106-115 | V4+5      | wd3  | >> 10
  // 116-125 | Y5        |      | >> 20

  return v210::group_t{
    w10(v210_ptr[0],10), // Y0
    w10(v210_ptr[1],0),  // Y1
    w10(v210_ptr[1],20), // Y2
    w10(v210_ptr[2],10), // Y3
    w10(v210_ptr[3],0),  // Y4
    w10(v210_ptr[3],20), // Y5

    w10(v210_ptr[0],0),  // U0+1
    w10(v210_ptr[1],10), // U2+3
    w10(v210_ptr[2],20), // U4+5

    w10(v210_ptr[0],20), // V0+1
    w10(v210_ptr[2],0),  // V2+3
    w10(v210_ptr[3],10)  // V4+5
  };
}

px16_t v210::read16(size_t x, size_t y) const
{
  auto [py,pu,pv] = read10(x,y);
  return {py,pu,pv};
}

px10_t v210::read10(size_t x, size_t y) const
{
  // see read_group
  const uint32_t* v210_ptr = reinterpret_cast<const uint32_t*>(data + (y*par.row_size[0]) + ((x/6)*16));
  const size_t mod=x%6;
  switch(mod)
  {
  // Y0 U0+1 V0+1
  case 0: return {w10(v210_ptr[0],10),w10(v210_ptr[0],0),w10(v210_ptr[0],20)};
  // Y1 U0+1 V0+1
  case 1: return {w10(v210_ptr[1],0),w10(v210_ptr[0],0),w10(v210_ptr[0],20)};
  // Y2 U2+3 V2+3
  case 2: return {w10(v210_ptr[1],20),w10(v210_ptr[1],10),w10(v210_ptr[2],0)};
  // Y3 U2+3 V2+3
  case 3: return {w10(v210_ptr[2],10),w10(v210_ptr[1],10),w10(v210_ptr[2],0)};
  // Y4 U4+5 V4+5
  case 4: return {w10(v210_ptr[3],0),w10(v210_ptr[2],20),w10(v210_ptr[3],10)};
  // Y5 U4+5 V4+5
  case 5: return {w10(v210_ptr[3],20),w10(v210_ptr[2],20),w10(v210_ptr[3],10)};
  }
  // unreachable
  assert(false);
}

px8_t v210::read8(size_t x, size_t y) const
{
  auto [py,pu,pv] = read10(x,y);
  return px8_t{static_cast<px8_t::value_type>(py),
              static_cast<px8_t::value_type>(pu),
        static_cast<px8_t::value_type>(pv)};
}

void v210::write16(size_t x, size_t y, const px16_t& v)
{
  auto [py,pu,pv] = v;
  write10(x,y,{bclamp(py,10),bclamp(pu,10),bclamp(pv,10)});
}

void v210::write10(size_t x, size_t y, const px10_t& v)
{
  // TODO
}

void v210::write8(size_t x, size_t y, const px8_t& v)
{
  const auto& [py,pu,pv] = v;
  write10(x,y,{py,pu,pv});
}
