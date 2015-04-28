// Copyright NVIDIA Corporation 2014
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

#include <dp/gl/Config.h>
#include <dp/gl/Object.h>
#include <dp/gl/Buffer.h>

namespace dp
{
  namespace gl
  {
    class VertexArrayObject : public Object
    {
      public:
        DP_GL_API static VertexArrayObjectSharedPtr create();
        DP_GL_API virtual ~VertexArrayObject();

      public:
        template <typename T> void setAttribute( GLint location, std::vector<T> const& values, GLenum usage );
        template <typename T> void setAttribute( GLint location, BufferSharedPtr const& buffer );
        template <typename T> void setIndices( std::vector<T> const& indices, GLenum usage );
        template <typename T> void setIndices( BufferSharedPtr const& buffer );

      protected:
        DP_GL_API VertexArrayObject();

      private:
                  template <typename T> void setAttribute( std::map<GLint,BufferSharedPtr>::iterator it );
        DP_GL_API                       void setIndices();

      private:
        std::map<GLint,BufferSharedPtr>  m_attributes;
        BufferSharedPtr                  m_indices;
    };


    template <typename T>
    inline void VertexArrayObject::setAttribute( GLint location, std::vector<T> const& values, GLenum usage )
    {
      std::map<GLint,BufferSharedPtr>::iterator it = m_attributes.find( location );
      if ( it == m_attributes.end() )
      {
        it = m_attributes.insert( std::make_pair( location, Buffer::create(dp::gl::Buffer::CORE, usage, GL_ARRAY_BUFFER) ) ).first;
      }
      it->second->setSize(values.size() * sizeof(T));
      it->second->update(values.data());

      setAttribute<T>( it );
    }

    template <typename T>
    inline void VertexArrayObject::setAttribute( GLint location, BufferSharedPtr const& buffer )
    {
      std::map<GLint,BufferSharedPtr>::iterator it = m_attributes.find( location );
      if ( it == m_attributes.end() )
      {
        it = m_attributes.insert( std::make_pair( location, buffer ) ).first;
      }

      setAttribute<T>( it );
    }

    template <typename T>
    inline void VertexArrayObject::setAttribute( std::map<GLint,BufferSharedPtr>::iterator it )
    {
      glBindVertexArray( m_id );
      glBindBuffer( GL_ARRAY_BUFFER, it->second->getGLId() );

      if ( TypeTraits<typename TypeTraits<T>::componentType>::isInteger() )
      {
        glVertexAttribIPointer( it->first, TypeTraits<T>::componentCount(), TypeTraits<typename TypeTraits<T>::componentType>::glType(), 0, nullptr );
      }
      else
      {
        glVertexAttribPointer( it->first, TypeTraits<T>::componentCount(), TypeTraits<typename TypeTraits<T>::componentType>::glType(), GL_FALSE, 0, nullptr );
      }

      if ( !it->second || ( it->second->getSize() == 0 ) )
      {
        glDisableVertexAttribArray( it->first );
      }
      else
      {
        glEnableVertexAttribArray( it->first );
      }
    }

    template <typename T>
    inline void VertexArrayObject::setIndices( std::vector<T> const& indices, GLenum usage )
    {
      DP_STATIC_ASSERT( std::numeric_limits<T>::is_integer );
      if ( !m_indices )
      {
        m_indices = Buffer::create(dp::gl::Buffer::CORE, usage, GL_ELEMENT_ARRAY_BUFFER);
      }
      m_indices->setSize(indices.size() * sizeof(T));
      m_indices->update(indices.data());
    }

    template <typename T>
    inline void VertexArrayObject::setIndices( BufferSharedPtr const& buffer )
    {
      m_indices = buffer;
      setIndices();
    }

  } // namespace gl
} // namespace dp
