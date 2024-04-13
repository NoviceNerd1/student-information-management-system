#include <stdio.h>
// Interface Files
#include <user interface/ui-utils.h>
#include <user interface/ui-inputs.h>
// Include Files End
#include <systems/user-system.h>
#include <data management system/datamanager.h>
#include <student management system/student-management.h>
#include <course management system/course-management.h>
#include <lecturer management system/lecturer-management.h>
#include <programme management system/programme-management.h>
#include <login.h> 
#include <stdbool.h>
#include <user interface/ui-main.h>
//#include <user interface/ui-strings.h>

void welcome_menu() {

    int users = get_number_of_users();

    if(users == 0) {
        box_info("Welcome to the system. Please create a user to continue.");
        add_user_option(NULL);
    }

    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Login", login_menu);
    add_option(&menu, "Exit", exit_message);

    box_menu(&menu, "Welcome Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, NULL);
}

void login_menu() {
    char *username = loop_input("Enter username:", "Please enter a valid username.");
    char *password = loop_input("Enter password:", "Please enter a valid password.");

    struct User *user = login(username, password);

    if(!user) {
        printf("Invalid username or password. Please try again.\n");
        welcome_menu();
    } else {
        printf("Login successful!\n");
        main_menu(user);
    }
}

void main_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;
    int const user_role = user->role;

    if (user_role >= 0 && user_role <= 3) {
        // All Roles
        add_option(&menu, "Student Info", student_info_menu);
    }
    if (user_role >= 1 && user_role <= 3) {
        // Lecturer, System Admim and Programme Leader
        add_option(&menu, "Student Management", student_management_menu);
    }
    if (user_role >= 2 && user_role <= 3) {
        // Add options for Programme Leader and System Admin here
        add_option(&menu, "Course Management", course_management_menu);
        add_option(&menu, "Programme Management", programme_management_menu);
    }
    if (user_role == 3) {
        // System Admin
        add_option(&menu, "User Management", user_management_menu);
    }

    add_option(&menu, "Logout", logout_menu);
    box_menu(&menu, "Main Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void student_info_menu(struct User *user, int student_id) {
    enum Role user_role = user->role;
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "View Student Info", view_student_info_option);
    add_option(&menu, "View Student Course Info", view_student_course_info);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Student Info Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void user_management_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Add User", add_user_option);
    add_option(&menu, "Remove User", remove_user_options);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "User Management Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void course_management_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Add Course", add_course_option);
    add_option(&menu, "Remove Course", remove_course_option);
    add_option(&menu, "Update Course Record", update_course_record_option_menu);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Course Management Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}
void update_course_record_option_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Assign new lecturer", update_course_lecturer_option);
    add_option(&menu, "Update Course Programme", update_course_programme_option);
    add_option(&menu, "Update Course Name", update_course_name_option);
    add_option(&menu, "Return to previous menu", course_management_menu);

    box_menu(&menu, "Update Course Record Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void programme_management_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Add Programme", add_programme_option);
    add_option(&menu, "Remove Programme", remove_programme_option);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Programme Management Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}


void logout_menu(struct User *user) {
    printf("Logout successful!\n");
    user = NULL;
    welcome_menu();
}

// ! Leave this be i will handle these kind of functions - Mahmood

void add_user_option(struct User *user) {
   char *username = loop_input("Enter username:", "Please enter a valid username.");
    if(is_user_exist(username)){
        go_back_with_info("User already exists!", user, user_management_menu);
    }
    char *display_name = loop_input("Enter display name:", "Please enter a valid display name.");
    char *password = loop_input("Enter password:", "Please enter a valid password.");
    enum Role role = role_input("Enter role (0,1,2,3):");

    bool user_added = add_user(username, display_name, password, role);

    if(user == NULL) {
        go_back_with_info("User added successfully!", user, welcome_menu);
    }

    if(user_added) {
        struct User *user_data_created = read_user_record(username);
        int created_user_id = user_data_created->user_id;
        if(role == STUDENT) {
            int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
            if(!is_course_exist(course_id)) {
                go_back_with_info("Course does not exist!", user, user_management_menu);
            }
            bool was_student_added = add_student_record(created_user_id, course_id);
            if(!was_student_added) printf("There was an issue adding the student data!\n");
        }
        go_back_with_info("User added successfully!", user, user_management_menu);
    } else {
        go_back_with_info("There was an error while trying to add the user!", user, user_management_menu);
    }
}

