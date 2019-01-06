//
// Created by Shepelev Alexey on 27.04.17.
//

#ifndef H_PROBLEM_TUPLE_H
#define H_PROBLEM_TUPLE_H

#include <memory>
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <type_traits>
//#include <boost/type_index.hpp>
//using boost::typeindex::type_id_with_cvr;


template<typename... Others>
class Tuple;

template<>
class Tuple<> {
public:
    static const std::size_t size = 0;

    template<typename... Others>
    bool isSameTo(const Tuple<Others...>&, bool) const {
        return false;
    }

    void swap(Tuple<>&) {}

    template<typename... Others>
    bool operator==(const Tuple<Others...>&) const {
        return false;
    }

    bool operator==(const Tuple<>&) const {
        return true;
    }

    template<typename... Others>
    bool operator<(const Tuple<Others...>&) const {
        return true;
    }

    bool operator<(const Tuple<>&) const {
        return false;
    }
};

template<>
bool Tuple<>::isSameTo(const Tuple<>&, bool isSame) const {
    return isSame;
};

template<typename First, typename... Others>
auto makeTuple(First&& first, Others&& ... others);


template<typename First, typename... Others>
class Tuple<First, Others...> {

    First first_;
    Tuple<Others...> others_;

public:

    static const std::size_t size = 1 + sizeof...(Others);


    typedef First ValueType;

    const First& getFirst() const& {
        return first_;
    }

    First&& getFirst()&& {
        return std::move(first_);
    }

    First& getFirst()& {
        return first_;
    }

    const Tuple<Others...>& getOthers() const& {
        return others_;
    }

    Tuple<Others...>&& getOthers()&& {
        return std::move(others_);
    }

    Tuple<Others...>& getOthers()& {
        return others_;
    }

    ~Tuple() = default;

    Tuple() :
            first_(std::move(First())),
            others_(std::move(Tuple<Others...>())) {
    }

    Tuple(const First& first, const Others& ... others) :
            first_(first),
            others_(others...) {
    }

    template<typename UFirst, typename... UOthers>
    Tuple(UFirst&& first, UOthers&& ... others) :
            first_(std::forward<First>(first)),
            others_(std::forward<Others>(others)...) {
    }

    template<typename UFirst, typename... UOthers>
    Tuple(const Tuple<UFirst, UOthers...>& another) :
            first_(another.getFirst()),
            others_(another.getOthers()) {
    }

    template<typename UFirst, typename... UOthers>
    Tuple(Tuple<UFirst, UOthers...>& another) :
            first_(another.getFirst()),
            others_(another.getOthers()) {
    }

    Tuple(Tuple&& another) :
            first_(another.getFirst()),
            others_(another.getOthers()) {
    }

    template<typename UFirst, typename... UOthers>
    Tuple& operator=(const Tuple<UFirst, UOthers...>& another) {
        first_ = another.getFirst();
        others_ = another.getOthers();
        return *this;
    };

    Tuple& operator=(Tuple&& another) {
        first_ = std::move(another.getFirst());
        others_ = std::move(another.getOthers());
        return *this;
    };

    template<typename UFirst, typename... UOthers>
    bool isSameTo(const Tuple<UFirst, UOthers...>& another, bool isSame = true) const {
        if (!isSame)
            return false;
        if (sizeof...(Others) != sizeof...(UOthers))
            return false;
        isSame &= std::is_same<First, UFirst>::value;
        return others_.isSameTo(another.getOthers(), isSame);
    };

    bool isSameTo(const Tuple<>&, bool isSame = true) const {
        isSame = false;
        return false;
    }

    template<typename UFirst, typename... UOthers>
    void swap(Tuple<UFirst, UOthers...>& another) {
        if (!isSameTo(another))
            return;
        std::swap(first_, another.first_);
        others_.swap(another.others_);
    };

};


