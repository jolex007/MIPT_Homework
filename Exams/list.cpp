// Реализовать для std::list конструкторы, деструктор, операторы присваивания,
// push_back и pop_back, push_front и pop_front

#include <cstdio>
#include <iterator>
#include <iostream>

using namespace std;

template<typename T>
class List {
public:
    List();
    List(const List& other);
    explicit List(size_t number, const T& value = T());
    List(List&& other) noexcept ;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    ~List();
    void PushBack(const T& value);
    void PushFront(const T& value);
    void PopBack();
    void PopFront();

private:
    void delete_();
    
private:
    struct Node {
        T* element;
        Node* prev;
        Node* next;

        explicit Node(T* elem, Node* prev = nullptr, Node* next = nullptr) : element(elem), prev(prev), next(next) {}
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(Node&&) = delete;
        ~Node() {
            delete element;
        }
    };

    size_t size_;
    Node* begin_;
    Node* end_;
    
public:
    
    struct Iterator : std::iterator <
        std::bidirectional_iterator_tag,
        T,
        std::size_t,
        T*,
        T&>
    {
    private:
        const List& list;
        Node* curr_node;
        
        Iterator(const List& other, Node* ptr) : list(other), curr_node(ptr) {
        }
        
    public:
        
        Iterator(const Iterator& other) : list(other.list), curr_node(other.curr_node) {
        }
        
        Iterator& operator=(const Iterator& other) {
            list = other.list;
            curr_node = other.curr_node;
            
            return *this;
        }
        
        bool operator==(const Iterator& rhs) {
            return curr_node == rhs.curr_node;
        }
        
        bool operator!=(const Iterator& rhs) {
            return curr_node != rhs.curr_node;
        }
        
        T& operator*() const {
            return *curr_node->element;
        }
        
        T* operator->() const {
            return &curr_node->element;
        }
        
        Iterator& operator++() {
            curr_node = curr_node->next;
            
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator prev(*this);
            curr_node = curr_node->next;
            
            return prev;
        }
        
        Iterator& operator--() {
            curr_node = curr_node->prev;
            
            return *this;
        }
        
        Iterator operator--(int) {
            Iterator prev(*this);
            curr_node = curr_node->prev;
            
            return prev;
        }
        
        template <typename U>
        friend class List;
    };
    
    Iterator begin() {
        return Iterator(*this, begin_);
    }
    
    Iterator end() {
        return Iterator(*this, end_);
    }
};

template<typename T>
List<T>::List() : size_(0), begin_(new Node(nullptr)), end_(begin_) {}

template<typename T>
List<T>::List(const List<T> &other) : size_(other.size_),
                                      begin_(new Node(size_ > 0 ? new T(*other.begin_->element) : nullptr)),
                                      end_(size_ > 0 ? new Node(nullptr) : begin_) {
    if (size_ == 0) {
        return;
    }

    Node* previous = begin_;
    auto iterator = other.begin();
    ++iterator;
    for (size_t i = 1; i < size_; ++i, ++iterator) {
        previous->next = new Node(new T(*iterator), previous);
        previous = previous->next;
    }

    previous->next = end_;
    end_->prev = previous;
}

template<typename T>
List<T>::List(size_t number, const T &value) : size_(number),
                                               begin_(new Node(size_ > 0 ? new T(value) : nullptr)),
                                               end_(size_ > 0 ? new Node(nullptr) : begin_) {
    if (size_ == 0) {
        return;
    }

    Node* previous = begin_;
    for (size_t i = 1; i < size_; ++i) {
        previous->next = new Node(new T(value), previous);
        previous = previous->next;
    }

    previous->next = end_;
    end_->prev = previous;
}

template<typename T>
List<T>::List(List<T> &&other) noexcept : size_(other.size_), begin_(other.begin_), end_(other.end_) {
    other.size_ = 0;
    other.begin_ = other.end_ = new Node(nullptr);
}

template<typename T>
void List<T>::delete_() {
    while (begin_ != end_) {
        begin_ = begin_->next;
        delete begin_->prev;
        begin_->prev = nullptr;
    }
    
    delete end_;
    end_ = nullptr;
}

template<typename T>
List<T> & List<T>::operator=(List<T> &&other)  noexcept {
    if (this == &other) {
        return *this;
    }
    
    delete_();
    
    size_ = other.size_;
    begin_ = other.begin_;
    end_ = other.end_;
    other.size_ = 0;
    other.begin_ = other.end_ = new Node(nullptr);
    
    return *this;
}

template<typename T>
List<T> & List<T>::operator=(const List<T> &other) {
    if (this == &other) {
        return *this;
    }
    
    delete_();
    
    size_ = other.size_;
    begin_ = new Node(size_ > 0 ? new T(other.begin_->element) : nullptr);
    end_ = (size_ > 0 ? new Node(nullptr) : begin_);
    
    if (size_ == 0) {
        return *this;
    }
    
    Node previous = begin_;
    auto iterator = other.begin();
    ++iterator;
    for (size_t i = 1; i < size_; ++i, ++iterator) {
        previous.next = new Node(new T(*iterator), previous);
        previous = previous.next;
    }
    
    previous->next = end_;
    end_->prev = previous;
    
    return *this;
}

template<typename T>
List<T>::~List() {
    delete_();
}

template<typename T>
void List<T>::PushBack(const T &value) {
    Node* elem = new Node(new T(value), end_->prev, end_);
    if (end_->prev) {
        end_->prev->next = elem;
    } else {
        begin_ = elem;
    }
    
    end_->prev = elem;
    size_++;
}

template<typename T>
void List<T>::PushFront(const T &value) {
    Node* elem = new Node(new T(value), nullptr, begin_);
    begin_->prev = elem;
    begin_ = elem;
    size_++;
}

template<typename T>
void List<T>::PopBack() {
    end_ = end_->prev;
    delete end_->next;
    end_->next = nullptr;
    delete end_->element;
    end_->element = nullptr;
    size_--;
}

template<typename T>
void List<T>::PopFront() {
    begin_ = begin_->next;
    delete begin_->prev;
    begin_->prev = nullptr;
    size_--;
}

int main() {
    List<int> a;
    
    a.PushBack(5);
    a.PushBack(6);
    a.PushBack(7);
    
    for (auto& it : a) {
        std::cout << it;
    }
    
    
    return 0;
}
