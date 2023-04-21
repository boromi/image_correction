image_correction
================
Image post-processing project in C++ with the aim to rectify airborne hyperspectral
imagery using auxiliary data from an onboard IMU(Inertial Measurement Unit) and a GPS.
The image was taken by a push-broom scanner.

Steps of the process:
-specifying the image (it is a simplified case, it uses only one spectrum, so the input is a simple 2 dimensional JPG file)
-specifying the auxiliary measurements
-geometric correction is calculated and applied to each line of the image
-improved image displayed

https://github.com/boromi/image_correction/blob/master/imu_gps_data/image.png
