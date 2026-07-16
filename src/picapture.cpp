// Note: This is just a copy of the week 7 lab code where we needed to modify the PiCapture 
// program provided. All the code was done locally on the Pi itself. 

// #include <opencv2/opencv.hpp>
// #include <sys/time.h>

// // Import from Week 6 Thresholding example
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <iostream>

// using namespace cv;

// int main()
// {
//     // Open the video camera.
//     std::string pipeline = "libcamerasrc"
//         " ! video/x-raw, width=800, height=600" // camera needs to capture at a higher resolution
//         " ! videoconvert"
//         " ! videoscale"
//         " ! video/x-raw, width=400, height=300" // can downsample the image after capturing
//         " ! videoflip method=rotate-180" // remove this line if the image is upside-down
//         " ! appsink drop=true max_buffers=2";
//     cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
//     if(!cap.isOpened()) {
//         printf("Could not open camera.\n");
//         return 1;
//     }

//     // Create the OpenCV window
//     cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);
//     cv::Mat frame;

//     // Measure the frame rate - initialise variables
//     int frame_id = 0;
//     timeval start, end;
//     gettimeofday(&start, NULL);

//     // 1. Modify the PiCapture code to add in colour thresholding, based on the program from last week. 

//     // Create a control window (code from Week 6 Thresholding example)
//     // Note: changed the default slider values to be thresholding for green to make testing easier
//     namedWindow("Control", WINDOW_AUTOSIZE);
//     int iLowH = 40;
//     int iHighH = 85;
    
//     int iLowS = 80;
//     int iHighS = 255;
    
//     int iLowV = 50;
//     int iHighV = 255;

//     // Create trackbars in "Control" window
//     createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
//     createTrackbar("HighH", "Control", &iHighH, 179);

//     createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
//     createTrackbar("HighS", "Control", &iHighS, 255);

//     createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
//     createTrackbar("HighV", "Control", &iHighV, 255);

//     // Create the thresholded display window
//     namedWindow("Thresholded", WINDOW_AUTOSIZE);

//     // Allow for adjustable strucutring element sizes for open and close operations
//     int morphSize = 3; 
//     createTrackbar("MorphSize", "Control", &morphSize, 20); // morphSize (1 - 20)

//     for(;;) {
//         if (!cap.read(frame)) {
//             printf("Could not read a frame.\n");
//             break;
//         }

//         // Show frame
//         cv::imshow("Camera", frame);
//         cv::waitKey(1);

//         // Measure the frame rate
//         frame_id++;
//         if (frame_id >= 30) {
//             gettimeofday(&end, NULL);
//             double diff = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
//             printf("30 frames in %f seconds = %f FPS\n", diff, 30/diff);
//             frame_id = 0;
//             gettimeofday(&start, NULL);
//         }

//         // Convert this frame to HSV (code from Week 6 Thresholding example)
//         Mat hsv_img;
//         cvtColor(frame, hsv_img, COLOR_BGR2HSV);
        
//         // Threshold the image
//         Mat thresh_img;
// 		inRange(hsv_img, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), thresh_img);

//         // 2. Enhance your thresholding image using morphological open and close operations. 
//         // Generate the structure element in OpenCV (from Week 7 Lecture Notes)
//         int size = (morphSize < 1) ? 1 : morphSize; // prevents crashing when slider hits 0
//         Mat kernel = getStructuringElement(MORPH_RECT, Size(size, size)); 

//         // Applying morphological operations (from Week 7 Lecture Notes)
//         // Morphological open tends to eliminate "salt" noise and clean up the background. 
//         morphologyEx(thresh_img, thresh_img, MORPH_OPEN, kernel); 
        
//         // Morphological close tends to eliminate "pepper" noise and tidy up the foreground. 
//         morphologyEx(thresh_img, thresh_img, MORPH_CLOSE, kernel);

//         // 3. Locate the detected object by calculating its centre of mass position. 
//         // Print the identified location to the terminal
//         Moments m = moments(thresh_img, true); 
//         if (m.m00 > 0) { 
//             double x = m.m10 / m.m00; 
//             double y = m.m01 / m.m00;
//             printf("Object detected at (x=%.1f, y=%.1f)\n", x, y);
//         }

//         // Show the live camera feed
//         imshow("Camera", frame);
// 		// Show the thresholded image
// 		imshow("Thresholded", thresh_img);  
//     }

//     // Free the camera 
//     cap.release();
//     return 0;
// }