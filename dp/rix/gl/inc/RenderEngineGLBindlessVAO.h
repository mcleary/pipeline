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


#pragma once

#include <dp/rix/gl/inc/VertexStateGL.h>
#include <dp/rix/gl/inc/RenderEngineGLDrawCall.h>

namespace dp
{
  namespace rix
  {
    namespace gl
    {

      template <> struct VertexAttributeCache<VertexCacheBindlessVAO> : public VertexStateGL
      {
        struct AttributeCacheEntry
        {
        };

        struct VertexDataCache : public dp::rix::core::HandledObject
        {
          VertexDataCache()
            : m_vao(0)
          {
          }

          ~VertexDataCache()
          {
            glDeleteVertexArrays( 1, &m_vao );
          }

          GLuint               m_vao;
        };


        struct GeometryInstanceCacheEntry : public GeometryInstanceGL::Cache
        {
          GLuint                   m_vao;
          RenderEngineGLDrawCall   m_drawCall;
        };

        void beginFrame();
        void endFrame();

        void update( GeometryInstanceGLHandle gi, VertexDataCache& vertexCache );

        void updateGeometryInstanceCacheEntry( GeometryInstanceGLHandle gi, GeometryInstanceCacheEntry& geometryInstanceCacheEntry, AttributeCacheEntry *attributeCacheEntry );
        void renderGeometryInstance( GeometryInstanceCacheEntry const& giCacheEntry );
      };
    } // namespace gl
  } // namespace rix
} // namespace dp
