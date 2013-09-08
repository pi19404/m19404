#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <set>

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
//Mat lum;
//Mat cyx;
//Mat nei;
//Mat b;
//namespace trg {
////---------------------------------------------------------------------------------------------
////---------------------------------------------------------------------------------------------
//typedef unsigned char UC;
////---------------------------------------------------------------------------------------------
//const char d8[8][2] = {{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1}};
////---------------------------------------------------------------------------------------------
//struct Rgb {UC r, g, b;};
//struct Rect {int x1, y1, x2, y2;};
//struct Dot {int x, y;};
//int w;
//int h;
////---------------------------------------------------------------------------------------------


//std::vector<Rect> res;
////---------------------------------------------------------------------------------------------
//const int dw = 24;
//const int dh = 24;
//const int h_min = 6;
//const int h_max = 66;
//const int ns_min = 2;
//const double pl_min = 0.5;
////---------------------------------------------------------------------------------------------
////---------------------------------------------------------------------------------------------

//void print_b(const char* path) {
//    freopen(path, "wt", stdout);
//    for (int y = 0; y < h; ++y) {
//        for (int x = 0; x < w; ++x)
//            printf("%c", (b[y][x] ? (char)(b[y][x] + 48) : ' '));
//        putchar('\n');
//    }
//}

////---------------------------------------------------------------------------------------------

//void calc_lums(Mat &raw) {
//    Mat lum=Mat(raw.rows,raw.cols,CV_8UC(1));
//    Mat cyx=Mat(raw.rows,raw.cols,CV_8U(1));
//    Mat nei=Mat(raw.rows,raw.cols,CV_8U(1));
//    Mat b=Mat(raw.rows,raw.cols,CV_8U(1));

//    uchar *lump=lum.ptr<uchar>(0);
//    uchar *cyxp=cyx.ptr<uchar>(0);
//    uchar *rawp=raw.ptr<uchar>(0);
//    uchar *neip=nei.ptr<uchar>(0);
//    uchar *bp=b.ptr<uchar>(0);
//    w=raw.cols;
//    h=raw.rows;
//    int x, y;
//    int zh = h / 1;
//    int zw = w / 3;
//    int y1 = 0;
//    int y2 = y1 + zh;
//    while (y1 < h) {
//        int x1 = 0;
//        int x2 = x1 + zw;
//        while (x1 < w) {
//            int sm = 0;
//            for (y = y1; y < std::min(y2, h); ++y) {
//                for (x = x1; x < std::min(x2, w); ++x) {
//                    int index=y*raw.cols+3*x;
//                    int index1=y*raw.cols+x;
//                    Rgb px = {rawp[index+2],rawp[index+1],rawp[index]};
//                    // ITU-R 601-2 luma transform:
//                    lum[index1] = (UC)(px.r * 0.299 + px.g * 0.587 + px.b * 0.114);
//                    sm += lum[index1];
//                }
//            }
//            double sr = sm * 1.0 / (std::min(y2, h) - y1) / (std::min(x2, w) - x1);
//            double cc = 0;
//            for (y = y1; y < std::min(y2, h); ++y)
//                for (x = x1; x < std::min(x2, w); ++x)
//                {
//                      int index=y*raw.cols+1*x;
//                       cc += fabs(sr - lump[index]);
//                       cc = cc / (std::min(y2, h) - y1) / (std::min(x2, w) - x1);
//                }
//            for (y = y1; y < std::min(y2, h); ++y)
//                for (x = x1; x < std::min(x2, w); ++x)
//                {
//                    int index=y*raw.cols+1*x;
//                    cyxp[index] = (UC)std::max(35.0, cc);
//                }
//            x1 = x2;
//            x2 = x1 + zw;
//        }
//        y1 = y2;
//        y2 = y1 + zh;
//    }

