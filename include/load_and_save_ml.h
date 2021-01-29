//./main -img_root /home/kyu/Desktop/Final_dataset_collection/the_first_yolo/final_dataset  -save data/the_first_cycle.xml  -data the_first_cycle_contours/final_contours.data
//./main -img_root "the directory of image files" -save "the place model save in e.g) data/file_name.xml" -data "location of the contour file"

#ifndef LOAD_AND_SAVE_ML_H
#define LOAD_AND_SAVE_ML_H

// System Headers
#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include "opencv2/ml.hpp"
// Opencv Headers
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "contour.h"
#include "kuwahara.h"


//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;

#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]
#define Mpixel(image,x,y) ((uchar *)((image).data +(y)*((image).step)))[(x)*((image).channels())]//gray color space

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "CV_8U"; break;
    case CV_8S:  r = "CV_8S"; break;
    case CV_16U: r = "CV_16U"; break;
    case CV_16S: r = "CV_16S"; break;
    case CV_32S: r = "CV_32S"; break;
    case CV_32F: r = "CV_32F"; break;
    case CV_64F: r = "CV_64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}


bool
load_and_save_ml( const string& data_filename, const string& img_root,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      int ml_technique);
bool
read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses );

void 
help(bool swap_the_role_train_to_test,int ntrain_samples, int ntest_samples, int ml_technique);

int
Count_Column_Numb(const string& filename);

class Machine_Learning_Data_Preparation{
// protected:
    // int hello;
public:
    Mat data;
    Mat responses;
    Mat CEs_data;
    string img_root;

    vector<string> imgname;
    int ntrain_samples;
    int ntest_samples;
    int class_count;
    Ptr<TrainData> *tdata;
    string filename_to_save;
    string filename_to_load;

    int k_fold_value;
    int n_total_samples;
    int the_number_of_data;
    int block;
    int value;
    int ml_technique;
    // Ptr<TrainData> tdata;

    // int t(){
    //  return hello;
    // }
    Mat *train_data;
    Mat *test_data;
    
    Mat *CEs_train_data;
    Mat *CEs_test_data;

    vector<string> *train_img_path;
    vector<string> *test_img_path;

    Mat *train_responses;
    Mat *test_responses;
    Mat *train_responses_int;
    Mat *test_responses_int;
// public:
    Machine_Learning_Data_Preparation(){cout<<"Success Intialize"<<endl;}
    ~Machine_Learning_Data_Preparation(){
        delete tdata[k_fold_value];
        delete train_data;
        delete test_data;
        delete train_responses;
        delete test_responses;
        delete train_responses_int;
        delete test_responses_int;

        delete CEs_train_data;
        delete CEs_test_data;

        delete train_img_path;
        delete test_img_path;
    }

    Machine_Learning_Data_Preparation(Mat i_data, Mat i_responses, Mat i_CEs_data, vector<string> i_imgname, string i_img_root, 
        int i_ntrain_samples, int i_ntest_samples, 
        const string& i_filename_to_save, const string& i_filename_to_load, int the_number_of_class, int the_number_of_CEs);

    // void Split(int input_value,int technique){
    void Main_Process(int technique);
    bool Split_train_test_data(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                    Mat *train_responses_int, Mat *test_responses_int, Mat *CEs_train_data, Mat *CEs_test_data,
                    vector<string> *train_img_path, vector<string> *test_img_path, 
                    vector<string> _imgname, Mat data, Mat responses, Mat CEs_data,
                    int block,int the_number_of_data, int n_total_samples, int ntest_samples, int technique);
    void Save_Path_of_Splited_Test_with_Train(int k_fold_value, string folder_path_for_saving_text, string root_path, vector<string> train_img_path, vector<string> test_img_path);
    inline TermCriteria TC(int iters, double eps);
};

Machine_Learning_Data_Preparation::Machine_Learning_Data_Preparation(Mat i_data, Mat i_responses, Mat i_CEs_data, vector<string> i_imgname, string i_img_root,
    int i_ntrain_samples, int i_ntest_samples, 
    const string& i_filename_to_save, const string& i_filename_to_load, int the_number_of_class, int the_number_of_CEs ){    
    // cout<<"filename_to_save: "<<&filename_to_save<<endl;
    data=i_data;
    responses=i_responses;
    ntrain_samples=i_ntrain_samples;
    ntest_samples=i_ntest_samples;
    filename_to_save=i_filename_to_save;
    filename_to_load=i_filename_to_load;
    imgname=i_imgname;
    img_root=i_img_root;

    class_count=the_number_of_class;

    // the_number_of_data=data.cols;
    the_number_of_data=the_number_of_CEs;
    // cout<<"the_number_of_data: "<<the_number_of_data<<endl;
    n_total_samples=ntrain_samples+ntest_samples;
    k_fold_value=n_total_samples/ntest_samples;
    CEs_data=i_CEs_data;
    // train_data = Mat::zeros( p_ntrain_samples, the_number_of_data, CV_32F );
    //      test_data = Mat::zeros( p_ntest_samples, the_number_of_data, CV_32F );


    //      train_responses = Mat::zeros( p_ntrain_samples, p_class_count, CV_32F );
    // test_responses = Mat::zeros( p_ntest_samples, p_class_count, CV_32F );
    //       train_responses_int = Mat::zeros( p_ntrain_samples, 1, CV_32F );
    // test_responses_int = Mat::zeros( p_ntest_samples, 1, CV_32F );
}

