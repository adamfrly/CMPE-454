// world.cpp


#include "world.h"
#include "lander.h"
#include "ll.h"
#include "gpuProgram.h"
#include "strokefont.h"

#include <sstream>
#include <iomanip>

//const float textAspect = 0.7;	// text width-to-height ratio (you can use this for more realistic text on the screen)


void World::updateState( float elapsedTime )

{
  // See if any keys are pressed for thrust

  if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS) // right arrow
    lander->rotateCW( elapsedTime );

  if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS) // left arrow
    lander->rotateCCW( elapsedTime );

  if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS) // down arrow
    lander->addThrust( elapsedTime );

  // Update the position and velocity

  lander->updatePose( elapsedTime );

  // See if the lander has touched the terrain

  vec3 closestTerrainPoint = landscape->findClosestPoint( lander->centrePosition() );
  float closestDistance = ( closestTerrainPoint - lander->centrePosition() ).length();

  // Find if the view should be zoomed

  zoomView = (closestDistance < ZOOM_RADIUS);

  // Check for landing or collision and let the user know
  //
  // Landing is successful if the vertical speed is less than 1 m/s and
  // the horizontal speed is less than 0.5 m/s.
  //
  // SHOULD ALSO CHECK THAT LANDING SURFACE IS HORIZONAL, BUT THIS IS
  // NOT REQUIRED IN THE ASSIGNMENT.
  //
  // SHOULD ALSO CHECK THAT THE LANDER IS VERTICAL, BUT THIS IS NOT
  // REQUIRED IN THE ASSIGNMENT.

  // YOUR CODE HERE
}



void World::draw()

{
  mat4 worldToViewTransform;

  if (!zoomView) {

    // Find the world-to-view transform that transforms the world
    // to the [-1,1]x[-1,1] viewing coordinate system, with the
    // left edge of the landscape at the left edge of the screen, and
    // the bottom of the landscape BOTTOM_SPACE above the bottom edge
    // of the screen (BOTTOM_SPACE is in viewing coordinates).

    float s = 2.0 / (landscape->maxX() - landscape->minX());

    worldToViewTransform
      = translate( -1, -1 + BOTTOM_SPACE, 0 )
      * scale( s, s, 1 )
      * translate( -landscape->minX(), -landscape->minY(), 0 );

  } else {

    // Find the world-to-view transform that is centred on the lander
    // and is 2*ZOOM_RADIUS wide (in world coordinates).

    // YOUR CODE HERE
      float z = 2.0 * ZOOM_RADIUS;                    //Not sure if this is how to get zoom factor
      if (z < landscape->maxX() / ZOOM_RADIUS) {
          z += 0.5;                                     //Update zoom view until it reaches the maximum
      }
      float s = z / (landscape->maxX() - landscape->minX());    //Get scaling factor
      worldToViewTransform
          = translate(0, BOTTOM_SPACE, 0)
          * scale(s, s, 1)
          * translate(-lander->centrePosition().x, -lander->centrePosition().y, 0);     //Translate so that lander is centered
  }

  // Draw the landscape and lander, passing in the worldToViewTransform
  // so that they can append their own transforms before passing the
  // complete transform to the vertex shader.

  landscape->draw( worldToViewTransform );
  lander->draw( worldToViewTransform );

  // Draw the heads-up display (i.e. all text).

  stringstream ss;
  stringstream sf;
  stringstream sc;

  drawStrokeString( "LUNAR LANDER", -0.2, 0.85, 0.06, glGetUniformLocation( myGPUProgram->id(), "MVP") );

  ss.setf( ios::fixed, ios::floatfield );
  ss.precision(1);

  ss << "SPEED " << lander->speed() << " m/s";
  drawStrokeString( ss.str(), -0.95, 0.75, 0.04, glGetUniformLocation( myGPUProgram->id(), "MVP") );

  // YOUR CODE HERE (modify the above code, too)

  sf.setf(ios::fixed, ios::floatfield);
  sf.precision(1);
  sc.setf(ios::fixed, ios::floatfield);
  sc.precision(1);

  sf << "FUEL " << lander->fuel;
  drawStrokeString(sf.str(), -0.95, 0.70, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display remaining fuel
  sc << "SCORE " << score;
  drawStrokeString(sc.str(), -0.95, 0.65, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current score

}