void view_all_user_option(struct User *user) {
    go_back_with_info("View All Users", user, user_management_menu);
}

void remove_user_options(struct User *user) {
    char *username = loop_input("Enter username:", "Please enter a valid username.");
    if(strcmp(user->username, username) == 0) {
        go_back_with_info("You cannot remove yourself!", user, user_management_menu);
    }
    struct User *user_to_delete = read_user_record(username);
    if(!user_to_delete) {
        go_back_with_info("There is no user with this username please try again!", user, user_management_menu);
    }
    if(user_to_delete->role == 0) {
        do
        {
            bool is_student_removed = remove_student_record(user_to_delete->user_id);
        } while (is_student_record_exist(user_to_delete->user_id));
        // if(!is_student_record_exist(user_to_delete->user_id)) {
        //     go_back_with_info("The student record for this user which is a student could not be removed so the user will not be removed!", user, user_management_menu);
        // }

        // if(!is_student_removed) {
        //     return printf("The student record for this user which is a student could not be removed so the user will not be removed!");
        //     user_management_menu(user);
        // }
    }
    bool user_removed = remove_user(username);
    if(user_removed) {
        go_back_with_info("User removed successfully!", user, user_management_menu);
    } else {
        go_back_with_info("There was an error when trying to remove the user!", user, user_management_menu);
    }
}

///@brief Student Info Options

void view_student_info_option(struct User *user) {
    if(user->role == 0) {
        struct StudentRecord *student_info = get_student_record(user->user_id);
        if(student_info == NULL) {
            go_back_with_info("No student record found for this user.", user, student_info_menu);
        } else {
            create_box("Student Info");
            char displayname[100];
            sprintf(displayname, "Display Name: %s", user->display_name);
            add_info(displayname);
            char username[100];
            sprintf(username, "Username: %s", user->username);
            add_info(username);
            char user_id[100];
            sprintf(user_id, "User ID: %d", user->user_id);
            add_info(user_id);

            char student_courses_count[100];
            struct Course **courses = get_all_student_courses(user->user_id);
            int courses_count = 0;
            while(courses[courses_count] != NULL) {
                courses_count++;
            }
            sprintf(student_courses_count, "Courses attending: %d", courses_count);
            add_info(student_courses_count);

            close_box();

            go_back_with_info(NULL, user, student_info_menu);
        }
    } else {
        int user_id = loop_number_input("Enter user id:", "Please enter a valid user id.");
        struct StudentRecord *student_info = get_student_record(user_id);
        struct User *studentUser = read_user_record_with_id(user_id);
        if(student_info == NULL) {
            go_back_with_info("No student record found for this user.", user, student_info_menu);
        } else {
            create_box("Student Info");
            char displayname[100];
            sprintf(displayname, "Display Name: %s", studentUser->display_name);
            add_info(displayname);
            char username[100];
            sprintf(username, "Username: %s", studentUser->username);
            add_info(username);
            char user_id[100];
            sprintf(user_id, "User ID: %d", studentUser->user_id);
            add_info(user_id);

            char student_courses_count[100];
            struct Course **courses = get_all_student_courses(studentUser->user_id);
            int courses_count = 0;
            while(courses[courses_count] != NULL) {
                courses_count++;
            }
            sprintf(student_courses_count, "Courses attending: %d", courses_count);
            add_info(student_courses_count);

            close_box();

            go_back_with_info(NULL, user, student_info_menu);
        }
    }
}

