cbuffer ConstantBuffer
{
	matrix viewProjectionMatrix;
	float lifeTime;
	float currentTime;
};

struct VertexInput
{
	float4 basePosition : BASE_POSITION;
	float2 instancePosition : INSTANCE_POSITION;
	float spawnTime : INSTANCE_SPAWN_TIME;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float brightness : BRIGHTNESS;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.position = input.basePosition + float4(input.instancePosition.x, input.instancePosition.y, 0.0f, 0.0f);
	output.position = mul(output.position, viewProjectionMatrix);

	output.brightness = 1.0f - (currentTime - input.spawnTime) / lifeTime;

	return output;
}