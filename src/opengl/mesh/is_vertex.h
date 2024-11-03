//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_IS_VERTEX_H
#define PUMAROBOT_IS_VERTEX_H

#include<concepts>
#include <vector>

template<typename T>
concept is_vertex = requires(T v) {
    { T::getSizes() } -> std::same_as<std::vector<int>>;
    { T::getTypes() } -> std::same_as<std::vector<int>>;
    { T::getOffsets() } -> std::same_as<std::vector<int>>;
};

#endif //PUMAROBOT_IS_VERTEX_H