void Machine_Learning_Data_Preparation::Main_Process(int technique){
    //Split data
    value=0;
    cout<<"train_responses: "<<train_responses<<endl;
    // cout<<"p_class_count: "<<p_class_count<<endl;
    n_total_samples=ntrain_samples+ntest_samples;
    ml_technique=technique;
    train_data=new Mat[k_fold_value];
    test_data=new Mat[k_fold_value];
    train_responses=new Mat[k_fold_value];
    test_responses=new Mat[k_fold_value];
    train_responses_int=new Mat[k_fold_value];
    test_responses_int=new Mat[k_fold_value];

    
    CEs_train_data=new Mat[k_fold_value];
    CEs_test_data=new Mat[k_fold_value];


    train_img_path=new vector<string>[k_fold_value];
    test_img_path=new vector<string>[k_fold_value];
    
    tdata=new Ptr<TrainData>[k_fold_value];

    while(1){
        if(value==k_fold_value){break;}
        train_data[value] = Mat::zeros( ntrain_samples, the_number_of_data, CV_32F );
        test_data[value] = Mat::zeros( ntest_samples, the_number_of_data, CV_32F );

        CEs_train_data[value] = Mat::zeros( ntrain_samples, the_number_of_data, CV_32F );//20 is CEs values
        CEs_test_data[value] = Mat::zeros( ntest_samples, the_number_of_data, CV_32F );//20 is CEs values

        if(ml_technique==0){
            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32F );
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32F );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32F );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32F );
        }else if(ml_technique==1){
            // train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32S );
            // test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32S );
            //train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32S );
            // test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32S );

            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32S);
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32S );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32S );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32S );
        }else if(ml_technique==2){
            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32F );
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32F );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32F );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32F );

            // train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32FC1 );
            // test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32FC1 );
            // train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32FC1 );
            // test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32FC1 );
            
        }else{
            
        }
        
        // cout<<"p_ntest_samples: "<<p_ntest_samples<<endl;
        int block=ntest_samples*value;
        // cout<<"k_fold_value: "<<k_fold_value<<endl;
        // getchar();


        Split_train_test_data(&train_data[value], &test_data[value], &train_responses[value], &test_responses[value],
                    &train_responses_int[value], &test_responses_int[value], &CEs_train_data[value], &CEs_test_data[value],
                    &train_img_path[value], &test_img_path[value],
                    imgname, data, responses, CEs_data,
                    block, the_number_of_data, n_total_samples, ntest_samples,ml_technique);

        // cout<<"train_data[value]: "<<train_data[value]<<endl;
        // getchar();
        if(ml_technique==0){//ANN
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses[value]);//train_responses: 2col many rows
        }else if(ml_technique==1){//Boost
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses_int[value]);//train_responses_int: 1 col many rows
            // tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses[value]);//train_responses_int: 1 col many rows
        }else if(ml_technique==2){//RF
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses_int[value]);
        }else{

        }
        Save_Path_of_Splited_Test_with_Train(value,"Data_splited_path", img_root, train_img_path[value], test_img_path[value]);
        


        value++;
        // cout<<"train_data: "<<train_data[]<<endl;
        // getchar();
    }
    // cout<<"k_fold_value: "<<k_fold_value<<endl;
    // getchar();
    // tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
    // tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses_int);
        // cout<<"block: "<<block<<endl;

        // cout<<"test_responses: "<<test_responses<<endl;
        // cout<<"end"<<endl;
        // getchar();
}
void Machine_Learning_Data_Preparation::Save_Path_of_Splited_Test_with_Train(int k_fold_value, string folder_path_for_saving_text, string root_path, vector<string> train_img_path, vector<string> test_img_path){
    ofstream file_train(folder_path_for_saving_text+"/The_first_cycle_train"+to_string(k_fold_value)+".txt");
    ofstream file_test(folder_path_for_saving_text+"/The_first_cycle_test"+to_string(k_fold_value)+".txt");


    for(vector<string>::const_iterator i = train_img_path.begin(); i != train_img_path.end(); ++i){
        file_train<<root_path+"/"+*i;
        file_train<<endl;
    }

    for(vector<string>::const_iterator i = test_img_path.begin(); i != test_img_path.end(); ++i){
        file_test<<root_path+"/"+*i;
        file_test<<endl;
    }

    file_train.close();
    file_test.close();
}
bool Machine_Learning_Data_Preparation::Split_train_test_data(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                    Mat *train_responses_int, Mat *test_responses_int, Mat *CEs_train_data, Mat *CEs_test_data,
                    vector<string> *train_img_path, vector<string> *test_img_path, 
                    vector<string> _imgname, Mat data, Mat responses, Mat CEs_data,
                    int block,int the_number_of_data, int n_total_samples, int ntest_samples, int technique)
{
    int i_train=0;
    int i_test=0;

    // cout<<"imgname: "<<imgname<<endl;
    // for (std::vector<string>::const_iterator i = imgname.begin(); i != imgname.end(); ++i)
    //     std::cout << *i << endl;
    // getchar();
    // cout<<"n_total_samples: "<<n_total_samples<<endl;
    // getchar();
    // cout<<"size: "<<imgname.size()<<endl;
    // getchar();
    // vector<string>::const_iterator img_name_index = imgname.begin();
    for(int i=0;i<n_total_samples;i++){
        // int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
        int cls_label = responses.at<int>(i);// - 'A'; //change to numerical classes, still they read as chars
        // cout << "labels " << cls_label << endl;
        // cout<<"i:"<<i<<endl;
        if( (i>=block)&&(i<block+ntest_samples) ){
            // cout<<"i:"<<i<<endl;
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            test_img_path->push_back(imgname[i]);
            // test_img_path->push_back(*img_name_index);
            // img_name_index++;
            // getchar();
            if(ml_technique==1){
                if(cls_label==1){
                    test_responses->at<int>(i_test, cls_label) = 1;
                    // test_responses_int->at<int>(i_test,0)=cls_label;
                    test_responses_int->at<int>(i_test,0)=1;
                }
            }else{
                test_responses->at<float>(i_test, cls_label) = 1.f;
                test_responses_int->at<float>(i_test,0)=cls_label;
            }
            
        }else{//test part
            // img_name_index++;
            // cout<<"i:"<<i<<endl;
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            train_img_path->push_back(imgname[i]);
            if(ml_technique==1){

                // getchar();
                if(cls_label==1){
                    train_responses->at<int>(i_train, cls_label) = 1;
                    // train_responses_int->at<int>(i_train,0)=cls_label; 
                    train_responses_int->at<int>(i_train,0)=1; 
                }
            }else{
                train_responses->at<float>(i_train, cls_label) = 1.f;
                train_responses_int->at<float>(i_train,0)=cls_label; 
            }
            
        }
        for(int j=0;j<the_number_of_data;j++){
            if( (i>=block)&&(i<block+ntest_samples) ){
                // cout<<"imgname[j]: "<<imgname[j]<<endl;
                // test_img_path->push_back(imgname[j]);
                if(ml_technique==1){
                    test_data->at<int>(i_test,j)=data.at<int>(i,j);
                    CEs_test_data->at<int>(i_test,j)=CEs_data.at<int>(i,j);
                }else{
                    test_data->at<float>(i_test,j)=data.at<float>(i,j);
                    CEs_test_data->at<float>(i_test,j)=CEs_data.at<float>(i,j);
                }
            }else{
                // cout<<"imgname[j]: "<<imgname[j]<<endl;
                // train_img_path->push_back(imgname[j]);
                if(ml_technique==1){
                    train_data->at<int>(i_train,j)=data.at<int>(i,j);
                    CEs_train_data->at<int>(i_train,j)=CEs_data.at<int>(i,j);
                }else{
                    train_data->at<float>(i_train,j)=data.at<float>(i,j);
                    CEs_train_data->at<float>(i_train,j)=CEs_data.at<float>(i,j); 
                }
                
            }
        }
        if( (i>=block)&&(i<block+ntest_samples) ){
            i_test++;
        }else{
            i_train++;
                
        }
    }
 //    cout<<"n_total_samples: "<<n_total_samples<<endl;
 //    cout<<"end"<<endl;
 //    // getchar();

 //    cout<<"i_train: "<<i_train<<endl;
    // cout<<"i_test: "<<i_test<<endl;
    // getchar();
    return true;
}
inline TermCriteria Machine_Learning_Data_Preparation::TC(int iters, double eps){
    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}

template <class T>
T *Creat_Default_ML_Class(){
    return new T();
}

template <class T>
T *Creat_ML_Class(int p1, int p2, float p3, int p4, int p5){
    return new T(p1, p2, p3, p4, p5);
}
// template <class T>
// T *Creat_ML_Class(double p1, int p2, double p3){
//     return new T(p1, p2, p3);
// }
// template <class T>
// T *Creat_ML_Class(double p1, int p2, double p3){
//     return new T(p1, p2, p3);
// }
// template <class T>
// T *Creat_ML_Class(double p1, int p2, double p3){
//     return new T(p1, p2, p3);
// }



struct Test_Matrices{
    Mat target_color_img;
    Mat target_gray_img;
    Mat next_gray_img;
    Mat prev_gray_img;
    Mat next_ku_img;
    Mat prev_ku_img;
    Mat target_ku_img;
    Mat subtracted_gray_img;
    Mat subtracted_gray_img2;
    Mat thresholded_img;
    Mat ROI_img;
    int type;
    /*type detail
    -1=critical error(fail target load)
    0=fail next and prev file load
    1=success prev load
    2=success next load
    3=success next and prev load
    4=else from load
    */
};
//This is using for advanced test


class Parent_ML{
public:
    
    // Mat *confusion_matrix;
    Mat *confusion_matrix_test;
    Mat *confusion_matrix_train;

    // float *accuracy;
    float *accuracy_test;
    float *accuracy_train;
    // char **result_buffer;
    // float sum_accuracy=0;
    float sum_accuracy_test=0;
    float sum_accuracy_train=0;

    int ml_technique;
    // float mean;
    float mean_test;
    float mean_train;
    
    // float variance=0;
    float variance_test=0;
    float variance_train=0;
    
    // float sta_dev;
    float sta_dev_test;
    float sta_dev_train;

    char *result_buffer;
    char **result_buffer_test;
    char **result_buffer_train;

    // char *final_result_buffer;
    char *final_result_buffer_test;
    char *final_result_buffer_train;

    Machine_Learning_Data_Preparation *ml;
    Parent_ML();
    ~Parent_ML();
    Mat Original_Image_to_Kuwahara_Image(Mat input_gray);
    