void view_student_course_info(struct User *user) {
    if(user->role == 0) {
        int student_id = user->user_id;
        if(!is_student_record_exist(student_id)) {
            go_back_with_info("This is not a student!", user, student_info_menu);
        }
        int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
        if(!is_course_exist(course_id)) {
            go_back_with_info("This is not a course!", user, student_info_menu);
        }
        if(!is_student_in_course(student_id, course_id)) {
            go_back_with_info("This student is not in this course!", user, student_info_menu);
        } else {
            struct StudentRecord *student_info = get_student_record(student_id);
            struct Course *course = read_course_record(course_id);
            char viewing_student[100];
            sprintf(viewing_student, "Student ID: %d", student_id);
            char course_name[100];
            sprintf(course_name, "Course Name: %s", course->course_name);
            char lecturer[100];
            sprintf(lecturer, "Lecturer: %s", read_user_record_with_id(course->lecturer_id)->display_name);
            char attendance[100];
            sprintf(attendance, "Classes attended: %d", get_specific_student_record(student_id, 1, course_id));
            char score[100];
            sprintf(score, "Score: %d", get_specific_student_record(student_id, 2, course_id));
            create_box("Student Course Info");
            add_info(viewing_student);
            add_info(course_name);
            add_info(lecturer);
            add_info(attendance);
            add_info(score);
            close_box();
            go_back_with_info(NULL, user, student_info_menu);
        }
    } else {
        int student_id = loop_number_input("Enter student id:", "Please enter a valid student id.");
        if(!is_student_record_exist(student_id)) {
            go_back_with_info("This is not a student!", user, student_info_menu);
        }
        int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
        if(!is_course_exist(course_id)) {
            go_back_with_info("This is not a course!", user, student_info_menu);
        }
        if(!is_student_in_course(student_id, course_id)) {
            go_back_with_info("This student is not in this course!", user, student_info_menu);
        } else {
            struct StudentRecord *student_info = get_student_record(student_id);
            struct Course *course = read_course_record(course_id);
            char viewing_student[100];
            sprintf(viewing_student, "Student ID: %d", student_id);
            char course_name[100];
            sprintf(course_name, "Course Name: %s", course->course_name);
            char lecturer[100];
            sprintf(lecturer, "Lecturer: %s", read_user_record_with_id(course->lecturer_id)->display_name);
            char attendance[100];
            sprintf(attendance, "Classes attended: %d", get_specific_student_record(student_id, 1, course_id));
            char score[100];
            sprintf(score, "Score: %d", get_specific_student_record(student_id, 2, course_id));
            create_box("Student Course Info");
            add_info(viewing_student);
            add_info(course_name);
            add_info(lecturer);
            add_info(attendance);
            add_info(score);
            close_box();
            go_back_with_info(NULL, user, student_info_menu);
        }
    }
}


void view_student_attendance_record_option(struct User *user, int student_id) {
    if(user->role == 0) {
        if (!is_student_record_exist(user->user_id)) {
            
            go_back_with_info("You are not a student, how did u get here", user, student_info_menu);
        }
        int student_attandance = get_specific_student_record(user->user_id, 1, 0);
        go_back_with_info("The attandance of the student for course 0 (default)", user, student_info_menu);
    } else {
        int student_id = loop_number_input("Enter student id:", "Please enter a valid student id.");
        if(!is_student_record_exist(student_id)) {
            go_back_with_info("This is not a student!", user, student_info_menu);
            // student_info_menu(user, student_id);
        }
        printf("View Student Attendance Record for student: %d\n", student_id);
        go_back_with_info("This", user, student_info_menu);
    }
}

///@brief Student Management Options

void student_management_menu(struct User *user, int student_id) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Add Student Record", add_student_record_option);
    add_option(&menu, "Update Student Record", update_student_record_option);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Student Management Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);

}

