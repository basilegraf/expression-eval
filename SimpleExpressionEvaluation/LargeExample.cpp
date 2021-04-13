#include <stdio.h>
#include <vector>
#include <exception>

#include <string>

#include <cmath>

#include <chrono>

#include "LargeExample.h"
 


std::string getLargeExampleExpression()
{
    std::string expression_string =
        "H = sqrt(pow(X[1], 2) + pow(PDFC[8] + X[0], 2));"
        "Ht = (PDFC[8]*Xt[0] + X[0]*Xt[0] + X[1]*Xt[1])/H;"
        "Htt = (-pow(Ht, 2) + PDFC[8]*Xtt[0] + X[0]*Xtt[0] + X[1]*Xtt[1] + pow(Xt[0], 2) + pow(Xt[1], 2))/H;"
        "omega = acos(0.5*(pow(H, 2) - pow(PDFC[10], 2) + pow(PDFC[9], 2))/(H*PDFC[9]));"
        "omegat = Ht*(-H + PDFC[9]*cos(omega))/(H*PDFC[9]*sin(omega));"
        "tmp100 = sin(omega);"
        "tmp101 = PDFC[9]*cos(omega);"
        "omegatt = (-H*Htt - H*pow(omegat, 2)*tmp101 - pow(Ht, 2) - 2*Ht*PDFC[9]*omegat*tmp100 + Htt*tmp101)/(H*PDFC[9]*tmp100);"
        "xi = acos(0.5*(-pow(H, 2) + pow(PDFC[10], 2) + pow(PDFC[9], 2))/(PDFC[10]*PDFC[9]));"
        "xit = H*Ht/(PDFC[10]*PDFC[9]*sin(xi));"
        "xitt = (H*Htt + pow(Ht, 2) - PDFC[10]*PDFC[9]*pow(xit, 2)*cos(xi))/(PDFC[10]*PDFC[9]*sin(xi));"
        "alpha = atan2(X[1], PDFC[8] + X[0]);"
        "tmp102 = cos(alpha);"
        "tmp103 = sin(alpha);"
        "alphat = (-Xt[0]*tmp103 + Xt[1]*tmp102)/(PDFC[8]*tmp102 + X[0]*tmp102 + X[1]*tmp103);"
        "tmp104 = cos(alpha);"
        "tmp105 = sin(alpha);"
        "tmp106 = 2*alphat;"
        "tmp107 = pow(alphat, 2);"
        "tmp108 = tmp105*tmp107;"
        "alphatt = (PDFC[8]*tmp108 + X[0]*tmp108 - X[1]*tmp104*tmp107 - Xt[0]*tmp104*tmp106 - Xt[1]*tmp105*tmp106 - Xtt[0]*tmp105 + Xtt[1]*tmp104)/(PDFC[8]*tmp104 + X[0]*tmp104 + X[1]*tmp105);"
        "theta = alpha + omega;"
        "thetat = alphat + omegat;"
        "thetatt = alphatt + omegatt;"
        "tmp109 = pow(thetat, 2);"
        "tmp110 = theta + xi;"
        "tmp111 = PDFC[6]*cos(tmp110);"
        "tmp112 = 2*thetat*xit;"
        "tmp113 = pow(xit, 2);"
        "tmp114 = PDFC[8]*cos(xi);"
        "tmp115 = tmp111 - tmp114;"
        "tmp116 = PDFC[1]*(tmp109*tmp111 + tmp111*tmp112 + tmp113*tmp115);"
        "tmp117 = cos(theta);"
        "tmp118 = PDFC[5]*tmp117;"
        "tmp119 = PDFC[0]*tmp118;"
        "tmp120 = tmp109*tmp119;"
        "tmp121 = PDFC[8]*sin(xi);"
        "tmp122 = PDFC[6]*sin(tmp110);"
        "tmp123 = -tmp121 + tmp122;"
        "tmp124 = PDFC[1]*xitt;"
        "tmp125 = sin(theta);"
        "tmp126 = PDFC[5]*tmp125;"
        "tmp127 = PDFC[0]*tmp126;"
        "tmp128 = thetatt*(PDFC[1]*tmp122 - tmp127) + tmp123*tmp124;"
        "tmp129 = tmp109*tmp127;"
        "tmp130 = tmp115*tmp124;"
        "tmp131 = PDFC[1]*tmp111;"
        "tmp132 = thetatt*(-tmp119 + tmp131);"
        "tmp133 = tmp109*tmp122;"
        "tmp134 = tmp112*tmp122;"
        "tmp135 = PDFC[1]*PDFC[7];"
        "tmp136 = PDFC[0]*PDFC[7];"
        "tmp137 = PDFC[6]*PDFC[8];"
        "tmp138 = PDFC[1]*tmp117*tmp137;"
        "tmp139 = PDFC[9]*tmp131;"
        "tmp140 = +PDFC[1];"
        "tmp141 = tmp125*tmp137;"
        "tmp142 = PDFC[9]*tmp113;"
        "fDFC[0] = tmp116 - tmp120 + tmp128;"
        "fDFC[1] = PDFC[1]*(tmp113*tmp123 + tmp133 + tmp134) - tmp129 - tmp130 - tmp132;"
        "fDFC[2] = -tmp135 - tmp136;"
        "fDFC[3] = PDFC[1]*PDFC[4]*(tmp113*(tmp121 - tmp122) - tmp133 - tmp134) + PDFC[4]*tmp129 + PDFC[4]*(tmp130 + tmp132) + tmp123*tmp135 - tmp126*tmp136;"
        "fDFC[4] = PDFC[4]*tmp116 - PDFC[4]*tmp120 + PDFC[4]*tmp128 + tmp135*(PDFC[9] - tmp111 + tmp114) + tmp136*(-PDFC[8] + tmp118);"
        "fDFC[5] = PDFC[1]*(PDFC[9]*tmp133 + PDFC[9]*tmp134 + tmp109*tmp141 + tmp112*tmp141 - tmp121*tmp142 + tmp122*tmp142) + PDFC[8]*tmp129 - thetatt*(-PDFC[2] - PDFC[3] + PDFC[8]*tmp119 + tmp138 + tmp139) + xitt*(PDFC[3] + pow(PDFC[8], 2)*tmp140 + PDFC[9]*tmp114*tmp140 - 2.0*tmp138 - tmp139);";
    return expression_string;
}



