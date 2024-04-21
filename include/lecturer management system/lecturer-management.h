#ifndef LECTURER_MANGEMENT_H
#define LECTURER_MANGEMENT_H
#include <stdbool.h>

bool does_lecturer_exist(char *lecturer_username);
bool assign_lecturer_to_course(int course_id, char *lecturer_username);

#endif // COURSE_MANGEMENT_H