void add_student_record_option(struct User *user) {
    int student_id = loop_number_input("Enter student id:", "Please enter a valid student id.");
    if(!is_user_exist_with_id(student_id)) {
        go_back_with_info("User does not exist!", user, student_management_menu);
    } else {
        if(read_user_record_with_id(student_id)->role != 0) {
            go_back_with_info("This is not a student!", user, student_management_menu);
        } else {
            int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
            if(!is_course_exist(course_id)) {
            go_back_with_info("Course does not exist!", user, student_management_menu);
            } else {
                if(is_student_in_course(student_id, course_id)) {
                    go_back_with_info("Student is already in this course!", user, student_management_menu);
                } else {
                    bool student_record_added = add_student_record(student_id, course_id);
                    if(student_record_added) {
                        go_back_with_info("Student record added successfully!", user, student_management_menu);
                    } else {
                        go_back_with_info("There was an error while trying to add the student record!", user, student_management_menu);
                    }
                }
            }
        }
    }

}

void update_student_record_option(struct User *user) {
    if(user->role == 0) go_back_with_info("You are a student, how did u get here", user, student_management_menu);
    int student_id = loop_number_input("Enter student id:", "Please enter a valid student id.");
    int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");

    struct StudentRecord *student_info = get_student_record(student_id);
    if(student_info == NULL) {
        go_back_with_info("No student record found for this student!", user, student_management_menu);
    }
    if(course_id < 0) {
        go_back_with_info("Invalid course id!", user, student_management_menu);
    }

    if(!is_course_exist(course_id)) {
        go_back_with_info("Course does not exist!", user, student_management_menu);
    } else {
        if(!is_student_in_course(student_id, course_id)) {
            go_back_with_info("Student is not in this course!", user, student_management_menu);
        } else {
            int type = 0;
            do {
              type = loop_number_input("Enter type (attandance, score)(1,2):", "Please enter a valid type.");  
            } while (type <= 0 || type >= 3);

            if(type == 1) {
                char *value = loop_input("Enter attandance (Present, Absent)(p,a):", "Please enter p for present and a for Absent!");

                if (strcmp(value, "p") == 0 || strcmp(value, "P") == 0 || strcmp(value, "present") == 0 || strcmp(value, "Present") == 0){
                    bool make_student_present = update_student_attandance(student_id, course_id, 1);
                    if(!make_student_present) {
                        go_back_with_info("There was an error while trying to update the student's attandance!", user, student_management_menu);
                    } else {
                        go_back_with_info("Updated student's attandance!", user, student_management_menu);
                    }
                } else if (strcmp(value, "a") == 0 || strcmp(value, "A") == 0 || strcmp(value, "absent") == 0 || strcmp(value, "Absent") == 0) {
                    bool make_student_present = update_student_attandance(student_id, course_id, 0);
                    if(!make_student_present) {
                        go_back_with_info("There was an error while trying to update the student's attandance!", user, student_management_menu);
                    } else {
                        go_back_with_info("Updated student's attandance!", user, student_management_menu);
                    }
                } else {
                    char *value = loop_input("Enter attandance (Present, Absent)(p,a):", "Please enter p for present and a for Absent!");
                }
            } else if (type == 2) {
                int value = loop_number_input("Enter score:", "Please enter a valid score.");
                bool student_data_added = add_student_data(student_id, course_id, 2, value);
                if(!student_data_added) {
                    go_back_with_info("There was an error while trying to add the student data!", user, student_management_menu);
                } else {
                    go_back_with_info("Updated student's score!", user, student_management_menu);
                }
            }
        }
    }
}

///@brief Course Management Options

