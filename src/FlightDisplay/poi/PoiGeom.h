//
// Created by zdanek on 30.07.24.
//

#ifndef SRC_FLIGHTDISPLAY_POI_POI_GEOM_H_
#define SRC_FLIGHTDISPLAY_POI_POI_GEOM_H_

enum PoiGeomType {
    gPoiPolygon,
    gPoiPolyline,
    gPoiCircle,
    gPoiPoint,
    gPoiSvg,
    gPoiInvalid
};

class PoiGeom {
public:
    virtual ~PoiGeom() {}
    virtual PoiGeomType geomType() const = 0;

    int type() const{
        return (int) geomType();
    }
private:


};

#endif //SRC_FLIGHTDISPLAY_POI_POI_GEOM_H_
