#include <data management system/datamanager.h>
#include <stdbool.h>

struct StudentRecord** get_student_record(int user_id) {
    if(!is_student_record_exist(user_id)){
        return NULL;
    }
    return read_student_record(user_id);
}

bool add_student_record(int user_id) {
    struct StudentRecord *student_object = create_student_record_object(user_id, 0, 0, 0);
    return create_student_record(student_object);
}

bool remove_student_record(int user_id) {
    return delete_student_record(user_id);
}



int get_specific_student_record(int student_id, int type, int course_id) {
    if(!is_student_record_exist(student_id)) {
        return -1; // ? Student record doesnt exist
    }
    struct StudentRecord *student = read_student_record_with_specific_course(student_id, course_id);
    if(type == 1) {
        return student->attendance;
    } else if(type == 2) {
        return student->score;
    }
    return -1;
}

bool add_student_data(int student_id, int course_id, int type, int value) {
    if(!is_student_record_exist(student_id)) {
        return false;
    }
    struct StudentRecord *student = read_student_record_with_specific_course(student_id, course_id);
    if(type == 1) {
        student->attendance += value;
    } else if(type == 2) {
        student->score += value;
    }
    return update_student_record(student);
}
bool remove_student_data(int student_id, int course_id, int type, int value) {
    return add_student_data(student_id, course_id, type, -value);
}

bool update_student_attandance(int student_id, int course_id, int value) {
    return add_student_data(student_id, course_id, 1, value);
}
