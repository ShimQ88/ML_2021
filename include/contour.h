#ifndef CONTOUR_H
#define CONTOUR_H

#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>


#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <cstdio>
#include <bits/stdc++.h>
#include <glob.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;
using namespace cv::ml;
using namespace chrono;

using namespace cv;
#define n_CE 21


#define PI 3.1415926535897932384626433832795029L

#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]

template<typename T>
static Ptr<T> load_classifier(const string& filename_to_load);

int Find_The_Object_Contour(std::vector<vector<Point>>contours,Point center_of_object);

void EllipticFourierDescriptors(std::vector<Point>& contour, std::vector<float> &CE);

void SkinTresholding(Mat3b& frame);

int FindTheLargestContour(std::vector<vector<Point>>contours);

void run_contour(char* argv);

class Directory_Creater{
public:
    string current_full_folder_path_roi;
    string current_full_folder_path_contour;
    string current_full_folder_path_ori;

    // string temp_file_name;
    string folder_name_root;
    
    string folder_name_roi;
    string folder_name_contour;
    string folder_name_ori_img;

    string contour_dir;

    int max_numb_of_files_in_folder;
    int current_file_numb;
    int current_folder_numb;
    
    bool is_root_folder_exsist=false;
    bool is_all_folder_exsist=false;

    ofstream contour_file;

    Directory_Creater(int i_max_numb_of_files_in_folder, string i_folder_name_root, string i_folder_name_roi, string i_folder_name_contour, string i_folder_name_ori_img){
        max_numb_of_files_in_folder=i_max_numb_of_files_in_folder;
        
        folder_name_root=i_folder_name_root;
        folder_name_roi=i_folder_name_roi;
        folder_name_contour=i_folder_name_contour;
        folder_name_ori_img=i_folder_name_ori_img;
        current_folder_numb=0;
        current_file_numb=0;
        
        Creating_Root_Directory();
        Creating_Child_Directories();
    }
    bool Main_Process(Mat roi_img, Mat contour_img, Mat ori_img ,string file_name,
    	string class_numb,float x, float y, float w, float h, string CE){
        int i=0;
        while(1){
            if((is_root_folder_exsist==true)&&(is_all_folder_exsist==true)){break;}//break condition;
            
            is_root_folder_exsist=Creating_Root_Directory();
            is_all_folder_exsist=Creating_Child_Directories();
            
            // if(i>105){//happen when loop occur forever
            //     cout<<"error from creating folder"<<endl;
            //     return "false";
            // }
            i++;
        }
        bool file_check;
        if((current_folder_numb==0)&&(current_file_numb==0)){//the program generate the first time
        	while(1){
        		file_check=Creating_Grand_Child_Directories();	
        		if(file_check==false){
        			current_folder_numb++;
        		}else{
        			break;
        		}
        	}
        	contour_dir = folder_name_root+folder_name_contour+to_string(current_folder_numb)+"/contour_"+to_string(current_folder_numb)+".txt";
        	contour_file.open(contour_dir);
        }

        Write_Img(roi_img, current_full_folder_path_roi, file_name, "_ROI.jpg");
        Write_Img(contour_img, current_full_folder_path_contour, file_name, "_contour.jpg");
        string temp_file_name=Write_Img(ori_img, current_full_folder_path_ori, file_name, ".jpg");
        
        current_file_numb++;

        bool is_file_reached_max_numb_capacity=Info_String_Merge(class_numb, temp_file_name,x , y, w, h, CE);
        return is_file_reached_max_numb_capacity;
    }
    
