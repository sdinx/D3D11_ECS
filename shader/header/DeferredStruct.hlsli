struct  VSDeferredInput
{
		float3  position : POSITION;
		float2  texcoord : TEXCOORD0;
		float4  color : COLOR;
};


struct  PSDeferredInput
{
		float4  position : SV_POSITION;
		float2  texcoord : TEXCOORD0;
		float4  color : COLOR;
};

