#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/String.h>
#include <cstdlib>

class GenerateNoise
{
    private:
        ros::Subscriber ImuSub , ImgSubLeft , ImgSubRight;
        ros::Publisher ImuNoisedPub , ImgNoisedPub;

        std::string ImuTopicSub , ImgTopicSubLeft , ImgTopicSubRight , ImuTopicPub , ImgTopicPubLeft , ImgTopicPubRight;
        double ScaleLinAccel , ScaleAngVel;

    public:
        
        GenerateNoise(ros::NodeHandle *nh)
        {
            nh->getParam("/imu_topic_sub",ImuTopicSub);
            nh->getParam("/img_topic_sub_left",ImgTopicSubLeft);
            nh->getParam("/img_topic_sub_right",ImgTopicSubRight);
            nh->getParam("/imu_topic_pub",ImuTopicPub);
            nh->getParam("/img_topic_pub_left",ImgTopicPubLeft);
            nh->getParam("/img_topic_pub_right",ImgTopicPubRight);

            nh->getParam("/scale_lin_accel",ScaleLinAccel);
            nh->getParam("/scale_ang_vel",ScaleAngVel);

            ImuSub = nh->subscribe(ImuTopicSub,1000,&GenerateNoise::ImuCallback,this);
            ImgSubLeft = nh->subscribe(ImgTopicSubLeft,1000,&GenerateNoise::ImgLeftCallback,this);
            ImgSubRight = nh->subscribe(ImgTopicSubRight,1000,&GenerateNoise::ImgRightCallback,this);
            
            ImuNoisedPub = nh->advertise<sensor_msgs::Imu>(ImuTopicPub,1000);

        }

        //imu callback
        void ImuCallback(const sensor_msgs::Imu &msg)
        {
            ImuNoisedPub.publish(PseudoRandomNoise(msg));
        }

        // img callback for left cam
        void ImgLeftCallback(const sensor_msgs::Image &msg)
        {

        }

        // img callback for right img
        void ImgRightCallback(const sensor_msgs::Image &msg)
        {

        }

        // get random number , helper function for PseudoRandomNoise()
        double GetRand()
        {
            return (rand()-(RAND_MAX/2))/(RAND_MAX/2);
        }

        //pseudo random noise generator for imu data
        sensor_msgs::Imu PseudoRandomNoise(const sensor_msgs::Imu &msg)
        {
            sensor_msgs::Imu ImuNoiseMsg;

            ImuNoiseMsg.header.stamp = msg.header.stamp;
            ImuNoiseMsg.header.frame_id = "noised";
            
            ImuNoiseMsg.linear_acceleration.x += (ScaleLinAccel*GetRand())+msg.linear_acceleration.x;
            ImuNoiseMsg.linear_acceleration.y += (ScaleLinAccel*GetRand())+msg.linear_acceleration.y;
            ImuNoiseMsg.linear_acceleration.z += (ScaleLinAccel*GetRand())+msg.linear_acceleration.z;
            ImuNoiseMsg.angular_velocity.x += (ScaleAngVel*GetRand())+msg.angular_velocity.x;
            ImuNoiseMsg.angular_velocity.y += (ScaleAngVel*GetRand())+msg.angular_velocity.y;
            ImuNoiseMsg.angular_velocity.z += (ScaleAngVel*GetRand())+msg.angular_velocity.z;

            return ImuNoiseMsg;
        }

        // function to get image from msg
        
};


int main(int argc,char **argv)
{
    ros::init(argc,argv,"GenerateNoise");
    ros::NodeHandle nh;

    GenerateNoise GN = GenerateNoise(&nh);
    
    ros::spin();
}