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
	Mat im1;
	cvtColor(input, lab, CV_BGR2Lab);

	//Applying Bilateral Filter
	bilateralFilter(lab, bilateral, 15, 80, 80);

	//Luminance Quantization
	vector<Mat> lab_p;
	split(lab, lab_p);
	int q = 15;
	for(int i = 0; i < lab_p[0].size().width; i++){
		for(int j = 0; j < lab_p[0].size().height; j++){
			float value = lab_p.at<float>(i,j);
			int patamar = value / q;
			if(
		}
	}
	

	//Applying Difference of Gaussians
	Mat g1, g2;
	GaussianBlur(lab,g1,Size(1,1),0);
	GaussianBlur(lab,g2,Size(5,5),0);
	dog = g1 - g2;
	dog = Scalar::all(255) - dog;
	cvtColor(dog,dog,CV_RGB2GRAY);

	//Generating presentation
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
