//
// Created by botanic on 8/4/16.
//

#ifndef ICFPC2016_SKELETON_H
#define ICFPC2016_SKELETON_H

#include <cairo.h>
#include <inttypes.h>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <cstdio>
#include <gmpxx.h>

#include "xypair.h"
#include "polygon.h"

using std::vector;
using std::pair;
using std::make_pair;

class Skeleton {
    vector<polygon> polys;
public:
    bool load(const char* fname) {
        FILE *fp=fopen(fname, "r");
        if (fp == nullptr) return false;
        char line_h[2049];
        char *line= line_h;

        size_t get=2048;
        __ssize_t read;

        fseek(fp, SEEK_SET, 0);

        if ( (read = getline(&line, &get, fp)) < 0) {fclose(fp); return false; }

        long nPolys = strtol(line, nullptr, 10);
        if ( nPolys == 0 ) { fclose(fp); return false; }
        polys.resize((unsigned long) nPolys);

        for (long pl=0; pl!=nPolys; ++pl ) {
            if ( (read = getline(&line, &get, fp)) < 0) {fclose(fp); return false; }
            long nVert = strtol(line, nullptr, 10);
            if ( nVert == 0 ) { fclose(fp); return false; }
            while (nVert--){
                if ( (read = getline(&line, &get, fp)) < 0) {fclose(fp); return false; }
                if (!polys[pl].add_vertex(line)) { fclose(fp); return false; }
            }
        }

        if ( (read = getline(&line, &get, fp)) < 0) {fclose(fp); return false; }
        long nEdges = strtol(line, nullptr, 10);
        if ( nEdges == 0 ) { fclose(fp); return false; }
        while (nEdges--){
            if ( (read = getline(&line, &get, fp)) < 0) {fclose(fp); return false; }
            if (!polys[0].add_edge(line)) { fclose(fp); return false; }
        }

        fclose(fp);
        return true;
    }
    void draw(cairo_t *draw_ctxt, bool color){
        mpq_t minx, miny;
        mpq_t xf, xt, yf, yt;

        mpq_inits(minx, miny, xf, xt, yf, yt, nullptr);

        bool set = true;
        for(auto&pl:polys){
            for(auto&ed:pl.vertices){
                if (set) {
                    set = false;
                    mpq_set(minx, ed.x);
                    mpq_set(miny, ed.y);
                } else {
                    if (mpq_cmp(minx, ed.x) > 0) mpq_set(minx, ed.x);
                    if (mpq_cmp(miny, ed.y) > 0) mpq_set(miny, ed.y);
                }
            }
        }

        int i=0;
        for(auto&pl:polys){
            for(auto&ed:pl.edges) {
                mpq_sub(xf, ed.first.x, minx);
                mpq_sub(xt, ed.second.x, minx);
                mpq_sub(yf, ed.first.y, miny);
                mpq_sub(yt, ed.second.y, miny);

                //rotate against (minx, miny)
                if ((i & 7) == 7) ++i;
                if (color) {
                    cairo_set_source_rgb(draw_ctxt, (i % 2), ((i >> 1) % 2), (i >> 2) % 2);
                    cairo_move_to(draw_ctxt, 10 + 10 * i, 10);
                    cairo_line_to(draw_ctxt, 10 + 10 * i, 60);
                }

                cairo_move_to(draw_ctxt, 100+400.0*mpq_get_d(xf), 600-400.0*mpq_get_d(yf));
                cairo_line_to(draw_ctxt, 100+400.0*mpq_get_d(xt), 600-400.0*mpq_get_d(yt));
                cairo_stroke(draw_ctxt);
                ++i;
            }
        }

        mpq_clears(minx, miny, xf, xt, yf, yt, nullptr);

        //cairo_stroke(draw_ctxt);
        cairo_set_source_rgb(draw_ctxt,1,0,0);
        cairo_set_line_width(draw_ctxt, 0.5);
        cairo_move_to(draw_ctxt, 100, 600);
        cairo_line_to(draw_ctxt, 100, 200);
        cairo_line_to(draw_ctxt, 500, 200);
        cairo_line_to(draw_ctxt, 500, 600);
        cairo_line_to(draw_ctxt, 100, 600);

    }
};


#endif //ICFPC2016_SKELETON_H