// Expression version for the ExprTk lib
std::string getLargeExampleExpressionExprtk()
{
    std::string expression_string =
        "var H := sqrt(pow(X[1], 2) + pow(PDFC[8] + X[0], 2));"
        "var Ht := (PDFC[8]*Xt[0] + X[0]*Xt[0] + X[1]*Xt[1])/H;"
        "var Htt := (-pow(Ht, 2) + PDFC[8]*Xtt[0] + X[0]*Xtt[0] + X[1]*Xtt[1] + pow(Xt[0], 2) + pow(Xt[1], 2))/H;"
        "var omega := acos(0.5*(pow(H, 2) - pow(PDFC[10], 2) + pow(PDFC[9], 2))/(H*PDFC[9]));"
        "var omegat := Ht*(-H + PDFC[9]*cos(omega))/(H*PDFC[9]*sin(omega));"
        "var tmp100 := sin(omega);"
        "var tmp101 := PDFC[9]*cos(omega);"
        "var omegatt := (-H*Htt - H*pow(omegat, 2)*tmp101 - pow(Ht, 2) - 2*Ht*PDFC[9]*omegat*tmp100 + Htt*tmp101)/(H*PDFC[9]*tmp100);"
        "var xi := acos(0.5*(-pow(H, 2) + pow(PDFC[10], 2) + pow(PDFC[9], 2))/(PDFC[10]*PDFC[9]));"
        "var xit := H*Ht/(PDFC[10]*PDFC[9]*sin(xi));"
        "var xitt := (H*Htt + pow(Ht, 2) - PDFC[10]*PDFC[9]*pow(xit, 2)*cos(xi))/(PDFC[10]*PDFC[9]*sin(xi));"
        "var alpha := atan2(X[1], PDFC[8] + X[0]);"
        "var tmp102 := cos(alpha);"
        "var tmp103 := sin(alpha);"
        "var alphat := (-Xt[0]*tmp103 + Xt[1]*tmp102)/(PDFC[8]*tmp102 + X[0]*tmp102 + X[1]*tmp103);"
        "var tmp104 := cos(alpha);"
        "var tmp105 := sin(alpha);"
        "var tmp106 := 2*alphat;"
        "var tmp107 := pow(alphat, 2);"
        "var tmp108 := tmp105*tmp107;"
        "var alphatt := (PDFC[8]*tmp108 + X[0]*tmp108 - X[1]*tmp104*tmp107 - Xt[0]*tmp104*tmp106 - Xt[1]*tmp105*tmp106 - Xtt[0]*tmp105 + Xtt[1]*tmp104)/(PDFC[8]*tmp104 + X[0]*tmp104 + X[1]*tmp105);"
        "var theta := alpha + omega;"
        "var thetat := alphat + omegat;"
        "var thetatt := alphatt + omegatt;"
        "var tmp109 := pow(thetat, 2);"
        "var tmp110 := theta + xi;"
        "var tmp111 := PDFC[6]*cos(tmp110);"
        "var tmp112 := 2*thetat*xit;"
        "var tmp113 := pow(xit, 2);"
        "var tmp114 := PDFC[8]*cos(xi);"
        "var tmp115 := tmp111 - tmp114;"
        "var tmp116 := PDFC[1]*(tmp109*tmp111 + tmp111*tmp112 + tmp113*tmp115);"
        "var tmp117 := cos(theta);"
        "var tmp118 := PDFC[5]*tmp117;"
        "var tmp119 := PDFC[0]*tmp118;"
        "var tmp120 := tmp109*tmp119;"
        "var tmp121 := PDFC[8]*sin(xi);"
        "var tmp122 := PDFC[6]*sin(tmp110);"
        "var tmp123 := -tmp121 + tmp122;"
        "var tmp124 := PDFC[1]*xitt;"
        "var tmp125 := sin(theta);"
        "var tmp126 := PDFC[5]*tmp125;"
        "var tmp127 := PDFC[0]*tmp126;"
        "var tmp128 := thetatt*(PDFC[1]*tmp122 - tmp127) + tmp123*tmp124;"
        "var tmp129 := tmp109*tmp127;"
        "var tmp130 := tmp115*tmp124;"
        "var tmp131 := PDFC[1]*tmp111;"
        "var tmp132 := thetatt*(-tmp119 + tmp131);"
        "var tmp133 := tmp109*tmp122;"
        "var tmp134 := tmp112*tmp122;"
        "var tmp135 := PDFC[1]*PDFC[7];"
        "var tmp136 := PDFC[0]*PDFC[7];"
        "var tmp137 := PDFC[6]*PDFC[8];"
        "var tmp138 := PDFC[1]*tmp117*tmp137;"
        "var tmp139 := PDFC[9]*tmp131;"
        "var tmp140 := +PDFC[1];"
        "var tmp141 := tmp125*tmp137;"
        "var tmp142 := PDFC[9]*tmp113;"
        "fDFC[0] := tmp116 - tmp120 + tmp128;"
        "fDFC[1] := PDFC[1]*(tmp113*tmp123 + tmp133 + tmp134) - tmp129 - tmp130 - tmp132;"
        "fDFC[2] := -tmp135 - tmp136;"
        "fDFC[3] := PDFC[1]*PDFC[4]*(tmp113*(tmp121 - tmp122) - tmp133 - tmp134) + PDFC[4]*tmp129 + PDFC[4]*(tmp130 + tmp132) + tmp123*tmp135 - tmp126*tmp136;"
        "fDFC[4] := PDFC[4]*tmp116 - PDFC[4]*tmp120 + PDFC[4]*tmp128 + tmp135*(PDFC[9] - tmp111 + tmp114) + tmp136*(-PDFC[8] + tmp118);"
        "fDFC[5] := PDFC[1]*(PDFC[9]*tmp133 + PDFC[9]*tmp134 + tmp109*tmp141 + tmp112*tmp141 - tmp121*tmp142 + tmp122*tmp142) + PDFC[8]*tmp129 - thetatt*(-PDFC[2] - PDFC[3] + PDFC[8]*tmp119 + tmp138 + tmp139) + xitt*(PDFC[3] + pow(PDFC[8], 2)*tmp140 + PDFC[9]*tmp114*tmp140 - 2.0*tmp138 - tmp139);";
    return expression_string;
}
 

 

