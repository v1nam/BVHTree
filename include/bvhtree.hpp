#pragma once

#include <functional>
#include <vector>
#include "raylib.h"

struct Object {
    Vector2 pos;
    Vector2 velocity;
    Vector2 min;
    Vector2 max;
    int radius;
};


class BVHTree {
private:
    Rectangle bbox = Rectangle{0, 0, 0, 0};
    BVHTree *left = nullptr;
    BVHTree *right = nullptr;
    bool isLeaf = false;
    int leafSize = 2;
    std::vector<Vector2> objs;

    static Rectangle boundingBox(std::vector<Object> &cells);

    static Rectangle split(std::vector<Object> &objects);

public:
    explicit BVHTree(std::vector<Object> &objects);

    void makeInterior(std::vector<Object> &objects);

    void makeLeaf(std::vector<Object> &objects);

    void destroy();

    void draw(Texture &tex, bool drawRect = true);
};