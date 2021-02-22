// world.cpp


#include "world.h"
#include "lander.h"
#include "ll.h"
#include "gpuProgram.h"
#include "strokefont.h"
#include <sstream>
#include <iomanip>

//const float textAspect = 0.7;	// text width-to-height ratio (you can use this for more realistic text on the screen)

float timeOfGame = 0;

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
  if (!landedFlag) {    
      timeOfGame += elapsedTime;
  }


  int idx = landscape->findLandingSegment(lander->getPosition());
  altitude = landscape->getAltitude(idx, lander->getPosition(), 5);
  if (altitude < 0.1 && !landedFlag) {   //Check to see if lander is close enough to the landscape to land
      if (abs(lander->getVelocity().x) < 0.5 && abs(lander->getVelocity().y) < 1) {     //Check to make sure lander is moving slow enough
          landedFlag = true;
          winFlag = true;
          score += 20;
          lander->stop();     //Stop the lander
      }
      else {
          landedFlag = true;
          winFlag = false;
          lander->stop();     //Stop the lander
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
      float s = 1 / (2 * ZOOM_RADIUS);
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
  drawStrokeString(ss.str(), -0.95, 0.60, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display remaining fuel
  ss.str("");
  ss << "SCORE " << score;
  drawStrokeString(ss.str(), -0.95, 0.70, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current score
  ss.str("");
  ss << "ALTITUDE " << altitude;
  drawStrokeString(ss.str(), 0.3, 0.70, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current altitude
  ss.str("");
  ss << "TIME " << timeOfGame;
  drawStrokeString(ss.str(), -0.95, 0.65, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));       //Display current time
  ss.str("");

  //Display vertical and horizontal speed:

  float velocityX = lander->getVelocity().x;
  ss << "HORIZONTAL SPEED " << abs(velocityX);
  drawStrokeString(ss.str(), 0.3, 0.65, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));
  ss.str("");

  float velocityY = lander->getVelocity().y;
  ss << "VERTICAL SPEED " << abs(velocityY);
  drawStrokeString(ss.str(), 0.3, 0.60, 0.04, glGetUniformLocation(myGPUProgram->id(), "MVP"));
  ss.str("");

  //Determining which arrow to draw and drawing it (all rotations are in radians):
  setupArrowVAO();

  glBindVertexArray(arrowVAO);
  if (velocityX > 0) {          //Positive horizontal speed, arrow points right
      worldToViewTransform = translate(0.93, 0.67, 0) * scale(0.02, 0.02, 1) * rotate(-1.5708, vec3(0, 0, 1));        //Rotate by 90 degrees
      glUniformMatrix4fv(glGetUniformLocation(myGPUProgram->id(), "MVP"), 1, GL_TRUE, &worldToViewTransform[0][0]);
  }
  else if (velocityX < 0) {     //Negative horizontal speed, arrow points left
      worldToViewTransform = translate(0.89, 0.67, 0) * scale(0.02, 0.02, 1) * rotate(-4.7124, vec3(0, 0, 1));        //Rotate by 270 degrees
      glBindVertexArray(arrowVAO);
      glUniformMatrix4fv(glGetUniformLocation(myGPUProgram->id(), "MVP"), 1, GL_TRUE, &worldToViewTransform[0][0]);
  }
  glLineWidth(2.0);
  glDrawArrays(GL_LINES, 0, numArrowVerts);
  if (velocityY > 0) {          //Positive vertical speed, arrow points up
      worldToViewTransform = translate(0.88, 0.61, 0) * scale(0.02, 0.02, 1);        //Don't rotate
      glBindVertexArray(arrowVAO);
      glUniformMatrix4fv(glGetUniformLocation(myGPUProgram->id(), "MVP"), 1, GL_TRUE, &worldToViewTransform[0][0]);
  }
  else if (velocityY < 0) {     //Negative vertical speed, arrow points down
      worldToViewTransform = translate(0.88, 0.61, 0) * scale(0.02, 0.02, 1) * rotate(-3.1415, vec3(0, 0, 1));        //Rotate by 180 degrees
      glBindVertexArray(arrowVAO);
      glUniformMatrix4fv(glGetUniformLocation(myGPUProgram->id(), "MVP"), 1, GL_TRUE, &worldToViewTransform[0][0]);
  }
  glLineWidth(2.0);
  glDrawArrays(GL_LINES, 0, numArrowVerts);

  //Display win/loss messages:
  if (landedFlag) {
      if (winFlag) {
          ss << "GAME WON: +20 Points!";
          drawStrokeString(ss.str(), -0.6, 0.30, 0.06, glGetUniformLocation(myGPUProgram->id(), "MVP"));
          ss.str("");
          lander->stop();     //Stop the lander
      }
      else if (!winFlag) {
          ss << "YOU'VE CRASHED: GAME OVER";
          drawStrokeString(ss.str(), -0.6, 0.30, 0.06, glGetUniformLocation(myGPUProgram->id(), "MVP"));
          ss.str("");
          lander->stop();     //Stop the lander
      }
  }


  

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

    numArrowVerts = sizeof(arrowVerts) / sizeof(GLfloat);

    // ---- Create a VAO for this object ----

    glGenVertexArrays(1, &arrowVAO);
    glBindVertexArray(arrowVAO);

    // Store the vertices

    GLuint VBO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numArrowVerts * sizeof(GLfloat), arrowVerts, GL_STATIC_DRAW);

    // define the position attribute

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Done

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}