    void Main_Process(Machine_Learning_Data_Preparation *&prepared_data);
    float Accuracy_Calculation(const Mat& confusion_matrix);
    bool Calculate_standard_deviation();
    float Calculate_Mean(float i_sum_accuracy, Machine_Learning_Data_Preparation *i_ml);
    float Calculate_Variance(float i_accuracy[],float i_mean, Machine_Learning_Data_Preparation *i_ml);
    float Calculate_Standard_Deviation(float i_variance);
    
    Mat test_and_save_classifier(const Ptr<StatModel>& model,const Mat& data, const Mat& responses, int ntrain_samples, int rdelta, const string& filename_to_save, int ml_technique);
    Mat Advanced_Test_and_Save_Classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const vector<string> imgname,
                                     const string& file_root, int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique );
    Mat Advanced_test_and_save_classifier2(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const Mat& data_contours,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique );
    
    bool Read_Original_Image(string& full_path, Mat& output_ori_img);//Advanced_Test_and_Save_Classifier
    void Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name);
    Test_Matrices part1(string root_target, string input_name,string root_object_dataset1, string root_object_dataset2);


    virtual void Intialize()=0;
    virtual Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml)=0;
    virtual float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml)=0;
    virtual float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml)=0;
    virtual void Calculate_Result()=0;
    virtual void Return_Parameter()=0;
    virtual string Head_Parameter()=0;
};

Parent_ML::Parent_ML(){
}

bool Parent_ML::Read_Original_Image(string& full_path, Mat& output_ori_img){
    output_ori_img=imread(full_path,0);
    if( output_ori_img.empty()==false ){
        return true;
    }else{

        int delimiter=0;
        int idx=0;

        delimiter = full_path.find('.');
        
        string extracted_name_without_ext = full_path.substr(0,delimiter);

        string extracted_name_with_ext;
        bool success=false;
        for(int i=0;i<4;i++){
            string ext;
            if(i==0){
                ext=".JPG";
            }else if(i==1){
                ext=".jpeg";
            }else if(i==2){
                ext=".JPEG";
            }else if(i==3){
                ext=".bmp";
            }else{//in case
                ext=".jpg";
            }
            extracted_name_with_ext=extracted_name_without_ext+ext;
            // cout<<"extracted_name_with_ext: "<<extracted_name_with_ext<<endl;
            // getchar();
            output_ori_img=imread(extracted_name_with_ext,0);
            if( output_ori_img.empty()==false ){
                success=true;
                break;
            }
        }
        if(success==true){
            full_path=extracted_name_with_ext;
            return true;
        }
    }
    return false;
}

void Parent_ML::Main_Process(Machine_Learning_Data_Preparation *&prepared_data){
    ml=prepared_data;

    // confusion_matrix=new Mat[ml->k_fold_value];
    confusion_matrix_test=new Mat[ml->k_fold_value];
    confusion_matrix_train=new Mat[ml->k_fold_value];

    // accuracy=new float[ml->k_fold_value];
    accuracy_test=new float[ml->k_fold_value];
    accuracy_train=new float[ml->k_fold_value];

    result_buffer=new char[50];
    result_buffer_test=new char*[ml->k_fold_value];
    result_buffer_train=new char*[ml->k_fold_value];

    // final_result_buffer=new char[50];
    final_result_buffer_test=new char[50];
    final_result_buffer_train=new char[50];
    

    // for(int i=0;i<ml->k_fold_value;i++){
    //     result_buffer[i]=new char[50];
    // }
    for(int i=0;i<ml->k_fold_value;i++){
        result_buffer_test[i]=new char[50];
    }
    for(int i=0;i<ml->k_fold_value;i++){
        result_buffer_train[i]=new char[50];
    }

    // cout<<"done read structure"<<endl;
    // getchar();
    Intialize();
    // Calculate_Result();
    // Calculate_standard_deviation();

    //virtual functions
    confusion_matrix_test=Calculate_Confusion_Matrices(0,ml);
    accuracy_test=Calculate_Accuracies(confusion_matrix_test, ml);
    sum_accuracy_test=Calculate_Sum_Accuracy(accuracy_test, ml);

    // cout<<"confusion_matrix_test: "<<confusion_matrix_test[0]<<endl;
    // getchar();
    confusion_matrix_train=Calculate_Confusion_Matrices(1,ml);
    accuracy_train=Calculate_Accuracies(confusion_matrix_train, ml);
    sum_accuracy_train=Calculate_Sum_Accuracy(accuracy_train, ml);
    
    //parent functions
    mean_test=Calculate_Mean(sum_accuracy_test, ml);
    variance_test=Calculate_Variance(accuracy_test, mean_test, ml);
    sta_dev_test=Calculate_Standard_Deviation(variance_test);

    mean_train=Calculate_Mean(sum_accuracy_train, ml);
    variance_train=Calculate_Variance(accuracy_train, mean_train, ml);
    sta_dev_train=Calculate_Standard_Deviation(variance_train);

    Return_Parameter();
    
    // getchar();


}

Parent_ML::~Parent_ML(){
    
    // for(int i=0;i<ml->k_fold_value;i++){
    //     delete result_buffer[i];
    // }

    for(int i=0;i<ml->k_fold_value;i++){
        delete result_buffer_test[i];
    }

    for(int i=0;i<ml->k_fold_value;i++){
        delete result_buffer_train[i];
    }

    delete result_buffer;
    // delete accuracy;
    delete accuracy_test;
    delete accuracy_train;

    // delete confusion_matrix;
    delete confusion_matrix_test;
    delete confusion_matrix_train;
}

float Parent_ML::Calculate_Mean(float i_sum_accuracy, Machine_Learning_Data_Preparation *i_ml){
    float out_mean=i_sum_accuracy/i_ml->k_fold_value;
    return out_mean;
}

float Parent_ML::Calculate_Variance(float i_accuracy[],float i_mean, Machine_Learning_Data_Preparation *i_ml){
    float out_variance=0;
    for(int j=0; j < i_ml->k_fold_value; j++){
        out_variance=out_variance+(i_accuracy[j]-i_mean)*(i_accuracy[j]-i_mean);
    }
    out_variance=out_variance/i_ml->k_fold_value;
    return out_variance;
}

float Parent_ML::Calculate_Standard_Deviation(float i_variance){
    float out_sta_dev=sqrt(i_variance);
    return out_sta_dev;
}

