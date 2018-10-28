struct  VS_OUTPUT
{
		float4  pos : SV_Position;
		float3  nor : NORMAL;
		float  dep : DEPTH_VIEW_SPACE;
};

struct  PS_OUTPUT
{
		float  depth : SV_Target0;
		float3  normal : SV_Target1;
		float4  diffuse : SV_Target2;
		float  specular : SV_Target3;
};