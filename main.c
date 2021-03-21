#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double f1(double x); // 3/((x-1)^2+1)
double f2(double x); // sqrt(x + 0.5)
double f3(double x); // exp(-x)
double dvf1(double x); // -6(x-1)/((x-1)^2+1)^2
double dvf2(double x); // 1/(2*sqrt (x + 0.5))
double dvf3(double x); // -exp(-x)

double eps1 = 1e-4; //precision for root calculation
double eps2 = 1e-5; //precision for integral calculation

int opt[3] = {0, 0, 0};
//flags for options:
//[0] - ipx
//[1] - iter
//[2] - intg

//----------------------------------------------------------------------------------------------------------------------

//functions for testing

double tf1(double x) {
  return x*x - 12*x + 3;
}

double tf2(double x) {
  return 3*x*x - 2*x + 15;
}

double tf3(double x) {
  return 2*x - 12;
}

double tf4(double x) {
  return 6*x - 2;
}

double tf5(double x) {
  return 3*x*x - 2*x - 7;
}

double tf6(double x) {
  return 2*x*x + 6*x - 19;
}

double tf7(double x) {
  return 6*x - 2;
}

double tf8(double x) {
  return 4*x + 6;
}

double tf9(double x) {
  return -2*x*x + x + 31;
}

double tf10(double x) {
  return -x*x + x + 6;
}

double tf11(double x) {
  return -4*x + 1;
}

double tf12(double x) {
  return -2*x + 1;
}

double tf13(double x) {
  return 1.25*x*x*x*x - 3;
}

double tf14(double x) {
  return x*x + x;
}

double tf15(double x) {
  return 2/(x*x*x);
}

//----------------------------------------------------------------------------------------------------------------------

//root is calculated with tangent method
double
root(double (*f)(double),
     double (*g)(double),
     double (*df)(double),
     double (*dg)(double),
     double a,
     double b,
     double eps)
{
  unsigned iter_count = 0;
  double c;
  int cond1 = (f(a) - g(a) < 0); //F is increasing
  double midF = f(a/2 + b/2) - g(a/2 + b/2);
  double averF = (f(a) - g(a) + f(b) - g(b)) / 2;
  int cond2 = (midF > averF); //F is higher than chord

  if (cond1 ^ cond2){ //1 case
    double d = b, appr_sign;
    do {
      c = d - (f(d) - g(d))/(df(d) - dg(d));
      d = c;
      ++iter_count;
      // 0 if same sign
      // 1 if different sign
      appr_sign = ((f(c - eps) - g(c - eps)) * (f(c) - g(c)) < 0);
    } while (!appr_sign);
  }
  else{ //2 case
    double d = a, appr_sign;
    do {
      c = d - (f(d) - g(d))/(df(d) - dg(d));
      d = c;
      ++iter_count;
      // 0 if same sign
      // 1 if different sign
      appr_sign = ((f(c + eps) - g(c + eps)) * (f(c) - g(c)) < 0);
    } while (!appr_sign);
  }
  if (opt[1]) {
    printf("Number of iterations - %u\n\n", iter_count);
  }
  if (opt[0]) {
    printf("Intersection point`s abscissa: %0.5lf\n\n", c);
  }
  return c;
}

//integral is calculated with Simpson`s formula
double
integral(double (*f)(double), double a, double b, double eps)
{
  int n = 20;
  double h = (b - a) / n;
  double In = f(a) + f(b); //F0 + Fn
  double F2 = 0, F4 = 0;

  for (int i = 1; i < n; ++i){
    if (i % 2 == 0) {
      F2 += f(a + i * h);
    }
    else {
      F4 += f(a + i * h);
    }
    //if i is odd than 4*Fi is added
    //else 2*Fi is added
  }

  In = h / 3 * (In + 2*F2 + 4*F4);
  double I2n = In;

  //Runge`s rule:
  //Increasing n and In for approximation of the integral
  do {
    In = I2n;
    I2n = f(a) + f(b) + 2 * (F2 + F4);
    F2 = In - f(a) - f(b);
    F4 = 0;
    for (int i = 1; i < 2 * n; i += 2){
      I2n += 4 * f(a + i * h / 2);
      F4 += f(a + i * h / 2);
    }
    n *= 2;
    h /= 2;
    I2n *= h / 3;
  } while (fabs(In - I2n) >= 15 * eps);

  if (opt[2]) {
    printf("Integral: %lf\n\n", I2n);
  }
  return I2n;
}

