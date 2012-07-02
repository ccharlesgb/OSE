uniform sampler2D texture;

vec4 ComputeDistancesPS(vec4 TexCoord)
{
	  vec4 color = sampler2D(texture, TexCoord);
	  //compute distance from center
	  float distance = color.a>0.3f?length(TexCoord - 0.5f):1.0f;
	  //save it to the Red channel
	  distance *= renderTargetSize.x;
      return vec4(distance,0,0,1);
}

vec4 DistortPS(float2 TexCoord  : TEXCOORD0)
{
	  //translate u and v into [-1 , 1] domain
	  float u0 = TexCoord.x * 2 - 1;
	  float v0 = TexCoord.y * 2 - 1;
	  
	  //then, as u0 approaches 0 (the center), v should also approach 0 
	  v0 = v0 * abs(u0);

      //convert back from [-1,1] domain to [0,1] domain
	  v0 = (v0 + 1) / 2;

	  //we now have the coordinates for reading from the initial image
	  float2 newCoords = float2(TexCoord.x, v0);

	  //read for both horizontal and vertical direction and store them in separate channels
	  float horizontal = sampler2D(texture, newCoords).r;
	  float vertical = sampler2D(texture, newCoords.yx).r;
      return vec4(horizontal,vertical ,0,1);
}


float GetShadowDistanceH(float2 TexCoord, float displacementV)
{
		float u = TexCoord.x;
		float v = TexCoord.y;

		u = abs(u-0.5f) * 2;
		v = v * 2 - 1;
		float v0 = v/u;
		v0+=displacementV;
		v0 = (v0 + 1) / 2;
		
		float2 newCoords = float2(TexCoord.x,v0);
		//horizontal info was stored in the Red component
		return sampler2D(shadowMapSampler, newCoords).r;
}

float GetShadowDistanceV(float2 TexCoord, float displacementV)
{
		float u = TexCoord.y;
		float v = TexCoord.x;
		
		u = abs(u-0.5f) * 2;
		v = v * 2 - 1;
		float v0 = v/u;
		v0+=displacementV;
		v0 = (v0 + 1) / 2;
		
		float2 newCoords = float2(TexCoord.y,v0);
		//vertical info was stored in the Green component
		return sampler2D(shadowMapSampler, newCoords).g;
}

vec4 DrawShadowsPS(float2 TexCoord  : TEXCOORD0)
{
	  // distance of this pixel from the center
	  float distance = length(TexCoord - 0.5f);
	  distance *= renderTargetSize.x;
	  //apply a 2-pixel bias
	  distance -=2;
	  
	  //distance stored in the shadow map
	  float shadowMapDistance;
	  
	  //coords in [-1,1]
	  float nY = 2.0f*( TexCoord.y - 0.5f);
	  float nX = 2.0f*( TexCoord.x - 0.5f);

	  //we use these to determine which quadrant we are in
	  if(abs(nY)<abs(nX))
	  {
		shadowMapDistance = GetShadowDistanceH(TexCoord,0);
	  }
	  else
	  {
	    shadowMapDistance = GetShadowDistanceV(TexCoord,0);
	  }
		
	  //if distance to this pixel is lower than distance from shadowMap, 
	  //then we are not in shadow
	  float light = distance < shadowMapDistance ? 1:0;

	  vec4 result = light;
	  result.b = length(TexCoord - 0.5f);
	  result.a = 1;
      return result;
}

/*
static const float minBlur = 0.0f;
static const float maxBlur = 5.0f;
static const int g_cKernelSize = 13;
static const float2 OffsetAndWeight[g_cKernelSize] =
{
    { -6, 0.002216 },
    { -5, 0.008764 },
    { -4, 0.026995 },
    { -3, 0.064759 },
    { -2, 0.120985 },
    { -1, 0.176033 },
    {  0, 0.199471 },
    {  1, 0.176033 },
    {  2, 0.120985 },
    {  3, 0.064759 },
    {  4, 0.026995 },
    {  5, 0.008764 },
    {  6, 0.002216 },
};

vec4 BlurHorizontallyPS(float2 TexCoord  : TEXCOORD0) : COLOR0
{
	  float sum=0;
	  float distance = sampler2D( texture, TexCoord).b;
	  
      for (int i = 0; i < g_cKernelSize; i++)
	  {    
        sum += sampler2D( texture, TexCoord + OffsetAndWeight[i].x * lerp(minBlur, maxBlur , distance)/renderTargetSize.x * float2(1,0) ).r * OffsetAndWeight[i].y;
      }
	  
	  vec4 result = sum;
	  result.b = distance;
	  result.a = 1;
      return result;
}

vec4 BlurVerticallyPS(float2 TexCoord  : TEXCOORD0) : COLOR0
{
	  float sum=0;
	  float distance = sampler2D( texture, TexCoord).b;
	  
      for (int i = 0; i < g_cKernelSize; i++)
	  {    
        sum += sampler2D( texture, TexCoord + OffsetAndWeight[i].x * lerp(minBlur, maxBlur , distance)/renderTargetSize.x * float2(0,1) ).r * OffsetAndWeight[i].y;
      }
	  
	  float d = 2 * length(TexCoord - 0.5f);
	  float attenuation = pow( saturate(1.0f - d),1.0f);
	  
	  vec4 result = sum * attenuation;
	  result.a = 1;
      return result;
}
*/