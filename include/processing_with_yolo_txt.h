#ifndef PROCESSING_WITH_YOLO_TXT
#define PROCESSING_WITH_YOLO_TXT

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>


// System Headers
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include <glob.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "contour.h"
#include "kuwahara.h"

//namespace
using namespace cv;
using namespace std;
using namespace chrono;

#define Mpixel(image,x,y) ((uchar *)((image).data +(y)*((image).step)))[(x)*((image).channels())]//gray color space
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]	//Blue color space
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]	//Green color space
#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]	//Red color space
#define total_number_file 100
#define kuwahara_level 11
// #define class_number "0"


struct YOLO_TXT_INFO{
	int class_numb;
	float accuracy;
	Point p1;
	Point p2;
};

struct YOLO_AND_CONTOUR_OUTPUT_INFO{
	int class_numb;
	string img_name;
	string contour_txt;
	float height;
	float width;
	float x;
	float y;
};

struct YOLO_IMAGES{
	Mat ori_img;
	Mat ROI;
	Mat contour_ROI;
	Mat Threaholded_ROI;
};

class YOLO{
private:
	string Dataset_Path;
	string Path_Yolo_Text="Yolo_txt/*";
	string Path_Final_Dataset="Final_dataset/yolo/*";//static path

	glob_t glob_yolo;
	glob_t glob_final_data_set;
	string folder_name_root;
    string folder_name_contour;
    string folder_name_ori_img;
    string folder_name_roi;
    string index_file_name;
public:
	YOLO();
	YOLO(string i_dataset_path);
	~YOLO();
	void Main_Process();
	void Main_Process_Video();
	YOLO_IMAGES Cropping_and_Threasholding_in_Video(YOLO_TXT_INFO yolo_info, Mat ori_img, string img_name);
	Mat Subtract_and_Merge_in_Video(Mat target_img, Mat background_img, Rect frame);
	void Read_List_of_File_from_Directories();
	string Name_Change_txt_to_jpg(string input_name);
	string Extract_Filename_from_Full_Path(string input_name);
	bool Scan_The_File_Exist_in_Final_dataset(string yolo_file_name);
	YOLO_TXT_INFO Extract_Yolo_txt_Info(string line);
	YOLO_TXT_INFO Read_txt_File_Store_Yolo_Info(string file_path);
	void Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name);
	Mat Subtract_and_Merge(Mat target_img, string full_path_prev_file, string full_path_next_file, Rect frame);
	YOLO_AND_CONTOUR_OUTPUT_INFO Create_Yolo_Info(YOLO_TXT_INFO yolo_info, Mat final_ROI, Mat target_ori_img, string img_name);
	void Write_File(YOLO_AND_CONTOUR_OUTPUT_INFO yolo_out, string full_path);
	bool Contour_Process(YOLO_IMAGES input_yolo_images, YOLO_AND_CONTOUR_OUTPUT_INFO& yolo_output);
	int Determine_Process_Index_Numb(string folder_name_root, string index_file_name);
	void Save_Index(int idx, string file_root, string file_name);
	YOLO_IMAGES Cropping_and_Threasholding(YOLO_TXT_INFO yolo_info, Mat ori_img, string img_name);
	bool Read_Original_Image(string& full_path, Mat& output_ori_img);
	Mat Original_Image_to_Kuwahara_Image(Mat input_gray);
};	
#endif