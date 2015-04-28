// Copyright NVIDIA Corporation 2011-2015
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <dp/rix/gl/inc/DataTypeConversionGL.h>
#include <dp/rix/gl/inc/TextureGL2DArray.h>
#include <dp/util/SharedPtr.h>

namespace dp
{
  namespace rix
  {
    namespace gl
    {
      using namespace dp::rix::core;

      TextureHandle TextureGL2DArray::create( const TextureDescription &textureDescription )
      {
        return new TextureGL2DArray( textureDescription );
      }

      TextureGL2DArray::TextureGL2DArray( const TextureDescription& description )
        : TextureGL( dp::gl::Texture2DArray::create( getGLInternalFormat( description )
                                                   , getGLPixelFormat( description.m_pixelFormat, getGLInternalFormat( description ) )
                                                   , getGLDataType( description.m_dataType )
                                                   , static_cast<GLsizei>(description.m_width)
                                                   , static_cast<GLsizei>(description.m_height)
                                                   , static_cast<GLsizei>(description.m_layers) )
                   , description.m_mipmaps )
      {
        DP_ASSERT( description.m_depth == 0 );
      }

      void TextureGL2DArray::upload(unsigned int mipMapLevel, unsigned int layer, const void* data)
      {
        dp::util::shared_cast<dp::gl::Texture2DArray>( getTexture() )->setData( data, layer, mipMapLevel );
      }

    } // namespace gl
  } // namespace rix
} // namespace dp
