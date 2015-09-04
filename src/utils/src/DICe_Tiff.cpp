// @HEADER
// ************************************************************************
//
//               Digital Image Correlation Engine (DICE)
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact:  Dan Turner (dzturne@sandia.gov)
//
// ************************************************************************
// @HEADER

#include <cassert>

#include <DICe_Tiff.h>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/tiff_dynamic_io.hpp>


namespace DICe{

void read_image_dimensions(const std::string & file_name,
  size_t & width,
  size_t & height){
  boost::gil::point2<std::ptrdiff_t> pt = boost::gil::tiff_read_dimensions(file_name);
  width = pt.x;
  height = pt.y;
}

void read_image(const std::string & file_name,
  intensity_host_view_t intensities){
  assert(file_name!="");
  boost::gil::gray8_image_t img;
  boost::gil::tiff_read_and_convert_image(file_name.c_str(),img);
  const size_t width = img.width();
  const size_t height = img.height();
  boost::gil::gray8c_view_t img_view = boost::gil::const_view(img);
  for (size_t y=0; y<height; ++y) {
    boost::gil::gray8c_view_t::x_iterator src_it = img_view.row_begin(y);
    for (size_t x=0; x<width;++x){
      intensities(x,y) = src_it[x];
    }
  }
}


void write_image(const std::string & file_name,
  const size_t width,
  const size_t height,
  intensity_host_view_t intensities){
  assert(file_name!="");
  boost::gil::gray8_image_t img(width,height);
  boost::gil::gray8_view_t img_view = boost::gil::view(img);
  for (size_t y=0; y<height; ++y) {
    boost::gil::gray8_view_t::x_iterator src_it = img_view.row_begin(y);
    for (size_t x=0; x<width;++x){
      src_it[x] = (boost::gil::gray8_pixel_t)(intensities(x,y));
    }
  }
  boost::gil::tiff_write_view(file_name.c_str(), img_view);
}


} // end namespace miniDICE