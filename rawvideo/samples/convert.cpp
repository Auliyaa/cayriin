//#include <rawvideo/yuv/v210.h>
//#include <rawvideo/yuv/yuv422.h>

//#include <iostream>

//int main()
//{
//  const size_t w=1920,h=1080;
//  rawvideo::v210 v210{w,h};
//  rawvideo::yuv422p10le yuv422p10le{w,h};

//  std::cout << " - frame size: "  << v210.frame_sz() << std::endl
//            << " - row size: " << v210.par.row_size[0] << std::endl;

//  auto f = rawvideo::fopen_ptr("/home/auliyaa/dev/media/1080p.raw", "rb");
//  v210.alloc();
//  yuv422p10le.alloc();
//  ::fread(v210.data, 1, v210.frame_sz(), f.get());

//#pragma omp parallel for collapse(2)
//  for (size_t yy=0; yy<h; ++yy)
//  {
//    for (size_t xx=0; xx<w; ++xx)
//    {
//      yuv422p10le.write10(xx,yy,v210.read10(xx,yy));
//    }
//  }

//  return 0;
//}

int main()
{

}
