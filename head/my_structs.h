//
// Created by 孟昱成 on 2021/2/2.
//

#ifndef DRAW_TRIANGLE_MY_STRUCTS_H
#define DRAW_TRIANGLE_MY_STRUCTS_H

typedef struct image {
    int width, height, nrChannels;
    unsigned char *data;

    void drop() {
        //销毁图片
        stbi_image_free(data);
    }
} Image;

#endif //DRAW_TRIANGLE_MY_STRUCTS_H
