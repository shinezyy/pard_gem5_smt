#ifndef __ZYY_LOG__
#define __ZYY_LOG__


#define log(format,...) \
    fprintf(stdout, "\33[1;34m%s,%s,%d: " format "\33[0m\n", \
            __FILE__, __func__, __LINE__, ## __VA_ARGS__), \
            fflush(stdout) \

#define log_var(x) \
    fprintf(stdout, "\33[1;34m%s,%s,%d: " "%s is %d" "\33[0m\n", \
            __FILE__, __func__, __LINE__, #x, x), \
            fflush(stdout) \


#endif
