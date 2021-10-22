#pragma once

#include <rawvideo/imgbuf.h>

namespace rawvideo
{

/**
 * @see https://wiki.multimedia.cx/index.php/V210
 * @see https://developer.apple.com/library/archive/technotes/tn2162/_index.html#//apple_ref/doc/uid/DTS40013070-CH1-TNTAG8-V210__4_2_2_COMPRESSION_TYPE
 * packed YUV 4:2:2 10bpc
 *
 * repeating pattern of 4x32-bits blocks, each block containing 3 components (2MSB are ignored)
 * each stride is aligned on 128-bytes
 */
struct v210: public imgbuf_t
{
  v210(size_t width=0, size_t height=0);

  // each 128-bit block represents 12 components (6 px)
  struct group_t
  {
    uint16_t y0,y1,y2,y3,y4,y5;
    uint16_t u0_1,u2_3,u4_5;
    uint16_t v0_1,v2_3,v4_5;
  };

  // returns a group of 6 components starting at x,y
  group_t read_group(size_t x, size_t y) const;

  virtual px16_t read16(size_t x, size_t y) const override;
  virtual px10_t read10(size_t x, size_t y) const override;
  virtual px8_t read8(size_t x, size_t y) const override;

  virtual void write16(size_t x, size_t y, const px16_t& v) override;
  virtual void write10(size_t x, size_t y, const px10_t& v) override;
  virtual void write8(size_t x, size_t y, const px8_t& v) override;
};

} // ::rawvideo
