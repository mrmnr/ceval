#ifndef CEVAL_FUNCTIONS
#define CEVAL_FUNCTIONS

#include "./tokens.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>

//constant definitions
#ifdef M_PI
#define CEVAL_PI M_PI
#else
#define CEVAL_PI 3.14159265358979323846
#endif
#ifdef M_E
#define CEVAL_E M_E
#else
#define CEVAL_E 2.71828182845904523536
#endif

#ifndef CEVAL_EPSILON
#define CEVAL_EPSILON 1e-2
#endif
#ifndef CEVAL_DELTA
#define CEVAL_DELTA 1e-6
#endif
#ifndef CEVAL_MAX_DIGITS
#define CEVAL_MAX_DIGITS 15
#endif
//these can be defined by the user before the include directive depending the desired level of precision

//helper function prototypes
void ceval_error(const char * , ...);
double ceval_gcd_binary(int, int);
char * ceval_shrink(char * );

//single argument funtion prototypes
double ceval_signum(double);
double ceval_asin(double);
double ceval_acos(double);
double ceval_atan(double);
double ceval_sin(double);
double ceval_cos(double);
double ceval_tan(double);
double ceval_sinh(double);
double ceval_cosh(double);
double ceval_tanh(double);
double ceval_rad2deg(double);
double ceval_deg2rad(double);
double ceval_int_part(double);
double ceval_frac_part(double);
double ceval_log10(double);
double ceval_ln(double);
double ceval_exp(double);
double ceval_factorial(double);
double ceval_positive_sign(double);
double ceval_negative_sign(double);
double ceval_abs(double);
double ceval_sqrt(double);
double ceval_sqrt(double);
double ceval_cbrt(double);
double ceval_ceil(double);
double ceval_floor(double);
double ceval_not(double);
double ceval_bit_not(double);

//double argument function prototypes
double ceval_sum(double, double, int);
double ceval_diff(double, double, int);
double ceval_prod(double, double, int);
double ceval_div(double, double, int);
double ceval_quotient(double, double, int);
double ceval_modulus(double, double, int);
double ceval_gcd(double, double, int);
double ceval_hcf(double, double, int);
double ceval_lcm(double, double, int);
double ceval_log(double, double, int);
double ceval_are_equal(double, double, int);
double ceval_not_equal(double, double, int);
double ceval_lesser(double, double, int);
double ceval_greater(double, double, int);
double ceval_lesser_s(double, double, int);
double ceval_greater_s(double, double, int);
double ceval_comma(double, double, int);
double ceval_power(double, double, int);
double ceval_atan2(double, double, int);
double ceval_and(double, double, int);
double ceval_or(double, double, int);
double ceval_bit_and(double, double, int);
double ceval_bit_xor(double, double, int);
double ceval_bit_or(double, double, int);
double ceval_bit_lshift(double, double, int);
double ceval_bit_rshift(double, double, int);

