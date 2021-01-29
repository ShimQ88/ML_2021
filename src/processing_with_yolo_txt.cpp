#include "processing_with_yolo_txt.h"

YOLO::YOLO(){
    folder_name_root="ROI_images_from_yolo_txt/";
    folder_name_contour="Contour/";
    folder_name_ori_img="Image/";
    folder_name_roi="ROI/";
    index_file_name="index.txt";

    // Creating_Root_Directory(folder_name_root);
    // Creating_Child_Directories(folder_name_root,folder_name_contour, folder_name_image, folder_name_roi);

    // cout<<"Construct called"<<endl;
}

YOLO::~YOLO(){
    // cout<<"Deconstuction called"<<endl;
}

YOLO::YOLO(string i_dataset_path){
    folder_name_root="ROI_images_from_yolo_txt/";
    folder_name_contour="Contour/";
    folder_name_ori_img="Image/";
    folder_name_roi="ROI/";
    index_file_name="index.txt";
    Dataset_Path=i_dataset_path;

    // Creating_Root_Directory(folder_name_root);
    // Creating_Child_Directories(folder_name_root,folder_name_contour, folder_name_image, folder_name_roi);

    // cout<<"Construct called"<<endl;
}


int YOLO::Determine_Process_Index_Numb(string folder_name_root, string index_file_name){
    int temp_i;
    ifstream load_index;
    string full_path=folder_name_root+index_file_name;
    load_index.open(full_path);
    string index_str;
    if(load_index){
        getline(load_index, index_str);
        temp_i=stoi(index_str);
    }else{
        temp_i=0;
    }
    load_index.close();
    return temp_i;
}

YOLO_IMAGES YOLO::Cropping_and_Threasholding_in_Video(YOLO_TXT_INFO yolo_info, Mat ori_img, string img_name){
    YOLO_IMAGES output_yolo_images;

    Rect window_frame(yolo_info.p1.x, yolo_info.p1.y, 
        yolo_info.p2.x-yolo_info.p1.x, yolo_info.p2.y-yolo_info.p1.y);
    
    Mat target_ROI=ori_img(window_frame).clone();

    Mat ROI=ori_img(window_frame).clone();
    string full_path_next_file;
    string full_path_prev_file;

    // Mat back_img=imread("test_00016.jpg",1);
    // Mat background_img=back_img(window_frame).clone();


    Mat back_img(2500, 2500, CV_8UC3, Scalar(255,255,255)); 
    Mat background_img=back_img(window_frame).clone();

    // cout<<"p10"<<endl;
    Return_Prev_and_Next_Full_File_Path(img_name, full_path_prev_file, full_path_next_file);
    // cout<<"p11"<<endl;

    Mat final_ROI=Subtract_and_Merge_in_Video(target_ROI, background_img, window_frame);
    // cout<<"p12"<<endl;
    // cvtColor(final_ROI, final_ROI, COLOR_BGR2GRAY);//color orginal image to gray scale
    // threshold(final_ROI, final_ROI, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // // median_filter(subtracted_frame,subtracted_frame,7);
    // medianBlur(final_ROI, final_ROI,5);


    output_yolo_images.ori_img=ori_img;
    output_yolo_images.ROI=ROI;
    output_yolo_images.contour_ROI=target_ROI;
    output_yolo_images.Threaholded_ROI=final_ROI;


    return output_yolo_images;
}

YOLO_IMAGES YOLO::Cropping_and_Threasholding(YOLO_TXT_INFO yolo_info, Mat ori_img, string img_name){
    YOLO_IMAGES output_yolo_images;

    Rect window_frame(yolo_info.p1.x, yolo_info.p1.y, 
        yolo_info.p2.x-yolo_info.p1.x, yolo_info.p2.y-yolo_info.p1.y);
    
    Mat target_ROI=ori_img(window_frame).clone();

    Mat ROI=ori_img(window_frame).clone();
    string full_path_next_file;
    string full_path_prev_file;
    // cout<<"p10"<<endl;
    Return_Prev_and_Next_Full_File_Path(img_name, full_path_prev_file, full_path_next_file);
    // cout<<"p11"<<endl;
    Mat final_ROI=Subtract_and_Merge(target_ROI, full_path_prev_file, full_path_next_file, window_frame);
    // cout<<"p12"<<endl;
    // cvtColor(final_ROI, final_ROI, COLOR_BGR2GRAY);//color orginal image to gray scale
    // threshold(final_ROI, final_ROI, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // // median_filter(subtracted_frame,subtracted_frame,7);
    // medianBlur(final_ROI, final_ROI,5);


    output_yolo_images.ori_img=ori_img;
    output_yolo_images.ROI=ROI;
    output_yolo_images.contour_ROI=target_ROI;
    output_yolo_images.Threaholded_ROI=final_ROI;


    return output_yolo_images;
}

