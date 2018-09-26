//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\TextureManager.h>
#include  <wrl/client.h>
#include  <cassert>
#include  <memory>
#include  <wincodec.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


TextureManager::TextureManager()
{
		// テクスチャ未設定時の初期テクスチャ
		CreateTexture( L"res/none.png" );
}


TextureManager::~TextureManager()
{
		m_textures.clear();
}


Image*  TextureManager::LoadImageFile( const  std::wstring  &filename )
{
		IWICImagingFactory  *iwicImageFactory;
		HRESULT hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS( &iwicImageFactory )
		);

		// decoderを作ってファイルを渡す
		Microsoft::WRL::ComPtr<IWICBitmapDecoder>  decoder;
		hr = iwicImageFactory->CreateDecoderFromFilename( filename.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder );
		if ( FAILED( hr ) ) {
				return  nullptr;
		}

		// decoderからframeを取得
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
		hr = decoder->GetFrame( 0, &frame );
		if ( FAILED( hr ) ) {
				return  nullptr;
		}

		// フレームからサイズとピクセルフォーマットとデータを得る
		UINT width, height;
		hr = frame->GetSize( &width, &height );
		if ( FAILED( hr ) ) {
				return  nullptr;
		}

		assert( width > 0 && height > 0 );

		// Determine format
		WICPixelFormatGUID pixelFormat;
		hr = frame->GetPixelFormat( &pixelFormat );
		if ( FAILED( hr ) ) {
				return  nullptr;
		}

		if ( pixelFormat != GUID_WICPixelFormat32bppRGBA ) {
				// 変換する
				Microsoft::WRL::ComPtr<IWICFormatConverter> FC;
				hr = iwicImageFactory->CreateFormatConverter( &FC );
				if ( FAILED( hr ) ) {
						return  nullptr;
				}

				hr = FC->Initialize( frame.Get(), GUID_WICPixelFormat32bppRGBA
						, WICBitmapDitherTypeErrorDiffusion
						, 0, 0, WICBitmapPaletteTypeCustom );
				if ( FAILED( hr ) ) {
						return  nullptr;
				}

				// copy
				Image*  image = new  Image( width, height, 4 );
				FC->CopyPixels( 0, image->nPixelBytes*image->nWidth, image->byBuffers.size(), &image->byBuffers[0] );
				return  image;
		}
		else {
				// copy
				Image*  image = new  Image( width, height, 4 );
				frame->CopyPixels( 0, image->nPixelBytes*image->nWidth, image->byBuffers.size(), &image->byBuffers[0] );
				return  image;

		}

		return  nullptr;
}


Graphics::TextureId  TextureManager::CreateTexture( const  std::wstring  &filename )
{
		ID3D11Texture2D*  texture = nullptr;
		ID3D11ShaderResourceView*  shaderResourceView = nullptr;
		ID3D11SamplerState*  sampler = nullptr;
		Image*  image = LoadImageFile( filename );
		if ( image == nullptr )
				return  TEXTURE_ID_INVALID;

		D3D11_TEXTURE2D_DESC  desc;
		desc.Width = image->nWidth;
		desc.Height = image->nHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA  initData;
		initData.pSysMem = &image->byBuffers[0];
		initData.SysMemPitch = image->nWidth * image->nPixelBytes;
		initData.SysMemSlicePitch = image->byBuffers.size();

		auto hr = pd3dDevice->CreateTexture2D( &desc, &initData, &texture );
		if ( FAILED( hr ) ) {
				return  TEXTURE_ID_INVALID;
		}// end if

		D3D11_SHADER_RESOURCE_VIEW_DESC  SRVDesc = { };
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = pd3dDevice->CreateShaderResourceView( texture, &SRVDesc, &shaderResourceView );
		if ( FAILED( hr ) )
		{
				return  TEXTURE_ID_INVALID;
		}// end if

		D3D11_SAMPLER_DESC  samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		hr = pd3dDevice->CreateSamplerState( &samplerDesc, &sampler );
		if ( FAILED( hr ) )
		{
				return  TEXTURE_ID_INVALID;
		}// end if

		// テクスチャの追加
		m_textures.push_back( new  Graphics::Texture( texture, shaderResourceView, sampler ) );

		// 要素番号を返却
		return  ( m_textures.size() - 1 );
}


void  TextureManager::SetTexture( Graphics::TextureId  textureId )
{
		// note: 高速化時の省略対象
		if ( textureId >= ( int ) m_textures.size() )
				return;

		m_textures[textureId]->SetTexture();
}


void  TextureManager::Release()
{

}