//----------------------------------------------------------------------------------------------------------------------

void
test(int argc, char const *argv[], int i) //option -test
{
  double (*func[15])(double) = {tf1, tf2, tf3, tf4, tf5,
                                tf6, tf7, tf8, tf9, tf10,
                                tf11, tf12, tf13, tf14, tf15};

  //checks if parameter r/i is entered
  if(++i == argc) {
    printf("Wrong format: parameter is needed\n");
    return;
  }
  // -test r
  if(strcmp(argv[i], "r") == 0) {
    if(argc - (i + 1) != 3) {
      printf("Wrong format: 3 arguments are expected\n");
      return;
    }
    ++i;
    switch (atoi(argv[i])) { //number of set
      case 1:
        root(func[0], func[1], func[2], func[3], atoi(argv[i + 1]), atoi(argv[i + 2]), eps1);
        break;
      case 2:
        root(func[4], func[5], func[6], func[7], atoi(argv[i + 1]), atoi(argv[i + 2]), eps1);
        break;
      case 3:
        root(func[8], func[9], func[10], func[11], atoi(argv[i + 1]), atoi(argv[i + 2]), eps1);
        break;
      default:
	break;
    }
  }
    //-test i
  else if(strcmp(argv[i], "i") == 0) {
    if(argc - (i + 1) != 3) {
      printf("Wrong format: 3 arguments are expected\n");
      return;
    }
    ++i;
    integral(func[atoi(argv[i]) + 11], atoi(argv[i + 1]), atoi(argv[i + 2]), eps2);
  }
  else {
    printf("Wrong parameter\n");
  }
}

void
help() //option -help
{
  char s[150];
  FILE *f = fopen("help.txt", "r");
  while(fgets(s, 150, f)) {
    printf("%s", s);
  }
  fclose(f);
}

void
funclist() //option -fl
{
  char s[100];
  FILE *f = fopen("funclist.txt", "r");
  while(fgets(s, 100, f)) {
    printf("%s", s);
  }
  fclose(f);
}

//----------------------------------------------------------------------------------------------------------------------

void
area()
{
  double p13 = root(f1, f3, dvf1, dvf3, -0.25, 0, eps1);
  double p23 = root(f2, f3, dvf2, dvf3, 0, 1, eps1);
  double p12 = root(f1, f2, dvf1, dvf2, 1.75, 3, eps1);
  //p13 < p23 < p12
  double area = integral(f1, p13, p12, eps2) - integral(f2, p23, p12, eps2) - integral(f3, p13, p23, eps2);
  printf("AREA = %0.3f\n", area);
}

int
main(int argc, char const *argv[]) {
  if (argc >= 2 && strcmp(argv[1], "-help") == 0) {
    help();
    return 0;
  }
  if (argc >= 2 && strcmp(argv[1], "-fl") == 0) {
    funclist();
    return 0;
  }
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-iter") == 0) {
      opt[1] = 1;
    }
    else if (strcmp(argv[i], "-ipx") == 0) {
      opt[0] = 1;
    }
    else if (strcmp(argv[i], "-intg") == 0) {
      opt[2] = 1;
    }
    else if (strcmp(argv[i], "-test") == 0) {
      opt[0] = 1;
      opt[1] = 1;
      opt[2] = 1;
      test(argc, argv, i);
      return 0;
    }
    else {
      printf("Wrong option\n");
      help();
      return 0;
    }
  }
  area();
  return 0;
}
