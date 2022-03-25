#include <iostream>
#include <time.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
using namespace std;

struct Cluster_Point
{
    int init_dot;
    double R;
    double G;
    double B;
    int Cluster_num;
};
struct Pixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    int Cluster_num;
};
struct Setting
{
    int cluster_point;
    int sequence;
    int pos_show;
    int Delay_sec;
    int Run_num;
    int Maximum_Run;
    int whole_pixel;
};
struct Timer
{
    double start_time;
    double duration;
    double curr_time;
    bool Timer_set;
    bool done;
};
class k_mean
{
    private:
    int row;
    int col;
    Setting Setting_;
    vector<Cluster_Point> cluster;
    vector<Pixel> pic_vec;
    string file_dst="/home/cona/image_file/lena.jpg";

    void show_menu();
    void init_picture();
    void picture_to_vector(cv::Mat &img);
    void random_cluster_point();
    void vector_to_picture(cv::Mat &img);
    void clustering_to_picture();
    void show_picture(cv::Mat& img);
    void wait_time(double sec);
    double distance(double x1,double y1,double z1,double x2,double y2,double z2);
    void do_clustering();

    public:
    k_mean();
    ~k_mean();
};
