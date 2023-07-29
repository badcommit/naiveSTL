//
// Created by Zhang Shawn on 7/28/23.
//

#ifndef NAIVESTL_REF_IMPL_H
#define NAIVESTL_REF_IMPL_H

#include <atomic>
#include <functional>
#include <memory>

namespace NaiveSTL::Impl {
    template<class T>
    struct _default_delete {
        void operator()(T *ptr) {

            delete ptr;

        }
    };

    template<class T>
    struct ref_cnt_t {
        using deleter_type = std::function<void(T *)>;
    private:
        std::atomic<size_t> ncount_;
        T *data_;
        deleter_type deleter_;
    public:

        explicit ref_cnt_t(T *p = nullptr, deleter_type pfunc = deleter_type(_default_delete<T>())) : ncount_(0),
                                                                                                      data_(p),
                                                                                                      deleter_(pfunc) {
            if (data_) {
                ncount_ = 1;
            }
        }

        ref_cnt_t(const ref_cnt_t &) = delete;

        ref_cnt_t &operator=(const ref_cnt_t &) = delete;

        ~ref_cnt_t() {
            if (ncount_ == 0) {
                deleter_(data_);
            }
        }

        size_t count() const {
            return ncount_.load();
        }

        T *get_data() const {
            return data_;
        }

        ref_cnt_t &operator++() {
            ++ncount_;
            return *this;
        }

        ref_cnt_t &operator--() {
            --ncount_;
            return *this;
        }

        ref_cnt_t operator--(int) {
            auto t = *this;
            --*this;
            return t;
        }


    };

    template<class T>
    bool operator==(const ref_cnt_t<T> &lhs, const ref_cnt_t<T> &rhs) {
        return lhs.get_data() == rhs.get_data();
    }

    template<class T>
    bool operator!=(const ref_cnt_t<T> &lhs, const ref_cnt_t<T> &rhs) {
        return !(lhs == rhs);
    }
}


#endif //NAIVESTL_REF_IMPL_H
