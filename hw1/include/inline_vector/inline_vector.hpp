#include <algorithm>
#include <stdexcept>
#include <initializer_list>

template<class T, std::size_t N, class Allocator = std::allocator<T>>
class InlineVector {

public:
    // Aliases for types
    using size_type = std::size_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;

private:
    // Class for handling dynamic data
    class DynData {

    public:
        // Constructor
        DynData() noexcept : allocator_(Allocator()), data_(nullptr), capacity_(0), size_(0) {}

        // Destructor
        ~DynData() {
            clear();
        }

        void clear() noexcept {
            if (data_) {
                for (size_type i = 0; i <= capacity_; i++) {
                    allocator_.destroy(data_ + i);
                }
                allocator_.deallocate(data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
                size_ = 0;
            }
        }

        reference operator[](size_type index) {
            if (index >= size_) {
                throw std::out_of_range("`InlineVector::DynData::operator[]` index out of range");
            }
            return data_[index];
        }

        const_reference operator[](size_type index) const {
            if (index >= size_) {
                throw std::out_of_range("`InlineVector::DynData::operator[]` index out of range");
            }
            return data_[index];
        }

        pointer begin() { return data_; }
        pointer end() { return data_ + size_; }
        const_pointer begin() const { return data_; }
        const_pointer end() const { return data_ + size_; }
        size_type size() const noexcept { return size_; }
        size_type capacity() const noexcept { return capacity_; }

        // Change capacity, allocate memory and move data
        void reserve(const size_type &new_capacity) {
            if (new_capacity > capacity_) {
                size_type new_dyn_capacity = (capacity_ == 0 ? 1 : capacity_ * 2);
                while (new_dyn_capacity < new_capacity) {
                    new_dyn_capacity *= 2;
                }
                pointer new_dyn_data = allocator_.allocate(new_dyn_capacity);
                std::uninitialized_copy(data_, data_ + capacity_, new_dyn_data);
                allocator_.deallocate(data_, capacity_);
                data_ = new_dyn_data;
                capacity_ = new_dyn_capacity;
            }
            size_ = new_capacity;
        }

    private:
        Allocator allocator_;
        pointer data_;
        size_type capacity_;
        size_type size_;
    };

    void sized_to_dyn() {
        std::copy(sized_data.begin(), sized_data.end(), dyn_data.begin());
    }

    void dyn_to_sized() {
        std::copy(dyn_data.begin(), dyn_data.begin() + N, sized_data.begin());
    }

public:
    // Simple constructor
    InlineVector() noexcept: size_(0), dyn_data(DynData()) {}

    // Initializer list constructor
    InlineVector(std::initializer_list<value_type> list) : InlineVector() {
        for (auto i : list) {
            push_back(i);
        }
    }

    // Copy constructor
    InlineVector(const InlineVector& other) : size_(other.size_) {
        if (other.size_ > N)
            dyn_data.reserve(other.size_);
        std::copy(other.begin(), other.end(), begin());
    }

    // Assignment operator
    InlineVector& operator=(const InlineVector& other) {
        if (this != &other) {
            size_ = other.size_;
            if (other.size_ > N) {
                dyn_data.clear();
                dyn_data.reserve(other.size_);
            }
            std::copy(other.begin(), other.end(), begin());
        }
        return *this;
    }

    // Destructor
    ~InlineVector() {
        clear();
    }

    // Vector size
    size_type size() const noexcept {
        return size_;
    }

    // Vector max size
    size_type capacity() const noexcept {
        return size_ <= N ? N : dyn_data.capacity();
    }

    // Check for emptiness
    bool empty() const noexcept {
        return size_ == 0;
    }

    // Index access to the element
    reference operator[](size_type index) {
        return const_cast<reference>(static_cast<const InlineVector&>(*this)[index]);
    }

    const_reference operator[](size_type index) const {
        if (index >= size_) {
            throw std::out_of_range("`InlineVector::operator[]` index out of range");
        }
        return size_ <= N ? sized_data[index] : dyn_data[index];
    }

    // First element access
    reference front() {
        return const_cast<reference>(static_cast<const InlineVector&>(*this).front());
    }

    const_reference front() const {
        if (size_ == 0) {
            throw std::out_of_range("`InlineVector::front()` vector is empty");
        }
        return size_ <= N ? sized_data[0] : dyn_data[0];
    }

    // Last element access
    reference back() {
        return const_cast<reference>(static_cast<const InlineVector&>(*this).back());
    }

    const_reference back() const {
        if (size_ == 0) {
            throw std::out_of_range("`InlineVector::back()` vector is empty");
        }
        return size_ <= N ? sized_data[size_ - 1] : dyn_data[size_ - 1];
    }

    // Adding element to the end
    void push_back(const_reference value) {
        if (size_ < N) {
            sized_data[size_++] = value;
        } else {
            dyn_data.reserve(size_ + 1);
            if (size_ == N)
                sized_to_dyn();
            dyn_data[size_++] = value;
        }
    }

    // Deleting last element
    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("`InlineVector::pop_back()` vector is empty");
        }
        --size_;
        dyn_data.reserve(size_);
        if (size_ == N) {
            dyn_to_sized();
            dyn_data.clear();
        }
    }

    // Iterator to the start of the vector
    iterator begin() noexcept {
        return size_ <= N ? sized_data.begin() : dyn_data.begin();
    }

    // Iterator to the end of the vector
    iterator end() noexcept {
        return size_ <= N ? sized_data.end() : dyn_data.end();
    }

    const_iterator begin() const noexcept {
        return size_ <= N ? sized_data.begin() : dyn_data.begin();
    }

    const_iterator end() const noexcept {
        return size_ <= N ? sized_data.end() : dyn_data.end();
    }

    // Clear vector from elements
    void clear() noexcept {
        if (size_ > N) {
            dyn_data.clear();
        }
        size_ = 0;
    }

    // Insert element at a given position
    iterator insert(const_iterator pos, const_reference value) {
        size_type index = pos - begin();

        if (index == size_) {
            push_back(value);
        } else if (size_ < N) {
            for (size_type i = size_; i > index; --i) {
                sized_data[i] = sized_data[i-1];
            }
            ++size_;
            sized_data[index] = value;
        } else {
            dyn_data.reserve(size_ + 1);
            if (size_ == N)
                sized_to_dyn();
            ++size_;
            std::copy(begin() + index, end(), begin() + index + 1);
            dyn_data[index] = value;
        }

        return begin() + index;
    }

    // Erase element at a given position
    iterator erase(const_iterator pos) {
        size_type index = pos - begin();
        if (pos < begin() || pos >= end()) {
            throw std::out_of_range("`InlineVector::erase` iterator out of range");
        }
        if (pos == end() - 1) {
            pop_back();
            return begin() + index;
        }
        std::move(begin() + index + 1, end(), begin() + index);
        --size_;
        if (size_ > N)
            dyn_data.reserve(size_);
        if (size_ == N)
            dyn_to_sized();
        return begin() + index;
    }

    // Equality check operator
    friend bool operator==(const InlineVector<T, N, Allocator>& lhs, const InlineVector<T, N, Allocator>& rhs) {
        if (lhs.size() != rhs.size() || lhs.capacity() != rhs.capacity()) {
            return false;
        }
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

private:
    size_type size_;
    DynData dyn_data;
    std::array<value_type, N> sized_data;
};