//----------------------------------------------------------------------------------
// File : IGraphicsRenderer.h
// Desc : Any versions a D3D rendering or OGL rendering interface.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_INTERFACE_GRAPHICS_RENDERER_
#define  _INCLUDED_INTERFACE_GRAPHICS_RENDERER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <IRenderable.h>
#include  <list>
#include  <vector>

//----------------------------------------------------------------------------------
// abstract interface
//----------------------------------------------------------------------------------
class  IGraphicsRenderer
{

protected:
		//----------------------------------------------------------------------------------
		// protected methods
		//----------------------------------------------------------------------------------
		std::list<IRenderable*>  m_renderObjects;

public:
		//----------------------------------------------------------------------------------
		// public methods
		//----------------------------------------------------------------------------------
		template<typename  T>  UINT  CreateRenderObjects( T  type );
		virtual  void  Rendering()const = 0;

};// class  IGraphicsRenderer


#endif // ! _INCLUDED_INTERFACE_GRAPHICS_RENDERER_