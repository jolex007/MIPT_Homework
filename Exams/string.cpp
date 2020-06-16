#include <iostream>

class string {
public:
    string();

    string(const char*);

    ~string();

    const std::size_t Size() const;

    void Resize(std::size_t new_size, char value = '\0');

    void Resize();

    bool Empty();

    void Reserve(std::size_t new_capacity);

    char Front();

    char Back();

    char& operator[](std::size_t position);

    const char& operator[](std::size_t position) const;

    void PushBack(const char& c);

    void PopBack();

    void Clear();

    string& operator+=(const string& str);

    string& operator=(const string& str);
    
private:
    std::size_t Size(const char* str);

private:
    char* buffer_;
    std::size_t capacity_;
    std::size_t size_;
};

string operator+(const string& first, const string& second) {
    string result = string();
    result += first;
    result += second;
    return result;
}

string::string() : size_(0), capacity_(1) {
   buffer_ = new char[1];
   buffer_[0] = '\0';
}

string::string(const char* str) {
    size_ = Size(str);
    capacity_ = size_;
    buffer_ = new char[size_ + 1];

    for (std::size_t i = 0; i < size_; i++) {
        buffer_[i] = str[i];
    }

    buffer_[capacity_] = '\0';
}

string::~string() {
    delete[] buffer_;
}

const std::size_t string::Size() const {
    return size_;
}

void string::Resize(std::size_t new_size, char value) {
    char* new_buffer = new char[new_size + 1];

    for (std::size_t i = 0; i < size_ && i < new_size; ++i) {
        new_buffer[i] = buffer_[i];
    }

    for (std::size_t i = size_; i < new_size; ++i) {
        new_buffer[i] = value;
    }

    new_buffer[new_size] = '\0';
    delete[] buffer_;
    buffer_ = new_buffer;
    size_ = new_size;
    capacity_ = new_size;
}

void string::Resize() {
    Resize(2 * size_);
}

bool string::Empty() {
    return size_ == 0;
}

void string::Reserve(std::size_t new_capacity) {
    char* new_buffer = new char[new_capacity + 1];

    for (std::size_t i = 0; i < size_ && i < new_capacity; ++i) {
        new_buffer[i] = buffer_[i];
    }

    delete[] buffer_;

    buffer_ = new_buffer;
    capacity_ = new_capacity;
}

char string::Front() {
    return buffer_[0];
}

char string::Back() {
    return buffer_[size_ - 1];
}

char& string::operator[](std::size_t position) {
    return buffer_[position];
}

const char& string::operator[](std::size_t position) const {
    return buffer_[position];
}

void string::PushBack(const char& c) {
    if (size_ >= capacity_) {
        Resize();
    }

    ++size_;
    buffer_[size_ - 1] = c;
    buffer_[size_] = '\0';
}

void string::PopBack() {
    if (!Empty()) {
        buffer_[size_ - 1] = '\0';
        size_--;
    }
}

void string::Clear() {
    size_ = 0;
}

string& string::operator+=(const string& str) {
    for (std::size_t i = 0; i < str.Size(); ++i) {
        PushBack(str[i]);
    }

    buffer_[size_] = '\0';
    return *this;
}

string& string::operator=(const string& str) {
    if (this == &str) {
        return *this;
    }

    if (str.Size() != size_) {
        Resize(str.Size());
    }

    for (std::size_t i = 0; i < size_; ++i) {
        buffer_[i] = str[i];
    }

    size_ = str.Size();
    
    return *this;
}

std::size_t string::Size(const char* str) {
    std::size_t size = 0;
    while (str[size] != '\0') {
        ++size;
    }
    return size;
}


int main()
{
    
    
    return 0;
}
