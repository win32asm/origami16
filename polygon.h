//
// Created by botanic on 8/5/16.
//

#ifndef ICFPC2016_POLYGON_H
#define ICFPC2016_POLYGON_H

#include <vector>
#include "xypair.h"

using std::vector;
using std::pair;

class polygon {

public:
    bool add_vertex(char *from) {
        vertices.emplace_back();
        return vertices.back().parse(from, nullptr);
    }

    bool add_edge(char *from) {
        char *a1;
        edges.push_back(std::make_pair(xypair(), xypair()));
        if (! edges.back().first.parse(from, &a1)) return false;
        if (! *a1) return false;
        return edges.back().second.parse(a1, nullptr);
    }



    vector<std::pair<xypair,xypair>> edges;
    vector<xypair> vertices;
};

#endif //ICFPC2016_POLYGON_H
