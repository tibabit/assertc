#ifndef SPEC_REPORTER_H_
#define SPEC_REPORTER_H_

#include "runner.h"

test_reporter_t *   spec_reporter_init();
void                spec_reporter_destroy(test_reporter_t *);

#endif  // SPEC_REPORTER_H_