//    //for (int i = 0; i < h; ++i) nei[i].resize(w, 0);
//    std::set<char> t, t1, t2;
//    t1.insert(6); t1.insert(7); t1.insert(0);
//    t2.insert(7); t2.insert(0); t2.insert(1);
//    for (int i = 1; i < h - 1; ++i) {
//        for (int j = 1; j < w - 1; ++j) {
//            int index=i*w+j;
//            char y = 0;
//            t.clear();
//            for (size_t k = 0; k < 8; ++k) {
//                if (abs((int)lump[index] - lum[i + d8[k][0]][j + d8[k][1]]) > cyxp[index]) {
//                    ++y;
//                    t.insert(k);
//                }
//            }
//            if (y == 3) {
//                if (*(t.begin()) + (y - 1) == *(--t.end()) || t == t1 || t == t2)
//                    neip[index] = y;
//                else
//                    neip[index] = -y;
//            }
//            else
//                neip[index] = y;
//        }
//    }

//    int ww = w / dw;
//    int hh = h / dh;
//    Mat m=Mat(hh,ww,CV_8UC(1));
//    uchar *mp=m.ptr<uchar>(0);
//    for (int dy = 0; dy <= dh / 2; dy += dh / 2) {

//        for (int i = 1 + dy; i < h; i += dh)
//            for (int j = 1; j < w; j += dw)
//            {
//                int index=((i-1-dy)/dh)+((j-1)*m.cols/dw);
//                m[index] = stroke_calc(i, j);
//            }
//        for (int i = 0; i < hh + 1; ++i) {
//            for (int j = 0; j < ww; ++j) {
//                int index1=j*m.cols+j;
//                if (m[index1] != 0 && m[i][j + 1] != 0 && m[i][j + 2] != 0 &&
//                    m[index1] + m[i][j + 1] + m[i][j + 2] > 3 * 800) {
//                    m[i][ww + 2] = 1;
//                    break;
//                }
//            }
//        }
//        for (int i = 0; i < hh + 1; ++i) {
//            if (m[i][ww + 2] == 0)
//                continue;
//            for (int j = 0; j < ww + 1; ++j) {
//                if (m[i][j] != 0) {
//                    int h1 = i * dh + 1 + dy;
//                    int h2 = h1 + dh;
//                    h2 = std::min(h - 1, h2);
//                    int w1 = j * dw + 1;
//                    int w2 = w1 + dw;
//                    w2 = std::min(w - 1, w2);
//                    for (int p = h1; p < h2; ++p) {
//                        b[p][w + 2] = 1;
//                        for (int q = w1; q < w2; ++q)
//                            if (nei[p][q] != 0) b[p][q] = 1;
//                    }
//                }
//            }
//        }
//    }
//    //print_b("C:/b1.txt");
//    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//    for (int i = 0; i < h; ++i) {
//        if (b[i][w + 2] == 0) continue;
//        int j = 0;
//        int ss = 0;
//        while (j < w) {
//            if (b[i][j] && b[i][j + 1] && b[i][j + 2]) {
//                int js = j;
//                int sm = 0;
//                int gap = 0;
//                while (j < w && gap < 30) {
//                    if (b[i][j]) {
//                        gap = 0;
//                        ++sm;
//                    }
//                    else
//                        ++gap;
//                    ++j;
//                }
//                if (sm > 20 && sm > (j - js - gap) * 0.4) {
//                    ss += j - js - gap;
//                    for (int k = js; k < j - gap; ++k)
//                        b[i][k] += 2;
//                }
//                ++j;
//            }
//            else ++j;
//        }
//        if (ss < 40) b[i].assign(w + 3, 0);
//        else b[i][w + 2] = 1;
//    }
//    //print_b("C:/b2.txt");
//    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//    int cnt = 0;
//    for (int i = 0; i < h + 1; ++i) {
//        if (b[i][w + 2] == 0) {
//            if (cnt > 0 && cnt < 8) {
//                for (int ii = i - cnt; ii < i; ++ii) {
//                    b[ii].assign(w + 3, 0);
//                }
//            }
//            cnt = 0;
//        }
//        else ++cnt;
//    }
//    //print_b("C:/b3.txt");
//    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//}



////---------------------------------------------------------------------------------------------