template<typename... Others, typename... UOthers>
bool operator==(const Tuple<Others...>& t1, const Tuple<UOthers...>& t2) {
    if (!t1.isSameTo(t2))
        return false;
    if (t1.getFirst() != t2.getFirst())
        return false;
    return t1.getOthers() == t2.getOthers();
};

template<typename... Others>
bool operator==(const Tuple<Others...>&, const Tuple<>&) {
    return false;
};

template<typename... Others>
bool operator==(const Tuple<>&, const Tuple<Others...>&) {
    return false;
};

template<typename... Others, typename... UOthers>
bool operator<(const Tuple<Others...>& t1, const Tuple<UOthers...>& t2) {
    if (sizeof...(Others) > sizeof...(UOthers))
        return false;
    if (sizeof...(Others) < sizeof...(UOthers))
        return true;
    if (!t1.isSameTo(t2))
        return false;
    if (t1.getFirst() > t2.getFirst())
        return false;
    if (t1.getFirst() < t2.getFirst())
        return true;
    return t1.getOthers() < t2.getOthers();
};

template<typename... Others>
bool operator<(const Tuple<Others...>&, const Tuple<>&) {
    return false;
};

template<typename... Others, typename... UOthers>
bool operator>(const Tuple<Others...>& t1, const Tuple<UOthers...>& t2) {
    return t2 < t1;
};

template<typename... Others, typename... UOthers>
bool operator<=(const Tuple<Others...>& t1, const Tuple<UOthers...>& t2) {
    return !(t1 > t2);
};

template<typename... Others, typename... UOthers>
bool operator>=(const Tuple<Others...>& t1, const Tuple<UOthers...>& t2) {
    return !(t1 < t2);
};

template<typename... Others, typename... UOthers>
bool operator!=(const Tuple<Others...>& t1, const Tuple<UOthers...>& t2) {
    return !(t1 == t2);
};

template<typename First, typename... Others>
auto makeTuple(First&& first, Others&& ... others) {
    return Tuple<std::decay_t<First>, std::decay_t<Others>...>(std::forward<First>(first),
                                                               std::forward<Others>(others)...);
};


template<std::size_t index, typename First, typename... Others>
struct GetterByIndex {
    typedef typename GetterByIndex<index - 1, Others...>::ReturnType ReturnType;

    static const ReturnType& get(const Tuple<First, Others...>& t) {
        return GetterByIndex<index - 1, Others...>::get(t.getOthers());
    }

    static ReturnType& get(Tuple<First, Others...>& t) {
        return GetterByIndex<index - 1, Others...>::get(t.getOthers());
    }

    static ReturnType&& get(Tuple<First, Others...>&& t) {
        return GetterByIndex<index - 1, Others...>::get(std::move(t).getOthers());
    }
};

template<typename First, typename... Others>
struct GetterByIndex<0, First, Others...> {
    typedef typename Tuple<First, Others...>::ValueType ReturnType;

    static const ReturnType& get(const Tuple<First, Others...>& t) {
        return t.getFirst();
    }

    static ReturnType& get(Tuple<First, Others...>& t) {
        return t.getFirst();
    }

    static ReturnType&& get(Tuple<First, Others...>&& t) {
        return std::move(t).getFirst();
    }
};

template<std::size_t index, typename First, typename... Others>
typename GetterByIndex<index, First, Others...>::ReturnType&&
get(Tuple<First, Others...>&& t) {
    return GetterByIndex<index, First, Others...>::get(std::move(t));
}

template<std::size_t index, typename First, typename... Others>
typename GetterByIndex<index, First, Others...>::ReturnType const&
get(const Tuple<First, Others...>& t) {
    return GetterByIndex<index, First, Others...>::get(t);
}

template<std::size_t index, typename First, typename... Others>
typename GetterByIndex<index, First, Others...>::ReturnType&
get(Tuple<First, Others...>& t) {
    return GetterByIndex<index, First, Others...>::get(t);
}


template<typename T, std::size_t N, typename... Others>
struct GetterIndexOfElemTypeT {
    static const std::size_t value = N;
};

