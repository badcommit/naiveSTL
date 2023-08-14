//
// Created by Zhang Shawn on 7/28/23.
//

#ifndef NAIVESTL_REF_H
#define NAIVESTL_REF_H

#include <atomic>
#include <functional>
#include <memory>

namespace NaiveSTL {
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

        auto count() const -> size_t {
            return ncount_.load();
        }

        auto get_data() const -> T * {
            return data_;
        }

        auto operator++() -> ref_cnt_t & {
            ++ncount_;
            return *this;
        }

        auto operator--() -> ref_cnt_t & {
            --ncount_;
            return *this;
        }

        auto operator--(int) -> ref_cnt_t {
            auto t = *this;
            --*this;
            return t;
        }


    };

    template<class T>
    auto operator==(const ref_cnt_t<T> &lhs, const ref_cnt_t<T> &rhs) -> bool {
        return lhs.get_data() == rhs.get_data();
    }

    template<class T>
    auto operator!=(const ref_cnt_t<T> &lhs, const ref_cnt_t<T> &rhs) -> bool {
        return !(lhs == rhs);
    }
}


#endif //NAIVESTL_REF_H
