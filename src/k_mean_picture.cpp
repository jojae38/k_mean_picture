#include "k_mean_picture.hpp"

k_mean::k_mean()
{  
    Setting_={3,1,1,1,0,10,0};
    show_menu();
    init_picture();
    while(true)
    {
        if(Setting_.Maximum_Run<=Setting_.Run_num)
            break;
        do_clustering();
    }
    clustering_to_picture();
}
k_mean::~k_mean()
{
    cout << "K_MEAN END"<<endl;
}
void k_mean::show_menu()
{   
    cout << "+======================================+"<<endl;
    do
    {
        cout << "/How many clustering point you want : ";
        cin >> Setting_.cluster_point;
        if(Setting_.cluster_point>=2)
            break;
        else
            cout <<"Wrong Value try again"<<endl;
    } while (true);
    do
    {
        cout << "/Do you want to see the sequence? (Yes =  1, No = 0) : ";
        cin >> Setting_.sequence;
        if(Setting_.sequence==0||Setting_.sequence==1)
            break;
        else   
            cout <<"Wrong Value try again"<<endl;
    } while (true);
    do
    {
        cout << "/Do you want to see clustering point pos? (Yes = 1, No = 0) : ";
        cin >> Setting_.pos_show;
        if(Setting_.pos_show==0||Setting_.pos_show==1)
            break;
        else   
            cout <<"Wrong Value try again"<<endl;
    } while (true);
    do
    {
        cout << "/Delay Sec per Sequence (recommend : 1~5 sec) : ";
        cin >> Setting_.Delay_sec;
        if(Setting_.Delay_sec>=0)
            break;
        else   
            cout <<"Wrong Value try again"<<endl;
    } while (true);

    do
    {
        cout << "/Maximum Run Number : ";
        cin >> Setting_.Maximum_Run;
        if(Setting_.Maximum_Run>=0)
            break;
        else   
            cout <<"Wrong Value try again"<<endl;
    } while (true);

    cout << "+======================================+"<<endl;
}

void k_mean::init_picture()
{
    cv::Mat img;
    img=cv::imread(file_dst);
    row=img.rows;
    col=img.cols;
    Setting_.whole_pixel=img.rows*img.cols;

    vector<Pixel> temp_vec(Setting_.whole_pixel);
    pic_vec=temp_vec;
    picture_to_vector(img);
    random_cluster_point();
    // vector_to_picture(picture);
    // show_picture(img);
}
void k_mean::show_picture(cv::Mat& img)
{
    string window_name="["+to_string(Setting_.Run_num)+"] k_mean clustered image"; 
    cv::namedWindow(window_name);
    cv::imshow(window_name,img);
    cv::waitKey(0)==27;
}

void k_mean::picture_to_vector(cv::Mat &img)
{
    uchar* pic_data=img.data;
    for(int row=0;row<img.rows;row++)
    {
        for(int col=0;col<img.cols;col++)
        {
            pic_vec[row*img.cols+col].B=pic_data[row*img.cols*3+col*3];
            pic_vec[row*img.cols+col].G=pic_data[row*img.cols*3+col*3+1];
            pic_vec[row*img.cols+col].R=pic_data[row*img.cols*3+col*3+2];
            pic_vec[row*img.cols+col].Cluster_num=0;
        }
    }
}

void k_mean::vector_to_picture(cv::Mat &img)
{
    uchar* pic_data=img.data;
    for(int row=0;row<img.rows;row++)
    {
        for(int col=0;col<img.cols;col++)
        {
            pic_data[row*img.cols*3+col*3]=pic_vec[row*img.cols+col].B;
            pic_data[row*img.cols*3+col*3+1]=pic_vec[row*img.cols+col].G;
            pic_data[row*img.cols*3+col*3+2]=pic_vec[row*img.cols+col].R;
        }
    }
}

void k_mean::clustering_to_picture()
{   
    cv::Mat img(cv::Size(row,col),CV_8UC3,{0,0,0});
    uchar* pic_data=img.data;
    for(int row=0;row<img.rows;row++)
    {
        for(int col=0;col<img.cols;col++)
        {
            pic_data[row*img.cols*3+col*3]=int(cluster[pic_vec[row*img.cols+col].Cluster_num].B);
            pic_data[row*img.cols*3+col*3+1]=int(cluster[pic_vec[row*img.cols+col].Cluster_num].G);
            pic_data[row*img.cols*3+col*3+2]=int(cluster[pic_vec[row*img.cols+col].Cluster_num].R);
        }
    }
    show_picture(img);
}
void k_mean::random_cluster_point()
{
    srand(time(NULL));
    for(int i=0;i<Setting_.cluster_point;i++)
    {
        Cluster_Point temp;
        temp.Cluster_num=i;
        temp.init_dot=rand()%(Setting_.whole_pixel-1);
        temp.B=pic_vec[temp.init_dot].B;
        temp.G=pic_vec[temp.init_dot].G;
        temp.R=pic_vec[temp.init_dot].R;
        cluster.push_back(temp);

        // cout << temp.init_dot<<endl;
        // cout << temp.B<<endl;
        // cout << temp.G<<endl;
        // cout << temp.R<<endl;      
    }
}

double k_mean::distance(double x1,double y1,double z1,double x2,double y2,double z2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
}
void k_mean::do_clustering()
{
    vector<int> cluster_count(Setting_.cluster_point);
    //Maximization
    for(int i=0;i<Setting_.whole_pixel;i++)
    {
        vector<double> dis(Setting_.cluster_point);
        for(int j=0;j<Setting_.cluster_point;j++)
        {
            dis[j]=distance(pic_vec[i].B,pic_vec[i].G,pic_vec[i].R,cluster[j].B,cluster[j].G,cluster[j].R);
        }
        int number=min_element(dis.begin(),dis.end())-dis.begin();
        pic_vec[i].Cluster_num=number;
        cluster_count[number]++;   
    }
    
    vector<double> cluster_B(Setting_.cluster_point);
    vector<double> cluster_G(Setting_.cluster_point);
    vector<double> cluster_R(Setting_.cluster_point);

    for(int i=0;i<Setting_.whole_pixel;i++)
    {
        cluster_B[pic_vec[i].Cluster_num]+=pic_vec[i].B/double(cluster_count[pic_vec[i].Cluster_num]);
        cluster_G[pic_vec[i].Cluster_num]+=pic_vec[i].G/double(cluster_count[pic_vec[i].Cluster_num]);
        cluster_R[pic_vec[i].Cluster_num]+=pic_vec[i].R/double(cluster_count[pic_vec[i].Cluster_num]);
    }
    
    for(int j=0;j<Setting_.cluster_point;j++)
    {
        cluster[j].B=cluster_B[j];
        cluster[j].G=cluster_G[j];
        cluster[j].R=cluster_R[j];
    }

    Setting_.Run_num++;
}