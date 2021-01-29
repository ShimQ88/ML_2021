//./main -data data/resource10.data
// Project Headers


// System Headers
#include <iostream>
#include <fstream>
#include <cmath>  

// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"


//THis is for tesing
#include <sys/stat.h>//This is using for mkdir
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
#include "semi_auto_filtering_v2.h"
#include "final_filtering.h"
#include "processing_with_yolo_txt.h"

///////////////////////////////

//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;


extern int Semi_Auto_Filtering(int argc,char *argv[]);
extern void Semi_Auto_Filtering_info();
extern int run_kuwahara(int argc,char *argv[]);
// extern int run_filtering(int argc,char *argv[]);
extern int run_filtering(int argc,char *argv[]);
extern void run_contour(char* argv);
extern bool load_and_save_ml( const string& data_filename, const string& img_root,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      int ml_technique);
extern void run_gnuplot(int technique);

#define TECHNIQUE 2//0:ANN 1:ADA 2:RF

void info(){
    cout<<"hello this is the program for machine learning"<<endl;
    cout<<"choose which technique you want to test for it"<<endl;
    cout<<"0 = neural_network"<<endl;
    cout<<"1 = ada_boost"<<endl;
    cout<<"2 = random_forest"<<endl;
    // cout<<"-detect"<<endl;
}

