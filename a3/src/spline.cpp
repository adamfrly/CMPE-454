/* spline.cpp
 */


#include "spline.h"
#include "main.h"


#define DIVS_PER_SEG 20         // number of samples of on each spline segment (for arc length parameterization)

float Spline::M[][4][4] = {

  { { 0, 0, 0, 0},              // Linear
    { 0, 0, 0, 0},
    { 0,-1, 1, 0},
    { 0, 1, 0, 0}, },

  { { -0.5,  1.5, -1.5,  0.5 }, // Catmull-Rom
    {  1.0, -2.5,  2.0, -0.5 },
    { -0.5,  0.0,  0.5,  0.0 },
    {  0.0,  1.0,  0.0,  0.0 } },

  { { -0.1667, 0.5,   -0.5,    0.1667 }, // B-spline
    {  0.5,   -1.0,    0.5,    0.0    },
    { -0.5,    0.0,    0.5,    0.0    },
    {  0.1667, 0.6667, 0.1667, 0.0    } }
  
};


const char * Spline::MName[] = {
  "linear",
  "Catmull-Rom",
  "B-spline",
  ""
};


// Evaluate the spline at parameter 't'.  Return the value, tangent
// (i.e. first derivative), or binormal, depending on the
// 'returnDerivative' parameter.
// 
// The spline is continuous, so the first data point appears again
// after the last data point.  t=0 at the first data point and t=n-1
// at the n^th data point.  For t outside this range, use 't modulo n'.
//
// Use the change-of-basis matrix in M[currSpline].


vec3 Spline::eval( float t, evalType type )

{
  // YOUR CODE HERE
  
  
  // for t outside [0,data.size()), move t into range

  // Find the 4 control points for this t, and the u value in [0,1] for this interval.
    float u = t - floor(t);
    int siz = int(floor(t)) % data.size();
    vec3 q_back1, q, q_up1, q_up2;
    if (siz == 0) {
        q_back1 = data[data.size() - 1];
    }
    else {
        q_back1 = data[siz - 1];
    }

    q = data[siz];
    q_up1 = data[(siz + 1) % data.size()];
    q_up2 = data[(siz + 2) % data.size()];


    vec3 v[] = { q_back1, q, q_up1, q_up2 };    // Compute v
    
    // Compute Mv
    vec3 Mv[4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            Mv[i][j] = 0;
            for (int n = 0; n < 4; n++) {
                Mv[i][j] += M[currSpline][i][n] * v[n][j];
            }
        }
    }

    vec3 T;

  // If type == VALUE, return the value = T (Mv)
    if (type == VALUE) {
        for (int i = 0; i < 3; i++) {
            T[i] = (Mv[0][i] * u * u * u + Mv[1][i] * u * u + Mv[2][i] * u + Mv[3][i]);
        }
    }


  // If type == TANGENT, return the value T' (Mv)
    if (type == TANGENT) {
        for (int j = 0; j < 3; j++) {
            T[j] = (Mv[0][j] * 3 * u * u  + Mv[1][j] * 2 * u  + Mv[2][j]);
        }
    }

  return T;
}


// Find a local coordinate system at t.  Return the axes x,y,z.  y
// should point as much up as possible and z should point in the
// direction of increasing position on the curve.


void Spline::findLocalSystem( float t, vec3 &o, vec3 &x, vec3 &y, vec3 &z )

{
#if 0

  // YOUR CODE HERE

#else
  
  o = vec3(0,0,0);
  x = vec3(1,0,0);
  y = vec3(0,1,0);
  z = vec3(0,0,1);

#endif
}


mat4 Spline::findLocalTransform( float t )

{
  vec3 o, x, y, z;

  findLocalSystem( t, o, x, y, z );

  mat4 M;
  M.rows[0] = vec4( x.x, y.x, z.x, o.x );
  M.rows[1] = vec4( x.y, y.y, z.y, o.y );
  M.rows[2] = vec4( x.z, y.z, z.z, o.z );
  M.rows[3] = vec4( 0, 0, 0, 1 );

  return M;
}


// Draw a point and a local coordinate system for parameter t


void Spline::drawLocalSystem( float t, mat4 &MVP )

{
  vec3 o, x, y, z;

  findLocalSystem( t, o, x, y, z );

  mat4 M;
  M.rows[0] = vec4( x.x, y.x, z.x, o.x );
  M.rows[1] = vec4( x.y, y.y, z.y, o.y );
  M.rows[2] = vec4( x.z, y.z, z.z, o.z );
  M.rows[3] = vec4( 0, 0, 0, 1 );

  M = MVP * M * scale(6,6,6);
  axes->draw(M);
}


