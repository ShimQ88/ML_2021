#ifndef SEMI_AUTO_FILTERING_V2
#define SEMI_AUTO_FILTERING_V2

// System Headers
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <glob.h>
#include <sys/stat.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <bits/stdc++.h> 
//namespace
using namespace cv;
using namespace std;

class ROI_SUCCESS{
private:
    glob_t glob_contour, glob_yolo;
    int idx;
    int the_number_of_files;
    char *final_path_contour;
    char *final_path_yolo;
    char *parent_path_contour;
    char *parent_path_ori_img;
    Mat *image_contour;
    Mat *image_original;
    string *file_read;
    string folder_name_root;
    string folder_name_contour;
    string folder_name_yolo;
    Mat merged_img;
    string selected_idx; 
public:
    ROI_SUCCESS();
    ~ROI_SUCCESS();
    bool Creating_Root_Directory();
    bool Creating_Child_Directories();
    void Main_Process();
    string Decision_Part();
    bool Digit_Check(string input_digit);
    bool Determine_final_path_contour();
    bool Path_Check();
    int Determine_Folder_Numb();
    void Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name );
    void Read_Info_Contour_Yolo();
    void Save_Info_of_Selected_Imgs();
    void Save_Yolo(string img_name, string yolo_data);
    bool Name_Check(string temp_ori_name, string name_ori_img);
    bool Load_Img_and_Store_to_Mat();
    bool Load_Img_and_Store_to_Mat_V2();
    void Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[], int the_number_of_files);
};
#endif