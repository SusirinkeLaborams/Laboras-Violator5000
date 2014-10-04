struct PixelInput
{
	float4 position : SV_POSITION;
	float brightness : BRIGHTNESS;
};

float4 main(PixelInput input) : SV_TARGET
{
	return float4(input.brightness, input.brightness, input.brightness, 1.0f);
}