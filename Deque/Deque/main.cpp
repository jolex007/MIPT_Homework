#include <iostream>
#include <cstdint>
#include <cstring>

template <typename T>
class Deque {
private:
    T * array;
    size_t size;
    size_t capacity;
    size_t begin;
    
    int GetPos(int pos)
    {
        return ((int)begin + pos + (int)capacity) % (int)capacity;
    }
    
    size_t GetPos(size_t pos)
    {
        return (begin + pos + capacity) % capacity;
    }
    
    void Resize (size_t new_capacity)
    {
        T * new_array = new T[new_capacity];
        size_t new_begin = (new_capacity - size) / 2;
        if (GetPos(size) <= begin) {
            size_t size_left = capacity - begin;
            memcpy(new_array + new_begin, array + begin, size_left * sizeof(T));
            memcpy(new_array + new_begin + size_left, array, (size - size_left) * sizeof(T));
        } else {
            memcpy(new_array + new_begin, array + begin, size * sizeof(T));
        }
        delete [] array;
        array = new_array;
        capacity = new_capacity;
        begin = new_begin;
    }
    
    void CheckSize ()
    {
        if (size == capacity) {
            Resize(capacity * 2);
        } else if (size < capacity / 4 && capacity > 4) {
            Resize(capacity / 2);
        }
    }
    
public:
    
    Deque ()
    {
        begin = 2;
        capacity = 4;
        size = 0;
        array = new T[capacity];
    }
    
    Deque (int _capacity_)
    {
        capacity = _capacity_;
        begin = capacity / 2;
        size = 0;
        array = new T[capacity];
    }
    
    void print()
    {
        for (int i = 0; i < capacity; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << "\n";
    }
    
    void PushBack (T element)
    {
        size_t position = GetPos(size);
        array[position] = element;
        size++;
        
        CheckSize();
    }
    
    void PushFront (T element)
    {
        int position = GetPos(-1);
        begin = position;
        array[GetPos(0)] = element;
        size++;
        
        CheckSize();
    }
    
    T& Front()
    {
        if (size == 0) {
            throw _THROW_BAD_ALLOC;
        }
        return array[GetPos(0)];
    }
    
    T& Back()
    {
        if (size == 0) {
            throw _THROW_BAD_ALLOC;
        }
        return array[GetPos(size - 1)];
    }
    
    void PopBack ()
    {
        if (size == 0) {
            return;
        }
        size--;
        
        CheckSize();
    }
    
    void PopFront ()
    {
        if (size == 0) {
            return;
        }
        begin = GetPos(1);
        size--;
        
        CheckSize();
    }
    
    T& operator[] (size_t indx)
    {
        return array[GetPos(indx)];
    }
    
    size_t Size()
    {
        return size;
    }
};


int main()
{
    Deque<int> deque(4);
    
    
    return 0;
}
