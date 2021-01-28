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
  vec3 up(0.0, 1.0, 0.0);
  if (closestDistance < 10e-2 || closestTerrainPoint * up < 0) { // When the closest distance is this small, the ship can pass through the landscape accidently
      lander->stop();               // Maybe we need to check if the "up" vector dotted with our closestTerrainPoint vector is negative. (This didn't work)
      if (lander->velocity.x < 0.5 && lander->velocity.y < 1.0) {
          
      }
  }

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

      float s = 1 / (2 * ZOOM_RADIUS);    //Get scaling factor
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

  drawStrokeString( "LUNAR LANDER", -0.2, 0.85, 0.06, glGetUniformLocation( myGPUProgram->id(), "MVP") );

  ss.setf( ios::fixed, ios::floatfield );
  ss.precision(1);

  ss << "SPEED " << lander->speed() << " m/s";
  drawStrokeString( ss.str(), -0.95, 0.75, 0.04, glGetUniformLocation( myGPUProgram->id(), "MVP") );
  ss.str("");

  // YOUR CODE HERE (modify the above code, too)

  ss << "FUEL " << lander->fuel;
  drawStrokeString(ss.str(), -0.95, 0.70, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display remaining fuel
  ss.str("");
  
  ss << "SCORE " << score;
  drawStrokeString(ss.str(), -0.95, 0.65, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current score
  ss.str("");
  
  ss << "ALTITUDE " << 1.5; // getAltitude();
  drawStrokeString(ss.str(), -0.95, 0.60, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current altitude
  ss.str("");
  

  // I suspect that from here down is extremely inefficent, idk what VAO/VBO lines you need to repeat
  setupArrowVAO();
  float s = 22.5 / (maxX() - minX()); // Trial and error on a good scaling value
  float directionX = (lander->velocity.x > 0) ? 4.71 : 1.57; // Determine direction of horizontal arrow
  float directionY = (lander->velocity.y > 0) ? 0 : 3.14; // Determine direction of vertical arrow

  // Print vertical speed
  ss << "VERTICAL SPEED " << lander->velocity.y;
  drawStrokeString(ss.str(), 0.20, 0.65, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current score
  ss.str("");

  worldToViewTransform = rotate(directionY, vec3(0, 0, 1)) * scale(s, s, 1); // Idk how to get these arrow translated into the right positions
  glBindVertexArray(arrowVAO);
  glUniformMatrix4fv(glGetUniformLocation(myGPUProgram->id(), "MVP"), 1, GL_TRUE, &worldToViewTransform[0][0]);
  glLineWidth(2.0);
  glDrawArrays(GL_LINES, 0, numArrowVerts);

  // Print horizontal speed
  ss << "HORIZONTAL SPEED " << lander->velocity.x;
  drawStrokeString(ss.str(), 0.20, 0.70, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));

  worldToViewTransform = rotate(directionX, vec3(0, 0, 1)) * scale(s, s, 1); // Idk how to get these arrow translated into the right positions
  glBindVertexArray(arrowVAO);
  glUniformMatrix4fv(glGetUniformLocation(myGPUProgram->id(), "MVP"), 1, GL_TRUE, &worldToViewTransform[0][0]); 
  glLineWidth(2.0);
  glDrawArrays(GL_LINES, 0, numArrowVerts);

}


// Create an arrow VAO
//
// This needs 'numArrowVerts' and 'arrowVAO' defined in world.h

void World::setupArrowVAO()

{
  // create an arrow from line segments

  GLfloat arrowVerts[] = {
    0,    1,    0,    -1,
    0,    1,    0.5, 0.25,
    0,    1,   -0.5, 0.25,
    0.5, 0.25, -0.5, 0.25
  };

  numArrowVerts = sizeof(arrowVerts)/sizeof(GLfloat);

  // ---- Create a VAO for this object ----

  glGenVertexArrays( 1, &arrowVAO );
  glBindVertexArray( arrowVAO );

  // Store the vertices

  GLuint VBO;
  
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, numArrowVerts*sizeof(GLfloat), arrowVerts, GL_STATIC_DRAW );

  // define the position attribute

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  // Done

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
}
