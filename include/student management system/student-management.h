#ifndef STUDENT_MANAGEMENT_H
#define STUDENT_MANAGEMENT_H

/// @brief Get student record, this one is not complete
/// @param user_id The user id of the student to get
struct StudentRecord* get_student_record(int user_id);
/// @brief Add the record of the student
/// @param user_id The user id of the student, because student record and user data is linked with this
/// @return True if student record was added, otherwise returns false
bool add_student_record(int user_id);
/// @brief Remove the record of the student
/// @param user_id The user id of the student, because student record and user data is linked with this
/// @return True if the student record is removed, otherwise returns false
bool remove_student_record(int user_id);
/// @brief Get specific record of the student from a specific COURSE
/// @param student_id The students user id
/// @param type Type of record, attandance or score
/// @param course_id The course id to get the data for
/// @return Int value of the given type
int get_specific_student_record(int student_id, int type, int course_id);
/// @brief Add student data to the student record
/// @param student_id The students user id
/// @param course_id The course id to add the data to
/// @param type 
/// @param value 
/// @return 
int add_student_data(int student_id, int course_id, int type, int value);

#endif // !STUDENT_MANAGEMENT_H