int main(int argc, char *argv[]){
    // cout<<"hell"<<endl;
    float accuracy;
    float value;
	string filename_to_save = "";
    string filename_to_load = "";
    string data_filename = "letter-recognition.data";
    string multiple_data_filename[3];
    multiple_data_filename[0]="";
    multiple_data_filename[1]="";
    multiple_data_filename[2]="";
    string img_root;

    string multiple_filename_to_save[3];
    multiple_filename_to_save[0] = "";
    multiple_filename_to_save[1] = "";
    multiple_filename_to_save[2] = "";
    bool train_and_test_mode=false;
    bool multiple_mode=false;
    info();
    // getchar();
    int numb_data=0;
    for(int i = 1; i<argc; i++){
        if(strcmp(argv[i],"-data2")==0){ // flag "-data letter_recognition.xml"
        	i++; 
            data_filename = argv[i];
        }else if( strcmp(argv[i],"-save") == 0 ){ // flag "-save filename.xml"
            // i++;
            // filename_to_save = argv[i];
            i++;
            int j=0;
            while(1){
                if(strcmp(argv[i],"-data") == 0){
                    break;
                }
                multiple_filename_to_save[j]=argv[i];
                i++;
                j++;
            }
            i--;
	    cout << "filename to save is "<< filename_to_save << endl;
        }else if( strcmp(argv[i],"-load") == 0){                 // flag "-load filename.xml"
            i++;
            filename_to_load = argv[i];
        }else if(strcmp(argv[i],"-contour") == 0){// flag "-contour" train and test
            run_contour(argv[2]);
            exit(1);//terminate program
        }else if(strcmp(argv[i],"-data") == 0){// flag "-multiple filename1.xml filename2.xml filename3.xml" train and test
            multiple_mode=true;
            i++;
            int j=0;
            while(1){
                if(i==argc){
                    break;
                }
                multiple_data_filename[j]=argv[i];
                i++;
                j++;
            }
            numb_data=j;
        }else if(strcmp(argv[i],"-img_root") == 0){// flag "-multiple filename1.xml filename2.xml filename3.xml" train and test
            i++;
            img_root=argv[i];
            // cout<<"main img_root: "<<img_root<<endl;
            // getchar();

        }else if(strcmp(argv[i],"-detect") == 0){
            int terminate_value=run_kuwahara(argc,argv);
            cout<<"terminate_value: "<<terminate_value<<endl;
            ofstream index_save;
            index_save.open ("ROI_images/index.txt");
            index_save << to_string(terminate_value);
            index_save.close();
            exit(10);
            // return terminate_value;

        }else if(strcmp(argv[i],"-filtering") == 0){
            int terminate_value=Semi_Auto_Filtering(argc,argv);
            cout<<"terminate_value: "<<terminate_value<<endl;
            
            if(terminate_value==10000000){
                cout<<"The error code -1, the command is not correct"<<endl;
            }else if(terminate_value==20000000){
                cout<<"The error code -2,  user press the ESC to stop programming"<<endl;
            }else{
            }
            ofstream index_save;
            index_save.open ("ROI_success/index.txt");
            index_save << to_string(terminate_value);
            index_save.close();    
            
            exit(10);
            // return 0;
        }else if(strcmp(argv[i],"-yolo") == 0){ 
            string temp_s;
            stringstream temp_ss;
            temp_ss << argv[2];
            temp_ss >> temp_s;
            // cout<<temp_s<<endl;
            YOLO yolo_class(temp_s);
            yolo_class.Main_Process();
            exit(9);
            
        }else if(strcmp(argv[i],"-video_yolo") == 0){ 
            string temp_s;
            stringstream temp_ss;
            temp_ss << argv[2];
            temp_ss >> temp_s;
            // cout<<temp_s<<endl;
            YOLO yolo_class(temp_s);
            yolo_class.Main_Process_Video();
            exit(9);
        }else if(strcmp(argv[i],"-filtering_v2") == 0){
            ROI_SUCCESS test;
            test.Main_Process();
            
            // cout<<"done2"<<endl;
            exit(10);
            // return 0;
        }else if(strcmp(argv[i],"-final_filtering") == 0){
            
            The_LAST_FILTERING test;

            test.Main_Process();
            
            cout<<"done2"<<endl;
            exit(10);
            // return 0;    
        }else if(strcmp(argv[i],"-yolo_prev") == 0){
            ifstream file_yolo("ROI_success/yolo/yolo.txt");//This is using for not overwrite
            ifstream file_contour("ROI_success/contour/contour.txt");//This is using for not overwrite
            
            String temp_path;
            if(file_contour){
                while(getline(file_contour,temp_path)){
                    int delimiter=0;
                    delimiter = temp_path.find(',');
                    temp_path = temp_path.substr(delimiter+1);
                    delimiter = temp_path.find(',');
                    temp_path = temp_path.substr(0, delimiter);
                    delimiter = temp_path.find('.');
                    temp_path = temp_path.substr(0, delimiter);
                    temp_path = temp_path+".txt";
                    ofstream yolo_save("ROI_success/yolo/"+temp_path);
                    String temp_yolo;
                    if(file_yolo){
                        getline(file_yolo,temp_yolo);
                        cout<<"temp_yolo: "<<temp_yolo<<endl;
                        cout<<"temp_path: "<<temp_path<<endl;
                        yolo_save<<temp_yolo;
                        
                    }
                    yolo_save.close();
                }
            }

            
        }

    }
    // #ifndef ADA_BOOST_H
    // build_boost_classifier( data_filename, filename_to_save, filename_to_load,0.5,false, &accuracy,&value );
    if(multiple_mode==true){
        // cout<<"numb_data: "<<numb_data<<endl;
        // getchar();
        for(int i=0;i<numb_data;i++){
            cout<<"multiple_data_filename[i]:"<<multiple_data_filename[i]<<endl;
            getchar();
            // cout<<"multiple_filename_to_save[i]:"<<multiple_mode_filename_to_save[i]<<endl;
            // load_and_save_ml(multiple_data_filename[i], multiple_filename_to_save[i], filename_to_load,0.85,TECHNIQUE);
            load_and_save_ml(multiple_data_filename[i], img_root, multiple_filename_to_save[i], filename_to_load,0.8,TECHNIQUE);
            
        }
    }else{
        // load_and_save_ml(data_filename, filename_to_save, filename_to_load,0.85,TECHNIQUE);
        load_and_save_ml(data_filename, img_root, filename_to_save, filename_to_load,0.8,TECHNIQUE);    
    }
    
    
    // cout<<"accuracy: "<<accuracy<<endl;
    cout<<"argv[1]: "<<argv[1]  <<endl;
    if(strcmp(argv[1],"-filtering_v2")==0){

    }else{
        run_gnuplot(TECHNIQUE);    
    }
    

    // build_boost_classifier(data_filename, filename_to_save, filename_to_load, 0.7, true );
    // build_random_forest_classifier( data_filename, filename_to_save, filename_to_load );
}