float Parent_ML::Accuracy_Calculation(const Mat& i_confusion_matrix){
    // load classifier from the specified file
    float out_accuracy;
    float total_accurate=i_confusion_matrix.at<int>(0,0)+i_confusion_matrix.at<int>(1,1);
    float total_number_of_values=i_confusion_matrix.at<int>(0,0)+i_confusion_matrix.at<int>(0,1)+
    i_confusion_matrix.at<int>(1,0)+i_confusion_matrix.at<int>(1,1);
    out_accuracy=total_accurate/total_number_of_values;
    return out_accuracy;
}
//////////
Mat Parent_ML::Original_Image_to_Kuwahara_Image(Mat input_gray){
    /*Memory Allocation*/
    // cout<<"input_gray.size(): "<<input_gray.size()<<endl;
    Mat output=Mat::zeros(input_gray.size(), IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
    double** integral_image=new double*[input_gray.cols+1];
    double** squared_integral_image=new double*[input_gray.cols+1];
    for(int i = 0; i < input_gray.cols+1; ++i){
        integral_image[i] = new double[input_gray.rows+1];
        squared_integral_image[i] = new double[input_gray.rows+1];
    }

    int kuwahara_level=11;//have to be odd number
    Integral_Gray_Initialize(input_gray,integral_image,squared_integral_image);//create summed-table to integral_image array.
    Kuwahara_Filter_Gray_With_Sum_Table(input_gray,output,integral_image,squared_integral_image,kuwahara_level);//Applying kuwahara filter to output using integral_image.
    
    /*Memory deallocation*/
    for(int i = 0; i < input_gray.cols+1; ++i) {
        delete [] integral_image[i];
        delete [] squared_integral_image[i];
    }
    delete [] integral_image;
    delete [] squared_integral_image;

    // Rect window_frame(kuwahara_level/2, kuwahara_level/2, 
    //     output.cols-(kuwahara_level-1), output.rows-(kuwahara_level-1) );

    // Mat final_output=output(window_frame).clone();


    return output;

}

void Parent_ML::Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name){    

    //the process extract extension
    int delimiter = input_name.find('.');
    string ext=input_name.substr(delimiter);
    input_name=input_name.substr(0,delimiter);
    // cout<<"full name: "<<input_name<<endl;
    // cout<<"ext: "<<ext<<endl;
    ////////////////////////////

    // string extracted_numb=input_name.substr(input_name.size()-6,2);
    string extracted_numb=input_name.substr(input_name.size()-3,3);
    string extracted_file_info=input_name.substr(0,input_name.size()-3);
    // input_name.substr(input_name.size()-6,2);
    input_name.substr(input_name.size(),3);

    // cout<<"extracted_file_info: "<<extracted_file_info<<endl;
    // cout<<"extracted_numb: "<<extracted_numb<<endl;
    int numb=stoi(extracted_numb);
    int next_numb;
    int prev_numb;
    string prev_file;
    string next_file;
    if((numb==0)||(numb==999)){//the case of 4digits required
        extracted_numb=input_name.substr(input_name.size()-4,4);
        extracted_file_info=input_name.substr(0,input_name.size()-4);
        numb=stoi(extracted_numb);
        next_numb=numb+1;
        prev_numb=numb-1;
        prev_file=extracted_file_info+to_string(prev_numb)+ext;
        next_file=extracted_file_info+to_string(next_numb)+ext;
    }else{
        next_numb=numb+1;
        prev_numb=numb-1;

        //when the value under 10 name affect
        if(prev_numb<10){
            prev_file=extracted_file_info+"00"+to_string(prev_numb)+ext;
        }else if((prev_numb>=10)&&(prev_numb<100)){
            prev_file=extracted_file_info+"0"+to_string(prev_numb)+ext;
        }else{
            prev_file=extracted_file_info+to_string(prev_numb)+ext;
        }

        if(next_numb<10){
            next_file=extracted_file_info+"00"+to_string(next_numb)+ext;
        }else if((next_numb>=10)&&(next_numb<100)){
            next_file=extracted_file_info+"0"+to_string(next_numb)+ext;
        }else{
            next_file=extracted_file_info+to_string(next_numb)+ext;
        }
    }
    
 
    
    
    // cout<<"prev_numb: "<<prev_numb<<endl;
    // cout<<"next_numb: "<<next_numb<<endl;
    
    

    // cout<<"prev_file: "<<prev_file<<endl;
    // cout<<"next_file: "<<next_file<<endl;
    // getchar();
    // string full_path_next_file=Dataset_Path+next_file;
    // string full_path_prev_file=Dataset_Path+prev_file;

    prev_name=prev_file;
    next_name=next_file;
    
}

Test_Matrices Parent_ML::part1(string root_target, string input_name,string root_object_dataset1, string root_object_dataset2){
    
    
    Test_Matrices final_output;

    //load the previous and next file names
    string prev_name, next_name;
    Return_Prev_and_Next_Full_File_Path(input_name, prev_name, next_name);

    //making the full path
    string full_target_path=root_target+"/"+input_name;
    string full_next_path=root_object_dataset1+"/"+next_name;
    string full_prev_path=root_object_dataset1+"/"+prev_name;
    
    Mat temp_target_img=imread(full_target_path,0);
    Mat temp_color_target_img=imread(full_target_path,1);
    if(temp_target_img.empty()==true){
        final_output.type=-1;
        return final_output;//critical error
    }


    Mat temp_next_img;
    Mat temp_prev_img;

    bool is_next_success=Read_Original_Image(full_next_path, temp_next_img);
    bool is_prev_success=Read_Original_Image(full_prev_path, temp_prev_img);

    if(is_prev_success==false && is_next_success==false){//the case could not find the files from dataset1
        // cout<<"the second try"<<endl;
        full_next_path=root_object_dataset2+"/"+next_name;
        full_prev_path=root_object_dataset2+"/"+prev_name;
        is_next_success=Read_Original_Image(full_next_path, temp_next_img);
        is_prev_success=Read_Original_Image(full_prev_path, temp_prev_img);
        
        // cout<<"is_next_success: "<<is_next_success<<endl;
        // cout<<"is_prev_success: "<<is_prev_success<<endl;

    }

    if(is_prev_success==false && is_next_success==false){//the last check
        final_output.type=0;
    }else if(is_prev_success==true && is_next_success==false){
        final_output.type=1;
    }else if(is_prev_success==false && is_next_success==true){
        final_output.type=2;
    }else if(is_prev_success==true && is_next_success==true){
        final_output.type=3;
    }else{
        final_output.type=4;
    }

    //resize process
    resize(temp_target_img, temp_target_img, cv::Size(640,640));//resized
    resize(temp_color_target_img, temp_color_target_img, cv::Size(640,640));//resized
    if(temp_prev_img.empty()==false){
        resize(temp_prev_img, temp_prev_img, cv::Size(640,640));//resized
    }

    if(temp_next_img.empty()==false){
        resize(temp_next_img, temp_next_img, cv::Size(640,640));//resized        
    }

    final_output.target_color_img=temp_color_target_img;
    final_output.target_gray_img=temp_target_img;
    final_output.next_gray_img=temp_next_img;
    final_output.prev_gray_img=temp_prev_img;
    



    return final_output;
}


