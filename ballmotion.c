
        while (true)
        {
            // move ball along x-axis
            move(ball, xvelocity, yvelocity);

            // bounce off right edge of window
            if (getX(ball) + RADIUS >= getWidth(window))
            {
                xvelocity = -xvelocity;
            }

            // bounce off left edge of window
            else if (getX(ball) <= 0)
            {
                xvelocity = -xvelocity;
            }
        
            // bounce off top edge of window
            else if (getY(ball) <= 0)
            {
                yvelocity = -yvelocity;
            }
            
//         detect a collision with detectcollision   else if (detectCollision(paddle) + RADIUS >= 600)
//            {
//                yvelocity = -yvelocity;
//            }

            // linger before moving again
            pause(10);
        }
