#include <data management system/datamanager.h>
#include <stdbool.h>

bool does_course_exist(int course_id) {
    return is_course_exist(course_id);
}

bool is_student_in_course(int student_id, int course_id) {
    bool student_exist = is_student_record_exist(student_id);
    if(!student_exist) {
        return false;
    }
    struct StudentRecord *student = read_student_record_with_specific_course(student_id, course_id);
    return student != NULL;
}

bool add_course(int programme_id, char* course_name, int lecturer_id) {
    bool course_exist = is_course_exist(course_id);
    if(course_exist) {
        return false;
    }
    return create_course_record(programme_id, course_name, lecturer_id);
}