Mat Parent_ML::Advanced_Test_and_Save_Classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const vector<string> imgname,
                                     const string& file_root, int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique )
{
    // int nsamples_all = data.rows;
    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    Mat confusion_Matrix = Mat::zeros( 2, 2, CV_32S );
    
    // getchar();
    int problem_val=0;
    system_clock::time_point start = system_clock::now();
    for(int i=0; i<nsamples_all; i++){
        // Mat sample = data.row(i);
        // int actual_value=responses.at<int>(i)-48;
        int actual_value;

        if(ml_technique==1){
            actual_value=responses.at<int>(i);
        }else{
            actual_value=responses.at<float>(i);
        }
        string data_root1="/home/kyu/Desktop/rodents_data";
        string data_root2="/home/kyu/Desktop/birds";

        Test_Matrices mat_collection=part1(file_root, imgname[i],data_root1, data_root2);        
        string temp_file_root;


        if(mat_collection.type==0){
            // cout<<"error to load the subtraction image"<<endl;
            // getchar();
            // CE_save<<imgname[i];
            // CE_save<<endl;
            continue;
        }

        // cout<<"p1"<<endl;
        if(mat_collection.type==1){
            mat_collection.subtracted_gray_img = mat_collection.prev_gray_img - mat_collection.target_gray_img;
            
            // mat_collection.prev_ku_img=Original_Image_to_Kuwahara_Image(mat_collection.prev_gray_img);
            // mat_collection.subtracted_gray_img=mat_collection.prev_ku_img-mat_collection.target_ku_img;

        }else if(mat_collection.type==2){
            mat_collection.subtracted_gray_img = mat_collection.next_gray_img-mat_collection.target_gray_img;
            // mat_collection.next_ku_img=Original_Image_to_Kuwahara_Image(mat_collection.next_gray_img);    
            // mat_collection.subtracted_gray_img=mat_collection.next_ku_img-mat_collection.target_ku_img;

        }else if(mat_collection.type==3){

            Mat temp_mat1=mat_collection.prev_gray_img-mat_collection.target_gray_img;
            Mat temp_mat2=mat_collection.next_gray_img-mat_collection.target_gray_img;
            mat_collection.subtracted_gray_img=temp_mat1+temp_mat2;

            // mat_collection.prev_ku_img=Original_Image_to_Kuwahara_Image(mat_collection.prev_gray_img);    
            // mat_collection.next_ku_img=Original_Image_to_Kuwahara_Image(mat_collection.next_gray_img);

            // Mat temp_mat1=mat_collection.prev_ku_img-mat_collection.target_ku_img;
            // Mat temp_mat2=mat_collection.next_ku_img-mat_collection.target_ku_img;
            // // mat_collection.subtracted_gray_img=mat_collection.prev_ku_img-mat_collection.target_ku_img;
            // // mat_collection.subtracted_gray_img2=mat_collection.next_ku_img-mat_collection.target_ku_img;
            // mat_collection.subtracted_gray_img=temp_mat1+temp_mat2;

            // // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
        }else{
            // cout<<"error P2"<<endl;
            // getchar();
            continue;
        }



        // cout<<"current size: "<<ku_current_img.size()<<endl;
        // cout<<"next size: "<<ku_next_img.size()<<endl;
        // cout<<"prev size: "<<ku_prev_img.size()<<endl;
        // cout<<"p2"<<endl;
        cv::threshold(mat_collection.subtracted_gray_img, mat_collection.thresholded_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // cv::threshold(mat_collection.subtracted_gray_img, mat_collection.subtracted_gray_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
        medianBlur(mat_collection.thresholded_img, mat_collection.thresholded_img, 7);
        // imshow("final_output",final_output);
        // waitKey(0);

        

        Point *p_start_roi_window;
        Point *p_end_roi_window;

        p_start_roi_window=new Point[200];//approx numb
        p_end_roi_window=new Point[200];//approx numb

        
        // Mat blob_window=Mat::zeros(final_output.size(),0);//default size initializing

        // cout<<"THis is point 2"<<endl;
        int count_numb=blob(mat_collection.thresholded_img, mat_collection.thresholded_img, p_start_roi_window, p_end_roi_window);
        
        // cout<<"THis is point 2-1"<<endl;
        if(count_numb==-100){
            // cout<<"Skip: too much blob"<<endl;
            // is_initialize_success=false;
            // cout<<"error"<<endl;
            continue;

        }//segmental fault

        Point p_center_of_object=draw_rect_box(mat_collection.target_color_img, p_start_roi_window, p_end_roi_window, 200);
        int cropping_size=determine_frame_size(mat_collection.thresholded_img, p_center_of_object,20,20);
        if(cropping_size==-1||cropping_size==-2){
            // cout<<"sizeing error"<<endl;
            // is_initialize_success=false;
            continue;
        }

        Mat out=Cropping_ROI(mat_collection.thresholded_img,p_center_of_object,cropping_size);
        
        Mat out1=Cropping_ROI(mat_collection.target_gray_img,p_center_of_object,cropping_size);
        out1=Original_Image_to_Kuwahara_Image(out1);
        Mat final_out;
        if(mat_collection.type==1){
            Mat out2=Cropping_ROI(mat_collection.prev_gray_img,p_center_of_object,cropping_size);
            out2=Original_Image_to_Kuwahara_Image(out2);
            final_out=out2-out1;
        }else if(mat_collection.type==2){
            Mat out2=Cropping_ROI(mat_collection.next_gray_img,p_center_of_object,cropping_size);
            out2=Original_Image_to_Kuwahara_Image(out2);
            final_out=out2-out1;
        }else if(mat_collection.type==3){
            Mat out2=Cropping_ROI(mat_collection.prev_gray_img,p_center_of_object,cropping_size);
            Mat out3=Cropping_ROI(mat_collection.next_gray_img,p_center_of_object,cropping_size);
            out2=Original_Image_to_Kuwahara_Image(out2);
            out3=Original_Image_to_Kuwahara_Image(out3);
            Mat temp_out1=out2-out1;
            Mat temp_out2=out3-out1;
            final_out=temp_out1+temp_out2;
            // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
        }else{
            cout<<"error P2"<<endl;
            getchar();
            // CE_save<<error_txt;
            // CE_save<<contour_txt;
            // CE_save<<endl;
            continue;
        }



        
        // cout<<"s1"<<endl;
        std::vector<vector<Point>>contours;

        Point ROI_mid_p;
        ROI_mid_p.x=out.rows/2;
        ROI_mid_p.y=out.cols/2;

        // imshow("out",out);
        // waitKey(0);
        // cv::threshold(out, out, 0, 255, THRESH_BINARY | THRESH_OTSU);
        cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // findContours(out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
        findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);

        // int largestcontour=FindTheLargestContour(contours);
        int object_i = Find_The_Object_Contour(contours,ROI_mid_p);

        if(object_i==-1){
            // is_initialize_success=false;
            // cout<<"contour fail"<<endl;
            // contour_txt="";
            continue;
        }

        // cout<<"s2"<<endl;
        Mat mat_contour=Mat::zeros(out.size(),CV_8UC3);
        Scalar color=CV_RGB(255,0,0);
        drawContours(mat_contour, contours, object_i, color,2,8);
        // cout<<"s3"<<endl;
        

        // cout<<"s3"<<endl;
        vector<float>CE;
        // cout<<"largestcontour: "<<largestcontour<<endl;
        // imshow("mat_contour", mat_contour);
        // waitKey(0);
        EllipticFourierDescriptors(contours[object_i],CE);
        // cout<<"s4"<<endl;
        
        // for(int i=1;i<n_CE;i++){
        //     cout<<"CE[i]: "<<CE[i]<<endl;
        // }
        // cout<<"data.row(i): "<<data.row(i)<<endl;
         // cout<<"data.name()"<< data.name()<< endl;


        
        // Mat sample=Mat::zeros(cv::Size(0,20),CV_32FC1);
        Mat sample=Mat::zeros(1, ml->the_number_of_data, CV_32FC1);
        // cout<<"data: "<<data.row(i)<<endl;
        Mat temp=data.row(i);
        string ty =  type2str( temp.type() );
        // printf("Matrix: %s %dx%d \n", ty.c_str(), temp.cols, temp.rows );


        // Mat sample= Mat::zeros( n_CE-1, 1, CV_32F );
        for(int i=1;i<ml->the_number_of_data+1;i++){
            sample.at<float>(0,i-1)=CE[i];
            // cout<<"CE[i]: "<<CE[i]<<endl;
        }
        // cout<<"sample: "<<sample<<endl;


        


        // for(int i=0;i<4;i++){
        //     sample.at<float>(0,i)=(float)CE[i];
        //     // Mpixel(sample,0,i)=(float)CE[i];
        // }
        // cout<<"haha1"<<endl;
        float r = model->predict( sample );
        // cout<<"haha2"<<endl;
        int r_int=(int)round(r);//random forrest case
        // cout<<"haha3"<<endl;
        // cout<<"s4.1"<<endl;
        // cout<<"predict: "<<r<<endl;


        // imshow("mat_contour",mat_contour);
        // waitKey(0);


        // imshow("ku_img",ku_img);
        // if(temp_next_img.empty()==false){
        //     imshow("temp_next_img",temp_next_img);
        // }
        // // imshow("temp_next_img",temp_next_img);
        // if(temp_prev_img.empty()==false){
        //     imshow("temp_prev_img",temp_prev_img);
        // }
        // imshow("final_output",final_output);
        // waitKey(0);
        // getchar();

        // int actual_value=responses.at<int>(i);//this is work for tech 1
        // cout<<"actual_value: "<<responses.at<int>(i)<<endl;
        // getchar();
        // cout << "Actual_ha: " << responses.at<float>(i) << " row " << sample << endl;
        // cout << "Actual: " << actual_value << endl;
        // // cout<<"sample: "<<sample<<endl;
        // float r = model->predict( sample );
        // cout<<"predict: "<<r<<endl;
        // getchar();
        // cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        // getchar();
        // int r_int=r;
        // int r_int=(int)round(r);//random forrest case
        // if( r_int == actual_value ){ //prediction is correct
        if( r_int == actual_value ){ //prediction is correct
            // cout<<"correct"<<endl;
            training_correct_predict++;
            // getchar();
        }
        // cout<<"s5"<<endl;
        confusion_Matrix.at<int>(actual_value,r_int)=confusion_Matrix.at<int>(actual_value,r_int)+1;
        // cout<<"predict: "<<r<<endl;
        // cout<<"actual_value: "<<actual_value<<endl;

        // if(actual_value==0){
        //     if(r>0.3){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{

        //     }
        // }else{
        //     if(r<0.7){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{
                
        //     }
        // }
        


        // cout<<"confusion_Matrix: "<<confusion_Matrix<<endl;
        // getchar();


        // cout << "training_correct_predict = " << training_correct_predict << endl;
        // getchar();

        // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
        // float r = model->predict( sample );
        // cout << "Predict:  r = " << r << endl;
        // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
        //     training_correct_predict++;
   
    // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;

    }

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    // *accuracy=training_correct_predict*100.0/ntrain_samples;
    // getchar();
    if( filename_to_save.empty() )  {
        printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);

    }
    // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
