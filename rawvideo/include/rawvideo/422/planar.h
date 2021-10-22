#pragma once

#include <rawvideo/imgbuf.h>

namespace rawvideo
{

template<size_t __bits,typename __store_t>
struct imgbuf_422p: public imgbuf_t
{
  inline imgbuf_422p(size_t w=0, size_t h=0)
    : imgbuf_t(w,h)
  {
    par.bitdepth=__bits;
    par.plane_cnt=3;
    par.chroma={4,2,2};
    // https://samples.mplayerhq.hu/V-codecs/R210/bmd_pixel_formats.pdf
    par.row_size = {w*sizeof(__store_t), w*sizeof(__store_t)/2, w*sizeof(__store_t)/2};
    par.plane_size = {par.row_size[0]*h,par.row_size[1]*h,par.row_size[2]*h};
    par.plane_offset = {0,par.plane_size[0],par.plane_size[0]+par.plane_size[1]};
  }

  inline __px_t<__store_t,3> read(size_t x, size_t y) const
  {
    const size_t xy=y*par.width+x;
    const __store_t* const yp=reinterpret_cast<const __store_t*>(data+par.plane_offset[0]);
    const __store_t* const up=reinterpret_cast<const __store_t*>(data+par.plane_offset[1]);
    const __store_t* const vp=reinterpret_cast<const __store_t*>(data+par.plane_offset[2]);
    return {yp[xy],up[(xy/2)],vp[(xy/2)]};
  }

  inline void write(size_t x, size_t y, const __px_t<__store_t,3>& v)
  {
    const size_t xy=y*par.width+x;
    __store_t* yp=reinterpret_cast<__store_t*>(data+par.plane_offset[0]);
    __store_t* up=reinterpret_cast<__store_t*>(data+par.plane_offset[1]);
    __store_t* vp=reinterpret_cast<__store_t*>(data+par.plane_offset[2]);
    yp[xy]=v[0]; up[xy/2]=v[1]; vp[xy/2]=v[2];
  }

  inline virtual px8_t read8(size_t x, size_t y) const override
  {
    return out_cast<px8_t>(read(x,y));
  }
  inline virtual void write8(size_t x, size_t y, const px8_t& v) override
  {
    write(x,y,in_cast<px8_t>(v));
  }

  inline virtual px10_t read10(size_t x, size_t y) const override
  {
    return out_cast<px10_t>(read(x,y));
  }
  inline virtual void write10(size_t x, size_t y, const px10_t& v) override
  {
    write(x,y,in_cast<px10_t>(v));
  }

  inline virtual px16_t read16(size_t x, size_t y) const override
  {
    return out_cast<px16_t>(read(x,y));
  }
  inline virtual void write16(size_t x, size_t y, const px16_t& v) override
  {
    write(x,y,in_cast<px16_t>(v));
  }

private:
  // __store_t -> __cX_t
  template<typename pxX_t>
  static pxX_t out_cast(const __px_t<__store_t,3>& v)
  {
    return {
      static_cast<typename pxX_t::value_type>(v[0]),
      static_cast<typename pxX_t::value_type>(v[1]),
      static_cast<typename pxX_t::value_type>(v[2])
    };
  }

  static __px_t<__store_t,3> out_cast(const __px_t<__store_t,3>& v)
  {
    return v;
  }

  template<typename pxX_t>
  static __px_t<__store_t,3> in_cast(const pxX_t& v)
  {
    return {
      static_cast<__store_t>(v[0]),
      static_cast<__store_t>(v[1]),
      static_cast<__store_t>(v[2])
    };
  }

  static __px_t<__store_t,3> in_cast(const __px_t<__store_t,3>& v)
  {
    return v;
  }
};

using imgbuf_422p8le = rawvideo::imgbuf_422p<8,rawvideo::px8_t::value_type>;
using imgbuf_422p10le = rawvideo::imgbuf_422p<10,rawvideo::px10_t::value_type>;
using imgbuf_422p16le = rawvideo::imgbuf_422p<16,rawvideo::px16_t::value_type>;

} // ::rawvideo
