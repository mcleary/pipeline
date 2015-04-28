// Copyright NVIDIA Corporation 2013-2015
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

#if defined(GL_VERSION_4_3)

#include <dp/rix/gl/inc/VertexCacheGL.h>
#include <dp/rix/gl/RiXGL.h>
#include <dp/rix/gl/inc/RendererGLConfig.h>
#include <dp/rix/gl/inc/VertexStateGL.h>
#include <dp/rix/gl/inc/RenderEngineGLDrawCall.h>
#include <dp/rix/gl/inc/GeometryInstanceGL.h>
#include <dp/rix/gl/inc/ProgramPipelineGL.h>
#include <GL/glew.h>

namespace dp
{
  namespace rix
  {
    namespace gl
    {
      class IndicesGL;
      class VertexAttributesGL;
      template <typename VertexCache> class RenderEngineGLImpl;

      template <> struct VertexAttributeCache<VertexCacheVBOVAB> : public VertexStateGL
      {
        struct AttributeCacheEntry
        {
          GLuint  vbo;
          GLvoid* offset;
        };

        struct GeometryInstanceCacheEntry : public GeometryInstanceGL::Cache
        {
          VertexFormatId           m_formatId;
          VertexAttributesGLHandle m_vertexAttributesHandle;
          IndicesGLHandle          m_indicesHandle;
          AttributeCacheEntry      m_attributeCacheIndices;
          AttributeCacheEntry*     m_attributeCacheEntry;

          RenderEngineGLDrawCall   m_drawCall;
        };


        void beginFrame();
        void endFrame();

        void updateGeometryInstanceCacheEntry( GeometryInstanceGLHandle gi, GeometryInstanceCacheEntry& geometryInstanceCacheEntry, AttributeCacheEntry *attributeCacheEntry );
        void renderGeometryInstance( GeometryInstanceCacheEntry const& giCacheEntry );

        VertexFormat *m_currentVertexFormat;
      };

      inline void VertexAttributeCache<VertexCacheVBOVAB>::updateGeometryInstanceCacheEntry( GeometryInstanceGLHandle gi, GeometryInstanceCacheEntry& geometryInstanceCacheEntry, AttributeCacheEntry *attributeCacheEntry )
      {
        VertexAttributesGLSharedHandle const& vertexAttributes = gi->getGeometry()->getVertexAttributes();

        geometryInstanceCacheEntry.m_vertexAttributesHandle = vertexAttributes.get();

        VertexDataGLHandle vertexData = vertexAttributes->getVertexDataGLHandle();
        geometryInstanceCacheEntry.m_formatId = registerVertexFormat( vertexAttributes.get(), gi->m_programPipeline->m_activeAttributeMask );
        const VertexFormat &vertexFormat = m_vertexFormats[geometryInstanceCacheEntry.m_formatId];

        for ( unsigned int index = 0; index < vertexFormat.m_numStreams; ++index )
        {
          unsigned int streamId = vertexFormat.m_streamIds[index];
          AttributeCacheEntry& cacheEntry = attributeCacheEntry[index];
          BufferGL *buffer = vertexData->m_data[streamId].m_buffer;

          if ( buffer )
          {
            DP_ASSERT( buffer->getBuffer() );
            cacheEntry.vbo  = buffer->getBuffer()->getGLId();
            cacheEntry.offset = reinterpret_cast<GLvoid *>(vertexData->m_data[streamId].m_offset);
          }
          else
          {
            cacheEntry.vbo = 0;
            cacheEntry.offset = 0;
          }
        }

        geometryInstanceCacheEntry.m_attributeCacheEntry = attributeCacheEntry;

        // update indices data
        IndicesGLSharedHandle const& indices = gi->getGeometry()->getIndices();
        geometryInstanceCacheEntry.m_indicesHandle = indices.get();
        if ( indices && indices->getBufferHandle() )
        {
          DP_ASSERT( indices->getBufferHandle()->getBuffer() );
          geometryInstanceCacheEntry.m_attributeCacheIndices.vbo = indices->getBufferHandle()->getBuffer()->getGLId();
          geometryInstanceCacheEntry.m_attributeCacheIndices.offset = 0;
        }
        else
        {
          geometryInstanceCacheEntry.m_attributeCacheIndices.vbo = 0;
          geometryInstanceCacheEntry.m_attributeCacheIndices.offset = 0;
        }

        // update draw call
        geometryInstanceCacheEntry.m_drawCall.updateDrawCall( gi );
      }

      inline void VertexAttributeCache<VertexCacheVBOVAB>::beginFrame()
      {
        m_currentIS = nullptr;
        m_currentVA = nullptr;
      }

      inline void VertexAttributeCache<VertexCacheVBOVAB>::endFrame()
      {
      }

      inline void VertexAttributeCache<VertexCacheVBOVAB>::renderGeometryInstance( GeometryInstanceCacheEntry const& giCacheEntry )
      {
        // update format/attributes/indices
        if ( setVertexFormat( giCacheEntry.m_formatId) )
        {
          m_currentVertexFormat = &m_vertexFormats[giCacheEntry.m_formatId];
          setVertexFormatMask( m_currentVertexFormat->m_enabledMask );
          for ( unsigned int index = 0; index < m_currentVertexFormat->m_numAttributes; ++index )
          {
            VertexAttribute &entry = m_currentVertexFormat->m_attributes[index];
            glVertexAttribFormat( entry.index, entry.numComponents, entry.type, entry.normalized, entry.offset );
            glVertexAttribBinding( entry.index, entry.streamId );
          }
        }
        if ( m_currentVA != giCacheEntry.m_vertexAttributesHandle )
        {
          m_currentVA = giCacheEntry.m_vertexAttributesHandle;

          AttributeCacheEntry *attributeCacheEntry = giCacheEntry.m_attributeCacheEntry;
          for ( unsigned int streamId = 0; streamId < m_currentVertexFormat->m_numStreams; ++streamId )
          {
            AttributeCacheEntry const& entry = attributeCacheEntry[streamId];
            glBindVertexBuffer( streamId, entry.vbo, reinterpret_cast<GLintptr>(entry.offset), m_currentVertexFormat->m_streamStrides[streamId] );
          }
        }
        if ( giCacheEntry.m_indicesHandle )
        {
          if ( m_currentIS != giCacheEntry.m_indicesHandle )
          {
            m_currentIS = giCacheEntry.m_indicesHandle;
            setElementBuffer( giCacheEntry.m_attributeCacheIndices.vbo );
          }
        }

        giCacheEntry.m_drawCall.draw( this );
      }

    } // namespace gl
  } // namespace rix
} // namespace dp

// GL_VERSION_4_3
#endif
