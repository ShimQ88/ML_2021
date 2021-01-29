#include "final_filtering.h"

The_LAST_FILTERING::The_LAST_FILTERING(){
    folder_name_root="Final_dataset/";
    folder_name_contour="contour/";
    folder_name_yolo="yolo/";

    Creating_Root_Directory();
    Creating_Child_Directories();

    
    cout<<"Construct called"<<endl;
}

The_LAST_FILTERING::~The_LAST_FILTERING(){
    delete[] the_list_of_selected_idx;
    delete[] img_contour;
    delete[] img_yolo;
    delete[] file_info;
    delete[] file_info_name;
    cout<<"Deconstruction called"<<endl;
}

bool The_LAST_FILTERING::Creating_Root_Directory(){
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

bool The_LAST_FILTERING::Creating_Child_Directories(){
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

void The_LAST_FILTERING::Read_List_of_File_from_Directories(){
    string path_contour="ROI_success/contour/*";
    string path_yolo="ROI_success/yolo/*";

    char path_contour_char[path_contour.length()+1];
    
    for (int j=0; j<sizeof(path_contour_char);j++) { 
        path_contour_char[j] = path_contour[j]; 
        // cout<<"path_contour_char[j]:"<<path_contour_char[j]<<endl;  
    }

    char path_yolo_char[path_yolo.length()+1];

    for (int j=0; j<sizeof(path_yolo_char);j++) { 
        path_yolo_char[j] = path_yolo[j]; 
    } 
    cout<<"path_contour_char: "<<path_contour_char<<endl;
    cout<<"path_yolo_char: "<<path_yolo_char<<endl;

    glob(path_contour_char,GLOB_TILDE,NULL,&glob_contour);
    glob(path_yolo_char,GLOB_TILDE,NULL,&glob_yolo);
    // cout<<"glob_yolo.gl_pathv[0]:"<<glob_yolo.gl_pathv[0]<<endl;
    // cout<<"glob_contour.gl_pathc22: "<<glob_contour.gl_pathc<<endl;
}

void The_LAST_FILTERING::Main_Process(){
    idx=Determine_Process_Index_Numb();
    Read_List_of_File_from_Directories();
    int validation_code=Validate_Check(idx);//This is only work when the index number is 0
    cout<<"glob_contour.gl_pathc: "<<glob_contour.gl_pathc<<endl;
    //code  0: Success the process
    //code  1: The index number is not 0, it mean already finished scanning
    //code -1: fail to load the file of ROI_success/yolo_contour.txt
    //code -2: fail to create the file of Final_dataset/yolo_contour.txt
    cout<<"validation_code: "<<validation_code<<endl;
    int the_number_of_files_contour=glob_contour.gl_pathc;
    int the_number_of_files_yolo=glob_yolo.gl_pathc;

    // cout<<"the_number_of_file_on_grid: "<<the_number_of_file_on_grid<<endl;
    cout<<"the_number_of_files_contour: "<<the_number_of_files_contour<<endl;

    if((the_number_of_files_contour<the_number_of_file_on_grid+idx)||
        (the_number_of_files_yolo<the_number_of_file_on_grid+idx)){
        the_number_of_file_on_grid=the_number_of_files_contour%100;
        // cout<<"the_number_of_file_on_grid: "<<the_number_of_file_on_grid<<endl;
        // cout<<"the_number_of_files_contour: "<<the_number_of_files_contour<<endl;
        // getchar();
    }
    img_contour=new Mat[the_number_of_file_on_grid];
    img_yolo=new Mat[the_number_of_file_on_grid];
    file_info=new string[the_number_of_file_on_grid];
    file_info_name=new string[the_number_of_file_on_grid];

    Load_Mat_with_Index();
    Merge_Multi_Mats_to_Single_Mat_v2(img_contour,the_number_of_file_on_grid);
    string decision=Decision_Part();
    if(decision.compare("ND")==0){//I made this one owing to avoiding mistake input
        cout<<"input Error try again: Press enter"<<endl;
        the_list_of_selected_idx=new int[1];
        return;
    }else if(decision.compare("skip")==0){
        cout<<"You press skip"<<endl;
        
         Save_Info_of_Selected_Imgs();
         return;
    }

    // cout<<"decision: "<<endl;
    the_number_of_selected_idx=Count_the_Number_of_Index(selected_idx);
    

    the_list_of_selected_idx=new int[the_number_of_selected_idx];
    Save_index_to_int(selected_idx,the_list_of_selected_idx);
    Save_Info_of_Selected_Imgs();


}   
int The_LAST_FILTERING::Determine_Process_Index_Numb(){
    int temp_i;
    ifstream load_index;
    load_index.open("Final_dataset/index.txt");
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

int The_LAST_FILTERING::Validate_Check(int index){
    if(index==0){//only scanning the validation when index is zero
        ifstream file_load_yolo_contour;
        string path_in_yolo_contour="ROI_success/yolo_contour.txt";
        file_load_yolo_contour.open(path_in_yolo_contour);
        if(file_load_yolo_contour){

        }else{
            cout<<"fail to load "<< path_in_yolo_contour<< " file"<<endl;
            return -1;
        }

        string path_out_yolo_contour="Final_dataset/yolo_contour.txt";
        ofstream file_save_yolo_contour;
        file_save_yolo_contour.open(path_out_yolo_contour);//This is using for not overwrite
        
        if(file_save_yolo_contour){

        }else{
            cout<<"fail to load "<< path_out_yolo_contour<< " file"<<endl;
            return -2;
        }

        
        string read_line;
        while(getline(file_load_yolo_contour, read_line)){
            int delimiter=0;
            string temp_read_line=read_line;
            delimiter = temp_read_line.find(';');
            temp_read_line = temp_read_line.substr(delimiter+1);//ignore the class number
            
            delimiter = temp_read_line.find(';');
            string img_name_from_file=temp_read_line.substr(0,delimiter);
            // cout<<"name: "<<img_name<<endl;
            // cout<<"file_read[ind]: "<<file_read[ind]<<endl;
            int the_number_of_files_contour=glob_contour.gl_pathc;
            int the_number_of_files_yolo=glob_yolo.gl_pathc;
            int the_number_of_files;
            cout<<"the_number_of_files_contour111: "<<the_number_of_files_contour<<endl;
            cout<<"the_number_of_files_yolo: "<<the_number_of_files_yolo<<endl;
            // getchar();
            //determine the biggest number of folder.
            if(the_number_of_files_contour>=the_number_of_files_yolo){
                the_number_of_files=the_number_of_files_contour;
            }else{
                the_number_of_files=the_number_of_files_yolo;
            }
            


            bool is_contour_exist=false;
            bool is_yolo_exist=false;
            cout<<"the number of output: "<<the_number_of_files<<endl;
            cout<<"img_name_from_file: "<<img_name_from_file<<endl;
            // scanning the file name from yolo and contour folders

            for(unsigned int j=0; j<the_number_of_files; j++){                             
                if(is_contour_exist==false){
                    if(the_number_of_files_contour>j){

                        string img_name_from_dir_contour=glob_contour.gl_pathv[j];

                        //the process extract the only the file name
                        delimiter=0;
                        while(delimiter!=-1){
                            delimiter=img_name_from_dir_contour.find('/');
                            img_name_from_dir_contour=img_name_from_dir_contour.substr(delimiter+1);
                        }   
                        ////////////////////////////////

                        if(img_name_from_file.compare(img_name_from_dir_contour)==0){
                            is_contour_exist=true;
                            // cout<<"contour found"<<endl;
                            // getchar();
                        }
                    }
                }
                
                if(is_yolo_exist==false){
                    if(the_number_of_files_yolo>j){
                        string img_name_from_dir_yolo=glob_yolo.gl_pathv[j];

                        //the process extract the only the file name
                        delimiter=0;
                        while(delimiter!=-1){
                            delimiter=img_name_from_dir_yolo.find('/');
                            img_name_from_dir_yolo=img_name_from_dir_yolo.substr(delimiter+1);
                        }   
                        ////////////////////////////////


                        if(img_name_from_file.compare(img_name_from_dir_yolo)==0){
                            is_yolo_exist=true;
                            // cout<<"yolo found"<<endl;
                            // getchar();
                        }   
                    }
                }

                //final decision
                if(is_contour_exist==true&&is_yolo_exist==true){
                    file_save_yolo_contour<<read_line;
                    file_save_yolo_contour<<endl;
                    break;
                }
            }            
        }
        file_load_yolo_contour.close();
        file_save_yolo_contour.close();
        return 0;//return 0 successful

    }else{
        return 1;
        //return 1 successful is the case of not index 0
    }

}

void The_LAST_FILTERING::Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name ){
    
    string contour_line=resource_file;
    int delimiter = contour_line.find(';');
    string class_name = contour_line.substr(0,delimiter);//extract out class

    *yolo_data=class_name;
    *contour_data=class_name;

    *yolo_data=*yolo_data+',';
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
void The_LAST_FILTERING::Load_Mat_with_Index(){
    ifstream file_load_yolo_contour;
    string path_in_yolo_contour="Final_dataset/yolo_contour.txt";
    file_load_yolo_contour.open(path_in_yolo_contour);
    if(file_load_yolo_contour){

    }else{
        cout<<"fail to load "<< path_in_yolo_contour<< " file"<<endl;
        return ;
    }
    string read_line;
    
    int temp_i=0;
    while(1){
        if(temp_i==idx){break;}
        getline(file_load_yolo_contour, read_line);
        temp_i++;
    }

    int temp_j=0;
    while(1){
        if(temp_j==the_number_of_file_on_grid){break;}
        getline(file_load_yolo_contour, read_line);
        file_info[temp_j]=read_line;
        
        //the process extract the only the file name
        int delimiter=0;
        delimiter=read_line.find(';');
        read_line=read_line.substr(delimiter+1);
        delimiter=read_line.find(';');
        read_line=read_line.substr(0,delimiter);
    
        ////////////////////////////////
        file_info_name[temp_j]=read_line;
        cout<<"file_info[temp_j]: "<<file_info[temp_j]<<endl;
        cout<<"file_info_name[temp_j]: "<<file_info_name[temp_j]<<endl;
        
        string contour_name="ROI_success/contour/"+file_info_name[temp_j];
        string yolo_name="ROI_success/yolo/"+file_info_name[temp_j];
        img_contour[temp_j]=imread(contour_name,1);
        img_yolo[temp_j]=imread(yolo_name,1);

        cv::putText(img_contour[temp_j], //target image
        to_string(temp_j), //text
        cv::Point(75, 90), //top-left position
        cv::FONT_HERSHEY_DUPLEX,
        0.4,
        CV_RGB(255, 0, 0), //font color
        1);
        
        cv::putText(img_contour[temp_j], //target image
        "&&", //text
        cv::Point(15, 15), //top-left position
        cv::FONT_HERSHEY_DUPLEX,
        0.4,
        CV_RGB(255, 255, 0), //font color
        1);


        temp_j++;

    }
}
void The_LAST_FILTERING::Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[],int the_number_of_files){
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
string The_LAST_FILTERING::Decision_Part(){
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

    cout<<"Are you sure for deleting those datas?"<<endl;
    cout<<"y to continue, any button to finish this program: ";
    string decision;
    cin >> decision;
    return decision;
}

bool The_LAST_FILTERING::Digit_Check(string input_digit){
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

void The_LAST_FILTERING::Save_Info_of_Selected_Imgs(){
    string path_out_yolo ="Final_dataset/yolo.txt";
    string path_out_contour="Final_dataset/contour.txt";
    // string path_out_mix="Final_dataset/final_yolo_contour.txt";
    ofstream file_out_yolo;
    ofstream file_out_contour;
    // ofstream file_out_mix;
    file_out_yolo.open(path_out_yolo,std::ios_base::app);//This is using for not overwrite
    file_out_contour.open(path_out_contour,std::ios_base::app);//This is using for not overwrite
    for(int i=0;i<the_number_of_file_on_grid;i++){
        bool skip=false;

        for(int j=0;j<the_number_of_selected_idx;j++){
            // cout<<"the_list_of_selected_idx[j]: "<<the_list_of_selected_idx[j]<<endl;
            if(i==the_list_of_selected_idx[j]){
                skip=true;
            }
        }
        if(skip==true){
            continue;
        }else{
            string yolo_data, contour_data, name_ori_img;

            Splite_Data_Yolo_and_Contour_v3(file_info[i], &yolo_data, &contour_data, &name_ori_img);
            bool does_name_match=Name_Check(file_info_name[i],name_ori_img);
            if(does_name_match==true){
                cout<<"varified"<<endl;
            }else{
                cout<<"something wrong from name"<<endl;
                cout<<"file_info_name[i]: "<<file_info_name[i]<<endl;
                cout<<"name_ori_img: "<<name_ori_img<<endl;
                // return false;
            }
            string final_contour_path="Final_dataset/contour/";
            final_contour_path=final_contour_path+name_ori_img;

            string final_yolo_path="Final_dataset/yolo/";
            final_yolo_path=final_yolo_path+name_ori_img;
            


            imwrite(final_contour_path,img_contour[i]);
            imwrite(final_yolo_path,img_yolo[i]);
            
            // file_out<<file_read[temp_i]+"\n";
            // cout<<"yolo: "<<yolo_data<<endl;
            // cout<<"contour_data: "<<contour_data<<endl;
            file_out_yolo<<yolo_data;
            file_out_yolo<<endl;
            file_out_contour<<contour_data;
            file_out_contour<<endl;
            Save_Yolo(name_ori_img, yolo_data);
            cout<<"index:"<<i<<endl;
        }
    }
    file_out_contour.close();
    file_out_yolo.close();

    Save_Index(idx);

}
void The_LAST_FILTERING::Save_Index(int index){
    string path_out_index="Final_dataset/index.txt";
    ofstream file_out_index;
    
    file_out_index.open(path_out_index);//This is using for not overwrite
    file_out_index<<to_string(index+the_number_of_file_on_grid);
    file_out_index.close();

}
void The_LAST_FILTERING::Save_Yolo(string img_name, string yolo_data){
    int delimiter=0;
    delimiter = img_name.find('.');
    img_name = img_name.substr(0, delimiter);
    img_name = img_name+".txt";
    ofstream yolo_save("Final_dataset/yolo/"+img_name);
    yolo_save<<yolo_data;
    yolo_save<<endl;
    yolo_save.close();
}
bool The_LAST_FILTERING::Name_Check(string temp_ori_name, string name_ori_img){
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
int The_LAST_FILTERING::Count_the_Number_of_Index(string input_str){
    int delimiter=0;
    int index=0;
    delimiter = input_str.find(',');
    while(delimiter!=-1){//this is count the number of comma
        delimiter = input_str.find(',');
        input_str = input_str.substr(delimiter+1);
        cout<<"input_str: "<<input_str<<endl;
        index++;
    }
    // index++;//require to add one more index for counting the number of index
    return index;

}

void The_LAST_FILTERING::Save_index_to_int(string input_str, int input_int[]){
    int delimiter=0;
    int index=0;
    delimiter = input_str.find(',');
    if(delimiter==-1){
        input_int[index]=stoi(input_str);
        index++;
    }
    while(delimiter!=-1){//this is count the number of comma
        delimiter = input_str.find(',');
        string temp_input = input_str.substr(0, delimiter);
        input_str = input_str.substr(delimiter+1);
        cout<<"temp_input:"<<temp_input<<endl;
        getchar();
        input_int[index]=stoi(temp_input);
        index++;
    }

}