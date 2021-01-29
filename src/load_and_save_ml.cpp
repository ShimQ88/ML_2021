// Project Headers
#include "load_and_save_ml.h"
#define GET_VARIABLE_NAME(Variable) (#Variable)
int
Count_Column_Numb(const string& filename){//only two case sample work need to develop more
    int numb_of_data_cols;
    ifstream myfile(filename);
    ofstream shuffled_file("data/shuffle_output.txt");
    string line1[3000];
    string line2[3000];
    string temp_line;
    int i=0;
    int j=0;
    if (myfile.is_open())
    {
        // getline (myfile,line);  
        while ( getline (myfile,temp_line) )
        {
            if(i==0){
                numb_of_data_cols=count(temp_line.begin(), temp_line.end(), ',');    
            }
            
            if(temp_line[0]=='0')
                // if(temp_line[0]=='1')
            {
                // cout<<"hahaha"<<endl;
                line1[i]=temp_line;
                i++;
            }
            else if(temp_line[0]=='1')
                // else if(temp_line[0]=='9')
            {
                line2[j]=temp_line;
                j++;
            }
            else
            {
                cout<<"error"<<endl;
                getchar();
            }
            
        }
        
        // cout<<"numb_of_data_cols: "<<numb_of_data_cols<<endl;
        
        // cout<<"The first string: "<<temp_line[0];
        // getchar();
        myfile.close();
    }
    int i1=0;
    int j1=0;
    if (shuffled_file.is_open())
    {
        while(1){
            if( (i1==i)&&(j1==j) ){
                break;
            }
            if(i1!=i){
                shuffled_file <<line1[i1]+'\n';
                i1++;
            }
            if(j1!=j){
                shuffled_file << line2[j1]+'\n';
                j1++;
            }
            
        }
        shuffled_file.close();
    }
    
    return numb_of_data_cols;
}
bool
read_num_class_data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses )
{
    const int M = 1024;
    char buf[M+2];

    // Mat el_ptr(1, var_count, CV_32F);
    Mat el_ptr(1, var_count, CV_32F);
    int i;
    vector<int> responses;

    _data->release();
    _responses->release();

    FILE* f = fopen( filename.c_str(), "rt" );
    if( !f )
    {
        cout << "Could not read the database " << filename << endl;
        return false;
    }

    for(;;)
    {
        char* ptr;
        if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
            break;
        responses.push_back(buf[0]);
        //char test;
        //test=buf[0]+65;
        //responses.push_back(test);
        cout << "responses " << buf[0] << " " <<endl;//<<  endl;
        ptr = buf+2;
        for( i = 0; i < var_count; i++ )//in case of name is included
            // for( i = 0; i < var_count; i++ )//in case of name is excluded
        {
            int n = 0;
            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
            ptr += n + 1;
            cout<<"ptr: "<<ptr<<endl;
            getchar();
        }
        cout << el_ptr << endl;
        // getchar();
        if( i < var_count )
            break;
        _data->push_back(el_ptr);
    }
    fclose(f);
    Mat(responses).copyTo(*_responses);

    cout << "The database " << filename << " is loaded.\n";

    return true;
}

