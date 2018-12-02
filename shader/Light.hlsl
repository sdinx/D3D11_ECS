//-----------------------------------------------------------------------------------
// Light structure
//-----------------------------------------------------------------------------------
struct LightVSInput
{
    float4 position : POSITION;
};


struct LightPSInput
{
    float4 position : SV_POSITION;
};


struct LightPSOutput
{
    uint2 cluster : SV_Target;
};


LightPSInput vsmain( LightVSInput IN )
{
    LightPSInput OUT = (LightPSInput) 0;

    return OUT;
}


LightPSOutput psmain( LightPSInput IN )
{
    LightPSOutput OUT = (LightPSOutput) 0;


    return OUT;
}