// Draw the spline with even parameter spacing


void Spline::draw( mat4 &MV, mat4 &MVP, vec3 lightDir, bool drawIntervals )

{
  // Draw the spline
  
  vec3 *points = new vec3[ data.size()*DIVS_PER_SEG + 1 ];
  vec3 *colours = new vec3[ data.size()*DIVS_PER_SEG + 1 ];

  int i = 0;
  for (float t=0; t<data.size(); t+=1/(float)DIVS_PER_SEG) {
    points[i] = value( t );
    colours[i] = SPLINE_COLOUR;
    i++;
  }

  segs->drawSegs( GL_LINE_LOOP, points, colours, i, MV, MVP, lightDir );

  // Draw points evenly spaced in the parameter

  if (drawIntervals)
    for (float t=0; t<data.size(); t+=1/(float)DIVS_PER_SEG)
      drawLocalSystem( t, MVP );

  delete[] points;
  delete[] colours;
}


// Draw the spline with even arc-length spacing


void Spline::drawWithArcLength( mat4 &MV, mat4 &MVP, vec3 lightDir, bool drawIntervals )

{
  // Draw the spline
  
  vec3 *points = new vec3[ data.size()*DIVS_PER_SEG + 1 ];
  vec3 *colours =  new vec3[ data.size()*DIVS_PER_SEG + 1 ];

  int i = 0;
  for (float t=0; t<data.size(); t+=1/(float)DIVS_PER_SEG) {
    points[i] = value( t );
    colours[i] = SPLINE_COLOUR;
    i++;
  }

  segs->drawSegs( GL_LINE_LOOP, points, colours, i, MV, MVP, lightDir );

  // Draw points evenly spaced in arc length

  if (drawIntervals) {
    
    float totalLength = totalArcLength();

    for (float s=0; s<totalLength; s+=totalLength/(float)(data.size()*DIVS_PER_SEG)) {
      float t = paramAtArcLength( s );
      drawLocalSystem( t, MVP );
    }
  }

  delete[] points;
  delete[] colours;
}


// Fill in an arcLength array such that arcLength[i] is the estimated
// arc length up to the i^th sample (using DIVS_PER_SEG samples per
// spline segment).


void Spline::computeArcLengthParameterization()

{
  if (data.size() == 0)
    return;

  if (arcLength != NULL)
    delete [] arcLength;

  arcLength = new float[ data.size() * DIVS_PER_SEG + 1 ];

  // first sample at length 0

  arcLength[0] = 0;
  vec3 prev = value(0);

  maxHeight = prev.z;

  // Compute intermediate lengths

  int k = 1;
  
  for (int i=0; i<data.size(); i++)
    for (int j=(i>0?0:1); j<DIVS_PER_SEG; j++) {

      vec3 next = value( i + j/(float)DIVS_PER_SEG );

      arcLength[k] = arcLength[k-1] + (next-prev).length();
      prev = next;
      k++;

      if (next.z > maxHeight)
        maxHeight = next.z;
    }

  // last sample at full length

  vec3 next = value( data.size() );
  arcLength[k] = arcLength[k-1] + (next-prev).length();

  if (next.z > maxHeight)
    maxHeight = next.z;
  
  mustRecomputeArcLength = false;
}


// Find the spline parameter at a particular arc length, s.


float Spline::paramAtArcLength( float s )

{
  if (mustRecomputeArcLength)
    computeArcLengthParameterization();

  // Do binary search on arc lengths to find l such that 
  //
  //        arcLength[l] <= s < arcLength[l+1].

  if (s < 0)
    s += arcLength[ data.size() * DIVS_PER_SEG ];

  int l = 0;
  int r = data.size()*DIVS_PER_SEG;

  while (r-l > 1) {
    int m = (l+r)/2;
    if (arcLength[m] <= s)
      l = m;
    else
      r = m;
  }

  if (arcLength[l] > s || arcLength[l+1] <= s)
    return (l + 0.5) / (float) DIVS_PER_SEG;
  
  // Do linear interpolation in arcLength[l] ... arcLength[l+1] to
  // find position of s.

  float p = (s - arcLength[l]) / (arcLength[l+1] - arcLength[l]);

  // Return the curve parameter at s

  return (l+p) / (float) DIVS_PER_SEG;
}



float Spline::totalArcLength()

{
  if (data.size() == 0)
    return 0;

  if (mustRecomputeArcLength)
    computeArcLengthParameterization();

  return arcLength[ data.size() * DIVS_PER_SEG ];
}
