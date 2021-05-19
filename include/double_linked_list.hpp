# ifndef DOUBLE_LINKED_LIST_HPP
# define DOUBLE_LINKED_LIST_HPP

/**
 * @brief ListNode class for a double linked list.
 * 
 * @tparam T 
 */
template <class T>
class ListNode {
  public:
    std::shared_ptr<T> node_object;
    std::shared_ptr<ListNode<T>> next_node;
    std::shared_ptr<ListNode<T>> prev_node;

    /**
     * @brief Construct a new List Node object
     * 
     * @param node_object : std::shared_ptr<T>
     * @param prev_node_call : std::shared_ptr<ListNode<T>>
     * @param next_node_call : std::shared_ptr<ListNode<T>>
     */
    ListNode(std::shared_ptr<T> object_call, std::shared_ptr<ListNode<T>> prev_node_call,std::shared_ptr<ListNode<T>> next_node_call){
        node_object = object_call;
        next_node = next_node_call;
        prev_node = prev_node_call;
    };

    /**
     * @brief Construct a new empty List Node object
     * 
     */
    ListNode(){};

    /**
     * @brief Get the Object Shared Pointer.
     * 
     * @return std::shared_ptr<T> 
     */
    std::shared_ptr<T> getObjectSharedPointer(){
        return node_object;
    }
};

/**
 * @brief DoubleLinkedList class, a smart pointer double linked list.
 * 
 * @tparam T 
 */
template <class T>
class DoubleLinkedList  {
  private:
    int length;
    std::shared_ptr<ListNode<T>> head_node;
    std::shared_ptr<ListNode<T>> current_node;
    std::shared_ptr<ListNode<T>> tail_node;
  public:
    /**
     * @brief Construct a new Single Linked List object
     * 
     */
    DoubleLinkedList(){
        length = 0;
        current_node = nullptr;
        head_node = nullptr;
        tail_node = nullptr;
    }

    /**
     * @brief Get the Current node, or nullptr of no node is attached.
     * 
     * @return std::shared_ptr<T>
     */
    std::shared_ptr<T> getCurrentNode(){
        if(current_node != nullptr){
            return current_node->getObjectSharedPointer();
        }
        return nullptr;
    }
    /**
     * @brief Get the Head node, or nullptr of no node is attached.
     * 
     * @return std::shared_ptr<T>
     */
    std::shared_ptr<T> getHead() const{
        if(head_node != nullptr){
            return head_node->getObjectSharedPointer();
        }
        return nullptr;
    }
    /**
     * @brief Get the Tail node, or nullptr of no node is attached.
     * 
     * @return std::shared_ptr<T>
     */
    std::shared_ptr<T> getTail() const{
        if(tail_node != nullptr){
            return tail_node->getObjectSharedPointer();
        }
        return nullptr;
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
     * @brief append a new class to double linked list
     * 
     */
    void append(std::shared_ptr<T> object){
        std::shared_ptr<ListNode<T>> node = std::make_shared<ListNode<T>>(object, tail_node, nullptr);
        if(length == 0){
            Serial.println("first item added");
            head_node = std::move(node);
            current_node = head_node;
            tail_node = head_node;
        }else {
            tail_node->next_node = std::move(node);;
            tail_node = tail_node->next_node;
        }
        length++;
    }
    /**
     * @brief go to the next node in the linked list, true if succesfull false if unsuccesfull
     * 
     * @return true 
     * @return false 
     */
    bool gotToNextNode(){
        if(length == 0){
            return false;
        } else if(current_node->next_node == nullptr){
            return false;
        }
        current_node = current_node->next_node;
        return true;
    }

    /**
     * @brief go to the previous node in the linked list, true if succesfull false if unsuccesfull
     * 
     * @return true 
     * @return false 
     */
    bool goToPreviousNode(){
        if(length == 0){
            return false;
        } else if(current_node->prev_node == nullptr){
            return false;
        }
        current_node = current_node->prev_node;
        return true;
    }
    

    /**
     * @brief Set the current node to the head node
     */
    void setToStart(){
        current_node = head_node;
    }
};

#endif // DOUBLE_LINKED_LIST_HPP