//     Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//     float r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
// //    sample1 = (Mat_<float>(1,9) << );//4
// //    r = model->predict( sample1 );
// //    cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
    // cout<<"problem_val: "<<problem_val<<endl;
    // getchar();
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    return confusion_Matrix;
    
/**********************************************************************/
}


Mat Parent_ML::test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique )
{
    // int nsamples_all = data.rows;
    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    Mat confusion_Matrix = Mat::zeros( 2, 2, CV_32S );
    
    // getchar();
    int problem_val=0;
    for(int i=0; i<nsamples_all; i++){
        Mat sample = data.row(i);
        // int actual_value=responses.at<int>(i)-48;
        int actual_value;
        if(ml_technique==1){
            actual_value=responses.at<int>(i);
        }else{
            actual_value=responses.at<float>(i);
        }
        // int actual_value=responses.at<int>(i);//this is work for tech 1
        // cout<<"actual_value: "<<responses.at<int>(i)<<endl;
        // getchar();
        // cout << "Actual_ha: " << responses.at<float>(i) << " row " << sample << endl;
        cout << "Actual: " << actual_value << endl;
        // cout<<"sample: "<<sample<<endl;
        float r = model->predict( sample );
        cout<<"predict: "<<r<<endl;
        // getchar();
        // cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        // getchar();
        // int r_int=r;
        int r_int=(int)round(r);//random forrest case
        // if( r_int == actual_value ){ //prediction is correct
        if( r_int == actual_value ){ //prediction is correct
            training_correct_predict++;
        }
        confusion_Matrix.at<int>(actual_value,r_int)=confusion_Matrix.at<int>(actual_value,r_int)+1;
        // if(actual_value==0){
        //     if(r>0.3){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{

        //     }
        // }else{
        //     if(r<0.7){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{
                
        //     }
        // }
        


        // cout<<"confusion_Matrix: "<<confusion_Matrix<<endl;
        // getchar();


        // cout << "training_correct_predict = " << training_correct_predict << endl;
        // getchar();

        // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
        // float r = model->predict( sample );
        // cout << "Predict:  r = " << r << endl;
        // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
        //     training_correct_predict++;
   
    // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;

    }

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    // *accuracy=training_correct_predict*100.0/ntrain_samples;
    // getchar();
    if( filename_to_save.empty() )  {
        printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);

    }
    // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
//     Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//     float r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
// //    sample1 = (Mat_<float>(1,9) << );//4
// //    r = model->predict( sample1 );
// //    cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
    cout<<"problem_val: "<<problem_val<<endl;
    // getchar();
    return confusion_Matrix;
    
/**********************************************************************/
}

Mat Parent_ML::Advanced_test_and_save_classifier2(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const Mat& data_contours,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique )
{
    // int nsamples_all = data.rows;
    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    Mat confusion_Matrix = Mat::zeros( 2, 2, CV_32S );
    
    // getchar();
    int problem_val=0;
    for(int i=0; i<nsamples_all; i++){
        // Mat sample = data.row(i);
        Mat sample = data_contours.row(i);
        // int actual_value=responses.at<int>(i)-48;
        int actual_value;
        if(ml_technique==1){
            actual_value=responses.at<int>(i);
        }else{
            actual_value=responses.at<float>(i);
        }
        // int actual_value=responses.at<int>(i);//this is work for tech 1
        // cout<<"actual_value: "<<responses.at<int>(i)<<endl;
        // getchar();
        // cout << "Actual_ha: " << responses.at<float>(i) << " row " << sample << endl;
        cout << "Actual: " << actual_value << endl;
        // cout<<"sample: "<<sample<<endl;
        float r = model->predict( sample );
        cout<<"predict: "<<r<<endl;
        // getchar();
        // cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        // getchar();
        // int r_int=r;
        int r_int=(int)round(r);//random forrest case
        // if( r_int == actual_value ){ //prediction is correct
        if( r_int == actual_value ){ //prediction is correct
            training_correct_predict++;
        }
        confusion_Matrix.at<int>(actual_value,r_int)=confusion_Matrix.at<int>(actual_value,r_int)+1;
        // if(actual_value==0){
        //     if(r>0.3){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{

        //     }
        // }else{
        //     if(r<0.7){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{
                
        //     }
        // }
        


        // cout<<"confusion_Matrix: "<<confusion_Matrix<<endl;
        // getchar();


        // cout << "training_correct_predict = " << training_correct_predict << endl;
        // getchar();

        // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
        // float r = model->predict( sample );
        // cout << "Predict:  r = " << r << endl;
        // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
        //     training_correct_predict++;
   
    // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;

    }

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    // *accuracy=training_correct_predict*100.0/ntrain_samples;
    // getchar();
    if( filename_to_save.empty() )  {
        printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);

    }
    // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
//     Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//     float r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
// //    sample1 = (Mat_<float>(1,9) << );//4
// //    r = model->predict( sample1 );
// //    cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
    cout<<"problem_val: "<<problem_val<<endl;
    // getchar();
    return confusion_Matrix;
    
/**********************************************************************/
}


template<class T>//base template before specialized
class Child_ML : public Parent_ML{
public:
    // Machine_Learning *ml;
    void Intialize(){cout<<"error choose different technique"<<endl;}
    void Return_Parameter(){cout<<"error"<<endl;}
    string Head_Parameter(){return "error This is default child";}
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
        Mat *out_mat;
        return out_mat;
    }
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
        float *out_float;
        return out_float;
    }
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
        return 0;
    }
};

template<>
class Child_ML<ANN_MLP> : public Parent_ML{
public:
    Ptr<ANN_MLP> *model;
    // int t_method = ANN_MLP::BACKPROP;//default
    int t_method = ANN_MLP::BACKPROP;//default
    // int t_method = 1;//default
    int a_function = ANN_MLP::SIGMOID_SYM;//default
    int max_iter=100;
    double method_param=0.1;

    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        max_iter=p1;
        method_param=p3;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    void Calculate_Result();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Return_Parameter();
    string Head_Parameter();
};

float *Child_ML<ANN_MLP>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

Mat *Child_ML<ANN_MLP>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            // out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            // out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
        
    }
    return out_confusion_matrix;
}

float Child_ML<ANN_MLP>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter(i);
    }
    return out_sum_accuracy;
}

void Child_ML<ANN_MLP>::Intialize(){
    system_clock::time_point start = system_clock::now();
    // double p_method_param, int p_max_iter
    cout<<"start initialize ANN"<<endl;
    // cout<<"ml->k_fold_value: "<<ml->k_fold_value<<endl;
    model=new Ptr<ANN_MLP>[ml->k_fold_value];
    
    cout<<"ml->the_number_of_data: "<<ml->the_number_of_data<<endl;
    int layer_sz[] = { ml->the_number_of_data, 100, 100, ml->class_count };
    cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
    int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
    cout << " nlayers  " << nlayers << endl;    
    Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );
    // cout<<"ml->tdata[i]: "<<ml->tdata[0]<<endl;
    for(int i=0;i<ml->k_fold_value;i++){
        cout << "iteration ("<<i<<") Training the classifier (may take a few minutes)...\n";
        model[i] = ANN_MLP::create();
        model[i]->setLayerSizes(layer_sizes);
        model[i]->setActivationFunction(a_function, 0, 0);
        // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
        model[i]->setTermCriteria(ml->TC(max_iter,method_param));
        model[i]->train(ml->tdata[i]);
    }
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    cout << endl;
}