bool
read_num_class_data2( const string& filename, int var_count,
                     Mat* _data, Mat* _responses, vector<string> *_imgname )
{
    
    ifstream final_output;
    final_output.open(filename);//output file
    
    string line;
    
    string replace_str=" ";



    Mat el_ptr(1, var_count-1, CV_32F);
    // string
    // _data->release();
    // Mat _data2(1000, var_count, CV_32F);
    // _data=*_data2;
    
    vector<int> responses;
    vector<string> imgname;

    _data->release();
    _responses->release();
    // _imgname->release();

    if(final_output.is_open()){
        string class_numb;
        string img_name;
        
        int j=0;
        
        while ( getline (final_output,line) ){
            Mat contour_info(1, var_count, CV_32F);
            int delimiter=0;
            int i=0;
            int temp_i=0;
            while(true){
                // cout<<"line: "<<line<<endl;
                delimiter = line.find(',');
                if(i==0){
                    class_numb = line.substr(0,delimiter);
                    responses.push_back( stoi(class_numb) );
                }else if(i==1){
                    img_name = line.substr(0,delimiter);
                    _imgname->push_back(img_name);
                }else{
                    string temp_contour;
                    temp_contour = line.substr(0,delimiter);
                    // _data[j][temp_i]= stof(temp_contour);
                    // _data->push_back(stof(temp_contour));
                    // Mpixel(_data,i,temp_i)=stof(temp_contour);
                    el_ptr.at<float>(temp_i)=stof(temp_contour);
                    temp_i++;
                }
                line=line.substr(delimiter+1);
                // cout<<"line: "<<line;
                if(delimiter==-1){break;}
                
                // line.replace(delimiter,1,replace_str);
                // cout<<"el_ptr: "<<el_ptr<<endl;
                // getchar();
                i++;
            }
            _data->push_back(el_ptr);
            // cout<<"class_numb: "<<class_numb<<endl;
            // cout<<"img_name: "<<img_name<<endl;
            // cout<<"_data: "<<_data[0]<<endl;

            // cout<<"contour_info[3]: "<<contour_info[3]<<endl;
            // cout<<"contour_info[4]: "<<contour_info[4]<<endl;
            // getchar();
            j++;
        }
    }

    Mat(responses).copyTo(*_responses);
    // Mat(imgname).copyTo(_imgname);

    return true;
}



void help(int ntrain_samples, int ntest_samples,int ml_technique){
    cout<<endl<<endl<<endl<<endl<<endl;
    cout<<"#########################################################"<<endl;
    if(ml_technique==0){
        cout<<"######## machine learning technique: Neural Network. ####"<<endl;

    }else if(ml_technique==1){
        cout<<"######## machine learning technique: Ada Boost. #########"<<endl;
    }else if(ml_technique==2){
        cout<<"######## machine learning technique: Random Forest. #####"<<endl;
    }

    cout<<"######## mode: train by training sample.        ########"<<endl;
    cout<<"########       test by testing sample.           ########"<<endl;
    printf("######## Training the index number from 0 to %d ########\n",ntrain_samples);
    printf("######## Testing the index number from %d to %d ########\n",ntrain_samples+1, ntrain_samples+ntest_samples);
    cout<<"#########################################################"<<endl;
    cout<<"Press any key"<<endl;
}


//main_ml(){}
void Test(string input_file_name, string output_file_name){
    ifstream input_file(input_file_name);
    ofstream output_file(output_file_name);
    string read_line;

    int delimiter=0;
    getline(input_file,read_line);
    if((input_file.is_open())&&(output_file.is_open())){
        while(getline(input_file, read_line)){
            // strinh output_string;
            int delimiter=0;
            string temp_read_line=read_line;//copy string

            while(delimiter!=-1){
                delimiter = temp_read_line.find(',');
                string splited_value=temp_read_line.substr(0,delimiter);
                // cout<<"splited_value: "<<splited_value<<endl;
                // cout<<"splited_value.length(): "<<splited_value.length()<<endl;
                char str[splited_value.length()];
                bool is_digit=true;
                for (int i=0; i<splited_value.length(); i++){
                    str[i]=splited_value[i];
                    // cout<<"str[i]:"<<str[i]<<endl;
                    // getchar();
                    if((isdigit(str[i])==true)||(str[i]=='.')){
                        
                    }else{
                        cout<<"this is not digit"<<endl;
                        is_digit=false;
                        break;
                    }
                }
                if(is_digit==true){
                    if(delimiter==-1){
                        output_file<<splited_value;
                    }else{
                        output_file<<splited_value;
                        output_file<<',';    
                    }
                    
                }else{
                    // output_file<<splited_value;
                    // output_file<<',';
                    //doing nothing
                }
                temp_read_line = temp_read_line.substr(delimiter+1);//delete copied data
            }
            output_file<<"\n";
        }
    }

    input_file.close();
    output_file.close();
}

