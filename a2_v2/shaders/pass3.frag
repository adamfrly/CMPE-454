// Pass 3 fragment shader
//
// Output fragment colour based using
//    (a) Cel shaded diffuse surface
//    (b) wide silhouette in black

#version 300 es

uniform mediump vec3 lightDir;     // direction toward the light in the VCS
uniform mediump vec2 texCoordInc;  // texture coord difference between adjacent texels

in mediump vec2 texCoords;              // texture coordinates at this fragment

// The following four textures are now available and can be sampled
// using 'texCoords'

uniform sampler2D colourSampler;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;
uniform sampler2D laplacianSampler;

out mediump vec4 outputColour;          // the output fragment colour as RGBA with A=1


void main()

{
  mediump vec2 dummy = texCoords;  // REMOVE THIS ... It's just here because MacOS complains otherwise

  // [0 marks] Look up values for the depth and Laplacian.  Use only
  // the R component of the texture as texture2D( ... ).r

  // YOUR CODE HERE
  mediump vec3 depthVal = texture(depthSampler).r;			//Not sure if this is right or if it should even be vec1
  mediump vec3 lapVal = texture(laplacianSampler).r;

  // [1 mark] Discard the fragment if it is a background pixel not
  // near the silhouette of the object.

  // YOUR CODE HERE
  if(depthVal == 1){
		//discard as you're in the background
  }

  // [0 marks] Look up value for the colour and normal.  Use the RGB
  // components of the texture as texture2D( ... ).rgb or texture2D( ... ).xyz.

  // YOUR CODE HERE
  mediump vec3 texColour = texture( colorSampler ).rgb;
  mediump vec3 normVal = texture( normalSampler ).xyz;

  // [2 marks] Compute Cel shading, in which the diffusely shaded
  // colour is quantized into four possible values.  Do not allow the
  // diffuse component, N dot L, to be below 0.2.  That will provide
  // some ambient shading.  Your code should use the 'numQuata' below
  // to have that many divisions of quanta of colour.  Do not use '3'
  // in your code; use 'numQuanta'.  Your code should be very efficient.

  const int numQuanta = 3;		//Need to use this to get a "blending factor" to the diffuse color

  // YOUR CODE HERE
  mediump float NdotL = dot( normalize(normVal), lightDir );	//Used to compute N dot L
  mediump vec4 outputColour;
  
  //Don't know if we need this:
  if (NdotL < 0.2){
    NdotL = 0.0;
  }
  
  outputColour = (1/numQuanta)*NdotL;		//Something like this****
  
  
  if (NdotL > 0.95)														//Need to change from if/else statements to make efficient + get marks
    outputColour = 0.95 * vec4(colour,1.0); // 0.95 - 1.00  ->  0.95
  else if (NdotL > 0.70)
    outputColour = 0.70 * vec4(colour,1.0); // 0.70 - 0.95  ->  0.70
  else if (NdotL > 0.40)
    outputColour = 0.40 * vec4(colour,1.0); // 0.40 - 0.70  ->  0.40
  else
    outputColour = 0.10 * vec4(colour,1.0); // 0.00 - 0.40  ->  0.10
	

  // [2 marks] Look at the fragments in the 3x3 neighbourhood of
  // this fragment.  Your code should use the 'kernelRadius'
  // below and check all fragments in the range
  //
  //    [-kernelRadius,+kernelRadius] x [-kernelRadius,+kernelRadius]
  //
  // around this fragment.
  //
  // Find the neighbouring fragments with a Laplacian beyond some
  // threshold.  Of those fragments, find the distance to the closest
  // one.  That distance, divided by the maximum possible distance
  // inside the kernel, is the blending factor.
  //
  // You can use a large kernelRadius here (e.g. 10) to see that
  // blending is being done correctly.  Do not use '3.0' or '-0.1' in
  // your code; use 'kernelRadius' and 'threshold'.

  const mediump float kernelRadius = 3.0;
  const mediump float threshold = -0.1;

  // YOUR CODE HERE
  

  // [1 mark] Output the fragment colour.  If there is an edge
  // fragment in the 3x3 neighbourhood of this fragment, output a grey
  // colour based on the blending factor.  The grey should be
  // completely black for an edge fragment, and should blend to the
  // Phong colour as distance from the edge increases.  If these is no
  // edge in the neighbourhood, output the cel-shaded colour.
  
  // YOUR CODE HERE

  outputColour = vec4( 1.0, 0.0, 1.0, 1.0 );
}