//int stroke_calc(int p1, int q1) {
//    int p2 = std::min(h - 1, p1 + dh);
//    int q2 = std::min(w - 1, q1 + dw);
//    int nm = (p2 - p1) * (q2 - q1);
//    double u = 0.0;
//    std::vector<int> x(10);
//    for (int i = p1; i < p2; ++i) {
//        int fl = 0;
//        for (int j = q1; j < q2; ++j) {
//            if (nei[i][j] >= 0) ++x[nei[i][j]];
//            if (nei[i][j] == 0) ++fl;
//        }
//        if (fl == q2 - q1) u += 1.0;
//    }
//    for (int k = 0; k < 7; ++k)
//        if (x[k] == 0) return 0;
//    if (nm > (x[3] + x[6]) * 20)
//        return 0;
//    int cnt1 = x[3] * 16 + x[6] * 16;
//    int cnt2 = x[0];
//    x[9] = int(cnt1 * cnt2 * 1.0 / nm);
//    x[9] = int(x[9] * (1 + u / (p2 - p1)) * (1 + u / (p2 - p1)));
//    if (x[9] < 600 || x[9] > 3000) x[9] = 0;
//    return x[9];
//}

////---------------------------------------------------------------------------------------------

////---------------------------------------------------------------------------------------------

//void get_res() {
//    res.clear();
//    for (int i = 0; i < h; ++i) {
//        for (int j = 0; j < w; ++j) {
//            if (b[i][j] > 1) {
//                int cnt = 1;
//                int x1 = w;
//                int x2 = 0;
//                int y1 = h;
//                int y2 = 0;
//                b[i][j] *= -1;
//                Dot dt;
//                std::vector<Dot> v, ww;
//                dt.x = j;
//                dt.y = i;
//                v.push_back(dt);
//                while (!v.empty()) {
//                    ww.clear();
//                    for (size_t z = 0; z < v.size(); ++z) {
//                        Dot vv = v[z];
//                        for (size_t k = 0; k < 8; ++k) {
//                            int yy = vv.y + d8[k][0];
//                            int xx = vv.x + d8[k][1];
//                            if (xx < 0 || xx >= w || yy < 0 || yy >= h)
//                                continue;
//                            if (b[yy][xx] > 1) {
//                                b[yy][xx] *= -1;
//                                dt.x = xx;
//                                dt.y = yy;
//                                ww.push_back(dt);
//                                x1 = std::min(x1, xx);
//                                x2 = std::max(x2, xx);
//                                y1 = std::min(y1, yy);
//                                y2 = std::max(y2, yy);
//                            }
//                        }
//                    }
//                    v.swap(ww);
//                    cnt += (int)v.size();
//                }
//                int dx = x2 - x1 + 1;
//                int dy = y2 - y1 + 1;
//                double pl = cnt * 1.0 / dx / dy;
//                if (pl > pl_min && dy > h_min && dy < h_max && dx > dy * ns_min) {
//                    Rect r = {x1, y1, x2, y2};
//                    res.push_back(r);
//                }
//            }
//        }
//    }
//}

////---------------------------------------------------------------------------------------------

//std::vector<Rect> get_textlike_regions(const std::vector<std::vector<Rgb> > &raw) {
//    res.clear();
//    if (raw.empty()) return res;
//    w = raw[0].size();
//    h = raw.size();
//    calc_lums(raw);
//    count_neis();
//    build_b();
//    get_res();
//    return res;
//}

////---------------------------------------------------------------------------------------------

//} // End of namespace trg;

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
namespace trg {
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
typedef unsigned char UC;
//---------------------------------------------------------------------------------------------
const char d8[8][2] = {{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1}};
//---------------------------------------------------------------------------------------------
struct Rgb {UC r, g, b;};
struct Rect {int x1, y1, x2, y2;};
struct Dot {int x, y;};
int w;
int h;
//---------------------------------------------------------------------------------------------
std::vector<std::vector<int> > lum;
std::vector<std::vector<int> > cyx;
std::vector<std::vector<int> > nei;
std::vector<std::vector<int> > b;
std::vector<Rect> res;
//---------------------------------------------------------------------------------------------
const int dw = 24;
const int dh = 24;
const int h_min = 6;
const int h_max = 20;
const int ns_min = 2;
const double pl_min = 0.5;
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

void print_b(const char* path) {
    freopen(path, "wt", stdout);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x)
            printf("%c", (b[y][x] ? (char)(b[y][x] + 48) : ' '));
        putchar('\n');
    }
}