void Child_ML<ANN_MLP>::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}

void Child_ML<ANN_MLP>::Return_Parameter(){
    // sprintf(result_buffer[index], "%d, %d, %d, %lf, %d, %d, %f \n", index, t_method, a_function, method_param, max_iter, ml->class_count, accuracy[index]);  //header
}

string Child_ML<ANN_MLP>::Head_Parameter(){
    return "Index, Method_Type, a_function, MethodParameter, MaxIteration, ClassCount, Accuracy";
}

template<>


class Child_ML<Boost> : public Parent_ML{

private:
    int boost_type=0;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
    int weak_count=100;
    float weight_trim_rate=80.83;
    int max_depth=10;
public:
    Ptr<Boost> *model;
    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        boost_type=p1;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
        weak_count=p2;
        weight_trim_rate=p3;
        max_depth=p4;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    void Calculate_Result();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Return_Parameter();
    string Head_Parameter();
};
void Child_ML<Boost> ::Intialize(){
    model=new Ptr<Boost>[ml->k_fold_value];
    system_clock::time_point start = system_clock::now();
    for(int i=0;i<ml->k_fold_value;i++){
        cout << "iteration ("<<i<<") Training the classifier (may take a few minutes)...\n";
        model[i]=Boost::create();
        model[i]->setBoostType(boost_type);  //Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
        model[i]->setWeakCount(weak_count);       //the Gentle best=98;
        model[i]->setWeightTrimRate(weight_trim_rate);//the Gentle best=0.83;
        model[i]->setMaxDepth(max_depth);         //the Gentle best=2;
        model[i]->setUseSurrogates(false);
        model[i]->setPriors(Mat()); 
        cout << "Training the classifier (may take a few minutes)...\n";
        model[i]->train(ml->tdata[i]);
    }
    /*Caculate performance of program*/
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    cout << endl;
}


Mat *Child_ML<Boost>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            // out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            // out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
    }
    return out_confusion_matrix;
}

float *Child_ML<Boost>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

float Child_ML<Boost>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter();
    }
    return out_sum_accuracy;
}

void Child_ML<Boost> ::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}


void Child_ML<Boost>::Return_Parameter(){
    // sprintf(result_buffer[index], "%d, %d, %d, %f, %d, %d, %f \n", index, boost_type, weak_count, weight_trim_rate, max_depth, ml->class_count, accuracy[index]);  //header
}

string Child_ML<Boost>::Head_Parameter(){
    return "Index, BoostType, WeakCount, WeightTrimRate, MaxDepth, ClassCount, Accuracy";
}


template<>
class Child_ML<RTrees> : public Parent_ML{

private:
    int max_depth=12;
    int min_sample_count=5;
    float regression_accuracy=0.01f;
    int max_categories=2;
    int tc_value=10;

public:
    Ptr<RTrees> *model;
    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        max_depth=p1;
        min_sample_count=p2;
        regression_accuracy=p3;
        max_categories=p4;
        tc_value=p5;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    void Calculate_Result();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Return_Parameter();
    string Head_Parameter();
};

void Child_ML<RTrees>::Intialize(){
    // sprintf(final_result_buffer, "%d, %d, %f, %d, %d", max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
    // cout<<"THis is RF"<<endl;
    // getchar();
    
    model=new Ptr<RTrees>[ml->k_fold_value];
    system_clock::time_point start = system_clock::now();
    for(int i=0;i<ml->k_fold_value;i++){
        model[i] = RTrees::create();
        model[i]->setMaxDepth(max_depth);
        model[i]->setMinSampleCount(min_sample_count);
        model[i]->setRegressionAccuracy(regression_accuracy);
        model[i]->setUseSurrogates(false /* true */);
        model[i]->setMaxCategories(2);
        model[i]->setTermCriteria(ml->TC(tc_value,0));
        cout << "Training the classifier (may take a few minutes)...\n";
        // model[i]->setTermCriteria(TermCriteria(TermCriteria::COUNT, 50, 0));
        model[i]->train(ml->tdata[i]);
    }
     /*Caculate performance of program*/
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    // fps = 1000000/seconds;

    // cout<<"tc_value: "<<tc_value<<endl;
    // getchar();
    cout << endl;
}

float *Child_ML<RTrees>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

Mat *Child_ML<RTrees>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){

        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            // out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            // out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root, i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
        
    }
    return out_confusion_matrix;
}

float Child_ML<RTrees>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter(i);
    }
    return out_sum_accuracy;
}

void Child_ML<RTrees>::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     // confusion_matrix[i]=test_and_save_classifier(model[i], ml->train_data[i], ml->train_responses_int[i], ml->ntrain_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}
void Child_ML<RTrees>::Return_Parameter(){
    // cout<<"h"<<endl;
    sprintf(result_buffer, "%d, %d, %f, %d, %d, %f, %f\n", max_depth, min_sample_count, regression_accuracy, tc_value, ml->class_count, mean_test*100, mean_train*100);  //header
    // sprintf(result_buffer[index], "%d, %d, %d, %f, %d, %d, %f \n", index, max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
}

string Child_ML<RTrees>::Head_Parameter(){
    return "MaxDepth, RegressionAccuracy, MaxCategories, TermCritera, ClassCount, Test_Accuracy, Train_Accuracy";
}

template<>
class Child_ML<SVM> : public Parent_ML{

private:
    // int max_depth=12;
    // int min_sample_count=5;
    // float regression_accuracy=0.01f;
    // int max_categories=2;
    // int tc_value=100;

public:
    Ptr<SVM> *model;
    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        // max_depth=p1;
        // min_sample_count=p2;
        // regression_accuracy=p3;
        // max_categories=p4;
        // tc_value=p5;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Calculate_Result();
    void Return_Parameter();
    string Head_Parameter();
};

void Child_ML<SVM>::Intialize(){
    // sprintf(final_result_buffer, "%d, %d, %f, %d, %d", max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
    // cout<<"THis is RF"<<endl;
    // getchar();
    
    model=new Ptr<SVM>[ml->k_fold_value];
    
    for(int i=0;i<ml->k_fold_value;i++){
        model[i] = SVM::create();
        model[i]->setType(SVM::EPS_SVR);//C_SVC, NU_SVC, ONE_CLASS, EPS_SVR, NU_SVR
        model[i]->setKernel(SVM::RBF);//CUSTOM, LINEAR, POLY, RBF, SIGMOID, CHI2, INTER
        model[i]->setDegree(0);
        model[i]->setGamma(10);
        model[i]->setCoef0(0.0);
        model[i]->setC(5);
        model[i]->setNu(0);
        model[i]->setP(10);
        model[i]->setClassWeights(Mat());
        model[i]->setTermCriteria(ml->TC(100,0));
        // model[i]->setTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
        cout << "Training the classifier (may take a few minutes)...\n";
        // model[i]->setTermCriteria(TermCriteria(TermCriteria::COUNT, 50, 0));
        model[i]->train(ml->tdata[i]);
    }
    // cout<<"tc_value: "<<tc_value<<endl;
    // getchar();
    cout << endl;
}

float *Child_ML<SVM>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

Mat *Child_ML<SVM>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];
    for(int i=0; i< i_ml->k_fold_value; i++){
        if(type==0){//test
            out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
    }
    return out_confusion_matrix;
}

float Child_ML<SVM>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter(i);
    }
    return out_sum_accuracy;
}

