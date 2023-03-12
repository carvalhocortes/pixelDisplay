#ifndef FAST_LED_TO_X_Y_CORD_H
    #define FAST_LED_TO_X_Y_CORD_H

    void turnOffDisplay (void);
    int getPositionByXYCoordinates( int x, int y, int width, int height);
    void testDisplay (char function[], int duration, int testBrightness);

#endif