//---------------------------------------------------------------------------------------------

Mat l1,c1;
void calc_lums(Mat &raw) {
    cv::cvtColor(raw,l1,CV_BGR2GRAY);
    Scalar value=cv::mean(l1);
    l1.copyTo(c1);
    c1.setTo(cv::Scalar::all(value[0]));
    //c1=lum-value[0];
    //cv::abs()
    //Mat c2;
    cv::absdiff(l1,c1,c1);
    //cv::max(35,c1,c1);
    imshow("AAA",raw);

    Mat dx,dy;
    cv::Sobel(c1,dx,CV_32FC(1),1,0,3);
    cv::Sobel(c1,dy,CV_32FC(1),0,1,3);
    Mat kernel=cv::getStructuringElement(cv::MORPH_CROSS,Size(3,3));
    cv::morphologyEx(dx,dx,cv::MORPH_ERODE,kernel,Point(-1,-1),1);
    imshow("AAA1",dx);
    cv::waitKey(0);

}

//---------------------------------------------------------------------------------------------

void count_neis() {
    nei.clear();
    nei.resize(h);
    for (int i = 0; i < h; ++i) nei[i].resize(w, 0);
    std::set<char> t, t1, t2;
    t1.insert(6); t1.insert(7); t1.insert(0);
    t2.insert(7); t2.insert(0); t2.insert(1);
    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            char y = 0;
            t.clear();
            for (size_t k = 0; k < 8; ++k) {
                if (abs((int)lum[i][j] - lum[i + d8[k][0]][j + d8[k][1]]) > cyx[i][j]) {
                    ++y;
                    t.insert(k);
                }
            }
            if (y == 3) {
                if (*(t.begin()) + (y - 1) == *(--t.end()) || t == t1 || t == t2)
                    nei[i][j] = y;
                else
                    nei[i][j] = -y;
            }
            else
                nei[i][j] = y;
        }
    }
}

//---------------------------------------------------------------------------------------------

int stroke_calc(int p1, int q1) {
    int p2 = std::min(h - 1, p1 + dh);
    int q2 = std::min(w - 1, q1 + dw);
    int nm = (p2 - p1) * (q2 - q1);
    double u = 0.0;
    std::vector<int> x(10);
    for (int i = p1; i < p2; ++i) {
        int fl = 0;
        for (int j = q1; j < q2; ++j) {
            if (nei[i][j] >= 0) ++x[nei[i][j]];
            if (nei[i][j] == 0) ++fl;
        }
        if (fl == q2 - q1) u += 1.0;
    }
    for (int k = 0; k < 7; ++k)
        if (x[k] == 0) return 0;
    if (nm > (x[3] + x[6]) * 20)
        return 0;
    int cnt1 = x[3] * 16 + x[6] * 16;
    int cnt2 = x[0];
    x[9] = int(cnt1 * cnt2 * 1.0 / nm);
    x[9] = int(x[9] * (1 + u / (p2 - p1)) * (1 + u / (p2 - p1)));
    if (x[9] < 600 || x[9] > 3000) x[9] = 0;
    return x[9];
}

//---------------------------------------------------------------------------------------------

