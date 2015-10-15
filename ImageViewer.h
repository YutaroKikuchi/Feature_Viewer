#ifndef IMGVIEWER
#define IMGVIEWER

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "ImageKeeper.h"


class ImageViewer{
private:
	int numCam;
	int combineImgbyLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int combineImgbyRect(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int DrawLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int DrawLinesinLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out);
	int matchPoint(ImageKeeper from,ImageKeeper to,std::vector<cv::Point2f> &frompoints,std::vector<cv::Point2f> &topoints);

public:
	ImageViewer(int numcam){
		numCam = numcam;
	}

	int setWindow(){
		cv::namedWindow("hoge",CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO);
	}

	int showImgsbyLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*numCam,begin[0].getIMG().rows),CV_8UC3);

		combineImgbyLine(begin,end,output);

		DrawLinesinLine(begin,end,output);

		cv::imshow("hoge",output);

		return 0;
	}

	int showImgsbyRect(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end){

		cv::Mat output(cv::Size(begin[0].getIMG().cols*3,begin[0].getIMG().rows*(int)(numCam/3)),CV_8UC3);

		combineImgbyRect(begin,end,output);



		cv::imshow("hoge",output);

		cv::waitKey(0);
		return 0;
	}

};

int ImageViewer::combineImgbyLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out){
		
	if(end - begin !=numCam)
		return -1;

	cv::Rect rect;
	rect.width = begin[0].getIMG().cols;
	rect.height = begin[0].getIMG().rows;

	for(int i=0;i<end-begin;i++){
		rect.x = i*rect.width;
		rect.y = 0;

		cv::Mat roi(out,rect);
		begin[i].getIMG().copyTo(roi);
	}

	return 0;
}

int ImageViewer::combineImgbyRect(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out){

	if(end-begin!=numCam){
		return -1;
	}

	cv::Rect rect;
	rect.width = begin[0].getIMG().cols;
	rect.height = begin[0].getIMG().rows;

	for(int i=0;i<end-begin;i++){
		rect.x = (i%3)*rect.width;
		rect.y = (int)(i/3)*rect.height;

		cv::Mat roi(out,rect);
		begin[i].getIMG().copyTo(roi);		
	}
}

int ImageViewer::DrawLinesinLine(std::vector<ImageKeeper>::iterator begin,std::vector<ImageKeeper>::iterator end,cv::Mat out){

	float col = (float)begin[0].getCols(); float row = (float)begin[0].getRows();

	for(int i=0;i<end-begin;i++){

		for(int j=i+1;j<end-begin;j++){
			
			std::vector<cv::Point2f> frompoints,topoints;
			matchPoint(begin[i],begin[j],frompoints,topoints);

			std::cout << "begin[" << i << "]  " << "begin[" << j << "]" << std::endl;

			for(int k=0;k<frompoints.size();k++){
				cv::Point2f from = frompoints[k]+cv::Point2f(col*(i-1),0.0);
				cv::Point2f to = topoints[k]+cv::Point2f(col*(j-1),0.0);

				cv::line(out,from,to,cv::Scalar(0.0,0.0,200.0),2,8);
			}
		}

	}
	return 0;
}

int ImageViewer::matchPoint(ImageKeeper from,ImageKeeper to,std::vector<cv::Point2f> &frompoints,std::vector<cv::Point2f> &topoints){

	for(int i=0;i<from.getPointsSize();i++){
		int fromFID = from.getFeatureID(i);
		cv::Point2f point = to.getPointID(fromFID);

		if(point != cv::Point2f(0.0,0.0)){
			frompoints.push_back(from.getPointID(fromFID));
			topoints.push_back(point);
		}
	}

	return 0;
}
#endif
