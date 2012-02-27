#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <sstream>
#include <iostream>

// Select the platform
#include <FCam/N900.h>
namespace Plat = FCam::N900;

/** \file */

/***********************************************************/
/* A program that takes a single shot                      */
/*                                                         */
/* This example is a simple demonstration of the usage of  */
/* the FCam API.                                           */
/***********************************************************/

void errorCheck();

int main(int argc, char **argv) {

    // Make the image sensor
    Plat::Sensor sensor;

    // Make the lens
    Plat::Lens lens;


    float near = 0.05;
    float far = 0.5;
    int focusSteps = 9;

    int exposureMinPower = 13;
    int exposureMaxPower = 20;






    for(int focusStep = 0; focusStep < focusSteps; focusStep++)
    {

        float focusDistance = near + (far - near) * float(focusStep) / focusSteps;
        float diopters = 1.0 / focusDistance;

        lens.setFocus(diopters, -1.0);
        while(lens.focusChanging())
        {
            usleep(10000);
        }



        for (size_t exposurePower = exposureMinPower; exposurePower <= exposureMaxPower; exposurePower++) {
            FCam::Shot shot;

            shot.whiteBalance = 4100; // http://en.wikipedia.org/wiki/Fluorescent_lamp#Color_temperature

            //shot.exposure = pow(2, i+1);
            shot.exposure = pow(2, exposurePower);
            //shot.
            shot.gain = 1.0f;      // minimum ISO
            //shot.image = FCam::Image(1296, 984, FCam::RAW);
            shot.image = FCam::Image(2592, 1968, FCam::RAW);



            // Order the sensor to capture the shots
            sensor.capture(shot);

            // Check for errors
            errorCheck();

            // save the frame
            std::stringstream filenameStream;
            FCam::Frame frame = sensor.getFrame();
            filenameStream << "/home/user/MyDocs/FCamera/frame-" << frame.shot().exposure << "-" << lens.getFocus() << ".jpg";
            std::cout << "saving " << filenameStream.str() << std::endl;
//            FCam::saveDNG(frame, filenameStream.str());
            FCam::saveJPEG(frame, filenameStream.str());
        }
    }

    // Specify the output resolution and format, and allocate storage for the resulting image
    //shot[0].image = FCam::Image(2592, 1968, FCam::RAW);
    //shot[1].image = FCam::Image(1296, 984, FCam::RAW);
    //shot[2].image = FCam::Image(648, 492, FCam::RAW);


//    assert(sensor.shotsPending() == shot.size()); // There should be exactly this many shots

    // Retrieve the frame from the sensor
    //std::vector<FCam::Frame> frame(shot.size());
 /*   for (size_t i = 0; i < shot.size(); i++) {
        std::stringstream filenameStream;
        FCam::Frame frame = sensor.getFrame();
        filenameStream << "/home/user/MyDocs/FCamera/frame" << frame.shot().exposure << ".dng";
        cout << "saving " << filenameStream.str() << endl;
        FCam::saveDNG(frame, filenameStream.str());
    }*/

    sensor.stop();

    errorCheck();

    // Each frame should be the result of the shot we made
/*    for (size_t i = 0; i < shot.size(); i++) {
        printf("Checking frame %d\n", i);
        assert(frame[i].shot().id == shot[i].id);

        // This frame should be valid too
        assert(frame[i].valid());
        assert(frame[i].image().valid());
        printf("Frame %d OK!\n", i);
    }*/

    // Save the frames
    //FCam::saveDNG(frame[0], "/home/user/MyDocs/FCamera/frame0.dng");
    //FCam::saveDNG(frame[1], "/home/user/MyDocs/FCamera/frame1.dng");
    //FCam::saveDNG(frame[2], "/home/user/MyDocs/FCamera/frame2.dng");

    // Check that the pipeline is empty
    assert(sensor.framesPending() == 0);
    assert(sensor.shotsPending() == 0);

    return 0;
}




void errorCheck() {
    // Make sure FCam is running properly by looking for DriverError
    FCam::Event e;
    while (FCam::getNextEvent(&e, FCam::Event::Error)) {
        printf("Error: %s\n", e.description.c_str());
        if (e.data == FCam::Event::DriverMissingError) {
            printf("example1: FCam can't find its driver. Did you install "
                   "fcam-drivers on your platform, and reboot the device "
                   "after installation?\n");
            exit(1);
        }
        if (e.data == FCam::Event::DriverLockedError) {
            printf("example1: Another FCam program appears to be running "
                   "already. Only one can run at a time.\n");
            exit(1);
        }
    }
}
