# ifndef SUPPORT_HPP
# define SUPPORT_HPP

#include <Arduino.h>
#include "mbed.h"
#include "rtos.h"
#include <array>
#include <time.h>
#include <memory>

//file reading
#include <fstream>

//Oled screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

//Micro LIDAR
#include "Adafruit_VL53L0X.h"


//Internal humidity and temperature sensor
#include <Arduino_HTS221.h>
//internal baro sensor
#include <Arduino_LPS22HB.h>

// used in thread delays.
#define MS(x) std::chrono::milliseconds(x)

#define HOUR 3600000
#define MINUTE 60000
#define SECOND 1000
#define MIllI_SECOND 1

/**
 * @brief ListNode class, to be used instead of vectors
 * 
 * @tparam T 
 */
template <class T>
class ListNode {
  public:
    T object;
    ListNode* next_node;
    ListNode* prev_node;

    /**
     * @brief Construct a new List Node object
     * 
     * @param object : T
     * @param prev_node_call : ListNode* 
     * @param next_node_call : ListNode*
     */
    ListNode(T object_call, ListNode* prev_node_call, ListNode* next_node_call):
    object(object_call){
      this->next_node = next_node_call;
      this->prev_node = prev_node_call;
    };
};

/**
 * @brief SingleLinkedList class
 * 
 * @tparam T 
 */
template <class T>
class SingleLinkedList  {
  private:
    int length;
    ListNode<T>* head_node;
    ListNode<T>* current_node;
    ListNode<T>* tail_node;
  public:
    /**
     * @brief Construct a new Single Linked List object
     * 
     */
    SingleLinkedList(){
        length = 0;
        head_node = nullptr;
        current_node = nullptr;
        tail_node = nullptr;
    }
    /**
     * @brief Get the Current node
     * 
     * @return T& 
     */
    T& getCurrentNode(){
        return current_node->object;
    }
    /**
     * @brief Get the Head node
     * 
     * @return T& 
     */
    T& getHead() const{
        return head_node->object;
    }
    /**
     * @brief Get the Tail node
     * 
     * @return T& 
     */
    T& getTail() const{
        return tail_node->object;
    }
    /**
     * @brief Get the Length of the linked list
     * 
     * @return int 
     */
    int getLength(){
        return length;
    }
    /**
     * @brief append a new class to single linked list
     * 
     */
    void append(T object){
        ListNode<T> * node = new ListNode<T>(object, tail_node, nullptr);
        if(length == 0)
            current_node = tail_node = head_node = node;
        else {
            tail_node->next_node = node;
            tail_node = node;
        }
        length++;

    }
    /**
     * @brief go to the next node in the linked list, true if succesfull false of unsuccesfull
     * 
     * @return true 
     * @return false 
     */
    bool gotToNextNode(){
        if(length == 0){
            return false;
        }

        if(current_node->next_node == nullptr){
            return false;
        }

        current_node = current_node->next_node;
        return true;
    }
    /**
     * @brief Delete current node
     * 
     */
    void DeleteCurrent(){
        if(length == 0){
            return;
        }
        length--;
        ListNode<T> * temp = current_node;
        if(temp->prev_node != nullptr){
            temp->prev_node->next_node = temp->next_node;
        }
        if(temp->next_node != nullptr){
            temp->next_node->prev_node = temp->prev_node;
        }

        if(length == 0){
            head_node = current_node = tail_node = nullptr;
        }
        else if(current_node == head_node){
            current_node = head_node = head_node->next_node;
        }
        else if(current_node == tail_node){
            current_node = tail_node = tail_node->prev_node;
        }
        else{
            current_node = current_node->prev_node;
        }
        delete temp;
    }
    /**
     * @brief delete last node in linked list, true if succesfull false of unsuccesfull
     * 
     * @return true 
     * @return false 
     */
    bool deleteLastNode(){
        if(length == 0){
            return false;
        }
        current_node = tail_node;
        DeleteCurrent();
    }

    /**
     * @brief Set the current node to the head node
     */
    void setToStart(){
        current_node = head_node;
    }
};
#endif //SUPPORT_HPP