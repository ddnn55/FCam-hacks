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


    if(argc < 5)
    {
        std::cout << "Usage: " << argv[0] << " exposure_time_microseconds gain_multiplier focus_diopters white_balance_kelvin" << std::endl;
        exit(0);
    }

    float exposureTime = atoi(argv[1]);
    float gain = atof(argv[2]);
    float focusDistance = atof(argv[3]);
    float diopters = atof(argv[3]);
    float whiteBalance = atoi(argv[4]);



//    for(int focusStep = 0; focusStep < focusSteps; focusStep++)
//    {

//        float focusDistance = near + (far - near) * float(focusStep) / focusSteps;
        //float diopters = 1.0 / focusDistance;

        lens.setFocus(diopters, -1.0);
        while(lens.focusChanging())
        {
            usleep(10000);
        }



//        for (size_t exposurePower = exposureMinPower; exposurePower <= exposureMaxPower; exposurePower++) {
            FCam::Shot shot;

            shot.whiteBalance = whiteBalance; // http://en.wikipedia.org/wiki/Fluorescent_lamp#Color_temperature

            //shot.exposure = pow(2, i+1);
            shot.exposure = exposureTime;
            //shot.
            shot.gain = gain;
            //shot.image = FCam::Image(1296, 984, FCam::RAW);
            shot.image = FCam::Image(2592, 1968, FCam::RAW);



            // Order the sensor to capture the shots
            sensor.capture(shot);

            // Check for errors
            errorCheck();

            // save the frame
            std::stringstream filenameStream;
            FCam::Frame frame = sensor.getFrame();
            filenameStream << "/home/user/frame.jpg";
            std::cout << "saving " << filenameStream.str() << std::endl;
//            FCam::saveDNG(frame, filenameStream.str());
            FCam::saveJPEG(frame, filenameStream.str());
//        }
//    }


    sensor.stop();

    errorCheck();


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