//helper function definitions
void ceval_error(const char* error_format_string, ...) {
    #ifndef CEVAL_STOICAL
        // start whining
        printf("\n[ceval]: ");
        va_list args;
        va_start(args, error_format_string);
        vprintf(error_format_string, args);
        va_end(args);
        printf("\n");
    #endif
}
double ceval_gcd_binary(int a, int b) {
    if (a == 0 && b == 0)
        return 0;
    while (b) {
        a %= b;
        b ^= a;
        a ^= b;
        b ^= a;
    }
    return a;
}
char * ceval_shrink(char * x) {
    char * y = x;
    unsigned int len = 0;
    for (unsigned int i = 0; i < strlen(x); i++) {
        if(x[i] == ' ' || x[i] == '\n' || x[i] == '\t' || x[i] == '\r') {
            continue;
        } else {
            if(x[i]=='(' && x[i+1]==')') {
                // empty pairs of parantheses are ignored
                // simlar to c lang where {} are ignored as empty blocks of code
                i++;
                continue;
            }
            *(y + len) = (char)tolower(x[i]);
            len++;
        }
    }
    y[len] = '\0';
    return y;
}
//single argument function definitions
static double( * single_arg_fun[])(double) = {
    // double_arg_fun (first three tokens are whitespace and parantheses)
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, 
    NULL, NULL, NULL, NULL, 
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, 
    NULL, NULL,
    // single_arg_fun
    ceval_abs, ceval_exp, ceval_sqrt, ceval_cbrt,
    ceval_ln, ceval_log10, ceval_ceil, ceval_floor,
    ceval_signum, ceval_factorial, ceval_int_part, ceval_frac_part,
    ceval_deg2rad, ceval_rad2deg, ceval_sin, ceval_cos,
    ceval_tan, ceval_asin, ceval_acos, ceval_atan,
    ceval_sinh, ceval_cosh, ceval_tanh, ceval_not,
    ceval_bit_not, ceval_positive_sign, ceval_negative_sign,
    // number and constant tokens
    NULL, NULL, NULL
};
double ceval_signum(double x) {
    return (x == 0) ? 0 :
        (x > 0) ? 1 :
        -1;
}
double ceval_asin(double x) {
    if (x > 1 || x < -1) {
        ceval_error("Numerical argument out of domain");
        return (double)NAN;
    }
    return asin(x);
}
double ceval_acos(double x) {
    if (x > 1 || x < -1) {
        ceval_error("Numerical argument out of domain");
        return (double)NAN;
    }
    return acos(x);
}
double ceval_atan(double x) {
    return atan(x);
}
double ceval_sin(double x) {
    double sin_val = sin(x);
    //sin(pi) == 0.000000, but sin(pi-CEVAL_EPSILON) == -0.00000* and sin(pi+CEVAL_EPSILON) == +0.00000*
    //since the precision of pi (approx) is limited, it often leads to -0.0000 printed out as a result
    //thus, we assumse 0.0000 value for all |sin(x)|<=CEVAL_EPSILON
    return (fabs(sin_val) <= CEVAL_EPSILON) ? 0 : sin_val;
}
double ceval_cos(double x) {
    double cos_val = cos(x);
    return (fabs(cos_val) <= CEVAL_EPSILON) ? 0 : cos_val;
}
double ceval_tan(double x) {
    double tan_val = tan(x);
    if (fabs(ceval_modulus(x - CEVAL_PI / 2, CEVAL_PI, 0)) <= CEVAL_DELTA) {
        ceval_error("tan() is not defined for odd-integral multiples of pi/2");
        return (double)NAN;
    }
    return (fabs(tan_val) <= CEVAL_EPSILON) ? 0 : tan_val;
}
double ceval_rad2deg(double x) {
    return x / CEVAL_PI * 180;
}
double ceval_deg2rad(double x) {
    return x / 180 * CEVAL_PI;
}
double ceval_int_part(double x) {
    double x_i;
    modf(x, & x_i);
    return x_i;
}
double ceval_frac_part(double x) {
    double x_i, x_f;
    x_f = modf(x, & x_i);
    return x_f;
}
double ceval_log10(double x) {
    return ceval_log(10, x, 0);
}
double ceval_ln(double x) {
    return ceval_log(CEVAL_E, x, 0);
}
double ceval_exp(double x) {
    return ceval_power(CEVAL_E, x, 0);
}
double ceval_factorial(double x) {
    if (x < 0) {
        ceval_error("Numerical argument out of domain");
        return (double)NAN;
    }
    return tgamma(x + 1);
}
double ceval_positive_sign(double x) {
    return x;
}
double ceval_negative_sign(double x) {
    return -x;
}
double ceval_abs(double x) {
    return fabs(x);
}
double ceval_sqrt(double x) {
    if (x >= 0) return sqrt(x);
    ceval_error("sqrt(): can't operate on negative numbers");
    return (double)NAN;
}
double ceval_cbrt(double x) {
    return cbrt(x);
}
double ceval_ceil(double x) {
    return ceil(x);
}
double ceval_floor(double x) {
    return floor(x);
}
double ceval_sinh(double x) {
    return sinh(x);
}
double ceval_cosh(double x) {
    return cosh(x);
}
double ceval_tanh(double x) {
    return tanh(x);
}
double ceval_not(double x) {
    return ceval_are_equal(x, 0, 0);
}
double ceval_bit_not(double x) {
    if(ceval_frac_part(x) == 0) {
        return ~(int)x;
    } else {
        ceval_error("bit_not(): operand must be of integral type");
        return (double)NAN;
    }
}
//double argument function definitions
static double( * double_arg_fun[])(double, double, int) = {
    // double_arg_fun (first three tokens are whitespace and parantheses)
    NULL, NULL, NULL, ceval_comma,
    ceval_or, ceval_and, ceval_bit_or, ceval_bit_xor,
    ceval_bit_and, ceval_are_equal, ceval_not_equal, ceval_lesser,
    ceval_greater, ceval_lesser_s, ceval_greater_s, ceval_bit_lshift,
    ceval_bit_rshift, ceval_sum, ceval_diff, ceval_prod,
    ceval_div, ceval_modulus, ceval_quotient, ceval_power, 
    ceval_gcd, ceval_hcf, ceval_lcm, ceval_log,
    ceval_atan2, ceval_power,
    // single_arg_fun
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,
    // number and constant tokens
    NULL, NULL, NULL
};
double ceval_sum(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("sum(): function takes two arguments");
        return (double)NAN;
    }
    return a + b;
}
double ceval_diff(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("diff(): function takes two arguments");
        return (double)NAN;
    }
    return a - b;
}
double ceval_prod(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("prod(): function takes two arguments");
        return (double)NAN;
    }
    return a * b;
}
double ceval_div(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("div(): function takes two arguments");
        return (double)NAN;
    }
    if (b == 0 && a == 0) {
        ceval_error("0/0 is indeterminate...");
        ceval_error("Continuing evaluation with the assumption 0/0 = 1");
        return 1;
    } else if (b == 0) {
        ceval_error("Division by 0 is not defined...");
        ceval_error("Continuing evaluation with the assumption 1/0 = inf");
        return a * (double)INFINITY;
    }
    return a / b;
}
double ceval_modulus(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("modulo(): function takes two arguments");
        return (double)NAN;
    }
    if (b == 0) {
        ceval_error("Division by 0 is not defined...");
        ceval_error("Continuing evaluation with the assumption 1%0 = 0");
        return 0;
    }
    return fmod(a, b);
}
double ceval_quotient(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("quotient(): function takes two arguments");
        return (double)NAN;
    }
    //a = b*q + r
    //q = (a - r)/b
    if (b == 0 && a == 0) {
        ceval_error("0/0 is indeterminate...");
        ceval_error("Continuing evaluation with the assumption 0/0 = 1");
        return 1;

    } else if (b == 0) {
        ceval_error("Division by 0 is not defined...");
        ceval_error("Continuing evaluation with the assumption 1/0 = inf");
        return a * (double)INFINITY;
    }
    return (a - ceval_modulus(a, b, 0)) / b;
}
double ceval_gcd(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("gcd(): function takes two arguments");
        return (double)NAN;
    }
    double a_f = ceval_frac_part(a),
        b_f = ceval_frac_part(b);
    int a_i = (int)ceval_int_part(a),
        b_i = (int)ceval_int_part(b);
    if (a_f == 0 && b_f == 0) {
        return (double) ceval_gcd_binary(a_i, b_i);
    } else {
        ceval_error("gcd() takes only integral parameters");
        return (double)NAN;
    }
}
double ceval_hcf(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("hcf(): function takes two arguments");
        return (double)NAN;
    }
    return ceval_gcd(a, b, 0);
}
double ceval_lcm(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("lcm(): function takes two arguments");
        return (double)NAN;
    }
    return a * b / ceval_gcd(a, b, 0);
}
double ceval_log(double b, double x, int arg_check) {
    if (arg_check) {
        ceval_error("log(): function takes two arguments");
        return (double)NAN;
    }
    if (b == 0) {
        if (x == 0) {
            ceval_error("log(0,0) is indeterminate");
            return (double)NAN;
        } else {
            return 0;
        }
    }
    return log(x) / log(b);
}
double ceval_are_equal(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("==: function takes two arguments");
        return (double)NAN;
    }
    if (fabs(a - b) <= CEVAL_EPSILON) {
        return 1;
    } else {
        return 0;
    }
}
double ceval_not_equal(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("!=: function takes two arguments");
        return (double)NAN;
    }
    return (double)!(int)ceval_are_equal(a, b, 0);
}
double ceval_lesser(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("<=: function takes two arguments");
        return (double)NAN;
    }
    return (double)!(int)ceval_greater_s(a, b, 0);
}
double ceval_greater(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error(">=: function takes two arguments");
        return (double)NAN;
    }
    return (double)!(int)ceval_lesser_s(a, b, 0);
}
double ceval_lesser_s(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error("<: function takes two arguments");
        return (double)NAN;
    }
    return (b - a) >= CEVAL_EPSILON;
}
double ceval_greater_s(double a, double b, int arg_check) {
    if (arg_check) {
        ceval_error(">: function takes two arguments");
        return (double)NAN;
    }
    return (a - b) >= CEVAL_EPSILON;
}
double ceval_comma(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error(",: function takes two arguments");
        return (double)NAN;
    }
    return y;
}
double ceval_power(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("pow(): function takes two arguments");
        return (double)NAN;
    }
    if(x<0 && ceval_frac_part(y)!=0) {
        ceval_error("pow(): negative numbers can only be raised to integral powers");
        return (double)NAN;
    }
    return pow(x, y);
}
double ceval_atan2(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("atan2(): function takes two arguments");
        return (double)NAN;
    }
    return atan2(x, y);
}
double ceval_and(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("and(): function takes two arguments");
        return (double)NAN;
    }
    return (double) ((int)x && (int)y);
}
double ceval_or(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("or(): function takes two arguments");
        return (double)NAN;
    }
    return (double) ((int)x || (int)y);
}
double ceval_bit_and(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("bit_and(): function takes two arguments");
        return (double)NAN;
    }
    if(ceval_frac_part(x) == 0 && ceval_frac_part(y) == 0) {
        return (int)x & (int)y;
    } else {
        ceval_error("bit_and(): operands must be of integral type");
        return (double)NAN;
    }
}
double ceval_bit_xor(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("bit_xor(): function takes two arguments");
        return (double)NAN;
    }
    if(ceval_frac_part(x) == 0 && ceval_frac_part(y) == 0) {
        return (int)x ^ (int)y;
    } else {
        ceval_error("bit_xor(): operands must be of integral type");
        return (double)NAN;
    }
}
double ceval_bit_or(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("bit_or(): function takes two arguments");
        return (double)NAN;
    }
    if(ceval_frac_part(x) == 0 && ceval_frac_part(y) == 0) {
        return (int)x | (int)y;
    } else {
        ceval_error("bit_or(): operands must be of integral type");
        return (double)NAN;
    }
}
double ceval_bit_lshift(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("bit_lshift(): function takes two arguments");
        return (double)NAN;
    }
    if(ceval_frac_part(x) == 0 && ceval_frac_part(y) == 0) {
        return (int)x << (int)y;
    } else {
        ceval_error("bit_lshift(): operands must be of integral type");
        return (double)NAN;
    }

}
double ceval_bit_rshift(double x, double y, int arg_check) {
    if (arg_check) {
        ceval_error("bit_rshift(): function takes two arguments");
        return (double)NAN;
    }
    if(ceval_frac_part(x) == 0 && ceval_frac_part(y) == 0) {
        return (int)x >> (int)y;
    } else {
        ceval_error("bit_rshift(): operands must be of integral type");
        return (double)NAN;
    }
}
#endif
