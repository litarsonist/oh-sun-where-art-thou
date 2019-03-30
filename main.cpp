// Carley Warlick
// Saturday 30 March 2019
/* Description: Find the location of the sun using equations from Wikipedia and translate that information to something
 * both a user and a machine could understand to effectively simulate the motion of a solar panel. */

using namespace std;
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <cmath>

// couldn't figure out how to access a value for pi that was already in a library
const double pi() {
    return atan(1)*4;
}

// --------------------------------------------- GET CURRENT TIME AND DATE ---------------------------------------------

// get the current second
int getSecond() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    int second = timePtr->tm_sec;
    return second;
}

// get the current minute
int getMinute() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    int minute = timePtr->tm_min;
    return minute;
}

// get the current hour
int getHour() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    int hour = timePtr->tm_hour;
    return hour;
}

// get the current day of the month
int getDay() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    int dayOfMonth = timePtr->tm_mday;
    return dayOfMonth;
}

// get the current month of the year
int getMonth() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    int monthOfYear = timePtr->tm_mon + 1;
    return monthOfYear;
}

// get the current year
int getYear() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    int year = timePtr->tm_year + 1900;
    return year;
}

// --------------------------------------------- GET PALATABLE VALUES --------------------------------------------------

// convert the date values into something user can understand
string getPalatableDate() {

    int d = getDay();
    int mNum = getMonth();
    int y = getYear();

    string m;

    if (mNum == 1) {
        m = "January";
    }
    else if (mNum == 2) {
        m = "February";
    }
    else if (mNum == 3) {
        m = "March";
    }
    else if (mNum == 4) {
        m = "April";
    }
    else if (mNum == 5) {
        m = "May";
    }
    else if (mNum == 6) {
        m = "June";
    }
    else if (mNum == 7) {
        m = "July";
    }
    else if (mNum == 8) {
        m = "August";
    }
    else if (mNum == 9) {
        m = "September";
    }
    else if (mNum == 10) {
        m = "October";
    }
    else if (mNum == 11) {
        m = "November";
    }
    else if (mNum == 12) {
        m = "December";
    }

    string date = m + " " + to_string(d) + ", " + to_string(y);
    return date;

}

// convert the time values into something user can understand
string getPalatableTime() {

    int s = getSecond();
    int m = getMinute();
    int h = getHour();

    string currentTime = to_string(h) + ":" + to_string(m) + ":" + to_string(s);

    return currentTime;

}

// ------------------------------------- GET INTERMEDIATE VALUES FOR CALCULATIONS --------------------------------------

// convert the date into the Julian Day Number for later calculations
double getJulianDayNumber() {

    double m = getMonth();
    double d = getDay();
    double y = getYear();

    double term1 = (1461*(y+4800+(m-14)/12))/4;
    double term2 = (367*(m-2-12*((m-14)/12)))/12;
    double term3 = (3*((y+4900+(m-14)/12)/100))/4;
    double term4 = d-32075;

    double JDN = term1+term2-term3+term4;

    return JDN;

}

// convert the Julian Day Number and current time into the Julian Date for later calculations
double getJulianDate(double JDN) {

    double h = getHour();
    double m = getMinute();
    double s = getSecond();

    double JD = JDN + (h-12)/24 + m/1440 + s/86400;

    return JD;

}

// calculate the number of days since GNT
double getNumDaysGNT(double JD) {
    double n = JD - 2451545.0;
    return n;
}

// calculate mean longitude of the Sun, corrected for aberration of light
double getMeanLongSun(double n) {

    double L = 280.460 + 0.9856474*n;

    while (L > 360 || L < 0) {
        if (L > 360) {
            L = L - 360;
        } else {
            L = L + 360;
        }
    }

    return L; // degrees
}

// calculate the mean anomaly of the Earth in its orbit around the Sun
double getMeanAnom(double n) {

    double g = 357.528 + 0.9856003*n;

    while (g > 360 || g < 0) {
        if (g > 360) {
            g = g - 360;
        }
        else {
            g = g + 360;
        }
    }

    return g;
}

// calculate the ecliptic longitude of the Sun
double getLambda(long double L, long double g) {

    const double PI = pi();

    double term1 = sin(g*PI/180);
    double term2 = sin(2*g*PI/180);
    double lambda = L + 1.915*term1+0.020*term2;
    return lambda;

}

// calculate the epilictic latitude of the Sun
double getEclipticLatSun() {
    double beta = 0;
    return beta;
}

// distance from earth to sun in astronomical units
double getDistanceSun(float g) {

    double R = 1.00014 - 0.01671*cos(g) - 0.00014*cos(2*g);
    return R;

}

// calculate the obliquity of the ecliptic (approximately)
double getEpsilon(double n){
    double epsilon = 23.439 - 0.0000004*n;
    return epsilon;
}

// ----------------------------------------------- EQUATORIAL COORDINATES ----------------------------------------------

// calculate right ascension
double getAlpha(long double epsilon, long double lambda) {
    double alpha = atan2(cos(epsilon)*sin(lambda),cos(lambda));
    return alpha;
}

// calculate declination
double getDelta(long double epsilon, long double lambda) {
    double delta = asin(sin(epsilon)*sin(lambda));
    return delta;
}

// ------------------------------------------------ DISPLAY DATA -------------------------------------------------------

void showData(){

    string d = getPalatableDate();
    cout << "Today's date is: " << d << endl;

    string t = getPalatableTime();
    cout << "The current time is: " << t << endl;

    double JDN = getJulianDayNumber();
    cout << "The Julian Day Number is: " << JDN << endl;

    double JD = getJulianDate(JDN);
    cout << "The Julian Date is: " << JD << endl;

    double n = getNumDaysGNT(JD);
    cout << "The number of days since GNT: " << n << " days" << endl;

    double L = getMeanLongSun(n);
    cout << "The mean longitude of the Sun: " << L << " degrees" << endl;

    double g = getMeanAnom(n);
    cout << "The mean anomaly of the Earth in its orbit around the Sun: " << g << " degrees" << endl;

    double lambda = getLambda(L, g);
    cout << "Ecliptic Longitude: " << lambda << " degrees" << endl;

    double beta = getEclipticLatSun();
    cout << "Ecliptic Latitude: " << beta << " degrees" << endl;

    double R = getDistanceSun((float)g);
    cout << "Distance from Sun: " << R << " astronomical units" << endl;
    cout << "                   " << R*149.6*pow(10,6) << " km" << endl;
    cout << "                   " << (R*149.6*pow(10,6))/1.609 << " miles" << endl;

}

int main() {
    showData();
    return 0;
}