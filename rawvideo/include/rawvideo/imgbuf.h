#pragma once

#include <rawvideo/imgtypes.h>

#include <cstdlib>
#include <array>
#include <limits>

namespace rawvideo
{

struct imgbuf_t
{
  imgbuf_t(size_t width=0, size_t height=0);
  virtual ~imgbuf_t();
  imgbuf_t(const imgbuf_t&)=default;
  imgbuf_t(imgbuf_t&&)=default;

  // alloc inner buffer (when it's not provided by an external source).
  // in such case, the buffer ownership is left to the current instance
  // size defaults to frame_sz()
  void alloc(size_t sz=std::numeric_limits<size_t>::max());

  inline bool is_packed() const { return par.plane_cnt==1; }
  inline size_t frame_sz() const { return par.plane_size[0]+par.plane_size[1]+par.plane_size[2]; }

  virtual px16_t read16(size_t x, size_t y) const=0;
  virtual px10_t read10(size_t x, size_t y) const;
  virtual px8_t read8(size_t x, size_t y) const;

  virtual void write16(size_t x, size_t y, const px16_t& v)=0;
  virtual void write10(size_t x, size_t y, const px10_t& v);
  virtual void write8(size_t x, size_t y, const px8_t& v);

  struct
  {
    size_t width;
    size_t height;

    size_t bitdepth;
    size_t plane_cnt;

    std::array<size_t, 3> row_size;
    std::array<size_t, 3> plane_offset;
    std::array<size_t, 3> plane_size;
    std::array<size_t, 4> chroma;
  } par;

  uint8_t* data = nullptr;

  // TODO: add helpers around color space (tv/mpeg v/s full)
  // TODO: add helpers around color matrix (bt601, bt709, fcc, smpte240m)

  // convert to RGB 24-bits buffer (subclass may implement format-specific optimizations)
  virtual uint8_t* to_rgb24() const;

private:
  bool _data_own = false;
};

} // ::rawvideo
