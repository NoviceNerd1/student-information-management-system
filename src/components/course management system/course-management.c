#include <data management system/datamanager.h>
#include<lecturer management system/lecturer-management.h>
#include<programme management system/programme-management.h>
#include<student management system/student-management.h>
#include<course management system/course-management.h>

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
    return create_course_record(programme_id, course_name, lecturer_id);
}

bool remove_course(int course_id) {
    return delete_course_record(course_id);
}

bool update_course_lecturer(int course_id, int lecturer_id) {
    struct Course *course = read_course_record(course_id);
    if(course == NULL) {
        return false;
    } else {
        course->lecturer_id = lecturer_id;
        return update_course_record(course_id, course->programme_id, course->course_name, lecturer_id);
    }
}

bool update_course_programme(int course_id, int programme_id) {
    struct Course *course = read_course_record(course_id);
    if(course == NULL) {
        return false;
    } else {
        course->programme_id = programme_id;
        return update_course_record(course_id, programme_id, course->course_name, course->lecturer_id);
    }
}

bool update_course_name(int course_id, char* course_name) {
    struct Course *course = read_course_record(course_id);
    if(course == NULL) {
        return false;
    } else {
        return update_course_record(course_id, course->programme_id, course_name, course->lecturer_id);
    }
}