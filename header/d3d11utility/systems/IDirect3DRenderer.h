//----------------------------------------------------------------------------------
// file : IDirect3DRenderer.h
// desc : 
//----------------------------------------------------------------------------------


#ifndef  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_
#define  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility/Interface.h>
#include  <d3d11utility/Systems\IDirect3D.h>
#include  <IGraphicsRenderer.h>
#include  <d3d11utility/components/Renderable.h>
#include  <d3d11utility/systems/FbxLoader.h>
#include  <unordered_map>
#include  <vector>
#include  <Singleton.h>
#include  <wrl/client.h>


namespace  D3D11Utility
{
		enum  eMeshId
		{
				eCube,
				eSphere,
		};// enum eMeshId

		namespace  Systems
		{
				using  namespace  Microsoft;

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
						std::vector<Graphics::VertexBuffer*>  m_vertexBufferList;
						std::vector<Graphics::VertexShader*>  m_vertexShaderList;
						std::vector<Graphics::GeometryShader*>  m_geometryShaderList;
						std::vector<Graphics::PixelShader*>  m_pixelShaderList;
						std::unordered_map<size_t, FbxLoader>  m_fbxLoaderMap;

						/* rendering variables */
						WRL::ComPtr<ID3D11RenderTargetView>  m_pRTView = nullptr;
						WRL::ComPtr<ID3D11RenderTargetView>  m_pClusterRTView = nullptr;
						WRL::ComPtr<ID3D11DepthStencilView>  m_pDSView = nullptr;
						WRL::ComPtr<ID3D11Texture2D>  m_pRTTexture = nullptr;
						WRL::ComPtr<ID3D11Texture2D>  m_pDSTexture = nullptr;
						WRL::ComPtr<ID3D11Texture3D>  m_pClusterRTTexture = nullptr;
						WRL::ComPtr<ID3D11ShaderResourceView>  m_pRTShaderResourceView = nullptr;
						WRL::ComPtr<ID3D11ShaderResourceView>  m_pClusterRTShaderResourceView = nullptr;
						WRL::ComPtr<ID3D11ShaderResourceView>  m_pDSShaderResourceView = nullptr;
						WRL::ComPtr<ID3D11ShaderResourceView>  m_pSTShaderResourceView = nullptr;
						WRL::ComPtr<ID3D11UnorderedAccessView>  m_pClusterUAV = nullptr;
						WRL::ComPtr<ID3D11DepthStencilState>  m_pDepthStencilState;
						std::vector<RenderTarget>  m_renderTagets;
						ID3D11Buffer*  m_pVtxBuffer = nullptr;
						ID3D11SamplerState*  m_sampler = nullptr;
						ID3D11RasterizerState*  m_rasterStates[Graphics::MaxRasterMode];
						Graphics::VertexBuffer*  m_pVertexPtLight;
						Graphics::VertexShader*  m_pVShader;
						Graphics::PixelShader*  m_pPShader;
						Graphics::VertexShader*  m_pClusterVShader;
						Graphics::GeometryShader*  m_pClusterGShader;
						Graphics::PixelShader*  m_pClusterPShader;
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
						Graphics::VertexBuffer*  CreateVertexBuffer( const  FbxLoader&  fbxLoader );
						Graphics::VertexShader*  CreateVertexShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szVSModel = "vs_5_0", UINT  numLayouts = 0, D3D11_INPUT_ELEMENT_DESC*  layouts = nullptr );
						Graphics::GeometryShader*  CreateGeometryShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szGSModel = "gs_5_0" );
						Graphics::PixelShader*  CreatePixelShader( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szPSModel = "ps_5_0" );
						Graphics::VertexBuffer*  LoadFbxModel( FbxString  fileName );

						/* Getter */
						Graphics::VertexBuffer*  GetVertexBuffer( Graphics::MeshId  index );
						Graphics::VertexShader*  GetVertexShader( size_t  index )const;
						Graphics::GeometryShader*  GetGeometryShader( size_t  index )const;
						Graphics::PixelShader*  GetPixelShader( size_t  index )const;
						FbxLoader  GetFbxLoader( std::string  fileName )const;

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