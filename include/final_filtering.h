#ifndef FINAL_FILTERING
#define FINAL_FILTERING



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


class The_LAST_FILTERING{
private:
    glob_t glob_contour, glob_yolo;
    int idx;
    int the_number_of_selected_idx;
    int *the_list_of_selected_idx;
    Mat *img_contour;
    Mat *img_yolo;
    string *file_info;
    string *file_info_name;
    string folder_name_root;
    string folder_name_contour;
    string folder_name_yolo;
    string selected_idx;
    Mat merged_img;
public:
    int the_number_of_file_on_grid=100;
    The_LAST_FILTERING();
    ~The_LAST_FILTERING();
    bool Creating_Root_Directory();
    bool Creating_Child_Directories();
    void Read_List_of_File_from_Directories();
    void Main_Process();
    int Determine_Process_Index_Numb();
    int Validate_Check(int index);
    
    void Load_Mat_with_Index();
    void Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name );
    void Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[],int the_number_of_files);
    string Decision_Part();
    bool Digit_Check(string input_digit);
    void Save_Info_of_Selected_Imgs();
    int Count_the_Number_of_Index(string input_str);
    void Save_Yolo(string img_name, string yolo_data);
    bool Name_Check(string temp_ori_name, string name_ori_img);
    void Save_index_to_int(string input_str, int input_int[]);
    void Save_Index(int index);
    ////filtering version 2
    
};
#endif