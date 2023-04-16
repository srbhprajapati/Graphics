float4 vsMain(uint id : SV_VertexID) : SV_POSITION
{
	if (id == 0)
	{
		return float4(-0.25f, -0.25f, 0.5f, 1.0f);
	}
	else if (id == 1)
	{
		return float4(-0.25f, 0.25f, 0.5f, 1.0f);
	}
	else
	{
		return float4(0.25f, 0.25f, 0.5f, 1.0f);
	}
}

float4 psMain() : SV_TARGET
{
	return float4(0, 0, 0, 1);
}