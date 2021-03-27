// train.cpp

#include "train.h"
#include "main.h"


#define SPHERE_RADIUS 5.0
#define SPHERE_COLOUR 238/255.0, 106/255.0, 20/255.0


// Used for scaling and coloring the cube
#define CUBE_HEIGHT 5.0
#define CUBE_WIDTH 5.0
#define CUBE_LENGTH 10.0
#define CUBE_COLOUR 0/255.0, 196/255.0, 255/255.0
#define NUM_TRAINS 5


// Draw the train.
//
// 'flag' is toggled by pressing 'F' and can be used for debugging

 
void Train::draw( mat4 &WCStoVCS, mat4 &WCStoCCS, vec3 lightDir, bool flag )

{
#if 1

	mat4 M, MV, MVP;
	vec3 o, x, y, z;
	float t[NUM_TRAINS];

  // YOUR CODE HERE

	
	for (int i = 0; i < NUM_TRAINS; i++) {
		t[i] = spline->paramAtArcLength(pos - i * 12.5f);
		spline->findLocalSystem(t[i], o, x, y, z);

		M = spline->findLocalTransform(t[i]) * scale(CUBE_WIDTH, CUBE_HEIGHT, CUBE_LENGTH);
		MV = WCStoVCS * M;
		MVP = WCStoCCS * M;

		cube->draw(MV, MVP, lightDir, vec3(CUBE_COLOUR));
	}


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

	while (pos < 0) {
		pos += spline->totalArcLength();
	}

#else

  pos += 1;
  while (pos > spline->totalArcLength()) {
	  pos -= spline->totalArcLength();
  }


#endif
}
