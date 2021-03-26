// train.cpp

#include "train.h"
#include "main.h"


#define SPHERE_RADIUS 5.0
#define SPHERE_COLOUR 238/255.0, 106/255.0, 20/255.0


// Used for scaling and coloring the cube
#define CUBE_RADIUS 8.0
#define CUBE_COLOUR 0/255.0, 196/255.0, 255/255.0


// Draw the train.
//
// 'flag' is toggled by pressing 'F' and can be used for debugging

 
void Train::draw( mat4 &WCStoVCS, mat4 &WCStoCCS, vec3 lightDir, bool flag )

{
#if 1

  // YOUR CODE HERE

	float t = spline->paramAtArcLength(pos);

	// Draw cube

	vec3 o, x, y, z;
	spline->findLocalSystem(t, o, x, y, z);

	mat4 M = spline->findLocalTransform(t) * scale(SPHERE_RADIUS, SPHERE_RADIUS, SPHERE_RADIUS);
	mat4 MV = WCStoVCS * M;
	mat4 MVP = WCStoCCS * M;

	cube->draw(MV, MVP, lightDir, vec3(CUBE_COLOUR));


#else
  
  float t = spline->paramAtArcLength( pos );

  // Draw sphere
  
  vec3 o, x, y, z;
  spline->findLocalSystem( t, o, x, y, z );

  mat4 M   = translate( o ) * scale( SPHERE_RADIUS, SPHERE_RADIUS, SPHERE_RADIUS );
  mat4 MV  = WCStoVCS * M;
  mat4 MVP = WCStoCCS * M;

  sphere->draw( MV, MVP, lightDir, vec3( SPHERE_COLOUR ) );

#endif
}


void Train::advance( float elapsedSeconds )

{
#if 1

  // YOUR CODE HERE

	float t = spline->paramAtArcLength(pos);

	vec3 o, x, y, z;
	spline->findLocalSystem(t, o, x, y, z);

	vec3 z_norm = z.normalize();
	
	speed -= SPEED_INC * (z_norm * vec3(0, 0, 1));

	pos += speed * elapsedSeconds;

	while (pos > spline->totalArcLength()) {
		pos -= spline->totalArcLength();
	}

#else

  pos += 1;
  while (pos > spline->totalArcLength()) {
	  pos -= spline->totalArcLength();
  }


#endif
}
