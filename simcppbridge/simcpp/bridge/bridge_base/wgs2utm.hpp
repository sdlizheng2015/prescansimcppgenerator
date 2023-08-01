#include <iostream>
#include <proj.h>
#include <cmath>
#include <iomanip>

// Proj8

using namespace std;

bool wgs84toutm(double lat, double lng, double &x, double &y) {
    PJ_CONTEXT *Context{nullptr};
    PJ *pj{nullptr};
    PJ_COORD WSG84, UTM;
    std::string qstrUTM;
    int nUTMZone{-10000};
    bool bNorth{false};

    nUTMZone = (int)((lng + 186.0)/6.0);
    bNorth = lat > 0 ? true : false;

    //"+proj=utm +zone=50 +datum=WGS84 +units=m +no_defs";
    // "+proj=utm +zone=48 +south +datum=WGS84 +units=m +no_defs"
    qstrUTM = "+proj=utm +zone=" + std::to_string(nUTMZone);
    if(!bNorth)
    {
        qstrUTM += " +south ";
    }
    qstrUTM += " +datum=WGS84 +units=m +no_defs";

    pj = proj_create(Context, qstrUTM.c_str());

    if (pj == 0) {
        std::cerr << "wgs84 to utm failed\n";
        return false;
    }

    WSG84 = proj_coord(lng / 180 * M_PI, lat / 180 * M_PI, 0, 0);
    UTM = proj_trans (pj, PJ_FWD, WSG84);

    x = UTM.enu.e;
    y = UTM.enu.n;
    proj_destroy(pj);
    proj_context_destroy(Context);
    return true;
}


class GPS_UTM
{
private:
    PJ_CONTEXT *Context{nullptr};
    PJ *pj{nullptr};
    PJ_COORD WSG84, UTM;
    std::string qstrUTM;
    int nUTMZone{-10000};
    bool bNorth{false};
 
public:
    GPS_UTM(){
        Context = proj_context_create();
    };

    ~GPS_UTM(){
        proj_destroy(pj);
        proj_context_destroy(Context);
        std::cout << "GPS2UTM Destoried..." << std::endl;
    };

    bool wgs84toutm(double lat, double lng, double &x, double &y) {

        nUTMZone = (int)((lng + 186.0)/6.0);
        bNorth = lat > 0 ? true : false;

        //"+proj=utm +zone=50 +datum=WGS84 +units=m +no_defs";
        // "+proj=utm +zone=48 +south +datum=WGS84 +units=m +no_defs"
        qstrUTM = "+proj=utm +zone=" + std::to_string(nUTMZone);
        if(!bNorth)
        {
            qstrUTM += " +south ";
        }
        qstrUTM += " +datum=WGS84 +units=m +no_defs";

        pj = proj_create(Context, qstrUTM.c_str());

        if (pj == 0) {
            std::cerr << "wgs84 to utm failed\n";
            return false;
        }

        WSG84 = proj_coord(lng / 180 * M_PI, lat / 180 * M_PI, 0, 0);
        UTM = proj_trans (pj, PJ_FWD, WSG84);

        x = UTM.enu.e;
        y = UTM.enu.n;
        return true;
}


};

/*
int main(int argc, char const *argv[])
{
    double x, y;
    GPS_UTM wsg2utm;
    wsg2utm.wgs84toutm(22.744435950, 113.595417400, x, y);
    cout << fixed << setprecision(10)<<x << ": "<< y << endl;
    return 0;
}
*/
