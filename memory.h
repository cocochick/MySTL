#pragma once
#include <memory>
namespace cocochick{
    //todo:add deleter
    template<typename T>
    class shared_ptr{
    public:
        shared_ptr(): pointer(nullptr), ref_num(new size_t(0)){};
        shared_ptr(T*);
        shared_ptr(const shared_ptr&);
        shared_ptr(shared_ptr&&)    noexcept;
        shared_ptr& operator=(const shared_ptr&);
        shared_ptr& operator=(shared_ptr&&) noexcept;
        ~shared_ptr();
        size_t use_count()  const;
        T* get()    const;
        explicit operator bool() const;
        T& operator*()  const;
        T* operator->() const;
        void reset(T*);
        void swap(shared_ptr&);
        bool unique();
    private:
        T* pointer;
        size_t* ref_num;
    };

    template<typename T>
    shared_ptr<T>::shared_ptr(T* p){
        pointer = p;
        ref_num = new size_t(1);
    }

    template<typename T>
    shared_ptr<T>::shared_ptr(const shared_ptr& rhs): pointer(rhs.pointer), ref_num(rhs.ref_num){
        ++*ref_num;
    }

    template<typename T>
    shared_ptr<T>::shared_ptr(shared_ptr&& rhs) noexcept 
        : pointer(nullptr), ref_num(new size_t(0)){
            rhs.swap(*this);
    };

    template<typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& rhs){
        ++*rhs.ref_num;
        if(pointer == nullptr){
            delete ref_num;
        }
        else if(--*ref_num == 0){
            delete pointer;
            delete ref_num;
        }
        pointer = rhs.pointer;
        ref_num = rhs.ref_num;
        return *this;
    }

    template<typename T>
    shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& rhs)   noexcept{
        shared_ptr(std::move(rhs)).swap(*this);
        return *this;
    }

    template<typename T>
    shared_ptr<T>::~shared_ptr(){
        if(pointer == nullptr){
            delete ref_num;
        }
        else if(--*ref_num == 0){
            delete pointer;
            delete ref_num;
        }
    }

    template<typename T>
    size_t shared_ptr<T>::use_count()   const{
        return *ref_num;
    }

    template<typename T>
    T* shared_ptr<T>::get() const{
        return pointer;
    }

    template<typename T>
    shared_ptr<T>::operator bool()  const{
        return pointer != nullptr;
    }

    template<typename T>
    T& shared_ptr<T>::operator*()   const{
        return *pointer;
    }

    template<typename T>
    T* shared_ptr<T>::operator->()  const{
        return pointer;
    }

    template<typename T>
    void shared_ptr<T>::reset(T* p){
        shared_ptr<T>(p).swap(*this);
    }

    template<typename T>
    void shared_ptr<T>::swap(shared_ptr<T>& rhs){
        T* temp = std::move(rhs.pointer);
        rhs.pointer = std::move(pointer);
        pointer = std::move(temp);
        std::swap(ref_num, rhs.ref_num);
    }

    template<typename T>
    bool shared_ptr<T>::unique(){
        return *ref_num == 1;
    }


    template<typename T>
    class default_deletor{
    public:
        default_deletor() noexcept = default;
        void operator()(T* p){
            delete p;
        }
        default_deletor(const default_deletor&) noexcept {};
    };


    template<typename T, typename P = default_deletor<T>>
    class unique_ptr{
    public:
        unique_ptr() : pointer(nullptr), deletor() {};
        unique_ptr(T* p, P d = default_deletor<T>()) : pointer(p), deletor(d) {};
        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;
        unique_ptr& operator=(std::nullptr_t);
        ~unique_ptr();
        explicit operator bool() const;
        T& operator*()  const;
        T* operator->() const;
        T* get()    const;
        T* release();
        void reset(T* = nullptr);
    private:
        void swap(unique_ptr&);
        T* pointer;
        P deletor;
    };

    template<typename T, typename P>
    unique_ptr<T, P>& unique_ptr<T, P>::operator=(std::nullptr_t){
        deletor(pointer);
        pointer = nullptr;
        return *this;
    }

    template<typename T, typename P>
    unique_ptr<T, P>::~unique_ptr<T, P>(){
        deletor(pointer);
    }

    template<typename T, typename P>
    unique_ptr<T, P>::operator bool()   const{
        return pointer != nullptr;
    }

    template<typename T, typename P>
    T& unique_ptr<T, P>::operator*()   const{
        return *pointer;
    }

    template<typename T, typename P>
    T* unique_ptr<T, P>::operator->()   const{
        return pointer;
    }

    template<typename T, typename P>
    T* unique_ptr<T, P>::get()  const{
        return pointer;
    }
    
    template<typename T, typename P>
    T* unique_ptr<T, P>::release(){
        T* temp = std::move(pointer);
        pointer = nullptr;
        return temp;
    }

    template<typename T, typename P>
    void unique_ptr<T, P>::reset(T* p){
        unique_ptr<T, P>(p).swap(*this);
    }

    template<typename T, typename P>
    void unique_ptr<T, P>::swap(unique_ptr& rhs){
        std::swap(pointer, rhs.pointer);
    }

};