void Test2(string input_file_name, string output_file_name){
    ifstream input_file(input_file_name);
    ofstream output_file(output_file_name);
    string read_line;

    int delimiter=0;
    getline(input_file,read_line);
    if((input_file.is_open())&&(output_file.is_open())){
        while(getline(input_file, read_line)){
            // strinh output_string;
            int delimiter=0;
            string temp_read_line=read_line;//copy string
            int k=0;
            while(delimiter!=-1){
                delimiter = temp_read_line.find(',');
                string splited_value=temp_read_line.substr(0,delimiter);
                // cout<<"splited_value: "<<splited_value<<endl;
                // cout<<"splited_value.length(): "<<splited_value.length()<<endl;
                char str[splited_value.length()];
                bool is_digit=true;
                for (int i=0; i<splited_value.length(); i++){
                    str[i]=splited_value[i];
                    // cout<<"str[i]:"<<str[i]<<endl;
                    // getchar();
                    if((isdigit(str[i])==true)||(str[i]=='.')){
                        
                    }else{
                        cout<<"this is not digit"<<endl;
                        is_digit=false;
                        break;
                    }
                }
                // cout<<"k: "<<k<<endl;
                // getchar();
                if(is_digit==true){
                    if(delimiter==-1){
                        output_file<<splited_value;
                    }else{
                        if(k==0){
                            output_file<<"9,";
                            // cout<<"ha"<<endl;
                            // getchar();
                            // output_file<<',';    
                        }else{
                            output_file<<splited_value;
                            output_file<<',';    
                        }
                        
                    }
                    
                }else{
                    output_file<<splited_value;
                    output_file<<',';
                    // doing nothing
                }
                k++;
                temp_read_line = temp_read_line.substr(delimiter+1);//delete copied data
            }
            output_file<<"\n";
        }
    }

    input_file.close();
    output_file.close();
}

