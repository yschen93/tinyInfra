#include <iostream>
#include <memory>

namespace tinyInfra {

template<typename T, typename Deleter = std::default_delete<T>>
class unique_ptr
{
public:

    /*
     * name: Default Constructor
     */
    unique_ptr() noexcept
    : ptr_(nullptr)
    {

    }

    /*
     * name: Constructor with Parametor
     */
    explicit unique_ptr(T* ptr) noexcept
    : ptr_(ptr)
    {

    }

    /*
     * name: Constructor with Deleter
     */
    unique_ptr(T* ptr, Deleter d) noexcept
    : ptr_(ptr)
    , d_(d)
    {

    }

    /*
     * name: release
     * note: release ownership of resource and return pointer to resource
     */
    T* release() noexcept
    {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    /*
     * name: reset
     * note: release resource and get ownership of ptr
     */
    void reset(T* ptr = nullptr) noexcept
    {
        d_(ptr_);
        ptr_ = ptr;
    }

    /*
     * name: dereference
     */
    T& operator*()
    {
        return *ptr_;
    }

    /*
     * name: dereference
     * return: original pointer
     */
    T* get() noexcept
    {
        return ptr_;
    }

    T* operator->()
    {
        return ptr_;
    }

    /*
     * name: destructor
     */
    ~unique_ptr()
    {
        d_(ptr_);
        ptr_ = nullptr;
    }

    /*
     * note: Delete copy construct && copy assignment
     */
    unique_ptr(const unique_ptr& rhs) = delete;
    unique_ptr& operator=(const unique_ptr& rhs) = delete;

private:
    T* ptr_;
    Deleter d_;
};

} /* tinyInfra*/