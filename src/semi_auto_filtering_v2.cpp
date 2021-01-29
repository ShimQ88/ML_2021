#include "semi_auto_filtering_v2.h"

ROI_SUCCESS::ROI_SUCCESS(){
    folder_name_root="ROI_success/";
    folder_name_contour="contour/";
    folder_name_yolo="yolo/";

    Creating_Root_Directory();
    Creating_Child_Directories();
}

ROI_SUCCESS::~ROI_SUCCESS(){
    delete[] final_path_contour;
    delete[] final_path_yolo;
    delete[] parent_path_ori_img;
    delete[] parent_path_contour;
    delete[] image_contour;
    delete[] image_original;
    delete[] file_read;
    cout<<"Deconstruction called"<<endl;
    // getchar();
}

bool ROI_SUCCESS::Creating_Root_Directory(){
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

bool ROI_SUCCESS::Creating_Child_Directories(){
    int i=0;
    bool all_file_exsist=true;
    while(1){
        string folder_full_path_name;
        
        if(i==0){
            folder_full_path_name=folder_name_root+folder_name_contour;
        }else if(i==1){
            folder_full_path_name=folder_name_root+folder_name_yolo;
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
        if(i==1){break;}//loop breaker
        i++;
    }
    return all_file_exsist;//success making three files
}

void ROI_SUCCESS::Main_Process(){
    bool path_check=Determine_final_path_contour();
    if(path_check==false){
        cout<<"Path problem"<<endl;
        return;
    }
    cout<<"p1"<<endl;
    // bool is_the_number_of_files_correct=Load_Img_and_Store_to_Mat();
    //in case error when txt is located in the last of folder. 

    bool is_the_number_of_files_correct=Load_Img_and_Store_to_Mat_V2();
    //in case error when txt is located in the first of folder. 
    if(is_the_number_of_files_correct==false){
        cout<<"The second problem"<<endl;
        return;
    }
    // cout<<"p2"<<endl;
    Merge_Multi_Mats_to_Single_Mat_v2(image_contour,the_number_of_files);
    // cout<<"p3"<<endl;
    string decision=Decision_Part();
    if(decision.compare("ND")==0){//I made this one owing to avoiding mistake input
        cout<<"input Error try again: Press enter"<<endl;
        
    }else if(decision.compare("skip")==0){
        cout<<"skip and looking next folder"<<endl;
        ofstream index_save;
        index_save.open("ROI_success/index.txt");
        index_save<<to_string(++idx);
        index_save.close();

    }
    if(decision.compare("y")==0){//I made this one owing to avoiding mistake input
        Read_Info_Contour_Yolo();
        Save_Info_of_Selected_Imgs();
    }
    
    
    // cout<<"parent_path_contour:"<<parent_path_contour<<endl;
    // cout<<"parent_path_ori_img:"<<parent_path_ori_img<<endl;
    // cout<<"final_path_contour:"<<final_path_contour<<endl;
    // cout<<"final_path_yolo:"<<final_path_yolo<<endl;
    cout<<"done"<<endl;
}

string ROI_SUCCESS::Decision_Part(){
    imshow("merged_img", merged_img);
    waitKey(0);
    cout << "Choose what you want: ";
    cin >> selected_idx; // get user input from the keyboard
    if(selected_idx.compare("skip")==0){
        return "skip";
    }
    cout << "You choose: " << selected_idx <<endl;
    bool is_input_error=Digit_Check(selected_idx);
    if(is_input_error==false){
        cout << "The input error: " << selected_idx <<endl;
        return "ND";
    }

    cout<<"Are you sure for saving those datas?"<<endl;
    cout<<"y to continue, any button to finish this program: ";
    string decision;
    cin >> decision;
    return decision;
}

bool ROI_SUCCESS::Determine_final_path_contour(){
    ////determine path
    parent_path_contour=new char[30];
    parent_path_ori_img=new char[30];
    strcpy(parent_path_contour, "ROI_images2");    
    strcat(parent_path_contour,"/Contour/");

    strcpy(parent_path_ori_img, "ROI_images2");
    strcat(parent_path_ori_img,"/Image/");

    //////

    idx=Determine_Folder_Numb();
    string index_str=to_string(idx);
    
    int size_contour=strlen(parent_path_contour);
    int size_ori_img=strlen(parent_path_ori_img);
    
    final_path_contour=new char[size_contour+index_str.length()+2];
    final_path_yolo=new char[size_ori_img+index_str.length()+2];
    
    //process string to char
    char temp_char[index_str.length()]; 
    
    for (int j=0; j<sizeof(temp_char);j++) { 
        temp_char[j] = index_str[j]; 
    } 

    strcat(parent_path_contour, temp_char);
    strcat(parent_path_ori_img, temp_char);

    strcpy(final_path_contour, parent_path_contour);
    strcpy(final_path_yolo, parent_path_ori_img);

    bool is_path_exist=Path_Check();

    strcat(final_path_contour,"/*");
    strcat(final_path_yolo,"/*"); 

    return is_path_exist;
    
    
}
bool ROI_SUCCESS::Path_Check(){
    bool is_path_exist=true;
    struct stat info;
    if( stat( final_path_contour, &info ) != 0 ){
        printf( "cannot access %s\n", final_path_contour );
        is_path_exist=false;
    }else if( info.st_mode & S_IFDIR ){  // S_ISDIR() doesn't exist on my windows 
        printf( "%s is a directory\n", final_path_contour );
    }else{
        printf( "%s is no directory\n", final_path_contour );
        // is_path_exist=false;
    }

    if( stat( final_path_yolo, &info ) != 0 ){
        printf( "cannot access %s\n", final_path_yolo );
        is_path_exist=false;
    }else if( info.st_mode & S_IFDIR ){  // S_ISDIR() doesn't exist on my windows 
        printf( "%s is a directory\n", final_path_yolo );
    }else{
        printf( "%s is no directory\n", final_path_yolo );
        // is_path_exist=false;
    }
    return is_path_exist;
}

int ROI_SUCCESS::Determine_Folder_Numb(){
    int temp_i;
    ifstream load_index;
    load_index.open("ROI_success/index.txt");
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

void ROI_SUCCESS::Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name ){
    
    string contour_line=resource_file;
    int delimiter = contour_line.find(';');
    string class_name = contour_line.substr(0,delimiter);//extract out class

    *yolo_data=class_name;
    *contour_data=class_name;

    *yolo_data=*yolo_data+' ';
    *contour_data=*contour_data+',';

    contour_line=contour_line.substr(delimiter+1);

    delimiter = contour_line.find(';');

    *img_name=contour_line.substr(0,delimiter);
    cout<<"*img_name: "<<*img_name<<endl;
    // cout<<"contour_line: "<<contour_line<<endl;
    // getchar();

    contour_line=contour_line.substr(delimiter+1);

    delimiter = contour_line.find(';');

    *contour_data=*contour_data+*img_name;
    *contour_data=*contour_data+',';
    // cout<<"img_name:"<<*img_name<<endl;
    // getchar();
    // *yolo_data=*yolo_data+"hello";

    

    string yo_info=contour_line.substr(0,delimiter);//extract out yolo information
    contour_line=contour_line.substr(delimiter+1);
    string contour_info=contour_line;

    *yolo_data=*yolo_data+yo_info;
    *contour_data=*contour_data+contour_info;
    // cout<<"*yolo_data: "<<*yolo_data<<endl;
}

void ROI_SUCCESS::Read_Info_Contour_Yolo(){
    string contour_file="/contour_"+to_string(idx)+".txt";
    char contour_name_char[contour_file.length()+2];
    for(int j=0;j<sizeof(contour_name_char);j++){
        contour_name_char[j]=contour_file[j];
    }

    strcat(parent_path_contour,contour_name_char);


    // int parent_path_contour_size = sizeof(parent_path_contour) / sizeof(char); 
    // string myfile_path="";
    // for (int j=0; j<parent_path_contour_size; j++) { 
    //     myfile_path = myfile_path + parent_path_contour[j]; 
    // } 
    
    cout<<"sub_path_contour: "<<parent_path_contour<<endl;
    ifstream myfile;
    // string p="ROI_images/Contour/207/contour_207.txt";
    // myfile.open(parent_path_contour);
    myfile.open(parent_path_contour);
    // ifstream myfile( contour_name_char );
    // ifstream myfile( "ROI_images/Contour/200/contour_200.txt" );
    cout<<"parent_path_contour: "<<parent_path_contour<<endl;
    file_read=new string[glob_contour.gl_pathc];
    
    
    int ind=0;
    if(myfile){
        while(getline(myfile, file_read[ind])){
            // cout<<"file_read[ind]: "<<file_read[ind]<<endl;

            ind++;
        }
    }
    myfile.close();  
}
bool ROI_SUCCESS::Digit_Check(string input_digit){
    int delimiter=0;
    while(delimiter!=-1){
        delimiter = input_digit.find(',');
        string temp_digit = input_digit.substr(0, delimiter);

        input_digit = input_digit.substr(delimiter+1);

        char digit_str[temp_digit.length()];
        for(int i=0;i<temp_digit.length();i++){
            char digit_str[1];
            digit_str[0]=temp_digit[i];
            if(isdigit(digit_str[0])==false){
                return false;
            }
        }
        
    }
    return true;
    
}

void ROI_SUCCESS::Save_Info_of_Selected_Imgs(){
    string path_out_yolo ="ROI_success/yolo.txt";
    string path_out_contour="ROI_success/contour.txt";
    string path_out_mix="ROI_success/yolo_contour.txt";
    ofstream file_out_yolo;
    ofstream file_out_contour;
    ofstream file_out_mix;
    file_out_yolo.open(path_out_yolo,std::ios_base::app);//This is using for not overwrite
    file_out_contour.open(path_out_contour,std::ios_base::app);//This is using for not overwrite
    file_out_mix.open(path_out_mix,std::ios_base::app);//This is using for not overwrite

    int delimiter=0;
    while(delimiter!=-1){
        string yolo_data, contour_data, name_ori_img;

        delimiter = selected_idx.find(',');
        string temp_str = selected_idx.substr(0, delimiter);
        selected_idx = selected_idx.substr(delimiter+1);
        
        
        int temp_i=stoi(temp_str);       
        cout<<"file_read[temp_i]: "<<file_read[temp_i]<<endl;         
        
        Splite_Data_Yolo_and_Contour_v3(file_read[temp_i],&yolo_data, &contour_data, &name_ori_img);
        

        string temp_ori_name=glob_yolo.gl_pathv[temp_i];
        bool does_name_match=Name_Check(temp_ori_name,name_ori_img);
        if(does_name_match==true){
            cout<<"varified"<<endl;
        }else{
            cout<<"something wrong from name"<<endl;
            cout<<"temp_ori_name: "<<temp_ori_name<<endl;
            cout<<"name_ori_img: "<<name_ori_img<<endl;
            // return false;
        }

        string final_contour_path="ROI_success/contour/";
        final_contour_path=final_contour_path+name_ori_img;

        string final_yolo_path="ROI_success/yolo/";
        final_yolo_path=final_yolo_path+name_ori_img;
        


        imwrite(final_contour_path,image_contour[temp_i]);
        imwrite(final_yolo_path,image_original[temp_i]);
        
        // file_out<<file_read[temp_i]+"\n";
        // cout<<"yolo: "<<yolo_data<<endl;
        // cout<<"contour_data: "<<contour_data<<endl;
        file_out_yolo<<yolo_data;
        file_out_yolo<<endl;
        file_out_contour<<contour_data;
        file_out_contour<<endl;
        file_out_mix<<file_read[temp_i];
        file_out_mix<<endl;
        Save_Yolo(name_ori_img, yolo_data);

    }
    file_out_contour.close();
    file_out_yolo.close();
    file_out_mix.close();
    ofstream index_save;
    index_save.open("ROI_success/index.txt");
    index_save<<to_string(++idx);
    index_save.close();
}
void ROI_SUCCESS::Save_Yolo(string img_name, string yolo_data){
    int delimiter=0;
    delimiter = img_name.find('.');
    img_name = img_name.substr(0, delimiter);
    img_name = img_name+".txt";
    ofstream yolo_save("ROI_success/yolo/"+img_name);
    yolo_save<<yolo_data;
    yolo_save<<endl;
    yolo_save.close();
}

bool ROI_SUCCESS::Name_Check(string temp_ori_name, string name_ori_img){
    int delimiter=0;
    while(delimiter!=-1){
        delimiter = temp_ori_name.find('/');
        temp_ori_name =temp_ori_name.substr(delimiter+1);
    }
    cout<<"temp_ori_name: "<<temp_ori_name<<endl;
    if(temp_ori_name.compare(name_ori_img)==0){
        // cout<<"ori image is same"<<endl;
        return true;
    }else{
        // cout<<"ori image is different"<<endl;
        return false;
    }
}

bool ROI_SUCCESS::Load_Img_and_Store_to_Mat(){

    glob(final_path_contour,GLOB_TILDE,NULL,&glob_contour);
    glob(final_path_yolo,GLOB_TILDE,NULL,&glob_yolo);

    the_number_of_files=glob_contour.gl_pathc-1;
    // if(the_number_of_files!=the_number_of_file_in_folder){//the_number_of_file_in_folder is in kuwahara.h
    //     return false;
    // }
    image_contour=new Mat[the_number_of_files];
    image_original=new Mat[the_number_of_files];
    
    Size size2(100,100);
    for(unsigned int j=0; j<the_number_of_files; j++){
        cout<<"glob_contour.gl_pathv[j]: "<<glob_contour.gl_pathv[j]<<endl;
        cout<<"glob_yolo.gl_pathv[j]: "<<glob_yolo.gl_pathv[j]<<endl;

        image_contour[j]=imread(glob_contour.gl_pathv[j],1);
        image_original[j]=imread(glob_yolo.gl_pathv[j],1);
        resize(image_contour[j],image_contour[j],size2);//resize image
        cv::putText(image_contour[j], //target image
        to_string(j), //text
        cv::Point(15, 15), //top-left position
        cv::FONT_HERSHEY_DUPLEX,
        0.4,
        CV_RGB(255, 0, 0), //font color
        1);
    }
    return true;
}

bool ROI_SUCCESS::Load_Img_and_Store_to_Mat_V2(){

    glob(final_path_contour,GLOB_TILDE,NULL,&glob_contour);
    glob(final_path_yolo,GLOB_TILDE,NULL,&glob_yolo);

    the_number_of_files=glob_contour.gl_pathc-1;
    // if(the_number_of_files!=the_number_of_file_in_folder){//the_number_of_file_in_folder is in kuwahara.h
    //     return false;
    // }
    image_contour=new Mat[the_number_of_files];
    image_original=new Mat[the_number_of_files];
    
    Size size2(100,100);
    int i=1;
    for(unsigned int j=0; j<the_number_of_files; j++){
        // cout<<"glob_contour.gl_pathv[j]: "<<glob_contour.gl_pathv[j]<<endl;
        // cout<<"glob_yolo.gl_pathv[j]: "<<glob_yolo.gl_pathv[j]<<endl;

        image_contour[j]=imread(glob_contour.gl_pathv[i],1);
        image_original[j]=imread(glob_yolo.gl_pathv[j],1);
        resize(image_contour[j],image_contour[j],size2);//resize image
        cv::putText(image_contour[j], //target image
        to_string(j), //text
        cv::Point(15, 15), //top-left position
        cv::FONT_HERSHEY_DUPLEX,
        0.4,
        CV_RGB(255, 0, 0), //font color
        1);
        i++;
    }
    return true;
}

void ROI_SUCCESS::Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[],int the_number_of_files){
    Size size2(100,100);

    Mat blank_mat=Mat::zeros(size2,CV_8UC3);

    int mat_size;
    if( (the_number_of_files%10)==0 ){//the case of 10,20,30...100
        mat_size=(the_number_of_files/10);
    }else{//the case of 11,12,13,14,21,22,23,etc..
        mat_size=(the_number_of_files/10)+1;
    }
    Mat **out=new Mat*[mat_size];
    for(int i = 0; i < mat_size; ++i){
        out[i] = new Mat[10];
    }
    int k=0;
    Mat *temp_out=new Mat[mat_size];
    for(int i=0;i<mat_size;i++){
        for(int j=0; j<10;j++){
            if( the_number_of_files%10!=0 && i==mat_size-1 ){// the case of 
                if(k<the_number_of_files){
                    out[i][j]=resource_mat[k];
                    k++;
                }else{
                    out[i][j]=blank_mat;    
                }
            }else{
                out[i][j]=resource_mat[k];
                k++;    
            }
        }
        // cout<<"i: "<<i<<endl;
        hconcat( out[i], 10, temp_out[i]);
    }
    Mat final_out;
    vconcat( temp_out, mat_size, final_out );
    for(int i = 0; i < mat_size; ++i){
        delete [] out[i];
    }
    delete [] out;
    delete [] temp_out;

    merged_img=final_out;
}