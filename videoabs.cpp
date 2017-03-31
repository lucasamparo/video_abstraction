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
	Mat dog, pre_dog, bilateral, lab, lum, wrap;
	stringstream ss;
	ss << "img/imagem0.jpg";
	input = imread(ss.str().c_str(), CV_LOAD_IMAGE_COLOR);
	output = input.clone();
	Size sz1 = input.size();
	Size sz2 = output.size();

	if(! input.data ){
		cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	//RGB to Lab Conversion
	cvtColor(input, lab, CV_BGR2Lab);

	//Applying Bilateral Filter
	bilateralFilter(input, bilateral, 9, 50, 50);
	pre_dog = bilateral.clone();
	for(int i = 0; i < 5; i++){
		bilateralFilter(bilateral.clone(), bilateral, 9, 50, 50);
	}
	//imshow("bilateral", bilateral);
	cvtColor(bilateral, lab, CV_BGR2Lab);

	//Luminance Quantization
	int q = 5;
	for(int i = 0; i < lab.size().height; i++){
		for(int j = 0; j < lab.size().width; j++){
			Vec3b value = lab.at<Vec3b>(i,j);
			float tax = value.val[0] / 255.0;
			value.val[0] = (round(tax * q)/q)*255;
			lab.at<Vec3b>(i,j) = value;
		}
	}
	Mat tmp;
	cvtColor(lab, tmp, CV_Lab2BGR);
	//imshow("lab", tmp);

	//Applying Difference of Gaussians
	Mat g1, g2;
	int sub_factor;
	GaussianBlur(pre_dog,g1,Size(sqrt(1.6),sqrt(1.6)),0);
	GaussianBlur(pre_dog,g2,Size(25,25),0);
	dog = g2 - g1;
	bitwise_not(dog,dog);
	cvtColor(dog,dog,CV_Lab2BGR);
	cvtColor(dog,dog,CV_BGR2GRAY);
	dog = dog < 128;
	//imshow("dog", dog);

	//Generating presentation
	for(int i = 0; i < lab.size().height; i++){
		for(int j = 0; j < lab.size().width; j++){
			if((int)dog.at<uchar>(i,j) == 0){
				output.at<Vec3b>(i,j) = Vec3b(0,127,127);
			} else {
				output.at<Vec3b>(i,j) = lab.at<Vec3b>(i,j);
			}
		}
	}
	cvtColor(output, output, CV_Lab2BGR);

	Mat output2 = output.clone();
	for(int i = 0; i < bilateral.size().height; i++){
		for(int j = 0; j < bilateral.size().width; j++){
			if((int)dog.at<uchar>(i,j) == 0){
				output2.at<Vec3b>(i,j) = Vec3b(0,0,0);
			} else {
				output2.at<Vec3b>(i,j) = bilateral.at<Vec3b>(i,j);
			}
		}
	}
	//cvtColor(output, output, CV_Lab2BGR);

	Mat image;
	hconcat(input, output2, image);
	hconcat(image, output, image);
   	namedWindow( "Display window", WINDOW_AUTOSIZE );
    	imshow( "Display window", image );
    	
	waitKey(0);
	return 0;
}
