//----------------------------------------------------------------------------------
// file: TextureManager.h
// desc: テクスチャの作成と管理を行うシングルトンクラス
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
#include  <Singleton.h>
#include  <D3D11Utility.h>
#include  <d3d11utility\Interface.h>
#include  <d3d11utility\Graphics\Texture.h>
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
						nPixelBytes( pb ),
						byBuffers( w*h*pb )
				{}
				std::vector<BYTE>  byBuffers;
				INT  nWidth;
				INT  nHeight;
				INT  nPixelBytes;
		};// struct Image

		namespace  Systems
		{
				using  SystemId = int;

				class  TextureManager:  public  _Singleton<TextureManager>
				{
						friend  class  _Singleton<TextureManager>;

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

						Image*  LoadImageFile( const  std::wstring  &filename );

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						Graphics::TextureId  CreateTexture( const  std::wstring  &filename );

						void  SetDiffuse( Graphics::TextureId  textureId );
						void  SetNormal( Graphics::TextureId  textureId );
						void  Release();

				};// class TextureManager

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_TEXTURE_MANAGER_