#include "hypatia/coordinates/Geodetic.h"
#include "hypatia/CoordOps.h"

#include <math.h>

Geodetic::Geodetic () : m_alt(0.0) {
}

double CalculateESquared(double a, double b) {
    return ((a * a) - (b * b)) / (a * a);
}

double CalculateE2Squared(double a, double b) {
    return ((a * a) - (b * b)) / (b * b);
}

double sphsr(double a, double es, double sphi) {
    double sinSphi = sin(sphi);
    double dn = sqrt(1.0 - es * (sinSphi * sinSphi));
    return a * (1.0 - es) / (dn * dn * dn);
}

double sphsn(double a, double es, double sphi) {
    double sinSphi = sin(sphi);
    return a / sqrt(1.0 - es * (sinSphi * sinSphi));
}

double sphtmd(double ap, double bp, double cp, double dp, double ep,
              double sphi) {
    return (ap * sphi) - (bp * sin(2.0 * sphi)) + (cp * sin(4.0 * sphi))
    - (dp * sin(6.0 * sphi)) + (ep * sin(8.0 * sphi));
}

Geodetic::Geodetic( const UTM& _utm ) {
    double fe = 500000.0;
    double ok = 0.9996;
    // char cArray[] = "CDEFGHJKLMNPQRSTUVWX";
    
    double a = CoordOps::EARTH_EQUATORIAL_RADIUS_M;
    double f = 1/298.257223563;
    
    double recf = 1.0 / f;
    double b = a * (recf - 1) / recf;
    double eSquared = CalculateESquared(a, b);
    double e2Squared = CalculateE2Squared(a, b);
    double tn = (a - b) / (a + b);
    double ap = a * (1.0 - tn + 5.0 * ((tn * tn) - (tn * tn * tn)) / 4.0 + 81.0 *
                     ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 64.0);
    double bp = 3.0 * a * (tn - (tn * tn) + 7.0 * ((tn * tn * tn)
                                                   - (tn * tn * tn * tn)) / 8.0 + 55.0 * (tn * tn * tn * tn * tn) / 64.0)
    / 2.0;
    double cp = 15.0 * a * ((tn * tn) - (tn * tn * tn) + 3.0 * ((tn * tn * tn * tn)
                                                                - (tn * tn * tn * tn * tn)) / 4.0) / 16.0;
    double dp = 35.0 * a * ((tn * tn * tn) - (tn * tn * tn * tn) + 11.0
                            * (tn * tn * tn * tn * tn) / 16.0) / 48.0;
    double ep = 315.0 * a * ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 512.0;
    double nfn = 0;
    if ((_utm.zoneY <= 'M' && _utm.zoneY >= 'C')
        || (_utm.zoneY <= 'm' && _utm.zoneY >= 'c')) {
        nfn = 10000000.0;
    }
    double tmd = (_utm.northing - nfn) / ok;
    double sr = sphsr(a, eSquared, 0.0);
    double ftphi = tmd / sr;
    double t10, t11, t14, t15;
    for (int i = 0; i < 5; i++) {
        t10 = sphtmd(ap, bp, cp, dp, ep, ftphi);
        sr = sphsr(a, eSquared, ftphi);
        ftphi = ftphi + (tmd - t10) / sr;
    }
    sr = sphsr(a, eSquared, ftphi);
    double sn = sphsn(a, eSquared, ftphi);
    double s = sin(ftphi);
    double c = cos(ftphi);
    double t = s / c;
    double eta = e2Squared * (c * c);
    double de = _utm.easting - fe;
    t10 = t / (2.0 * sr * sn * (ok * ok));
    t11 = t * (5.0 + 3.0 * (t * t) + eta - 4.0 * (eta * eta) - 9.0 * (t * t)
            * eta) / (24.0 * sr * (sn * sn * sn) * (ok * ok * ok * ok));
    m_theta = ftphi - (de * de) * t10 + (de * de * de * de) * t11;

    // LONGITUDE
    t14 = 1.0 / (sn * c * ok);
    t15 = (1.0 + 2.0 * (t * t) + eta) / (6 * (sn * sn * sn) * c * (ok * ok * ok));
    double dlam = de * t14 - (de * de * de) * t15;
    double olam = MathOps::toRadians(_utm.zoneX * 6 - 183.0);
    m_phi = olam + dlam;

    m_alt = 0.0;
}

Geodetic::Geodetic ( double _lng, double _lat, double _alt, ANGLE_UNIT _angles_unit, DISTANCE_UNIT _alt_unit) {
    if ( _angles_unit == RADS ) {
        m_phi = _lng;
        m_theta = _lat;
    }
    else {
        m_phi = MathOps::toRadians( _lng );
        m_theta = MathOps::toRadians( _lat );
    }
    
    if ( _alt_unit == KM ) {
        m_alt = _alt;
    }
    else {
        m_alt = _alt * CoordOps::AU_TO_KM;
    }
}

Geodetic::~Geodetic () {
}

double Geodetic::getLongitude ( ANGLE_UNIT _type ) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_phi );
    }
    else {
        return m_phi;
    }
}

