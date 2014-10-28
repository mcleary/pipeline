// Copyright NVIDIA Corporation 2013
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

#include <dp/util/Observer.h>
#include <dp/util/SharedPtr.h>

namespace dp
{
  namespace sg
  {
    namespace xbar
    {
      SMART_TYPES( SceneObserver );

      class SceneTree;
      typedef SceneTree* SceneTreeWeakPtr;

      class SceneObserver : public dp::util::Observer
      {
      public:
        static SmartSceneObserver create( SceneTreeWeakPtr sceneTree );
        virtual ~SceneObserver();

      protected:
        SceneObserver( SceneTreeWeakPtr sceneTree );

        virtual void onNotify( const dp::util::Event &event, dp::util::Payload *payload );
        virtual void onDestroyed( const dp::util::Subject& subject, dp::util::Payload* payload );

      protected:
        SceneTreeWeakPtr m_sceneTree;
      };

    } // namespace xbar
  } // namespace sg
} // namespace dp
