//----------------------------------------------------------------------------------
// file: TextureManager.h
// desc: テクスチャの作成と管理を行う
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_TEXTURE_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_TEXTURE_MANAGER_

//----------------------------------------------------------------------------------
// include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Interface.h>
#include  <D3D11Utility\Graphics\Texture.h>
#include  <string>
#include  <vector>


namespace  D3D11Utility
{
		struct Image
		{
				Image() :
						nWidth( 0 ),
						nHeight( 0 ),
						nPixelBytes( 0 )
				{}
				Image( INT  w, INT  h, INT  pb ) :
						nWidth( w ),
						nHeight( h ),
						nPixelBytes( pb )
				{}
				std::vector<BYTE>  byBuffer;
				INT  nWidth;
				INT  nHeight;
				INT  nPixelBytes;
		};// struct Image

		namespace  Systems
		{
				using  SystemId = int;

				class  TextureManager
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						TextureManager();
						virtual  ~TextureManager();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						std::vector<Graphics::Texture*>  m_textures;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------

#if  defined( _UNICODE ) || ( UNICODE )
						Image*  LoadImageFile( const  std::wstring  &filename );
#else// Multibyte
						Image*  LoadImageFile( const  std::string  &filename );
#endif// UNICODE

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

#if  defined( _UNICODE ) || ( UNICODE )
						Graphics::TextureId  CreateTexture( const  std::wstring  &filename );
#else// Multibyte
						Graphics::TextureId  CreateTexture( const  std::string  &filename );
#endif// UNICODE

						void  Release();

				};// class TextureManager

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_TEXTURE_MANAGER_