double Geodetic::getLatitude ( ANGLE_UNIT _type ) const {
    if ( _type == DEGS ) {
        return MathOps::toDegrees( m_theta );
    }
    else {
        return m_theta;
    }
}

double Geodetic::getRadius ( DISTANCE_UNIT _type ) const {
    double pct = abs(sin(m_theta));
    double rad = CoordOps::EARTH_EQUATORIAL_RADIUS_KM * (1.-pct) + CoordOps::EARTH_POLAR_RADIUS_KM * pct;
    rad += m_alt * 0.001; // radious is in KM
    
    if ( _type == KM ) {
        return rad;
    }
    else if ( _type == AU ) {
        return rad * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        return rad * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return rad * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return rad;
    }
}

double Geodetic::getAltitude (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return m_alt;
    }
    else if ( _type == AU ) {
        return m_alt * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        return m_alt * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return m_alt * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return m_alt;
    }
};

Vector3 Geodetic::getVector (DISTANCE_UNIT _type) const {
    if (_type == KM) {
        return Vector3(*this);
    }
    else if ( _type == AU ) {
        return Vector3(*this) * CoordOps::KM_TO_AU;
    }
    else if ( _type == LY ) {
        return Vector3(*this) * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY;
    }
    else if ( _type == PC ) {
        return Vector3(*this) * CoordOps::KM_TO_AU * CoordOps::AU_TO_LY * CoordOps::LY_TO_PC;
    }
    else {
        return Vector3(*this);
    }
}

UTM Geodetic::getUTM() const {
    double fe = 500000.0;
    double ok = 0.9996;
    char cArray[] = "CDEFGHJKLMNPQRSTUVWX";
    
    UTM utm;
    
    double a = CoordOps::EARTH_EQUATORIAL_RADIUS_M;
    double f = 1/298.257223563;

    double lon = getLongitude(DEGS);
    double lat = getLatitude(DEGS);
    
    if (lon <= 0.0) {
        utm.zoneX = 30 + (int) (lon / 6.0);
    } else {
        utm.zoneX = 31 + (int) (lon / 6.0);
    }
    if (lat < 84.0 && lat >= 72.0) {
        // Special case: zone X is 12 degrees from north to south, not 8.
        utm.zoneY = cArray[19];
    } else {
        utm.zoneY = cArray[(int) ((lat + 80.0) / 8.0)];
    }
    if (lat >= 84.0 || lat < -80.0) {
        // Invalid coordinate; the vertical zone is set to the invalid
        // character.
        utm.zoneY = '*';
    }

    double recf = 1.0 / f;
    double b = a * (recf - 1.0) / recf;
    double eSquared = CalculateESquared(a, b);
    double e2Squared = CalculateE2Squared(a, b);
    double tn = (a - b) / (a + b);
    double ap = a * (1.0 - tn + 5.0 * ((tn * tn) - (tn * tn * tn)) / 4.0 + 81.0 *
                     ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 64.0);
    double bp = 3.0 * a * (tn - (tn * tn) + 7.0 * ((tn * tn * tn)
                                                   - (tn * tn * tn * tn)) / 8.0 + 55.0 * (tn * tn * tn * tn * tn) / 64.0)
    / 2.0;
    double cp = 15.0 * a * ((tn * tn) - (tn * tn * tn) + 3.0 * ((tn * tn * tn * tn)
                                                                - (tn * tn * tn * tn * tn)) / 4.0) / 16.0;
    double dp = 35.0 * a * ((tn * tn * tn) - (tn * tn * tn * tn) + 11.0
                            * (tn * tn * tn * tn * tn) / 16.0) / 48.0;
    double ep = 315.0 * a * ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 512.0;
    double olam = MathOps::toRadians(utm.zoneX * 6 - 183);
    double dlam = m_phi - olam;
    double s = sin(m_theta);
    double c = cos(m_theta);
    double t = s / c;
    double eta = e2Squared * (c * c);
    double sn = sphsn(a, eSquared, m_theta);
    double tmd = sphtmd(ap, bp, cp, dp, ep, m_theta);
    double t1 = tmd * ok;
    double t2 = sn * s * c * ok / 2.0;
    double t3 = sn * s * (c * c * c) * ok * (5.0 - (t * t) + 9.0 * eta + 4.0
                                             * (eta * eta)) / 24.0;
    double nfn = 0;
    if (m_theta < 0.0) nfn = 10000000.0;
    utm.northing = nfn + t1 + (dlam * dlam) * t2 + (dlam * dlam * dlam
                                                    * dlam) * t3 + (dlam * dlam * dlam * dlam * dlam * dlam) + 0.5;
    double t6 = sn * c * ok;
    double t7 = sn * (c * c * c) * (1.0 - (t * t) + eta) / 6.0;
    utm.easting = fe + dlam * t6 + (dlam * dlam * dlam) * t7 + 0.5;
    if (utm.northing >= 9999999.0) utm.northing = 9999999.0;
    
    return utm;
}
