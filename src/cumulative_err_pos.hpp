#pragma once
#ifndef H_599955_SRC_SUB
#define H_599955_SRC_SUB 1

// Taken from the demos minimal_subscriber lambda

#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32.hpp"

namespace simple_ros2_test
{
    
    class VelocitySubscriber : public rclcpp::Node
    {
        public:
            inline VelocitySubscriber() : Node("simple_imu_sub")
            {
                this->simVelXSubscription = this->create_subscription< std_msgs::msg::Float32 >(
                    // The topic to subscribe to
                    "robo_vel_x",
                    // qos?
                    10,
                    // The callback
                    [this](std_msgs::msg::Float32::UniquePtr msg)
                    {
                        this->vel[0] = msg->data;
                        velMux[0].unlock();
                    }
                );

                this->simVelYSubscription = this->create_subscription< std_msgs::msg::Float32 >(
                    // The topic to subscribe to
                    "robo_vel_y",
                    // qos?
                    10,
                    // The callback
                    [this](std_msgs::msg::Float32::UniquePtr msg)
                    {
                        this->vel[1] = msg->data;
                        velMux[1].unlock();
                    }
                );

                this->simVelZSubscription = this->create_subscription< std_msgs::msg::Float32 >(
                    // The topic to subscribe to
                    "robo_vel_z",
                    // qos?
                    10,
                    // The callback
                    [this](std_msgs::msg::Float32::UniquePtr msg)
                    {
                        this->vel[2] = msg->data;
                        velMux[2].unlock();
                    }
                );

                // Setup the listener for the time
                this->simTimeSubscription = this->create_subscription< std_msgs::msg::Float32 >(
                    // The topic to subscribe to
                    "simTime",
                    // qos?
                    10,
                    // The callback
                    [this](std_msgs::msg::Float32::UniquePtr msg)
                    {
                        // We want to make sure we get all packets at the same time 
                        // This does not work, I think
                        for (auto &&i : this->velMux) i.lock();
                        
                        for (size_t i = 0; i < 3; i++) this->deltaV[i] = this->velPrev[i] - this->vel[i];
                        
                        
                    }
                );

                this->simIMUPubX = this->create_publisher<std_msgs::msg::Float32>("simIMUPubX", 10);
                this->simIMUPubY = this->create_publisher<std_msgs::msg::Float32>("simIMUPubY", 10);
                this->simIMUPubZ = this->create_publisher<std_msgs::msg::Float32>("simIMUPubZ", 10);

            }

        private:
            rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr simVelXSubscription;
            rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr simVelYSubscription;
            rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr simVelZSubscription;

            std::mutex velMux[3] = { std::mutex(), std::mutex(), std::mutex() };

            rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr simTimeSubscription;
            rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr simIMUPubX;
            rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr simIMUPubY;
            rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr simIMUPubZ;

            float vel[3]            = { 0.0, 0.0, 0.0 };
            float velPrev[3]        = { 0.0, 0.0, 0.0 };
            float deltaV[3]         = { 0.0, 0.0, 0.0 };
            float reckonedPos[3]    = { 0.0, 0.0, 0.0 };
    };


//     class MinimalPublisher : public rclcpp::Node
// {
// public:
//   MinimalPublisher()
//   : Node("minimal_publisher"), count_(0)
//   {
//     publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
//     auto timer_callback =
//       [this]() -> void {
//         auto message = std_msgs::msg::String();
//         message.data = "Hello, world! " + std::to_string(this->count_++);
//         RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
//         this->publisher_->publish(message);
//       };
//     timer_ = this->create_wall_timer(500ms, timer_callback);
//   }

// private:
//   rclcpp::TimerBase::SharedPtr timer_;
//   rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
//   size_t count_;
// };


} // End namespace simple-ros2-test

#endif // H_599955_SRC_SUB