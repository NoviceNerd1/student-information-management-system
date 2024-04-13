#include <data management system/datamanager.h>
#include <lecturer management system/lecturer-management.h>
#include <course management system/course-management.h>
#include <stdbool.h>

bool does_lecturer_exist(char *lecturer_username) {
    if(!is_user_exist(lecturer_username)) {
        return false;
    }
    struct User *lecturer = read_user_record(lecturer_username);
    if(lecturer == NULL) {
        return false;
    }
    if (lecturer->role != LECTURER) {
        return false;
    } else {
        return true;
    }
}

bool assign_lecturer_to_course(int course_id, char *lecturer_username) {
    if(!does_lecturer_exist(lecturer_username)) {
        return false;
    }
    struct User *lecturer = read_user_record(lecturer_username);
    if(lecturer == NULL) {
        return false;
    }
    int lecturer_id = lecturer->user_id;
    return update_course_lecturer(course_id, lecturer_id);
}
