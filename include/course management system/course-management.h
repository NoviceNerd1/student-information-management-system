#ifndef COURSE_MANGEMENT_H
#define COURSE_MANGEMENT_H
#include <stdbool.h>

bool does_course_exist(int course_id);
bool is_student_in_course(int student_id, int course_id);
bool add_course(int programme_id, char* course_name, int lecturer_id);
bool remove_course(int course_id);

bool update_course_lecturer(int course_id, int lecturer_id);
bool update_course_programme(int course_id, int programme_id);
bool update_course_name(int course_id, char* course_name);


#endif // COURSE_MANGEMENT_H