
struct VertexIn
{
	float4 position : POSITION;
};

struct PixelIn
{
	float4 position : SV_POSITION;
};

PixelIn vsMain(VertexIn vert)
{
	PixelIn output;
	output.position = vert.position;

	return output;

}

float4 psMain() : SV_TARGET
{
	return float4(0, 1, 0, 1);
}