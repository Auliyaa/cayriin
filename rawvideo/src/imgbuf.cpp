#include <rawvideo/imgbuf.h>

using namespace rawvideo;

imgbuf_t::imgbuf_t(size_t width, size_t height)
{
  this->par.width = width;
  this->par.height = height;
}

imgbuf_t::~imgbuf_t()
{
  if (_data_own)
  {
    std::free(data);
  }
}

void imgbuf_t::alloc(size_t sz)
{
  if (sz == std::numeric_limits<size_t>::max())
  {
    sz = frame_sz();
  }
  data = static_cast<decltype(data)>(std::malloc(sz));
  _data_own = true;
}

px10_t imgbuf_t::read10(size_t x, size_t y) const
{
  px10_t r = read16(x,y);
  bclamp(r[0],10);
  bclamp(r[1],10);
  bclamp(r[2],10);
  return r;
}

px8_t imgbuf_t::read8(size_t x, size_t y) const
{
  auto [a,b,c] = read16(x,y);
  return {static_cast<px8_t::value_type>(a),
          static_cast<px8_t::value_type>(b),
        static_cast<px8_t::value_type>(c)};
}

void imgbuf_t::write10(size_t x, size_t y, const px10_t& v)
{

}

void imgbuf_t::write8(size_t x, size_t y, const px8_t& v)
{

}