void build_b() {
    int ww = w / dw;
    int hh = h / dh;
    //b.assign(h + 1, w + 3);
    //b.resize(h+1,vector<short>(w+3,0));

    b.clear();
    b.resize(h+1);
    for (int i = 0; i < h+1; ++i) b[i].resize(w+3, 0);

    for (int dy = 0; dy <= dh / 2; dy += dh / 2) {
        std::vector<std::vector<int> > m;

        m.clear();
        m.resize(hh+1);
        for (int i = 0; i < hh+1; ++i) m[i].resize(ww+3, 0);


//            m.resize(hh+1,std::vector<int>(ww+1));
        for (int i = 1 + dy; i < h; i += dh)
            for (int j = 1; j < w; j += dw)
                m[(i - 1 - dy) / dh][(j - 1) / dw] = stroke_calc(i, j);
        for (int i = 0; i < hh + 1; ++i) {
            for (int j = 0; j < ww; ++j) {
                if (m[i][j] != 0 && m[i][j + 1] != 0 && m[i][j + 2] != 0 &&
                    m[i][j] + m[i][j + 1] + m[i][j + 2] > 3 * 800) {
                    m[i][ww + 2] = 1;
                    break;
                }
            }
        }
        for (int i = 0; i < hh + 1; ++i) {
            if (m[i][ww + 2] == 0)
                continue;
            for (int j = 0; j < ww + 1; ++j) {
                if (m[i][j] != 0) {
                    int h1 = i * dh + 1 + dy;
                    int h2 = h1 + dh;
                    h2 = std::min(h - 1, h2);
                    int w1 = j * dw + 1;
                    int w2 = w1 + dw;
                    w2 = std::min(w - 1, w2);
                    for (int p = h1; p < h2; ++p) {
                        b[p][w + 2] = 1;
                        for (int q = w1; q < w2; ++q)
                            if (nei[p][q] != 0) b[p][q] = 1;
                    }
                }
            }
        }
    }
    //print_b("C:/b1.txt");
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    for (int i = 0; i < h; ++i) {
        if (b[i][w + 2] == 0) continue;
        int j = 0;
        int ss = 0;
        while (j < w) {
            if (b[i][j] && b[i][j + 1] && b[i][j + 2]) {
                int js = j;
                int sm = 0;
                int gap = 0;
                while (j < w && gap < 30) {
                    if (b[i][j]) {
                        gap = 0;
                        ++sm;
                    }
                    else
                        ++gap;
                    ++j;
                }
                if (sm > 20 && sm > (j - js - gap) * 0.4) {
                    ss += j - js - gap;
                    for (int k = js; k < j - gap; ++k)
                        b[i][k] += 2;
                }
                ++j;
            }
            else ++j;
        }
        if (ss < 40) b[i].assign(w + 3, 0);
        else b[i][w + 2] = 1;
    }
    //print_b("C:/b2.txt");
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    int cnt = 0;
    for (int i = 0; i < h + 1; ++i) {
        if (b[i][w + 2] == 0) {
            if (cnt > 0 && cnt < 8) {
                for (int ii = i - cnt; ii < i; ++ii) {
                    b[ii].assign(w + 3, 0);
                }
            }
            cnt = 0;
        }
        else ++cnt;
    }
    //print_b("C:/b3.txt");
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
}

//---------------------------------------------------------------------------------------------

void get_res() {
    res.clear();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (b[i][j] > 1) {
                int cnt = 1;
                int x1 = w;
                int x2 = 0;
                int y1 = h;
                int y2 = 0;
                b[i][j] *= -1;
                Dot dt;
                std::vector<Dot> v, ww;
                dt.x = j;
                dt.y = i;
                v.push_back(dt);
                while (!v.empty()) {
                    ww.clear();
                    for (size_t z = 0; z < v.size(); ++z) {
                        Dot vv = v[z];
                        for (size_t k = 0; k < 8; ++k) {
                            int yy = vv.y + d8[k][0];
                            int xx = vv.x + d8[k][1];
                            if (xx < 0 || xx >= w || yy < 0 || yy >= h)
                                continue;
                            if (b[yy][xx] > 1) {
                                b[yy][xx] *= -1;
                                dt.x = xx;
                                dt.y = yy;
                                ww.push_back(dt);
                                x1 = std::min(x1, xx);
                                x2 = std::max(x2, xx);
                                y1 = std::min(y1, yy);
                                y2 = std::max(y2, yy);
                            }
                        }
                    }
                    v.swap(ww);
                    cnt += (int)v.size();
                }
                int dx = x2 - x1 + 1;
                int dy = y2 - y1 + 1;
                double pl = cnt * 1.0 / dx / dy;

                if (pl > pl_min && dy > h_min && dy < h_max && dx > dy * ns_min) {
                    Rect r = {x1, y1, x2, y2};
                    res.push_back(r);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------

std::vector<Rect> get_textlike_regions(const std::vector<std::vector<Rgb> > &raw) {
    res.clear();
    if (raw.empty()) return res;
    w = raw[0].size();
    h = raw.size();
    /*calc_lums(raw);
    count_neis();
    build_b();
    get_res();
    return res;*/
}

//---------------------------------------------------------------------------------------------

} // End of namespace trg;
