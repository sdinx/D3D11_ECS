//----------------------------------------------------------------------------------
// File : IDrawable.h
// Desc : The interface class for an object rendering.
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_INTERFACE_DRAWABLE_
#define  _INCLUDED_INTERFACE_DRAWABLE_

//----------------------------------------------------------------------------------
// abstract interface
//----------------------------------------------------------------------------------
class  IRenderable
{

public:
		//----------------------------------------------------------------------------------
		// other
		//----------------------------------------------------------------------------------
		IRenderable()
		{}
		virtual  ~IRenderable()
		{}

protected:
		//----------------------------------------------------------------------------------
		// protected variables
		//----------------------------------------------------------------------------------
		bool  m_isRendering = false;


public:
		//----------------------------------------------------------------------------------
		// public variables
		//----------------------------------------------------------------------------------
		/* NOTHING */

protected:
		//----------------------------------------------------------------------------------
		// protected methods
		//----------------------------------------------------------------------------------
		/* NOTHING */

public:
		//----------------------------------------------------------------------------------
		// public methods
		//----------------------------------------------------------------------------------
		virtual  void  Rendering()const = 0;


}; // class IDrawable


#endif // ! _INCLUDED_INTERFACE_DRAWABLE_