bool YOLO::Contour_Process(YOLO_IMAGES input_yolo_images, YOLO_AND_CONTOUR_OUTPUT_INFO& yolo_output){
    Point ROI_mid_p;
    ROI_mid_p.x=input_yolo_images.Threaholded_ROI.rows/2;
    ROI_mid_p.y=input_yolo_images.Threaholded_ROI.cols/2;
    std::vector<vector<Point>>contours;
    findContours(input_yolo_images.Threaholded_ROI,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
    if(object_i==-1){//couldn't find any contour
        // cout<<"contour fail"<<endl;
        return false;
    }else{
        Scalar color=CV_RGB(255,0,0);
        drawContours(input_yolo_images.contour_ROI, contours, object_i, color, 3, 8);

        // imshow("final_ROI",final_ROI);
        // imshow("target_ROI",target_ROI);
        
        // YOLO_AND_CONTOUR_OUTPUT_INFO yolo_output=Create_Yolo_Info(temp_yolo_info, final_ROI, ori_img, img_name);
        

        // cout<<"yolo_output.class_numb: "<<yolo_output.class_numb<<endl;
        // cout<<"yolo_output.height: "<<yolo_output.height<<endl;
        // cout<<"yolo_output.width: "<<yolo_output.width<<endl;
        // cout<<"yolo_output.x: "<<yolo_output.x<<endl;
        // cout<<"yolo_output.y: "<<yolo_output.y<<endl;
        
        vector<float>CE;
        // cout<<"object_i: "<<object_i<<endl;
        
        EllipticFourierDescriptors(contours[object_i],CE);
        
        string contour_txt="";
        for(int i=1;i<n_CE;i++){
            contour_txt=contour_txt+to_string(CE[i]);
            if(i==n_CE-1){
                contour_txt=contour_txt+"\n";
                // cout<<"enter"<<endl;

                // strcat(contour_txt, "\n");
            }else{
                contour_txt=contour_txt+",";
                // strcat(contour_txt, ",");
            }
            
        }
        // cout<<"contour_txt: "<<contour_txt<<endl;
        yolo_output.contour_txt=contour_txt;
    }
    return true;

}

bool YOLO::Read_Original_Image(string& full_path, Mat& output_ori_img){
    output_ori_img=imread(full_path,1);
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
            output_ori_img=imread(extracted_name_with_ext,1);
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
void YOLO::Main_Process_Video(){
    Directory_Creater write_file(total_number_file,
        folder_name_root,
         folder_name_roi,
         folder_name_contour,
          folder_name_ori_img);
    bool is_file_reached_max_numb_capacity=false;

    int idx=Determine_Process_Index_Numb(folder_name_root, index_file_name);
    Read_List_of_File_from_Directories();
    cout<<"glob_yolo.gl_pathc: "<<glob_yolo.gl_pathc<<endl;
    cout<<"glob_final_data_set.gl_pathc: "<<glob_final_data_set.gl_pathc<<endl;
    
    YOLO_TXT_INFO temp_yolo_info;
    int i;
    for(i=idx;i<glob_yolo.gl_pathc;i++){
        string img_name=Name_Change_txt_to_jpg(glob_yolo.gl_pathv[i]);
        temp_yolo_info=Read_txt_File_Store_Yolo_Info(glob_yolo.gl_pathv[i]);
        string full_path=Dataset_Path+img_name;
        // cout<<"full_path: "<<full_path<<endl;
        // getchar();
        Mat ori_img;
        bool is_successfully_loaded_ori_img=Read_Original_Image(full_path, ori_img);

        if(is_successfully_loaded_ori_img==false){
            cout<<"fail to load original image"<<endl;
            continue;
        }
        // cout<<"p5"<<endl;
        // YOLO_IMAGES yolo_image_collection=Cropping_and_Threasholding(temp_yolo_info, ori_img, img_name);
        YOLO_IMAGES yolo_image_collection=Cropping_and_Threasholding_in_Video(temp_yolo_info, ori_img, img_name);
        // cout<<"p6"<<endl;
        YOLO_AND_CONTOUR_OUTPUT_INFO yolo_info_for_save=Create_Yolo_Info(temp_yolo_info, yolo_image_collection.contour_ROI, yolo_image_collection.ori_img, img_name);     
        // cout<<"p7"<<endl;
        bool is_found_contour=Contour_Process(yolo_image_collection,yolo_info_for_save);
        if(is_found_contour==true){
            is_file_reached_max_numb_capacity=write_file.Main_Process(yolo_image_collection.ROI, yolo_image_collection.contour_ROI, yolo_image_collection.ori_img, img_name,
            to_string(yolo_info_for_save.class_numb), 
            yolo_info_for_save.x, yolo_info_for_save.y, yolo_info_for_save.width, 
            yolo_info_for_save.height, yolo_info_for_save.contour_txt);
            // imshow("contour",yolo_image_collection.contour_ROI);
            // imshow("Threaholded_ROI",yolo_image_collection.Threaholded_ROI);

            // cout<<"p6"<<endl;
            // is_initialize_success=true;
            // cout<<"success initializing"<<endl;
            // Write_File(yolo_output, "test.txt");
            // Save_Index(idx, folder_name_root, index_file_name);
            // waitKey(0);

            if(is_file_reached_max_numb_capacity){break;}

        }else{

        }

    }
    i++;
    Save_Index(i, folder_name_root, index_file_name);
}


void YOLO::Main_Process(){
    Directory_Creater write_file(total_number_file,
        folder_name_root,
         folder_name_roi,
         folder_name_contour,
          folder_name_ori_img);
    bool is_file_reached_max_numb_capacity=false;

    int idx=Determine_Process_Index_Numb(folder_name_root, index_file_name);
	Read_List_of_File_from_Directories();
	// cout<<"glob_yolo.gl_pathc: "<<glob_yolo.gl_pathc<<endl;
 //    cout<<"glob_final_data_set.gl_pathc: "<<glob_final_data_set.gl_pathc<<endl;
    
    YOLO_TXT_INFO temp_yolo_info;
    int i;
    for(i=idx;i<glob_yolo.gl_pathc;i++){
        string img_name=Name_Change_txt_to_jpg(glob_yolo.gl_pathv[i]);
        bool is_exist_from_dataset=Scan_The_File_Exist_in_Final_dataset(img_name);//the case1: dataset is already arranged
        if(is_exist_from_dataset==true){continue;}//the First exception
        
        // cout<<"p1"<<endl;

        temp_yolo_info=Read_txt_File_Store_Yolo_Info(glob_yolo.gl_pathv[i]);
        
        // cout<<"p2"<<endl;
        if(temp_yolo_info.accuracy<=0.6){
            // cout<<"low accuracy skip"<<endl;
            continue;
        }//the second exception
        // cout<<"p3"<<endl;
        int current_class_numb=1;
        
        if(temp_yolo_info.class_numb!=current_class_numb){//wrong prediction
            cout<<"Wrong Prediction"<<endl;
            continue;
        }//the thrid exception
        // cout<<"p4"<<endl;
        string full_path=Dataset_Path+img_name;
        // cout<<"full_path: "<<full_path<<endl;
        Mat ori_img;
        bool is_successfully_loaded_ori_img=Read_Original_Image(full_path, ori_img);

        if(is_successfully_loaded_ori_img==false){
            cout<<"fail to load original image"<<endl;
            continue;
        }


        // cout<<"p5"<<endl;
        YOLO_IMAGES yolo_image_collection=Cropping_and_Threasholding(temp_yolo_info, ori_img, img_name);
        // cout<<"p6"<<endl;
        YOLO_AND_CONTOUR_OUTPUT_INFO yolo_info_for_save=Create_Yolo_Info(temp_yolo_info, yolo_image_collection.contour_ROI, yolo_image_collection.ori_img, img_name);     
        // cout<<"p7"<<endl;
        bool is_found_contour=Contour_Process(yolo_image_collection,yolo_info_for_save);
        if(is_found_contour==true){
            is_file_reached_max_numb_capacity=write_file.Main_Process(yolo_image_collection.ROI, yolo_image_collection.contour_ROI, yolo_image_collection.ori_img, img_name,
            to_string(yolo_info_for_save.class_numb), 
            yolo_info_for_save.x, yolo_info_for_save.y, yolo_info_for_save.width, 
            yolo_info_for_save.height, yolo_info_for_save.contour_txt);
            // imshow("contour",yolo_image_collection.contour_ROI);
            // imshow("Threaholded_ROI",yolo_image_collection.Threaholded_ROI);

            // cout<<"p6"<<endl;
            // is_initialize_success=true;
            // cout<<"success initializing"<<endl;
            // Write_File(yolo_output, "test.txt");
            // Save_Index(idx, folder_name_root, index_file_name);
            // waitKey(0);

            if(is_file_reached_max_numb_capacity){break;}

        }else{

        }
    }
    i++;
    Save_Index(i, folder_name_root, index_file_name);
}

void YOLO::Save_Index(int idx, string file_root, string file_name){
    string full_path=file_root+file_name;
    ofstream file;
    file.open(full_path);
    file<<to_string(idx);
    file.close();
}

void YOLO::Write_File(YOLO_AND_CONTOUR_OUTPUT_INFO yolo_out, string full_path){
    ofstream file;
    file.open(full_path,std::ios_base::app);
    file<<to_string(yolo_out.class_numb);
    file<<";";
    file<<yolo_out.img_name;
    file<<";";
    file<<to_string(yolo_out.x);
    file<<" ";
    file<<to_string(yolo_out.y);
    file<<" ";
    file<<to_string(yolo_out.width);
    file<<" ";
    file<<to_string(yolo_out.height);
    file<<";";
    file<<yolo_out.contour_txt;
    file.close();
}

YOLO_AND_CONTOUR_OUTPUT_INFO YOLO::Create_Yolo_Info(YOLO_TXT_INFO yolo_info, Mat final_ROI, Mat target_ori_img, string img_name){
    
    YOLO_AND_CONTOUR_OUTPUT_INFO yolo_out;
    float temp_x=( (yolo_info.p2.x-yolo_info.p1.x)/2 ) + yolo_info.p1.x;
    float temp_y=( (yolo_info.p2.y-yolo_info.p1.y)/2 ) + yolo_info.p1.y;
    float target_height=final_ROI.cols;
    float target_width=final_ROI.rows;

    yolo_out.img_name=img_name;
    yolo_out.class_numb=yolo_info.class_numb;
    yolo_out.height=target_height/(float)target_ori_img.rows;
    yolo_out.width=target_width/(float)target_ori_img.cols;
    yolo_out.x=temp_x/(float)target_ori_img.cols;
    yolo_out.y=temp_y/(float)target_ori_img.rows;

    return yolo_out;

}

Mat YOLO::Subtract_and_Merge_in_Video(Mat target_img, Mat background_img, Rect frame){

    Mat gray_target_img;
    cvtColor(target_img, gray_target_img, COLOR_BGR2GRAY);//color orginal image to gray scale
    

    // Mat background_img=imread(background_img_path,1);

    Mat gray_background_img;
    cvtColor(background_img, gray_background_img, COLOR_BGR2GRAY);//color orginal image to gray scale
        
    // cout<<"p22"<<endl;
    Mat ku_target_img=Original_Image_to_Kuwahara_Image(gray_target_img);
    threshold(ku_target_img, ku_target_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // threshold(ku_target_img, ku_target_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    // medianBlur(ku_target_img, ku_target_img,5);

    // cout<<"p23"<<endl;
    Mat ku_background_img=Original_Image_to_Kuwahara_Image(gray_background_img);
    threshold(ku_background_img, ku_background_img, 0, 255, THRESH_BINARY | THRESH_OTSU);

    // cout<<"p24"<<endl;
    Mat sub_final=ku_background_img-ku_target_img;
    // cout<<"p25"<<endl;
    medianBlur(sub_final, sub_final,5);
    // cout<<"p26"<<endl;
    // threshold(sub_final, sub_final, 0, 255, THRESH_BINARY | THRESH_OTSU);
    return sub_final;
}

Mat YOLO::Subtract_and_Merge(Mat target_img, string full_path_prev_file, string full_path_next_file, Rect frame){
    
    // cout<<"full_path_next_file: "<<full_path_next_file<<endl;
    // cout<<"full_path_prev_file: "<<full_path_prev_file<<endl;

    //reading part
    // Mat prev_img=imread(full_path_prev_file,1);
    // Mat next_img=imread(full_path_next_file,1);

    Mat prev_img=imread(full_path_prev_file,0);
    Mat next_img=imread(full_path_next_file,0);
    

    Mat sub_next, sub_prev;
    Mat gray_target_img;
    cvtColor(target_img, gray_target_img, COLOR_BGR2GRAY);//color orginal image to gray scale
    
    // cout<<"p22"<<endl;
    Mat ku_target_img=Original_Image_to_Kuwahara_Image(gray_target_img);
    threshold(ku_target_img, ku_target_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // threshold(ku_target_img, ku_target_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    // medianBlur(ku_target_img, ku_target_img,5);

    if (next_img.empty()==true){
        // cout<<"next_img file error"<<endl;
        // sub_next=Mat::zeros(target_img.size(),CV_8UC3);
        sub_next=Mat::zeros(target_img.size(),IMREAD_GRAYSCALE);
        // Size f_size=target_img.size();

        // f_size.width=f_size.width-(kuwahara_level-1);
        // f_size.height=f_size.height-(kuwahara_level-1);
        // sub_next=Mat::zeros(f_size,IMREAD_GRAYSCALE);
        

    }else{
        Mat ROI_next_img=next_img(frame).clone();
        Mat ku_ROI_next_img=Original_Image_to_Kuwahara_Image(ROI_next_img);
        threshold(ku_ROI_next_img, ku_ROI_next_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // threshold(ku_ROI_next_img, ku_ROI_next_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
        // medianBlur(ku_ROI_next_img, ku_ROI_next_img,5);
        sub_next=ku_ROI_next_img-ku_target_img;
        
        // imshow("next",ku_ROI_next_img);
        // sub_next=ROI_next_img-target_img;
    }

    if (prev_img.empty()==true){
        // cout<<"prev_img file error"<<endl;
        // sub_prev=Mat::zeros(target_img.size(),CV_8UC3);
        sub_prev=Mat::zeros(target_img.size(),IMREAD_GRAYSCALE);
        // Size f_size=target_img.size();

        // f_size.width=f_size.width-(kuwahara_level-1);
        // f_size.height=f_size.height-(kuwahara_level-1);
        // sub_prev=Mat::zeros(f_size,IMREAD_GRAYSCALE);
        
    }else{
        Mat ROI_prev_img=prev_img(frame).clone();
        Mat ku_ROI_prev_img=Original_Image_to_Kuwahara_Image(ROI_prev_img);
        threshold(ku_ROI_prev_img, ku_ROI_prev_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // threshold(ku_ROI_prev_img, ku_ROI_prev_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
        
        // medianBlur(ku_ROI_prev_img, ku_ROI_prev_img,5);
        sub_prev=ku_ROI_prev_img-ku_target_img;
        
        // imshow("prev",ku_ROI_prev_img);

        // sub_prev=ROI_prev_img-ku_target_img;
    }
    // imshow("ku_target_img",ku_target_img);
    // waitKey(0);


    // cvtColor(final_ROI, final_ROI, COLOR_BGR2GRAY);//color orginal image to gray scale
    // threshold(final_ROI, final_ROI, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // median_filter(subtracted_frame,subtracted_frame,7);
    // medianBlur(final_ROI, final_ROI,5);


    
    Mat sub_final=sub_next+sub_prev;
    medianBlur(sub_final, sub_final,5);
    // threshold(sub_final, sub_final, 0, 255, THRESH_BINARY | THRESH_OTSU);
    return sub_final;
}
Mat YOLO::Original_Image_to_Kuwahara_Image(Mat input_gray){
    /*Memory Allocation*/
    // cout<<"input_gray.size(): "<<input_gray.size()<<endl;
    Mat output=Mat::zeros(input_gray.size(), IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
    double** integral_image=new double*[input_gray.cols+1];
    double** squared_integral_image=new double*[input_gray.cols+1];
    for(int i = 0; i < input_gray.cols+1; ++i){
        integral_image[i] = new double[input_gray.rows+1];
        squared_integral_image[i] = new double[input_gray.rows+1];
    }

    // int kuwahara_level=11;//have to be odd number
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
void YOLO::Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name){    

    //the process extract extension
    int delimiter = input_name.find('.');
    string ext=input_name.substr(delimiter);
    // cout<<"ext: "<<ext<<endl;
    ////////////////////////////

    string extracted_numb=input_name.substr(input_name.size()-6,2);
    string extracted_file_info=input_name.substr(0,input_name.size()-6);
    input_name.substr(input_name.size()-6,2);

    int numb=stoi(extracted_numb);
    int prev_numb=numb-1;
    int next_numb=numb+1;
    
    // cout<<"prev_numb: "<<prev_numb<<endl;
    // cout<<"next_numb: "<<next_numb<<endl;
    
    string prev_file;
    string next_file;

    //when the value under 10 name affect
    if(prev_numb<10){
        prev_file=extracted_file_info+"0"+to_string(prev_numb)+ext;
    }else{
        prev_file=extracted_file_info+to_string(prev_numb)+ext;
    }

    if(next_numb<10){
        next_file=extracted_file_info+"0"+to_string(next_numb)+ext;
    }else{
        next_file=extracted_file_info+to_string(next_numb)+ext;
    }

    // cout<<"prev_file: "<<prev_file<<endl;
    // cout<<"next_file: "<<next_file<<endl;

    string full_path_next_file=Dataset_Path+next_file;
    string full_path_prev_file=Dataset_Path+prev_file;

    prev_name=full_path_prev_file;
    next_name=full_path_next_file;
    
}



YOLO_TXT_INFO YOLO::Read_txt_File_Store_Yolo_Info(string file_path){//This is not working if detected over 200
    ifstream file;
    file.open(file_path);
    int idx=0;
    YOLO_TXT_INFO yolo_info[200];
    YOLO_TXT_INFO output_yolo_info;
    if( file.is_open() ){
        string line;
        while ( getline(file,line) ){
            yolo_info[idx]=Extract_Yolo_txt_Info(line);
            // cout<<"line: "<<line<<endl;
            // cout<<"yolo_info.class_numb: "<<yolo_info.class_numb<<endl;
            // cout<<"yolo_info.accuracy: "<<yolo_info.accuracy<<endl;
            // cout<<"yolo_info.p1.x: "<<yolo_info.p1.x<<endl;
            // cout<<"yolo_info.p1.y: "<<yolo_info.p1.y<<endl;
            // cout<<"yolo_info.p2.x: "<<yolo_info.p2.x<<endl;
            // cout<<"yolo_info.p2.y: "<<yolo_info.p2.y<<endl;
            // getchar();
            idx++;
        }
        if(idx==1){
            // cout<<"The single line"<<endl;
            output_yolo_info=yolo_info[0];
        }else{
            // cout<<"The multiple line"<<endl;
            // getchar();
            int best_accuracy_idx=0;
            float the_highest_accuracy=-1;
            for(int i=0;i<idx;i++){
                if( the_highest_accuracy<yolo_info[i].accuracy ){
                    the_highest_accuracy=yolo_info[i].accuracy;
                    best_accuracy_idx=i;
                }

            }
            output_yolo_info=yolo_info[best_accuracy_idx];
        }
    }else{

    }
    return output_yolo_info;
}
YOLO_TXT_INFO YOLO::Extract_Yolo_txt_Info(string line){
    int delimiter=0;
    int idx=0;
    YOLO_TXT_INFO Yolo_info;
    while(1){//The process to remove extensive from the name full name 
        delimiter = line.find(',');
        if(delimiter==-1){break;}
        
        string extracted_info = line.substr(0,delimiter);
        
        line = line.substr(delimiter+1);

        if(idx==0){
            Yolo_info.class_numb=stoi(extracted_info);
        }else if(idx==1){
            Yolo_info.accuracy=stof(extracted_info);
        }else if(idx==2){
            Yolo_info.p1.x=stof(extracted_info);
        }else if(idx==3){
            Yolo_info.p1.y=stof(extracted_info);
        }else if(idx==4){
            Yolo_info.p2.x=stof(extracted_info);
        }else{

        }
        idx++;
    }
    Yolo_info.p2.y=stof(line);
    return Yolo_info;
}

bool YOLO::Scan_The_File_Exist_in_Final_dataset(string yolo_file_name){
    bool is_the_file_existing_in_dataset=false;
    for(int i=0;i<glob_final_data_set.gl_pathc;i++){
        string file_name_from_dataset=Extract_Filename_from_Full_Path(glob_final_data_set.gl_pathv[i]);

        // cout<<"file_name_from_dataset: "<<file_name_from_dataset<<endl;
        // cout<<"yolo_file_name: "<<yolo_file_name<<endl;
        if(yolo_file_name==file_name_from_dataset){
            is_the_file_existing_in_dataset=true;
            break;
        }
    }
    return is_the_file_existing_in_dataset;
}   

string YOLO::Name_Change_txt_to_jpg(string input_name){
    input_name=Extract_Filename_from_Full_Path(input_name);
    // cout<<"input_name: "<<input_name<<endl;
    
    int delimiter=0;
    while(1){//The process to remove extensive from the name full name 
        delimiter = input_name.find('.');
        if(delimiter==-1){break;}
        input_name = input_name.substr(0, delimiter);
        
        // cout<<"temp_name: "<<temp_name<<endl;
        // input_name = input_name.substr(delimiter+1);

        // char digit_str[temp_digit.length()];
        // for(int i=0;i<temp_digit.length();i++){
        //     char digit_str[1];
        //     digit_str[0]=temp_digit[i];
        //     if(isdigit(digit_str[0])==false){
        //         return false;
        //     }
        // }
    }
    string output_name=input_name+".jpg";
    return output_name;   
}

string YOLO::Extract_Filename_from_Full_Path(string input_name){
    int delimiter=0;
    string output_name;
    // cout<<"input_name: "<<input_name<<endl;
    // getchar();

    while(1){//The process to extract only the name from full path
        delimiter = input_name.find('/');
        if(delimiter==-1){break;}
        input_name = input_name.substr(delimiter+1);
    }
    // cout<<"input_name: "<<input_name<<endl;
    // cout<<"output_name: "<<output_name<<endl;
    // getchar();
    output_name=input_name;
    return output_name;
}



void YOLO::Read_List_of_File_from_Directories(){
    glob_t temp_glob_yolo;
    glob_t temp_glob_final_data_set;

    // string Path_Yolo_Text="Yolo_txt/*";
    // string Path_Final_Dataset="Final_dataset/yolo/*";//static path
    

    char path_yolo_char[Path_Yolo_Text.length()+1];
    char path_final_data_set_char[Path_Final_Dataset.length()+1];
    
    for (int j=0; j<sizeof(Path_Yolo_Text);j++) { 
        path_yolo_char[j] = Path_Yolo_Text[j];
    }

    for (int j=0; j<sizeof(Path_Final_Dataset);j++) { 
        path_final_data_set_char[j] = Path_Final_Dataset[j];
    }

    
    glob(path_yolo_char,GLOB_TILDE,NULL,&temp_glob_yolo);
    glob(path_final_data_set_char,GLOB_TILDE,NULL,&temp_glob_final_data_set);
    
    glob_yolo=temp_glob_yolo;
    glob_final_data_set=temp_glob_final_data_set;

    //This is debugging place checking whether the file read to success 
    // cout<<"glob_yolo.gl_pathv[0]:"<<glob_yolo.gl_pathv[0]<<endl;
    // cout<<"glob_final_data_set.temp_glob_final_data_set: "<<glob_final_data_set.gl_pathc<<endl;
    // getchar();
}
