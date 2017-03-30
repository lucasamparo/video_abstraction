#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;

int main( int argc, char** argv ){

    	Mat input, output;
	Mat dog, bilateral, lab, lum, wrap;
	stringstream ss;
	ss << "img/imagem1.jpg";
	input = imread(ss.str().c_str(), CV_LOAD_IMAGE_COLOR);
	output = input;
	Size sz1 = input.size();
	Size sz2 = output.size();

	if(! input.data ){
		cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	//RGB to Lab Conversion
	cvtColor(input, lab, CV_BGR2Lab);

	//Applying Bilateral Filter
	bilateralFilter(lab, bilateral, 15, 150, 150);

	//Luminance Quantization
	int q = 10;
	for(int i = 0; i < lab.size().height; i++){
		for(int j = 0; j < lab.size().width; j++){
			Vec3b value = lab.at<Vec3b>(i,j);
			float tax = value.val[0] / 255.0;
			value.val[0] = (round(tax * q)/q)*255;
			lab.at<Vec3b>(i,j) = value;
		}
	}
	cvtColor(lab, lab, CV_Lab2BGR);
	imshow("depois 1", lab);

	//Applying Difference of Gaussians
	Mat g1, g2;
	GaussianBlur(input,g1,Size(1,1),0);
	GaussianBlur(input,g2,Size(5,5),0);
	dog = g1 - g2;
	dog = Scalar::all(255) - dog;

	//Generating presentation
	addWeighted(dog, 0.5, lab, 0.5, 0.0, output);

	Mat image(sz1.height, sz1.width+sz2.width, CV_8UC3);
	image.adjustROI(0,0,0,-sz2.width);
	input.copyTo(image);

	image.adjustROI(0,0,-sz1.width,sz2.width);
	output.copyTo(image);

	image.adjustROI(0,0,sz1.width,0);
   	namedWindow( "Display window", WINDOW_AUTOSIZE );
    	imshow( "Display window", image );
    	
	waitKey(0);
	return 0;
}
