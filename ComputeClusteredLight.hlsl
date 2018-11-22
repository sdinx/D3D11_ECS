
const uint MAX_LIGHT_COUNT = 10000;

[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void main( uint3 DTid : SV_DispatchThreadID )
{

}