void Child_ML<SVM>::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     // confusion_matrix[i]=test_and_save_classifier(model[i], ml->train_data[i], ml->train_responses_int[i], ml->ntrain_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}
void Child_ML<SVM>::Return_Parameter(){
    // sprintf(result_buffer[index], "%d, %d, %d, %f, %d, %d, %f \n", index, max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
    
}

string Child_ML<SVM>::Head_Parameter(){
    return "Index, MaxDepth, RegressionAccuracy, MaxCategories, TermCritera, ClassCount, Accuracy";
}


class Write_File{
private:
    Parent_ML *final_ml;
    Machine_Learning_Data_Preparation *prepared_data;
    float mean;
    float variance; 
    float sta_dev; 
    int k_fold_value; 
    Mat *con_mat;
    char **buffer_file;
    string file_full_path;
    string file_the_best_full_path;
    string file_collection_full_path;
    ofstream file;
    ofstream file_the_best;
    ofstream file_collection;
public:
    Write_File();
    Write_File(Parent_ML *i_final_ml, Machine_Learning_Data_Preparation *i_prepared_data,string i_number_of_CE);
    ~Write_File();
    // void Main_Process(float mean, float variance,float sta_dev,int k_fold_value, Mat con_mat[],char **buffer_file);
    void Main_Process();
    void Write_Header(string header);
    string Create_file_path(string file_path, string file_name, string number_of_CE);
    bool The_File_Process();
    bool The_File_Collection_Process(float i_mean_test, float i_sta_dev_test, float i_mean_train, float i_sta_dev_train);
    bool The_Best_Process(float i_mean, float i_variance, float i_sta_dev, int i_k_fold_value,Mat *&i_con_mat, string type);
};

Write_File::Write_File(Parent_ML *i_final_ml, Machine_Learning_Data_Preparation *i_prepared_data, string i_number_of_CE){
    final_ml=i_final_ml;
    prepared_data=i_prepared_data;
    // mean=final_ml->mean;
    // variance=final_ml->variance; 
    // sta_dev=final_ml->sta_dev;
    // k_fold_value=prepared_data->k_fold_value;
    // con_mat=final_ml->confusion_matrix;
    // buffer_file=final_ml->result_buffer;
    // cout<<"i_number_of_CE: "<<i_number_of_CE<<endl;
    string file_path="resource/rf/";
    // file_full_path=Create_file_path(file_path,"min_sample_count_",i_number_of_CE);
    file_the_best_full_path=Create_file_path(file_path,"Calculate_standard_deviation_",i_number_of_CE);
    file_collection_full_path=Create_file_path(file_path,"accuracy_collection_",i_number_of_CE);

    // file.open(file_full_path, std::ios_base::app);
    file_collection.open(file_collection_full_path, std::ios_base::app);
    file_the_best.open(file_the_best_full_path, std::ios_base::app);

}

Write_File::~Write_File(){
    file.close();
    file_the_best.close();
    file_collection.close();
}
void Write_File::Write_Header(string header){
    file_collection<<header;
    file_collection<<endl;
    // file<<final_ml->Head_Parameter();
    file<<endl;
}
void Write_File::Main_Process(){
    // The_Best_Process();
    The_File_Collection_Process(final_ml->mean_test, final_ml->sta_dev_test, final_ml->mean_train, final_ml->sta_dev_train);
    // The_File_Process();

    // cout<<"final_ml->mean_test: "<<final_ml->mean_test<<endl;
    // cout<<"final_ml->variance_test: "<<final_ml->variance_test<<endl;
    // cout<<"final_ml->sta_dev_test: "<<final_ml->sta_dev_test<<endl;
    // cout<<"prepared_data->k_fold_value: "<<prepared_data->k_fold_value<<endl;
    // cout<<"final_ml->confusion_matrix_test: "<<final_ml->confusion_matrix_test<<endl;


    The_Best_Process(final_ml->mean_test, final_ml->variance_test, final_ml->sta_dev_test, 
        prepared_data->k_fold_value, final_ml->confusion_matrix_test, "test");
    The_Best_Process(final_ml->mean_train, final_ml->variance_train, final_ml->sta_dev_train, 
        prepared_data->k_fold_value, final_ml->confusion_matrix_train, "train");

}

string Write_File::Create_file_path(string file_path, string file_name, string number_of_CE){
    string file_name_extension=".txt";
    string full_file_name=file_name+number_of_CE+file_name_extension;
    string temp_file_full_path=file_path+full_file_name;
    cout<<"temp_file_full_path: "<<temp_file_full_path<<endl; 
    return temp_file_full_path;
}

bool Write_File::The_File_Process(){
    for(int i=0;i<k_fold_value;i++){
        file<<buffer_file[i];
    }
    return true;        
}

bool Write_File::The_File_Collection_Process(float i_mean_test, float i_sta_dev_test, float i_mean_train, float i_sta_dev_train){
    // char mse_buffer_test[70];
    // char mse_buffer_train[70];
    // sprintf(mse_buffer_test, "%1.f ± %1.f%% \n", i_mean_test*100, i_sta_dev_test*100);
    // sprintf(mse_buffer_train, "%1.f ± %1.f%% \n", i_mean_train*100, i_sta_dev_train*100);
    // file_collection<<"#Mean, Variance, Sta_dev, Mean Square Error";
    file_collection<<final_ml->result_buffer;
    // file_collection<<endl;
    // file_collection<<to_string(mean);
    // file_collection<<", ";
    // file_collection<<to_string(variance);
    // file_collection<<", ";
    // file_collection<<to_string(sta_dev);
    // file_collection<<", ";
    // file_collection<<mse_buffer;
    // file_collection<<endl;
    // file_collection<<mse_buffer_test;
    // file_collection<<", ";
    // file_collection<<to_string(i_mean_test);
    // file_collection<<endl; 
    
    return true;
}

bool Write_File::The_Best_Process(float i_mean, float i_variance, float i_sta_dev, int i_k_fold_value, Mat *&i_con_mat, string type){
    char mean_buffer[20],variance_buffer[40],sta_dev_buffer[40],mse_buffer[70];
    sprintf(mean_buffer, "#mean: %f \n", i_mean);
    sprintf(variance_buffer, "#variance: %f \n", i_variance);
    sprintf(sta_dev_buffer, "#sta_dev: %f \n", i_sta_dev);  //header
    sprintf(mse_buffer, "#Mean Square Error: %1.f ± %1.f%% \n", i_mean*100, i_sta_dev*100);
    cout<<"start Best"<<endl;
    cout<<"mean:"<<mean_buffer<<endl;
    cout<<"variance_buffer:"<<variance_buffer<<endl;
    cout<<"sta_dev_buffer:"<<sta_dev_buffer<<endl;
    cout<<"mse_buffer:"<<mse_buffer<<endl;
    cout<<"end Best"<<endl;
    file_the_best << "----------";
    file_the_best << type;
    file_the_best << "----------\n";
    if (file_the_best){
        // file_the_best<<"\n\n";    
        file_the_best<<mean_buffer;
        file_the_best<<variance_buffer;
        file_the_best<<sta_dev_buffer;
        file_the_best<<mse_buffer;
        file_the_best<<"\n\n";
        file_the_best<<"#Confusion Matrix\n";
        for(int i=0; i<i_k_fold_value; i++){
            char buffer[50];
            sprintf(buffer, "#k=%d\n", i);  //header
            file_the_best<<buffer;
            file_the_best<<"#";
            file_the_best<<i_con_mat[i].at<int>(0,0);
            file_the_best<<", ";
            file_the_best<<i_con_mat[i].at<int>(0,1);
            file_the_best<<"\n";
            file_the_best<<"#";
            file_the_best<<i_con_mat[i].at<int>(1,0);
            file_the_best<<", ";
            file_the_best<<i_con_mat[i].at<int>(1,1);
            file_the_best<<"\n\n";
        }
            
    }
    file_the_best << "--------";
    file_the_best << type;
    file_the_best << " end";
    file_the_best << "--------\n";
    return 0;
}
#endif // end of LOAD_AND_SAVE_ML_H