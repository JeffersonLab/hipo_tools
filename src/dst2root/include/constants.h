/**************************************/
/*																		*/
/*  Created by Nick Tyler             */
/*	University Of South Carolina      */
/**************************************/

#ifndef CONSTANTS_H_GUARD
#define CONSTANTS_H_GUARD

#define FORWARD_TAGGER 1
#define FORWARD_DETECTOR 2
#define CENTRAL_DETECTOR 4

// Detector
#define BMT 1
#define BST 2
#define CND 3
#define CTOF 4
#define CVT 5
#define DC 6
#define ECAL 7
#define FMT 8
#define FT 9
#define FTCAL 10
#define FTHODO 11
#define FTOF 12
#define FTTRK 13
#define HTCC 15
#define LTCC 16
#define RF 17
#define RICH 18
#define RTPC 19
#define HEL 20
#define BAND 21

// Layer
#define FTOF_1A 1
#define FTOF_1B 2
#define FTOF_2 3
#define PCAL 1
#define EC_INNER 4
#define EC_OUTER 7

// particle codes
static const int PROTON   = 2212;
static const int NEUTRON  = 2112;
static const int PIP      = 211;
static const int PIM      = -211;
static const int PI0      = 111;
static const int KP       = 321;
static const int KM       = -321;
static const int PHOTON   = 22;
static const int ELECTRON = 11;

// PDG particle masses in GeV/c2
static const double MASS_P     = 0.93827203;
static const double MASS_N     = 0.93956556;
static const double MASS_E     = 0.000511;
static const double MASS_PIP   = 0.13957018;
static const double MASS_PIM   = 0.13957018;
static const double MASS_PI0   = 0.1349766;
static const double MASS_KP    = 0.493677;
static const double MASS_KM    = 0.493677;
static const double MASS_G     = 0.0;
static const double MASS_OMEGA = 0.78265;

#endif
