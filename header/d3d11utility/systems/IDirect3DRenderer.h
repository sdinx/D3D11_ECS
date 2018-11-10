//----------------------------------------------------------------------------------
// file : IDirect3DRenderer.h
// desc : 
//----------------------------------------------------------------------------------


#ifndef  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_
#define  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <D3D11Utility\Systems\IDirect3D.h>
#include  <IGraphicsRenderer.h>
#include  <IRenderable.h>
#include  <D3D11Utility\Graphics\VertexShader.h>
#include  <D3D11Utility\Graphics\GeometryShader.h>
#include  <D3D11Utility\Graphics\PixelShader.h>
#include  <vector>
#include  <Singleton.h>


namespace  D3D11Utility
{
		namespace  Systems
		{

				class  IDirect3DRenderer :public  IGraphicsRenderer, public  _Singleton<IDirect3DRenderer>
				{
						friend  class  _Singleton<IDirect3DRenderer>;

						struct  RenderTarget
						{
								enum  eViews
								{
										Depth_Stencil = 0,
										Normal,
										Diffuse,
										Specular,
								};

								RenderTarget() :
										m_pRTView( nullptr ),
										m_pTexture( nullptr ),
										m_pSRView( nullptr )
								{}
								~RenderTarget()
								{
										Release();
								}
								void  Release()
								{
										SafeRelease( m_pRTView );
										SafeRelease( m_pTexture );
										SafeRelease( m_pSRView );
								}

								eViews  m_numViews;
								ID3D11RenderTargetView* m_pRTView = nullptr;
								ID3D11Texture2D*  m_pTexture = nullptr;
								ID3D11ShaderResourceView*  m_pSRView = nullptr;
						};

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						IDirect3DRenderer( ComponentManager*  pComponentManagerInstance = nullptr );
						~IDirect3DRenderer();

						using  eRTViews = RenderTarget::eViews;

				private:
						//----------------------------------------------------------------------------------
						// private  variables
						//----------------------------------------------------------------------------------

						static  const  unsigned  RT_ARRAY_COUNTS = 3;

						std::shared_ptr<IDirect3D>  m_pID3D;
						ComponentManager*  m_componentManager;
						std::vector<Graphics::VertexShader*>  m_vertexShaderList;
						std::vector<Graphics::GeometryShader*>  m_geometryShaderList;
						std::vector<Graphics::PixelShader*>  m_pixelShaderList;

						/* rendering variables */
						ID3D11RenderTargetView* m_pRTView = nullptr;
						ID3D11DepthStencilView*  m_pDSView = nullptr;
						ID3D11Texture2D*  m_pRTTexture = nullptr;
						ID3D11Texture2D*  m_pDSTexture = nullptr;
						ID3D11ShaderResourceView*  m_pRTShaderResourceView = nullptr;
						ID3D11ShaderResourceView*  m_pDSShaderResourceView = nullptr;
						ID3D11ShaderResourceView*  m_pSTShaderResourceView = nullptr;
						std::vector<RenderTarget>  m_renderTagets;
						ID3D11Buffer*  m_pVtxBuffer = nullptr;
						ID3D11SamplerState*  m_sampler = nullptr;
						ID3D11RasterizerState*  m_rasterState = nullptr;
						Graphics::VertexShader*  m_pVShader;
						Graphics::PixelShader*  m_pPShader;
						FLOAT  m_fClearColors[4] = { 0.0f,0.125f,0.3f,1.0f };

				public:
						//----------------------------------------------------------------------------------
						// public  variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private  methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public  methods
						//----------------------------------------------------------------------------------

						// レンダーターゲットビューの生成 ( MRT用 )
						HRESULT  CreateMultipleRenderTargetView();
						Graphics::VertexShader*  CreateVertexShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szVSModel = "vs_5_0", UINT  numLayouts = 0, D3D11_INPUT_ELEMENT_DESC*  layouts = nullptr );
						Graphics::GeometryShader*  CreateGeometryShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szGSModel = "gs_5_0" );
						Graphics::PixelShader*  CreatePixelShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szPSModel = "ps_5_0" );

						/* Getter */
						Graphics::VertexShader*  GetVertexShader( size_t  index );
						Graphics::GeometryShader*  GetGeometryShader( size_t  index );
						Graphics::PixelShader*  GetPixelShader( size_t  index );

						void  Release();
						void  Rendering()const;

						// index
						void  SetComponentManager( ComponentManager*  manager )
						{
								m_componentManager = manager;
						}

				};// class IDirect3DRenderer

		}// namespace Systems
}// namespace D3D11Utility

#endif // ! _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_