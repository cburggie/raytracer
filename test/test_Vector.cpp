
#include <raytracer.h>

#include <iostream>

#include <cmath>
#include <cassert>

#define DBL_CMP(X,Y) (std::abs((X) - (Y)) < 0.000001)

using namespace raytracer;


static void test_random();
static void test_randunit();
static void test_dot();
static void test_cross();
static void test_copy();
static void test_power();
static void test_normalize();
static void test_scale();
static void test_add();
static void test_subtract();
static void test_translate();
static void test_project();
static void test_unproject();
static void test_reflect();
static void test_refract();

int main() {

	RNG::init();

	test_random();
	test_randunit();
	test_dot();
	test_cross();
	test_copy();
	test_power();
	test_normalize();
	test_scale();
	test_add();
	test_subtract();
	test_translate();
	test_project();
	test_unproject();
	test_reflect();
	test_refract();

	std::cout << "raytracer::Vector passed all tests\n";

	return 0;
}



void test_refract() {
	RNG *rng = RNG::get();
	for (int i = 0; i < 10000; i++) {
		double index = 1.0 + rng->next();
		Vector n = Vector::randunit();
		Vector v = Vector::randunit();
		Vector r = v; r.refract(n,index);

		assert(r.dot(v) < 1.0); //r changed
		assert(DBL_CMP(r.dot(),1.0)); //still length 1.0

		//if we're shallower than critical angle
		if (index * std::sqrt(v.cross(n).dot()) > 1.0) {
			//check for reflection
			assert(r.dot(n) < 0.0);
		}

		else {
			//otherwise, make sure we're going the same direction
			assert(v.dot(n) * r.dot(n) > 0.0);
		}
	}
}



void test_reflect() {
	for (int i = 0; i < 10000; i++) {
		Vector n = Vector::randunit(),
			   v = Vector::randunit(),
			   r = v,
			   pv, pr, s;

		r.reflect(n);
		pv = v; pv.project(n);
		pr = r; pr.project(n);
		s = r; s.add(v);

		//make sure r has same length as v
		assert(DBL_CMP(r.dot(), v.dot()));

		//make sure r is on opposite side of n
		assert(DBL_CMP(pv.dot(), pr.dot()));

		//make sure v and r sum to a vector perpendicular to n
		assert(DBL_CMP(s.dot(n), 0.0));

		// make sure pr == -pv
		pv.add(pr);
		assert(DBL_CMP(pv.dot(), 0.0));
	}
}



void test_unproject() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w = Vector::randunit(),
			   u = v;
		u.unproject(w);
		assert(u.dot() < v.dot());
		assert(DBL_CMP(u.dot(w),0.0));
	}
}



void test_project() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w = Vector::randunit(),
			   p = v;
		p.project(w);
		assert(p.dot() < v.dot());
		assert(DBL_CMP(p.cross(w).dot(),0.0));
	}
}



void test_translate() {
	RNG *rng = RNG::get();
	for (int i = 0; i < 10000; i++) {
		Vector a = Vector::random(1.0),
			   b = Vector::random(1.0),
			   c = a, d = b;
		double s = rng->next();
		c.translate(b,s);
		d.scale(s);
		d.add(a);

		assert(DBL_CMP(c.dot(),d.dot()));
		assert(DBL_CMP(c.cross(d).dot(),0.0));
	}
}



void test_subtract() {
	for (int i = 0; i < 10000; i++){
		Vector u = Vector::random(1.0),
			   v = Vector::random(1.0),
			   w = u;
		w.subtract(v);
		assert(DBL_CMP(u.x-v.x,w.x));
		assert(DBL_CMP(u.y-v.y,w.y));
		assert(DBL_CMP(u.z-v.z,w.z));

		w.copy(u); w.subtract(u);
		assert(DBL_CMP(w.dot(),0.0));
	}
}



void test_add() {
	for (int i = 0; i < 10000; i++) {
		Vector u = Vector::random(1.0),
			   v = Vector::random(1.0),
			   w = u;
		w.add(v);
		assert(DBL_CMP(u.x+v.x,w.x));
		assert(DBL_CMP(u.y+v.y,w.y));
		assert(DBL_CMP(u.z+v.z,w.z));

		w.copy(u); w.add(u);
		assert(DBL_CMP(w.dot(), 4.0*u.dot()));
	}
}