bool Read_Test_Contour_file(Mat *_data, int column_count, string file_name){
    ifstream read_CEs(file_name);
    Mat el_ptr(1, column_count, CV_32F);
    if(read_CEs.is_open()){
        _data->release();
        string line;
        vector<float>temp_CEs;
        while ( getline (read_CEs,line) ){
            int delimiter=0;
            int temp_i=0;
            while(true){
                delimiter = line.find(',');
                string temp_val=line.substr(0,delimiter);
                line = line.substr(delimiter+1);
                // cout<<"temp_val: "<<temp_val<<endl;
                // getchar();
                el_ptr.at<float>(temp_i)=stof(temp_val);
                temp_i++;
                if(delimiter==-1){break;}
            }
            _data->push_back(el_ptr);
            // cout<<"one line done"<<endl;
        }
        return true;
    }else{
        return false;
    }

}
struct Test_Matrices2{
    Mat target_color_img;
    Mat target_gray_img;
    Mat next_gray_img;
    Mat prev_gray_img;
    Mat next_ku_img;
    Mat prev_ku_img;
    Mat target_ku_img;
    Mat subtracted_gray_img;
    Mat thresholded_img;
    Mat subtracted_gray_img2;
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
void Return_Prev_and_Next_Full_File_Path2(string input_name, string& prev_name, string& next_name){    

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
bool Read_Original_Image2(string& full_path, Mat& output_ori_img){
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

Test_Matrices2 part2(string root_target, string input_name,string root_object_dataset1, string root_object_dataset2){
    
    Test_Matrices2 final_output;

    //load the previous and next file names
    string prev_name, next_name;
    Return_Prev_and_Next_Full_File_Path2(input_name, prev_name, next_name);

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

    bool is_next_success=Read_Original_Image2(full_next_path, temp_next_img);
    bool is_prev_success=Read_Original_Image2(full_prev_path, temp_prev_img);

    if(is_prev_success==false && is_next_success==false){//the case could not find the files from dataset1
        // cout<<"the second try"<<endl;
        full_next_path=root_object_dataset2+"/"+next_name;
        full_prev_path=root_object_dataset2+"/"+prev_name;
        is_next_success=Read_Original_Image2(full_next_path, temp_next_img);
        is_prev_success=Read_Original_Image2(full_prev_path, temp_prev_img);
        
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

Mat Original_Image_to_Kuwahara_Image2(Mat input_gray){
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

void Create_CEs(Mat *_data, string file_root,int nsamples_all,vector<string> imgname){
    string data_root1="/home/kyu/Desktop/rodents_data";
    string data_root2="/home/kyu/Desktop/birds";

    ofstream CE_save("the_first_cycle_contours/CEs_for_test.txt");
    vector<vector<float>>CE;
    string error_txt="-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1";
    double fps=0.0;
    string contour_txt;
    for(int i=0; i<nsamples_all; i++){
        system_clock::time_point start = system_clock::now();
        Test_Matrices2 mat_collection=part2(file_root, imgname[i],data_root1, data_root2);        
        string temp_file_root;
        if(mat_collection.type==0){
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            // cout<<"error to load the subtraction image"<<endl;
            // getchar();
            CE_save<<imgname[i];
            CE_save<<endl;
            continue;
        }

        // cout<<"p1"<<endl;
        // mat_collection.target_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.target_gray_img);
        // mat_collection.target_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.target_gray_img);
        if(mat_collection.type==1){
            // cout<<"c1"<<endl;
            // mat_collection.prev_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.prev_gray_img);
            // mat_collection.subtracted_gray_img=mat_collection.prev_ku_img-mat_collection.target_ku_img;
            mat_collection.subtracted_gray_img = mat_collection.prev_gray_img - mat_collection.target_gray_img;
        }else if(mat_collection.type==2){
            // cout<<"c2"<<endl;
            // cout<<"i:"<<i<<endl;
            // if(i==689){
            //     imshow("mat_collection.next_gray_img",mat_collection.next_gray_img);
            //     imshow("mat_collection.target_gray_img",mat_collection.target_gray_img);
            //     waitKey(0);
            // }
            // mat_collection.next_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.next_gray_img);    
            // mat_collection.subtracted_gray_img=mat_collection.next_ku_img-mat_collection.target_ku_img;
            // Mat ta=mat_collection.target_gray_img;
            // Mat ne=mat_collection.next_gray_img;
            // if(ta.empty()==true){
            //     cout<<"ta empty"<<endl;
            // }
            // if(ne.empty()==true){
            //     cout<<"ne empty"<<endl;
            // }
            // cout << "Width_ta : " << ta.cols << endl;
            // cout << "Height_ta: " << ta.rows << endl;
            // cout << "------------" << endl;
            // cout << "Width_next : " << ne.cols << endl;
            // cout << "Height_next: " << ne.rows << endl;

            mat_collection.subtracted_gray_img = mat_collection.next_gray_img-mat_collection.target_gray_img;

        }else if(mat_collection.type==3){
            // cout<<"c3"<<endl;
            // mat_collection.prev_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.prev_gray_img);    
            // mat_collection.next_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.next_gray_img);

            // Mat temp_mat1=mat_collection.prev_ku_img-mat_collection.target_ku_img;
            // Mat temp_mat2=mat_collection.next_ku_img-mat_collection.target_ku_img;

            Mat temp_mat1=mat_collection.prev_gray_img-mat_collection.target_gray_img;
            Mat temp_mat2=mat_collection.next_gray_img-mat_collection.target_gray_img;
            mat_collection.subtracted_gray_img=temp_mat1+temp_mat2;


            // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
        }else{
            // cout<<"c4"<<endl;
            // cout<<"error P2"<<endl;
            // getchar();
            // CE_save<<error_txt;
            CE_save<<contour_txt;//error dealing
            CE_save<<endl;
            continue;
        }



        // cout<<"current size: "<<ku_current_img.size()<<endl;
        // cout<<"next size: "<<ku_next_img.size()<<endl;
        // cout<<"prev size: "<<ku_prev_img.size()<<endl;
        // cout<<"p2"<<endl;

        
        cv::threshold(mat_collection.subtracted_gray_img, mat_collection.thresholded_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // cv::threshold(mat_collection.subtracted_gray_img, mat_collection.subtracted_gray_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
        medianBlur(mat_collection.thresholded_img, mat_collection.thresholded_img, 7);
        // medianBlur(mat_collection.subtracted_gray_img, mat_collection.subtracted_gray_img, 7);
        // imshow("final_output",final_output);
        // waitKey(0);

        

        Point *p_start_roi_window;
        Point *p_end_roi_window;

        p_start_roi_window=new Point[200];//approx numb
        p_end_roi_window=new Point[200];//approx numb

        // Mat blob_window=Mat::zeros(final_output.size(),0);//default size initializing

        // cout<<"THis is point 2"<<endl;
        

        int count_numb=blob(mat_collection.thresholded_img, mat_collection.thresholded_img, p_start_roi_window, p_end_roi_window);
        // int count_numb=blob(mat_collection.subtracted_gray_img, mat_collection.subtracted_gray_img, p_start_roi_window, p_end_roi_window);
        
        // cout<<"THis is point 2-1"<<endl;
        if(count_numb==-100){
            cout<<"Skip: too much blob"<<endl;
            // is_initialize_success=false;
            cout<<"error"<<endl;
            // medianBlur(mat_collection.thresholded_img, mat_collection.thresholded_img, 21);
            // imshow("mat_collection.thresholded_img",mat_collection.thresholded_img);
            // waitKey(0);
            // count_numb=blob(mat_collection.thresholded_img, mat_collection.thresholded_img, p_start_roi_window, p_end_roi_window);
            // if(count_numb==-100){
            //     cout<<"error"<<endl;
            //     CE_save<<error_txt;
            //     CE_save<<endl;
            //     continue;   
            // }

            // imshow("mat_collection.thresholded_img",mat_collection.thresholded_img);
            // waitKey(0);
            // cv::threshold(mat_collection.subtracted_gray_img, mat_collection.thresholded_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
            // medianBlur(mat_collection.thresholded_img, mat_collection.thresholded_img, 7);
            // imshow("mat_collection.thresholded_img",mat_collection.thresholded_img);
            // waitKey(0);
            // getchar();
            CE_save<<error_txt;
            CE_save<<endl;
            cout<<"error from contour"<<endl;
            getchar();
            continue;

        }//segmental fault

        Point p_center_of_object=draw_rect_box(mat_collection.target_color_img, p_start_roi_window, p_end_roi_window, 200);
        int cropping_size=determine_frame_size(mat_collection.thresholded_img, p_center_of_object,20,20);
        // int cropping_size=determine_frame_size(mat_collection.subtracted_gray_img, p_center_of_object,20,20);
        if(cropping_size==-1||cropping_size==-2){
            // cout<<"sizeing error"<<endl;
            // is_initialize_success=false;
            cout<<"error from size"<<endl;
            // getchar();
            // CE_save<<error_txt;
            CE_save<<contour_txt;//temp error dealing
            CE_save<<endl;
            continue;
        }

        Mat out=Cropping_ROI(mat_collection.thresholded_img,p_center_of_object,cropping_size);
        // Mat out=Cropping_ROI(mat_collection.subtracted_gray_img,p_center_of_object,cropping_size);

        if(mat_collection.type==0){
            cout<<"error to load the subtraction image"<<endl;
            getchar();

            
            CE_save<<endl;
            continue;
        }

        // cout<<"k1"<<endl;
        // mat_collection.target_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.target_gray_img);
        // mat_collection.target_ku_img=Original_Image_to_Kuwahara_Image2(mat_collection.target_gray_img);
        Mat out1=Cropping_ROI(mat_collection.target_gray_img,p_center_of_object,cropping_size);
        out1=Original_Image_to_Kuwahara_Image2(out1);
        Mat final_out;
        if(mat_collection.type==1){
            Mat out2=Cropping_ROI(mat_collection.prev_gray_img,p_center_of_object,cropping_size);
            out2=Original_Image_to_Kuwahara_Image2(out2);
            final_out=out2-out1;
        }else if(mat_collection.type==2){
            Mat out2=Cropping_ROI(mat_collection.next_gray_img,p_center_of_object,cropping_size);
            out2=Original_Image_to_Kuwahara_Image2(out2);
            final_out=out2-out1;
        }else if(mat_collection.type==3){
            Mat out2=Cropping_ROI(mat_collection.prev_gray_img,p_center_of_object,cropping_size);
            Mat out3=Cropping_ROI(mat_collection.next_gray_img,p_center_of_object,cropping_size);
            out2=Original_Image_to_Kuwahara_Image2(out2);
            out3=Original_Image_to_Kuwahara_Image2(out3);
            Mat temp_out1=out2-out1;
            Mat temp_out2=out3-out1;
            final_out=temp_out1+temp_out2;
            // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
        }else{
            cout<<"error P2"<<endl;
            getchar();
            // CE_save<<error_txt;
            CE_save<<contour_txt;
            CE_save<<endl;
            continue;
        }

        
        // cout<<"s1"<<endl;
        std::vector<vector<Point>>contours;

        Point ROI_mid_p;
        ROI_mid_p.x=out.rows/2;
        ROI_mid_p.y=out.cols/2;

        // imshow("out",out);
        // waitKey(0);
        cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // medianBlur(final_out, final_out, 7);
        findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
        // int largestcontour=FindTheLargestContour(contours);
        int object_i = Find_The_Object_Contour(contours,ROI_mid_p);

        if(object_i==-1){
            // is_initialize_success=false;
            cout<<"contour fail"<<endl;
            getchar();
            // contour_txt="";
            // CE_save<<error_txt;
            CE_save<<contour_txt;//temp error dealing
            CE_save<<endl;
            continue;
        }

        // cout<<"s2"<<endl;
        Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
        Scalar color=CV_RGB(255,0,0);
        drawContours(mat_contour, contours, object_i, color,2,8);
        // cout<<"s3"<<endl;
        

        // cout<<"s3"<<endl;
        vector<float>temp_CE;
        // cout<<"largestcontour: "<<largestcontour<<endl;
        // imshow("mat_contour", mat_contour);
        // waitKey(0);
        EllipticFourierDescriptors(contours[object_i],temp_CE);
        contour_txt="";

        Mat el_ptr(1, n_CE, CV_32F);
        for(int i=1;i<n_CE;i++){
            contour_txt=contour_txt+to_string(temp_CE[i]);//This is for file
            el_ptr.at<float>(i-1)=temp_CE[i];//This is for contour data
            
            // strcat(contour_txt ,st );
            if(i==n_CE-1){
                contour_txt=contour_txt+"\n";
                // cout<<"enter"<<endl;

                // strcat(contour_txt, "\n");
            }else{
                contour_txt=contour_txt+",";
                // strcat(contour_txt, ",");
            }
            
        }
        CE_save<<contour_txt;
        _data->push_back(el_ptr);
        
        /*Caculate performance of program*/
        system_clock::time_point end = system_clock::now();
        double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        fps = 1000000/seconds;
        // cout << "frames " << fps << " seconds " << seconds << endl;
    }
    CE_save.close();

}

bool
load_and_save_ml( const string& data_filename, const string& img_root,
                      const string& filename_to_save,
                      const string& filename_to_load,
                      float percent_of_division,
                      int ml_technique)
{
    /*infomation 
       ml_technique= 1.neural_network 2.ada_boost 3.random_forest 
    */
    // string data_filename2="Final_dataset/contour_name_removed.data";
    Mat data;
    Mat responses;
    vector<string> imgname;
    int numb_of_data_cols=Count_Column_Numb(data_filename);//shuffle the file
    string name="data/shuffle_output.txt";
    string name2="data/shuffle_output_name_removed.txt";
    Test(name,name2);


    // Test2("Final_dataset/temp/contour_bird.txt","Final_dataset/temp/contour_bird_final.txt");
    // Test2("Final_dataset/temp/contour_rodent.txt","Final_dataset/temp/contour_rodent_final.txt");
    // exit(1);
    cout<<"numb_of_data_cols: "<<numb_of_data_cols<<endl;
    getchar();

    // bool ok = read_num_class_data( name, numb_of_data_cols, &data, &responses );//third parameter: FEATURES
    bool ok = read_num_class_data2( name, numb_of_data_cols, &data, &responses, &imgname);//third parameter: FEATURES
    


    // cout<<"data[10]: "<<data<<endl;
    // cout<<"responses[10]: "<<responses<<endl;
    // cout<<"imgname[1]: "<<imgname[1]<<endl;
    // cout<<"img_root: "<<img_root<<endl;
    // string img_full_path=img_root+"/"+imgname[20];
    // cout<<"img_full_path: "<<img_full_path<<endl;
    // Mat test=imread(img_full_path, 1);
    // imshow("test",test);
    // waitKey(0);

    // bool ok = read_num_class_data( name, 10, &data, &responses );//third parameter: FEATURES
    // bool ok = read_num_class_data( data_filename, numb_of_data_cols, &data, &responses );//third parameter: FEATURES
    system_clock::time_point start = system_clock::now();
    
    if( !ok ){
        cout<<"error from read file"<<endl;
        return ok;
    }
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // cout<<"responses: "<<responses<<endl;
    // getchar();
    // cout<<"data: "<<data<<endl;
    // getchar();
    //preparing part
    int nsamples_all = data.rows;
    double fps = 1000000/seconds;
    cout << "time seconds: " << fps << endl;

    Mat CEs_data;
    string contour_file_name="the_first_cycle_contours/CEs_for_test.txt";
    bool is_success_contour_read=Read_Test_Contour_file(&CEs_data,numb_of_data_cols-2,contour_file_name);
    if(is_success_contour_read==false){
        system_clock::time_point start = system_clock::now();
        Create_CEs(&CEs_data, img_root, nsamples_all, imgname);
        /*Caculate performance of program*/
        system_clock::time_point end = system_clock::now();
        seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        fps = 1000000/seconds;
        cout << "time seconds: " << fps << endl;
        getchar();
    }
    cout<<"nsamples_all: "<<nsamples_all<<endl;
    
    cout<<"CE done"<<endl;
    getchar();
    /*Division part*/
    int ntrain_samples = (int)round(nsamples_all*percent_of_division);//SPLIT
    int ntest_samples = (int)round(nsamples_all*(1-percent_of_division));//SPLIT

    //this process for dividing exactly with test value
    int remainder=ntrain_samples%ntest_samples;
    ntrain_samples = ntrain_samples-remainder;

    // cout<<"ntrain_samples: "<<ntrain_samples<<endl;
    // cout<<"ntest_samples: "<<ntest_samples<<endl;


    //Print Test number of samples    
    help(ntrain_samples,ntest_samples,ml_technique);
    getchar();
    // cout<<"responses: "<<responses<<endl;
    // cout<<"filename_to_save: "<<filename_to_save<<endl;

    

    
    ofstream acc;
    acc.open("resource/rf/accuracy_collection.txt");
    acc<<"i, MD, MSC, NT, MSE(index, setMaxDepth, setMinSampleCount, Number_of_Trees, Mean_Sqare_Error)\n";

    
    // int min_samp_count=5;
    // int i=0;
    // while(1){
    //     int max_dep=6;
    //     while(1){
    //         int TC=10000;
    //         while(1){
    //             Parent_ML *final_ml;
    //             if(ml_technique==0){
    //                 final_ml = Creat_ML_Class< Child_ML<ANN_MLP> >();  
    //             }else if(ml_technique==1){
    //                 final_ml = Creat_ML_Class< Child_ML<Boost> >();
    //             }else if(ml_technique==2){
    //                 final_ml = Creat_ML_Class< Child_ML<RTrees> >();
    //             }else{
    //                 cout<<"ml_technique code error"<<endl;
    //                 return false;
    //             }

    //             final_ml->Main_Process(prepared_data,max_dep,min_samp_count,TC);//doing main process
                
    //             // string numb_ce=to_string(prepared_data->the_number_of_data+1);//check number of CEs
    //             string numb_ce=to_string(i);//check number of CEs
                
    //             Write_File file_write(numb_ce);//writing file class
    //             // Write_File file_write(numb_ce);//writing file class
    //             // cout<<"seg"<<endl;
    //             // cout<<"final_ml->variance: "<<final_ml->variance<<endl;
    //             file_write.Main_Process(final_ml->mean, final_ml->variance, final_ml->sta_dev,prepared_data->k_fold_value,final_ml->confusion_matrix, final_ml->result_buffer);
                                
    //             acc<<to_string(i);
    //             acc<<", ";
    //             acc<<to_string(max_dep);
    //             acc<<", ";
    //             acc<<to_string(min_samp_count);
    //             acc<<", ";
    //             acc<<to_string(TC);
    //             acc<<", ";
    //             acc<<to_string(final_ml->mean);
    //             acc<<"\n";
    //             i++;
    //             if(TC>=10000){break;}
    //             TC=TC+1000;
    //         }
    //         max_dep=max_dep+1;
    //         if(max_dep>6){break;}
    //     }
    //     min_samp_count=min_samp_count+1;
    //     if(min_samp_count>=5){break;}
    // }
    // acc.close();
    // cout<<"eh"<<endl;
    // delete final_ml;
    
    // int i=0;
    int the_number_of_CEs=10;
    int the_number_of_class=2;
    Machine_Learning_Data_Preparation *prepared_data = new Machine_Learning_Data_Preparation(data, responses, CEs_data, imgname, img_root,ntrain_samples,ntest_samples, filename_to_save, filename_to_load, the_number_of_class, the_number_of_CEs);
    prepared_data->Main_Process(ml_technique);//data arrangement
    
    //parameters in ANN
    int max_iter=100;
    float method_param=0.01;
    //parameters in Boost
    int boost_type=0;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
    int weak_count=100;
    float weight_trim_rate=80.83;
    int max_depth=12;
    //parameters in Random Forest Trees
    //int max_depth=10;
    int min_sample_count=5;
    float regression_accuracy=0.01f;
    int max_categories=2;

    while(1){
        int tc_value=5000;
        for(int i=0;i<3;i++){
            Parent_ML *final_ml;
            if(ml_technique==0){
                final_ml = Creat_ML_Class< Child_ML <ANN_MLP> >(max_iter,0,method_param,0,0);//{max_iter,null,method_param,null,null}
            }else if(ml_technique==1){
                final_ml = Creat_ML_Class< Child_ML <Boost> >(boost_type,weak_count,weight_trim_rate,max_depth,0);
            }else if(ml_technique==2){
                final_ml = Creat_ML_Class< Child_ML <RTrees> >(max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
                // final_ml = Creat_ML_Class< Child_ML <SVM> >(max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
            }else{
                cout<<"ml_technique code error"<<endl;
                return false;
            }

            final_ml->Main_Process(prepared_data);//doing main process
            

            // string numb_ce=to_string(prepared_data->the_number_of_data+1);//check number of CEs
            // string numb_ce=to_string(i);//check number of CEs
            // cout<<"to_string(min_sample_count): "<<to_string(min_sample_count)<<endl;
            
            Write_File file_write(final_ml, prepared_data, to_string(min_sample_count));//writing file class
            
            // Write_File file_write(numb_ce);//writing file class
            // cout<<"seg"<<endl;
            // cout<<"final_ml->variance: "<<final_ml->variance<<endl;
            if(i==0){
                file_write.Write_Header(final_ml->Head_Parameter());
            }
            file_write.Main_Process();
            tc_value=tc_value+500;
        }
        if(min_sample_count==5){break;}
        min_sample_count=min_sample_count+2;

    }
    

   
                    
    // acc<<to_string(i);
    // acc<<", ";
    // acc<<to_string(max_dep);
    // acc<<", ";
    // acc<<to_string(min_samp_count);
    // acc<<", ";
    // acc<<to_string(TC);
    // acc<<", ";
    // acc<<to_string(final_ml->mean);
    // acc<<"\n";
    // i++;
    // if(TC>=10000){break;}
    // TC=TC+1000;
    // delete prepared_data;

    return true;
}