void LargeExampleFunc(double* X, double* Xt, double* Xtt, double* PDFC, double* fDFC)
{
    using namespace std;
	double H = sqrt(pow(X[1], 2) + pow(PDFC[8] + X[0], 2));
	double Ht = (PDFC[8]*Xt[0] + X[0]*Xt[0] + X[1]*Xt[1])/H;
	double Htt = (-pow(Ht, 2) + PDFC[8]*Xtt[0] + X[0]*Xtt[0] + X[1]*Xtt[1] + pow(Xt[0], 2) + pow(Xt[1], 2))/H;
	double omega = acos(0.5*(pow(H, 2) - pow(PDFC[10], 2) + pow(PDFC[9], 2))/(H*PDFC[9]));
	double omegat = Ht*(-H + PDFC[9]*cos(omega))/(H*PDFC[9]*sin(omega));
	double tmp100 = sin(omega);
	double tmp101 = PDFC[9]*cos(omega);
	double omegatt = (-H*Htt - H*pow(omegat, 2)*tmp101 - pow(Ht, 2) - 2*Ht*PDFC[9]*omegat*tmp100 + Htt*tmp101)/(H*PDFC[9]*tmp100);
	double xi = acos(0.5*(-pow(H, 2) + pow(PDFC[10], 2) + pow(PDFC[9], 2))/(PDFC[10]*PDFC[9]));
	double xit = H*Ht/(PDFC[10]*PDFC[9]*sin(xi));
	double xitt = (H*Htt + pow(Ht, 2) - PDFC[10]*PDFC[9]*pow(xit, 2)*cos(xi))/(PDFC[10]*PDFC[9]*sin(xi));
	double alpha = atan2(X[1], PDFC[8] + X[0]);
	double tmp102 = cos(alpha);
	double tmp103 = sin(alpha);
	double alphat = (-Xt[0]*tmp103 + Xt[1]*tmp102)/(PDFC[8]*tmp102 + X[0]*tmp102 + X[1]*tmp103);
	double tmp104 = cos(alpha);
	double tmp105 = sin(alpha);
	double tmp106 = 2*alphat;
	double tmp107 = pow(alphat, 2);
	double tmp108 = tmp105*tmp107;
	double alphatt = (PDFC[8]*tmp108 + X[0]*tmp108 - X[1]*tmp104*tmp107 - Xt[0]*tmp104*tmp106 - Xt[1]*tmp105*tmp106 - Xtt[0]*tmp105 + Xtt[1]*tmp104)/(PDFC[8]*tmp104 + X[0]*tmp104 + X[1]*tmp105);
	double theta = alpha + omega;
	double thetat = alphat + omegat;
	double thetatt = alphatt + omegatt;
	double tmp109 = pow(thetat, 2);
	double tmp110 = theta + xi;
	double tmp111 = PDFC[6]*cos(tmp110);
	double tmp112 = 2*thetat*xit;
	double tmp113 = pow(xit, 2);
	double tmp114 = PDFC[8]*cos(xi);
	double tmp115 = tmp111 - tmp114;
	double tmp116 = PDFC[1]*(tmp109*tmp111 + tmp111*tmp112 + tmp113*tmp115);
	double tmp117 = cos(theta);
	double tmp118 = PDFC[5]*tmp117;
	double tmp119 = PDFC[0]*tmp118;
	double tmp120 = tmp109*tmp119;
	double tmp121 = PDFC[8]*sin(xi);
	double tmp122 = PDFC[6]*sin(tmp110);
	double tmp123 = -tmp121 + tmp122;
	double tmp124 = PDFC[1]*xitt;
	double tmp125 = sin(theta);
	double tmp126 = PDFC[5]*tmp125;
	double tmp127 = PDFC[0]*tmp126;
	double tmp128 = thetatt*(PDFC[1]*tmp122 - tmp127) + tmp123*tmp124;
	double tmp129 = tmp109*tmp127;
	double tmp130 = tmp115*tmp124;
	double tmp131 = PDFC[1]*tmp111;
	double tmp132 = thetatt*(-tmp119 + tmp131);
	double tmp133 = tmp109*tmp122;
	double tmp134 = tmp112*tmp122;
	double tmp135 = PDFC[1]*PDFC[7];
	double tmp136 = PDFC[0]*PDFC[7];
	double tmp137 = PDFC[6]*PDFC[8];
	double tmp138 = PDFC[1]*tmp117*tmp137;
	double tmp139 = PDFC[9]*tmp131;
	double tmp140 = +PDFC[1];
	double tmp141 = tmp125*tmp137;
	double tmp142 = PDFC[9]*tmp113;
	fDFC[0] = tmp116 - tmp120 + tmp128;
	fDFC[1] = PDFC[1]*(tmp113*tmp123 + tmp133 + tmp134) - tmp129 - tmp130 - tmp132;
	fDFC[2] = -tmp135 - tmp136;
	fDFC[3] = PDFC[1]*PDFC[4]*(tmp113*(tmp121 - tmp122) - tmp133 - tmp134) + PDFC[4]*tmp129 + PDFC[4]*(tmp130 + tmp132) + tmp123*tmp135 - tmp126*tmp136;
	fDFC[4] = PDFC[4]*tmp116 - PDFC[4]*tmp120 + PDFC[4]*tmp128 + tmp135*(PDFC[9] - tmp111 + tmp114) + tmp136*(-PDFC[8] + tmp118);
	fDFC[5] = PDFC[1]*(PDFC[9]*tmp133 + PDFC[9]*tmp134 + tmp109*tmp141 + tmp112*tmp141 - tmp121*tmp142 + tmp122*tmp142) + PDFC[8]*tmp129 - thetatt*(-PDFC[2] - PDFC[3] + PDFC[8]*tmp119 + tmp138 + tmp139) + xitt*(PDFC[3] + pow(PDFC[8], 2)*tmp140 + PDFC[9]*tmp114*tmp140 - 2.0*tmp138 - tmp139);
    
};