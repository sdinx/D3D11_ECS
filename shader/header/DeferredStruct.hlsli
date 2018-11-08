struct  VSInput
{
		float3  position : POSITION;
		float2  texcoord : TEXCOORD0;
		float4  color : COLOR;
};


struct  PSInput
{
		float4  position : SV_POSITION;
		float2  texcoord : TEXCOORD0;
		float4  color : COLOR;
};

