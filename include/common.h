//
// Created by LONG on 2022/9/29.
//

#ifndef INC_100ASK_PROJECT1_COMMON_H
#define INC_100ASK_PROJECT1_COMMON_H
//lcd坐标系
typedef struct Region {
    int x;//左上角x
    int y;//左上角y
    int height;
    int width;
} Region;
//笛卡尔坐标系region，Cartesian意为笛卡尔坐标系
typedef struct Cartesian_region {
    int x;//左上角x
    int y;//左上角y
    int height;
    int width;
} Cartesian_region;
#endif //INC_100ASK_PROJECT1_COMMON_H
