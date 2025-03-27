#ifndef SRC_LIBS_INCLUDE_ASSERT_H
#define SRC_LIBS_INCLUDE_ASSERT_H

#define ENABLED_ASSERTIONS 1

#if ENABLED_ASSERTIONS
    #define ASSERT(ok, s) (assert(ok, __FILE__, __LINE__, s)); 
#else
    #define ASSERT(ok, s) (void)(0)
#endif

#define WARN(ok, s) (warn(ok, __FILE__, __LINE__, s));

void assert(int ok, const char* file, const unsigned int linenumber, const char * s);
void warn(int ok, const char* file, const unsigned int linenumber, const char* s);

#endif
