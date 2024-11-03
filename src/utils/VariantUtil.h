//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_VARIANTUTIL_H
#define PUMAROBOT_VARIANTUTIL_H

// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif //PUMAROBOT_VARIANTUTIL_H