    bool Info_String_Merge(string class_numb, string temp_file_name,float x, float y, float w, float h, string CE){
		// string CE_val=ku_ROI.get_contour_txt();
		bool is_file_reached_max_numb_capacity=false;
		string text_info;
		string final_string=class_numb;
		final_string=final_string+';';
		final_string=final_string+temp_file_name;
		final_string=final_string+';';
		

		string yo_x=to_string(x);
		string yo_y=to_string(y);
		string yo_w=to_string(w);
		string yo_h=to_string(h);
		// cout<<"yo_x: "<<yo_x<<endl;
		// cout<<"float_yo_x: "<<ku_ROI.get_yolo_x()<<endl;
		// final_string=final_string+',';
		final_string=final_string+yo_x;
		// cout<<"final_string: "<<final_string<<endl;
		// cout<<"yo_x: "<<yo_x<<endl;
		final_string=final_string+' ';
		final_string=final_string+yo_y;
		// cout<<"final_string: "<<final_string<<endl;
		final_string=final_string+' ';
		final_string=final_string+yo_w;
		// cout<<"final_string: "<<final_string<<endl;
		final_string=final_string+' ';
		final_string=final_string+yo_h;
		// cout<<"final_string: "<<final_string<<endl;
		final_string=final_string+';';
		final_string=final_string+CE;

		// return final_string;
		contour_file << final_string;

		if(current_file_numb==max_numb_of_files_in_folder){
			contour_file.close();
			is_file_reached_max_numb_capacity=true;
		}
		return is_file_reached_max_numb_capacity;
		
	}
    

    ~Directory_Creater(){
    }
    
    bool Creating_Root_Directory(){
        int size=folder_name_root.length();
        char dirname[size+1];
        strcpy(dirname, folder_name_root.c_str());
        int check = mkdir(dirname,0777);
        
        if(!check){ 
            printf("The folder %s Directory created\n", dirname);
            return false;
        }else{
            printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
            return true;
        }
    }

    bool Creating_Child_Directories(){
        int i=0;
        bool all_file_exsist=true;
        while(1){
            string folder_full_path_name;
            
            if(i==0){
                folder_full_path_name=folder_name_root+folder_name_roi;
            }else if(i==1){
                folder_full_path_name=folder_name_root+folder_name_contour;
            }else if(i==2){
                folder_full_path_name=folder_name_root+folder_name_ori_img;
            }else{
                cout<<"index error"<<endl;
                return false;
            }

            int size=folder_full_path_name.length();

            char dirname[size+1];
            strcpy(dirname, folder_full_path_name.c_str());
            int check = mkdir(dirname,0777);
            
            if(!check){ 
                printf("The folder %s Directory created\n", dirname);
                all_file_exsist=false;
            }else{
                printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
            }
            if(i==2){break;}//loop breaker
            i++;
        }
        return all_file_exsist;//success making three files
    }

    bool Creating_Grand_Child_Directories(){
        int i=0;
        bool is_success_create_file=true;
        while(1){
            string folder_full_path_name;
            
            if(i==0){
                folder_full_path_name=folder_name_root+folder_name_roi+to_string(current_folder_numb)+"/";
                current_full_folder_path_roi=folder_full_path_name;
            }else if(i==1){
                folder_full_path_name=folder_name_root+folder_name_contour+to_string(current_folder_numb)+"/";
                current_full_folder_path_contour=folder_full_path_name;
            }else if(i==2){
                folder_full_path_name=folder_name_root+folder_name_ori_img+to_string(current_folder_numb)+"/";
                current_full_folder_path_ori=folder_full_path_name;
            }else{
                cout<<"index error"<<endl;
                // return false;
                is_success_create_file=false;
            }
            // cout<<"current_full_folder_path_roi:"<<current_full_folder_path_roi<<endl;
            // getchar();
            
            int size=folder_full_path_name.length();

            char dirname[size+1];
            strcpy(dirname, folder_full_path_name.c_str());
            int check = mkdir(dirname,0777);
            
            if(!check){ 
                printf("The folder %s Directory created\n", dirname);
                
            }else{
                printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
                // current_folder_numb++;
                // continue;
                is_success_create_file=false;
            }
            if(i==2){break;}//loop breaker
            i++;
        }
        return is_success_create_file;


    }
    string Write_Img(Mat saving_img, string folder_name, string file_name, string file_type){
        int delimiter=0;
        while(delimiter!=-1){
            delimiter = file_name.find('/');
            file_name = file_name.substr(delimiter+1);
        }
        file_name = file_name.substr(0,file_name.find('.'));
        // cout<<"file_name: "<<file_name<<endl;

        string output_name=file_name+file_type;
        string path_ROI=folder_name+file_name+file_type;

        imwrite( path_ROI, saving_img);
        return output_name;
    }
};

#endif //end of CONTOUR_H

