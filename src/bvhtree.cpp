#include <algorithm>
#include <vector>

#include "bvhtree.hpp"
#include "raylib.h"

BVHTree::BVHTree(std::vector<Object> &objects) {
    objects.size() <= leafSize ? makeLeaf(objects) : makeInterior(objects);
}

void BVHTree::makeInterior(std::vector<Object> &objects) {
    bbox = split(objects);
    int mid = (int) objects.size() / 2;
    std::vector<Object> l(objects.begin(), objects.begin() + mid);
    std::vector<Object> r(objects.begin() + mid, objects.end());
    left = new BVHTree(l);
    right = new BVHTree(r);
}

void BVHTree::makeLeaf(std::vector<Object> &objects) {
    bbox = boundingBox(objects);
    isLeaf = true;
    for (Object &obj: objects)
        objs.push_back(obj.min);
}

Rectangle BVHTree::split(std::vector<Object> &objects) {
    Rectangle bbox_ = boundingBox(objects);
    std::nth_element(objects.begin(), objects.begin() + (int) objects.size() / 2, objects.end(),
                     [bbox_](Object &a, Object &b) -> bool {
                         return bbox_.width > bbox_.height ? a.min.x < b.min.x : a.min.y < b.min.y;
                     });
    return bbox_;
}

void BVHTree::destroy() {
    if (left)
        left->destroy();
    if (right)
        right->destroy();
    delete this;
}

void BVHTree::draw(Texture &tex, bool drawRect) {
    if (isLeaf) {
        for (Vector2 &pos: objs)
            DrawTextureV(tex, pos, WHITE);
    }
    if (drawRect)
        DrawRectangleLines((int) bbox.x, (int) bbox.y, (int) bbox.width, (int) bbox.height, BLACK);
    if (left)
        left->draw(tex, drawRect);
    if (right)
        right->draw(tex, drawRect);
}

Rectangle BVHTree::boundingBox(std::vector<Object> &cells) {
    std::vector<float> box = {cells[0].min.x, cells[0].max.x, cells[0].min.y, cells[0].max.y};

    for (Object &obj: cells) {
        if (obj.min.x < box[0])
            box[0] = obj.min.x;
        if (obj.max.x > box[1])
            box[1] = obj.max.x;
        if (obj.min.y < box[2])
            box[2] = obj.min.y;
        if (obj.max.y > box[3])
            box[3] = obj.max.y;
    }
    return Rectangle{box[0], box[2], (box[1] - box[0]), (box[3] - box[2])};
}
