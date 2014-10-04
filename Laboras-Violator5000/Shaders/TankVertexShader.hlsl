cbuffer MatrixBuffer
{
	matrix transformMatrix;
};

float4 main(float4 position : POSITION) : SV_POSITION
{
	position = mul(position, transformMatrix);
	return position;
}