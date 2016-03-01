#ifndef SPEC_REPORTER_H_
#define SPEC_REPORTER_H_

#include "runner.h"

string_t            get_level_spaces(int level);
string_t            get_message_template(boolean_t output, string_t format);
test_reporter_t *   spec_reporter_init();

#endif  // SPEC_REPORTER_H_
