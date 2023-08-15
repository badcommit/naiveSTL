//
// Created by Shawn Zhang on 8/14/23.
//

#ifndef NAIVESTL_FUNCTION_H
#define NAIVESTL_FUNCTION_H

#include <utility>


#include "memory.h"

namespace NaiveSTL {

    template <typename T>
    class function;

    template<typename Ret, typename ...Args>
    class function<Ret(Args...)> {
    private:
        using invoke_fn_t = Ret (*)(const function &, Args...);
        using copy_fn_t = void* (*)(const function &obj);
        using destroy_fn_t = void (*) (const function &obj);

        void *functor_;
        invoke_fn_t invoke_fn_;
        copy_fn_t copy_fn_;
        destroy_fn_t destroy_fn_;



        template <typename Functor>
        static void destroy_impl(const function& obj)
        {
            delete static_cast<Functor*>(obj.functor_);
        }
        template <typename F>
        static void *copy_impl(const function &obj) {
            return static_cast<void*>(new F(*static_cast<const F*>(obj.functor_)));
        }

        template <typename F>
        static auto invoke_impl(const function &obj, Args... args)->
        decltype(std::declval<F>()(static_cast<Args>(args)...)) {
            return (*static_cast<F*>(obj.functor_))(static_cast<Args>(args)...);
        }

        template <typename F>
        function(int dummy __attribute__((unused)), F&& functor): functor_(static_cast<void*>(new
                std::decay_t<F>(std::forward<F>(functor)))) {
            typedef std::decay_t<F>functor_type;
            static_assert(std::is_same<Ret, decltype(invoke_impl<functor_type>(
                    function(), std::declval<Args>()...))>::value, "invalid functor type");
            static_assert(std::is_copy_constructible<functor_type>::value,
                          "uncopyable functor type");
            invoke_fn_ = invoke_impl<functor_type>;
            destroy_fn_ = destroy_impl<functor_type>;
            copy_fn_ = copy_impl<functor_type>;
        }



    public:
        function(): functor_{nullptr}, copy_fn_{nullptr}, destroy_fn_{nullptr}, invoke_fn_{nullptr}  {}




        template<class F>
        requires std::is_invocable_v<F, Args...> && std::is_convertible_v<std::invoke_result_t<F, Args...>, Ret>
        function(F&& f): function(1, std::forward<F>(f)) {}

        ~function() {
            if(destroy_fn_){
                destroy_fn_(*this);
            }
        }

        operator bool() const noexcept {
            return functor_ != nullptr;
        }

        Ret operator()(Args... args) const {
            return (*invoke_fn_)(*this, args...);
        }

    };

}

#endif //NAIVESTL_FUNCTION_H
