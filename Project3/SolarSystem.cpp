#include "NBS.h"
#include "utils.h"
#include <cmath>
#include <iostream>


void SolarSystem::acceleration(int i, int j, double *ax, double *ay, double *az) {
	double denum;
	double x = pos_x[j][i];
	double y = pos_y[j][i];
	double z = pos_z[j][i];
	double this_r = sqrt(x*x + y * y + z * z);
	denum = G * m_centerMass / pow(this_r, m_beta);
	*ax = -x * denum;
	*ay = -y * denum;
	*az = -z * denum;
	for (int k = 0; k < m_m; k++) {
		if (j == k) {
			continue;
		}
		denum = G * massObjects[k].mass / (pow(r[j][k], m_beta));
		*ax -= (x - pos_x[k][i])*denum;
		*ay -= (y - pos_y[k][i])*denum;
		*az -= (z - pos_z[k][i])*denum;
	}
}

void SolarSystem::setCenterMass(double centerMass) {
	m_centerMass = centerMass;
}

void SolarSystem::setBeta(double beta) {
	m_beta = beta + 1;
}

void SolarSystemRelativistic::perihelionPrecession(int index, double finalTime, int n, int years) {
	double *per;
	per = verletSolveRel2D(index, finalTime, n, years);
	double r = sqrt(pow(per[0], 2) + pow(per[1], 2));
	
	std::cout << "Position of perihelion is: (" << per[0] << "," << per[1] << ")\n";
	std::cout << "Distance from sun: " << r << "\n";
	std::cout << "Argument of perihelion is: " << atan2(per[1], per[0])*206264.806 << " arcseconds/century.\n";
	delete[] per;
}


SolarSystemRelativistic::SolarSystemRelativistic(MassObject* initValue, int m)
	: SolarSystem(initValue, m) {
	l = createVector(m, 0);
	double lx, ly, lz;
	MassObject planet;
	for (int i = 0; i < m; i++) {
		planet = massObjects[i];
		lx = planet.y*planet.vz - planet.z*planet.vy;
		ly = planet.x*planet.vz - planet.z*planet.vx;
		lz = planet.x*planet.vy - planet.y*planet.vx;
		l[i] = sqrt(lx*lx + ly * ly + lz * lz);
	}
}

void SolarSystemRelativistic::acceleration(int i, int j, double *ax, double *ay, double *az) {
	double  denum, rel;
	double threell_cc = 3 * l[j] * l[j] / cc;
	double x = pos_x[j][i];
	double y = pos_y[j][i];
	double z = pos_z[j][i];
	double this_r = sqrt(x*x + y * y + z * z);
	rel = threell_cc / (this_r*this_r);
	denum = G * m_centerMass / pow(this_r, m_beta);
	*ax = -x * denum*(1 + rel);
	*ay = -y * denum*(1 + rel);
	*az = -z * denum*(1 + rel);
	for (int k = 0; k < m_m; k++) {
		if (j == k) {
			continue;
		}
		this_r = r[j][k];
		denum = G * massObjects[k].mass / (pow(this_r, m_beta));
		rel = threell_cc / (this_r*this_r);
		*ax -= (x - pos_x[k][i])*denum*(1 + rel);
		*ay -= (y - pos_y[k][i])*denum*(1 + rel);
		*az -= (z - pos_z[k][i])*denum*(1 + rel);
	}
}

double *SolarSystemRelativistic::verletSolveRel2D(int index, double finalTime, int n, int years) {
	double x0, xn, y0, yn, vx0, vy0, vxn, vyn, a, an;
	double r, rn, rmin;
	double *perihelion;
	double ll = l[index]*l[index];
	perihelion = createVector(0, 2);

	x0 = massObjects[index].x;
	y0 = massObjects[index].y;
	vx0 = massObjects[index].vx;
	vy0 = massObjects[index].vy;
	distance(0);

	double h = finalTime / (n + 1);
	double hh = h * h;
	int count = 0;
	r = sqrt(pow(x0, 2) + pow(y0, 2));
	a = -fourPiPi * (1.0 + 3.0*ll / (r*r*cc)) / pow(r, 3);

	for (int i = 0; i < n - n / 100; i++) {
		xn = x0 + h*vx0 + (hh / 2.0)*a*x0;
		yn = y0 + h*vy0 + (hh / 2.0)*a*y0;

		rn = sqrt(xn*xn + yn*yn);
		an = -fourPiPi*(1.0 + 3.0*ll/(rn*rn*cc))/pow(rn, 3);

		vxn = vx0 + (h / 2.0)*(an*xn + a*x0);
		vyn = vy0 + (h / 2.0)*(an*yn + a*y0);

		r = rn;
		a = an;
		x0 = xn;
		y0 = yn;
		vx0 = vxn;
		vy0 = vyn;
	}

	rmin = sqrt(pow(x0, 2) + pow(y0, 2));
	perihelion[0] = x0;
	perihelion[1] = y0;

	for (int i = 0; i < n / 100; i++) {
		xn = x0 + h * vx0 + (hh / 2.0)*a*x0;
		yn = y0 + h * vy0 + (hh / 2.0)*a*y0;

		r = sqrt(pow(xn, 2) + pow(yn, 2));
		if (r < rmin) {
			rmin = r;
			perihelion[0] = xn;
			perihelion[1] = yn;
		}

		rn = sqrt(xn*xn + yn*yn);
		an = -fourPiPi * (1.0 + 3.0*ll/(rn*rn*cc))/pow(rn, 3);

		vxn = vx0 + (h / 2.0)*(an*xn + a * x0);
		vyn = vy0 + (h / 2.0)*(an*yn + a * y0);

		r = rn;
		a = an;
		x0 = xn;
		y0 = yn;
		vx0 = vxn;
		vy0 = vyn;
	}
	return perihelion;
}
