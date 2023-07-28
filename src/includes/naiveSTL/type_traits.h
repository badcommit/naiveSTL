//
// Created by Shawn Zhang on 7/27/23.
//

#ifndef NAIVESTL_TYPE_TRAITS_H
#define NAIVESTL_TYPE_TRAITS_H


namespace NaiveSTL {
    namespace  {
        template<bool, class Ta, class Tb>
        struct IfThenElse;

        template<class Ta, class Tb>
        struct IfThenElse<false, Ta, Tb> {
            typedef Tb result;
        };
        template<class Ta, class Tb>
        struct IfThenElse<true, Ta, Tb>{
            typedef Ta result;
        };
    }

    struct _true_type{};
    struct _false_type{};

    template<class T>
    struct _type_traits{
        typedef _false_type has_trivial_default_constructor;
        typedef _false_type has_trivial_copy_constructor;
        typedef _false_type has_trivial_assignment_operator;
        typedef _false_type has_trivial_destructor;
        typedef _false_type is_POD_type;
    };

    template<>
    struct _type_traits<bool>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };

    template<>
    struct _type_traits<char>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };

    template<>
    struct _type_traits<int>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };

    template<>
    struct _type_traits<unsigned char>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };




}

#endif //NAIVESTL_TYPE_TRAITS_H