template<typename T, std::size_t N, typename... Others>
struct GetterIndexOfElemTypeT<T, N, T, Others...> {
    static const std::size_t value = N;
};

template<typename T, std::size_t N, typename First, typename... Others>
struct GetterIndexOfElemTypeT<T, N, First, Others...> {
    static const std::size_t value = GetterIndexOfElemTypeT<T, N + 1, Others...>::value;
};

template<typename T, typename First, typename... Others>
T& get(Tuple<First, Others...>& t) {
    return get<GetterIndexOfElemTypeT<T, 0, First, Others...>::value>(t);
}

template<typename T, typename First, typename... Others>
T&& get(Tuple<First, Others...>&& t) {
    return get<GetterIndexOfElemTypeT<T, 0, First, Others...>::value>(std::move(t));
}

template<typename T, typename First, typename... Others>
const T& get(const Tuple<First, Others...>& t) {
    return std::get<GetterIndexOfElemTypeT<T, 0, First, Others...>::value>(t);
}

template<typename T, typename UTuple, bool Enough, std::size_t TotalArgs, std::size_t... N>
struct Unpacker {
    static auto unpack(T&& addElem, UTuple&& t) {
        return Unpacker<T, UTuple, TotalArgs == 1 + sizeof...(N), TotalArgs, N..., sizeof...(N)
        >::unpack(std::forward<T>(addElem), std::forward<UTuple>(t));
    }
};

template<typename T, typename UTuple, std::size_t TotalArgs, std::size_t... N>
struct Unpacker<T, UTuple, true, TotalArgs, N...> {
    static auto unpack(T&& addElem, UTuple&& t) {
        return makeTuple(std::forward<T>(addElem), get<N>(std::forward<UTuple>(t))...);
    }
};

template<typename T, typename UTuple>
auto unpack(T&& addElem, UTuple&& t) {
    return Unpacker<T, UTuple, 0 == std::decay_t<UTuple>::size, std::decay_t<UTuple>::size
    >::unpack(std::forward<T>(addElem), std::forward<UTuple>(t));
}

template<std::size_t N, bool isLast, typename Tuple1, typename Tuple2>
struct Concatenator {
    static auto concatenate(Tuple1&& t1, Tuple2&& t2) {
        return unpack(get<N>(std::forward<Tuple1>(t1)),
                      Concatenator<N + 1, N + 1 == std::decay_t<Tuple1>::size, Tuple1, Tuple2
                      >::concatenate(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2)));
    }
};

template<std::size_t N, typename Tuple1, typename Tuple2>
struct Concatenator<N, true, Tuple1, Tuple2> {
    static auto concatenate(Tuple1&&, Tuple2&& t2) {
        return std::forward<Tuple2>(t2);
    }
};

template<typename Tuple1, typename Tuple2>
auto concatenateTwo(Tuple1&& t1, Tuple2&& t2) {
    return Concatenator<0, 0 == std::decay_t<Tuple1>::size, Tuple1, Tuple2
    >::concatenate(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2));
};

template <typename Tuple1, typename Tuple2, typename... Tuples>
struct TupleCat {
    static auto tupleCat (Tuple1&& t1, Tuple2&& t2, Tuples&&... ts) {
        return concatenateTwo(t1, TupleCat<Tuple2, Tuples...>::tupleCat(
                std::forward<Tuple1>(t2), std::forward<Tuples>(ts)...));
    }
};

template <typename Tuple1, typename Tuple2>
struct TupleCat<Tuple1, Tuple2> {
    static auto tupleCat (Tuple1&& t1, Tuple2&& t2) {
        return concatenateTwo(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2));
    }
};

template <typename Tuple1, typename Tuple2, typename... Tuples>
auto tupleCat(Tuple1&& t1, Tuple2&& t2, Tuples&&... ts) {
    return TupleCat<Tuple1, Tuple2, Tuples...>::tupleCat(
            std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<Tuples>(ts)...);
};



#endif //H_PROBLEM_TUPLE_H