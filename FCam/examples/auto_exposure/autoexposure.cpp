#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <sstream>
#include <iostream>

#include <FCam/N900.h>
namespace Plat = FCam::N900;

void errorCheck();

float computeIntensity(FCam::Frame frame)
{
    FCam::Image i = frame.image();
    assert(i.valid());

    i.lock();

        float averageIntensity = (*i(0, 0));
        int averageComponents = 1;

        for(int x = 0; x < i.width(); x++)
        {
            for(int y = 0; y < i.height(); y++)
            {
                averageComponents++;
                averageIntensity = ( 1.0 - 1.0 / averageComponents ) * averageIntensity + ( 1.0 / averageComponents ) * (*i(x, y));
            }
        }

    i.unlock();

    return averageIntensity;
}


int main(int argc, char **argv) {

    // Make the image sensor
    Plat::Sensor sensor;

    // Make the lens
    Plat::Lens lens;


    if(argc < 5)
    {
        std::cout << "Usage: " << argv[0] << " desired_intensity gain_multiplier focus_distance_meters white_balance_kelvin" << std::endl;
        exit(0);
    }

    float desiredIntensity = atof(argv[1]);
    float gain = atof(argv[2]);
    float focusDistance = atof(argv[3]);
    float whiteBalance = atoi(argv[4]);


    float diopters = 1.0 / focusDistance;

    lens.setFocus(diopters, -1.0);
    while(lens.focusChanging())
    {
        usleep(10000);
    }


    int exposureTimeLow  = 1;
    int exposureTimeHigh = 2000000;


    int take = 0;

    // binary search for desired image intensity
    while( exposureTimeHigh - exposureTimeLow > 1000 )
    {


        int trialExposure = (exposureTimeHigh + exposureTimeLow) / 2;

        FCam::Shot shot;

        shot.whiteBalance = whiteBalance;
        shot.exposure = trialExposure;
        shot.gain = gain;
        //shot.image = FCam::Image(160, 120, FCam::RGB24);
        shot.image = FCam::Image(1296, 984, FCam::RAW);

        sensor.capture(shot);

        errorCheck();

        FCam::Frame frame = sensor.getFrame();

        float intensity = computeIntensity(frame);

        std::cout << "[" << exposureTimeLow << ", " << exposureTimeHigh << "] - " << intensity << std::endl;
    std::stringstream filenameStream;
    filenameStream << "/home/user/frame-" << desiredIntensity << "-" << take << "-" << intensity << "-" << trialExposure << ".jpg";
    std::cout << "saving " << filenameStream.str() << std::endl;
    FCam::saveJPEG(frame, filenameStream.str());

        if( fabs(intensity - desiredIntensity) < 1.0 )
            break;

        if(intensity > desiredIntensity)
            exposureTimeHigh = trialExposure;
        else
            exposureTimeLow = trialExposure;

        take++;

    }


    sensor.stop();

    errorCheck();

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
            printf("autoexposure: FCam can't find its driver. Did you install "
                   "fcam-drivers on your platform, and reboot the device "
                   "after installation?\n");
            exit(1);
        }
        if (e.data == FCam::Event::DriverLockedError) {
            printf("autoexposure: Another FCam program appears to be running "
                   "already. Only one can run at a time.\n");
            exit(1);
        }
    }
}