void test_scale() {
	RNG *rng = RNG::get();

	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w = v;

		double s = rng->next();
		w.scale(s);

		assert(DBL_CMP(v.dot(w),s*v.dot()));
		assert(DBL_CMP(w.dot(w),s*s*v.dot()));
		assert(DBL_CMP(v.cross(w).dot(),0.0));
	}
}



void test_normalize() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w = v;
		w.normalize();
		assert(DBL_CMP(w.dot(),1.0));
		assert(DBL_CMP(v.cross(w).dot(),0.0));
	}
}



void test_power() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w = v;
		v.power(2.0);
		assert(DBL_CMP(v.x+v.y+v.z, w.dot()));
	}
}



void test_copy() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w;
		
		w.copy(v);
		assert(DBL_CMP(v.dot(),w.dot()));
		assert(DBL_CMP(v.dot(w),v.dot()));

		w.subtract(v);
		assert(DBL_CMP(w.dot(),0.0));

		w.copy(v.x,v.y,v.z);
		assert(DBL_CMP(v.dot(),w.dot()));
		assert(DBL_CMP(v.dot(w),v.dot()));

		w.subtract(v);
		assert(DBL_CMP(w.dot(),0.0));
	}
}



void test_cross() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0),
			   w = Vector::random(1.0),
			   a = v.cross(w),
			   b = Vector::cross(v,w);

		assert(a.dot() < 1.0);
		assert(b.dot() < 1.0);
		assert(DBL_CMP(a.dot(),b.dot()));
		assert(DBL_CMP(a.dot(v),0.0));
		assert(DBL_CMP(a.dot(w),0.0));
		assert(DBL_CMP(b.dot(v),0.0));
		assert(DBL_CMP(b.dot(w),0.0));
		
		a.add(w.cross(v));
		assert(DBL_CMP(a.dot(),0.0));

		b.add(Vector::cross(w,v));
		assert(DBL_CMP(b.dot(),0.0));
	}
}



void test_dot() {
	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0);
		Vector w = Vector::random(1.0);

		double vl = std::sqrt(v.dot()),
			   wl = std::sqrt(w.dot());

		assert(v.dot(w) < vl*wl);
		assert(w.dot(v) < vl*wl);
		assert(v.dot() < 1.0);
		assert(w.dot() < 1.0);
		assert(v.dot(v) < 1.0);
		assert(w.dot(w) < 1.0);
		assert(Vector::dot(v,v));
		assert(Vector::dot(w,w));

		assert(DBL_CMP(v.dot(w),w.dot(v)));
		assert(DBL_CMP(v.dot(w),Vector::dot(v,w)));
		assert(DBL_CMP(w.dot(v),Vector::dot(v,w)));
		assert(DBL_CMP(v.dot(w),Vector::dot(w,v)));
	}
}



void test_random() {


	int x = 0, y = 0, z = 0;

	for (int i = 0; i < 10000; i++) {
		Vector v = Vector::random(1.0);
		assert(v.dot() < 1.0);
		if (v.x < 0.0)
			x--;
		else
			x++;
		if (v.y < 0.0)
			y--;
		else
			y++;
		if (v.z < 0.0)
			z--;
		else
			z++;
	}

	assert(std::abs(x) < 1000);
	assert(std::abs(y) < 1000);
	assert(std::abs(z) < 1000);
}



void test_randunit() {
	int x = 0, y = 0, z = 0;

	for (int i = 0; i < 10000; i++) {
		Vector n = Vector::randunit();
		assert(DBL_CMP(n.dot(),1.0));
		if (n.x < 0.0)
			x--;
		else
			x++;
		if (n.y < 0.0)
			y--;
		else
			y++;
		if (n.z < 0.0)
			z--;
		else
			z++;
	}

	assert(std::abs(x) < 1000);
	assert(std::abs(y) < 1000);
	assert(std::abs(z) < 1000);
}