void add_course_option(struct User *user) {
    if(user->role == 0) go_back_with_info("You are a student, how did u get here", user, main_menu);
    int programme_id = loop_number_input("Enter programme id:", "Please enter a valid progamme id.");
    if(!does_programme_exist(programme_id)) {
        go_back_with_info("Programme does not exist!", user, course_management_menu);
    } else {
        char *course_name = loop_input("Enter course name:", "Please enter a valid course name.");
        char *lecturer_username = loop_input("Enter lecturer username:", "Please enter a valid lecturer username.");
        if(!does_lecturer_exist(lecturer_username)) {
            go_back_with_info("Lecturer does not exist or the specified user isnt a lecturer!", user, course_management_menu);
        }
        int lecturer_id = read_user_record(lecturer_username)->user_id;
        // TO add course
        if(add_course(programme_id, course_name, lecturer_id)) {
            go_back_with_info("Course added successfully!", user, course_management_menu);
        } else {
            go_back_with_info("There was an error while trying to add the course!", user, course_management_menu);
        }
    }
}
void remove_course_option(struct User *user) {
    int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
    if(!does_course_exist(course_id)) {
        go_back_with_info("Course does not exist!", user, course_management_menu);
    } else {
        if(remove_course(course_id)) {
            go_back_with_info("Course removed successfully!", user, course_management_menu);
        } else {
            go_back_with_info("There was an error while trying to remove the course!", user, course_management_menu);
        }
    }
}
void update_course_lecturer_option(struct User *user) {
    int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
    if(!does_course_exist(course_id)) {
        go_back_with_info("Course does not exist!", user, update_course_record_option_menu);
    } else {
        char *lecturer_username = loop_input("Enter lecturer username:", "Please enter a valid lecturer username.");
        if(!does_lecturer_exist(lecturer_username)) {
            go_back_with_info("Lecturer does not exist or the specified user isnt a lecturer!", user, update_course_record_option_menu);
        }
        if(assign_lecturer_to_course(course_id, lecturer_username)) {
            go_back_with_info("Lecturer assigned to course successfully!", user, update_course_record_option_menu);
        } else {
            go_back_with_info("There was an error while trying to assign the lecturer to the course!", user, update_course_record_option_menu);
        }
    }
}
void update_course_programme_option(struct User *user) {
    int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
    if(!does_course_exist(course_id)) {
        go_back_with_info("Course does not exist!", user, update_course_record_option_menu);
    } else {
        int programme_id = loop_number_input("Enter programme id:", "Please enter a valid programme id.");
        if(!does_programme_exist(programme_id)) {
            go_back_with_info("Programme does not exist!", user, update_course_record_option_menu);
        }
        if(update_course_programme(course_id, programme_id)) {
            go_back_with_info("Course programme updated successfully!", user, update_course_record_option_menu);
        } else {
            go_back_with_info("There was an error while trying to update the course programme!", user, update_course_record_option_menu);
        }
    }
}
void update_course_name_option(struct User *user) {
    int course_id = loop_number_input("Enter course id:", "Please enter a valid course id.");
    if(!does_course_exist(course_id)) {
        go_back_with_info("Course does not exist!", user, update_course_record_option_menu);
    } else {
        char *course_name = loop_input("Enter course name:", "Please enter a valid course name.");
        if(update_course_name(course_id, course_name)) {
            go_back_with_info("Course name updated successfully!", user, update_course_record_option_menu);
        } else {
            go_back_with_info("There was an error while trying to update the course name!", user, update_course_record_option_menu);
        }
    }

}

/// @brief Programme Management Options 
void add_programme_option(struct User *user) {
    char *programme_name = loop_input("Enter programme name:", "Please enter a valid programme name.");
    char *lecturer_username = loop_input("Enter lecturer username:", "Please enter a valid lecturer username.");
    if(!does_lecturer_exist(lecturer_username)) {
        go_back_with_info("Lecturer does not exist or the specified user isnt a lecturer!", user, programme_management_menu);
    } else {
        int lecturer_id = read_user_record(lecturer_username)->user_id;
        if(add_programme(programme_name, lecturer_id)) {
            go_back_with_info("Programme added successfully!", user, programme_management_menu);
        } else {
            go_back_with_info("There was an error while trying to add the programme!", user, programme_management_menu);
        }
    }
}

void remove_programme_option(struct User *user) {
    int programme_id = loop_number_input("Enter programme id:", "Please enter a valid programme id.");
    if(!does_programme_exist(programme_id)) {
        go_back_with_info("Programme does not exist!", user, programme_management_menu);
    } else {
        if(remove_programme(programme_id)) {
            go_back_with_info("Programme removed successfully!", user, programme_management_menu);
        } else {
            go_back_with_info("There was an error while trying to remove the programme!", user, programme_management_menu);
        }
    }
}

