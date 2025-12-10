#include <stdio.h>

// basic implementation of more maths functions necessary to compile everything
// !! not efficient or tested much, accuracy may be low on some functions !!
// these either map a float function onto a double implementation,
// or use an identity formula relying on other functions that are implemented.
// long doubles variants and some others are just stubs that return zero.

#define stublog(txt) fprintf(stderr, txt)
//#define stublog(txt) static int once=0; if (!once) fprintf(stderr, txt); once=1
                                        //; __builtin_trap() 
//#define stublog(txt)

double ceil(double v) {
    int i = (int)v;
    return (v > (double)i) ? (double)(i+1) : v;
}

double floor(double v) {
    int i = (int)v;
    return (double)i;
}

double copysign(double x, double y) {
    return y < 0 ? -x : x;
}

float fabs(float v) {
	return v < 0.0f ? -v : v;
}

long double nanl(const char *tagp) {
    stublog("STUB: nanl\n");
	return 0.0;
}

int nearbyintf(float x) {
	return nearbyint( (double)x );
}

float scalbnf(float x, int exp) {
    return (float)scalbn((double)x, exp);
}

long double scalbnl(long double x, int exp) {
    stublog("STUB: scalbnl\n");
    return 0.0;
}

float cosf(float x) {
	return cos( (double)x );
}

float sinf(float x) {
	return sin( (double)x );
}

float acosf(float x) {
	return acos( (double)x );
}

float asinf(float x) {
	return asin( (double)x );
}

double tan(double x) {
	return sin(x) / cos(x);
}

float tanf(float x) {
    return (float)tan( (double)x );
}

float tanhf(float x) {
    return (float)tanh( (double)x );
}

double atan(double x) {
	return asin( x / sqrtf(1.0 + x * x) );
}

float atanf(float x) {
    return (float)atan( (double)x );
}

double atan2(double y, double x) {
    double oo_hyp = 1.0 / sqrtf(x*x + y*y);
	return asin( y * oo_hyp ) - asin( x * oo_hyp);
}

float atan2f(float y, float x) {
    return (float)atan2( (double)y, (double)x );
}

double hypot(double x, double y) {
    return sqrtf(x*x + y*y);
}

double log(double x) {
    double v = log10(x) / log10(2.718281828459);
    //fprintf(stderr, "log(%f)=%f\n", x, v);
	return v;
}

double log2(double x) {
    double v = log10(x) / log10(2.0);
    //fprintf(stderr, "log2(%f)=%f\n", x, v);
	return v;
}

float log2f(float x)
{
    float v = (float)log2( (double)x );
    //fprintf(stderr, "log2f(%f)=%f\n", x, v);
	return v;
}

long double logbl(long double x) {
    stublog("STUB: logbl\n");
    return 0.0;
}

// https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
// Warning - this isn't very accurate: to 2 decimal places maybe, but better than nothing
// should be much more precise with large b
double fastPrecisePow(double a, double b) {
  // calculate approximation with fraction of the exponent
  int e = (int) b;
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;

  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }

  return r * u.d;
}

double pow(double x, double y) {
	return fastPrecisePow(x,y);
}

float powf(float x, float y) {
    return (float)pow((double)x, (double)y);
}

float expf(float x) {
    return (float)exp(x);
}

double frexp(double x) {
    stublog("STUB: frexp\n");
	return 0.0;
}

long double frexpl(long double x, int * expptr) {
    stublog("STUB: frexpl\n");
	return 0.0;
}

double ldexp(double x, int exp) {
    stublog("STUB: ldexp\n");
	return 0;
}

float ldexpf(float x, int exp) {
    stublog("STUB: ldexpf\n");
	return 0.0f;
}

int feclearexcept(int excepts) {
    stublog("STUB: feclearexcept\n");
    return 0;
}

int fetestexcept(int excepts) {
    stublog("STUB: fetestexcept\n");
    return 0;
}

typedef struct {
    unsigned long long      __fpsr;
    unsigned long long      __fpcr;
} fenv_t;

const fenv_t _FE_DFL_ENV = { 0 /* control */, 0 /* status */};

#include <sys/stat.h>

int mkfifoat(int fd, const char *path, mode_t mode) {
    stublog("STUB: mkfifoat\n");
    return 0;
}

int mknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev) {
    stublog("STUB: mknodat\n");
    return 0;
}

// ruby deps

double cbrt(double x) {
    stublog("STUB: cbrt\n");
    return 0.0;
}

double erfc( double arg ) {
    stublog("STUB: erfc\n");
    return 1.0 - arg;
}

double lgamma(double x) {
    stublog("STUB: lgamma\n");
    return 0.0;
}

double lgamma_r(double x, int *sign) {
    stublog("STUB: lgamma_r\n");
    *sign = 1.0;
    return 0.0;
}

double tgamma(double x) {
    stublog("STUB: tgamma\n");
    return 0.0;
}

double nextafter( double from, double to ) {
    stublog("STUB: nextafter\n");
    return 0.0;
}

// perl deps

int fegetround() {
    stublog("STUB: fegetround\n");
    return -1;
}

int fesetround( int round ) {
    stublog("STUB: fesetround\n");
    return -1;
}

double nexttoward( double from, long double to ) {
    stublog("STUB: nexttoward\n");
	return 0.0;
}

double remainder( double x, double y ) {
    stublog("STUB: remainder\n");
	return 0.0;
}

double remquo( double x, double y, int *quo ) {
    stublog("STUB: remquo\n");
	return 0.0;
}

double j0(double x) {
    stublog("STUB: j0\n");
	return 0.0;
}

double j1(double x) {
    stublog("STUB: j1\n");
	return 0.0;
}

double jn(int n, double x) {
    stublog("STUB: jn\n");
	return 0.0;
}

double y0(double x) {
    stublog("STUB: y0\n");
	return 0.0;
}

double y1(double x) {
    stublog("STUB: y1\n");
	return 0.0;
}

double yn(int n, double x) {
    stublog("STUB: yn\n");
	return 0.0;
}