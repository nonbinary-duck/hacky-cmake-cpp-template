#pragma once
#ifndef H_599955_SRC_SUB
#define H_599955_SRC_SUB 1

// Taken from the demos minimal_subscriber lambda

#include <iostream>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32.hpp"

namespace simple_ros2_test
{
    
    class SensorSubscriber : public rclcpp::Node
    {
        public:
            inline SensorSubscriber() : Node("simple_bubble_sensor_subscriber")
            {
                // Setup the listener for the time
                this->simTimeSubscription = this->create_subscription< std_msgs::msg::Float32 >(
                    // The topic to subscribe to
                    "simTime",
                    // qos?
                    10,
                    // The callback
                    [this](std_msgs::msg::Float32::UniquePtr msg)
                    {
                        this->simTime = msg->data;
                    }
                );

                // Setup the listener for the sensor
                this->subscription = this->create_subscription< std_msgs::msg::Bool >(
                    // The topic to subscribe to
                    "sensorTrigger",
                    // qos?
                    10,
                    // The callback
                    [this](std_msgs::msg::Bool::UniquePtr msg)
                    {
                        // If we've collided with something, turn left
                        if (msg->data || this->simTime - this->previousImpact < SensorSubscriber::IMPACT_ADJUST_TIME)
                        {
                            // Check if this impact is after the previous impact time
                            if (this->previousImpact + SensorSubscriber::IMPACT_ADJUST_TIME < this->simTime)
                                // If we're dealing with a new impact, record the incident
                                {this->previousImpact = this->simTime;
                                RCLCPP_INFO(this->get_logger(), "New collision detected...");}


                            // Move left for IMPACT_ADJUST_TIME
                            RCLCPP_INFO(this->get_logger(), "Rotating for '%f' seconds", SensorSubscriber::IMPACT_ADJUST_TIME - (this->simTime - this->previousImpact));
                            std_msgs::msg::Float32 msg; msg.data = -4.5;
                            this->leftMotorPub->publish(msg);
                            msg.data = 4.5;
                            this->rightMotorPub->publish(msg);
                        }
                        else
                        {
                            // Business as usual
                            RCLCPP_INFO(this->get_logger(), "Moving forward");
                            std_msgs::msg::Float32 msg; msg.data = 3.0;
                            this->leftMotorPub->publish(msg);
                            this->rightMotorPub->publish(msg);
                        }
                        
                    }
                );

                this->leftMotorPub = this->create_publisher<std_msgs::msg::Float32>("leftMotorSpeed", 10);
                this->rightMotorPub =this->create_publisher<std_msgs::msg::Float32>("rightMotorSpeed", 10);

                std_msgs::msg::Float32 msg; msg.data = 0.0;

                this->leftMotorPub->publish(msg);
                this->rightMotorPub->publish(msg);

            }

        private:
            rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription;
            rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr simTimeSubscription;
            rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr leftMotorPub;
            rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr rightMotorPub;

            float simTime = 0;
            float previousImpact = 0;
            constexpr static float IMPACT_ADJUST_TIME = 1;
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