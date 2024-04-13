#include <stdio.h>
// Interface Files
#include <user interface/ui-utils.h>
#include <user interface/ui-inputs.h>
// Include Files End
#include <systems/user-system.h>
#include <data management system/datamanager.h>
#include <student management system/student-management.h>
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
    }
    if (user_role == 3) {
        // Programe 
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

    add_option(&menu, "View Student Info", view_student_attendance_record_option);
    add_option(&menu, "Enrolled Courses", view_student_enrolled_courses);
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
    add_option(&menu, "View All Users", view_all_user_option);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "User Management Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void course_management_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Add Course", main_menu);
    add_option(&menu, "Remove Course", main_menu);
    add_option(&menu, "View All Courses", main_menu);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Course Management Menu");

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
            bool was_student_added = add_student_record(created_user_id);
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

void view_student_info(struct User *user) {
    if(user->role == 0) {
        struct StudentRecord *student_info = get_student_record(user->user_id);
        if(student_info == NULL) {
            printf("No student record found for this user.\n");
            student_info_menu(user, 0);
        } else {
            printf("Attandace: %d", student_info->attendance);
        }
    } else {
        int user_id = loop_number_input("Enter user id:", "Please enter a valid user id.");
        struct StudentRecord *student_info = get_student_record(user_id);
        if(student_info == NULL) {
            printf("No student record found for this user.\n");
            student_info_menu(user, 0);
        } else {
            printf("Attandance: %d", student_info->attendance); 
        }
    }
}

void view_student_enrolled_courses(struct User *user, int student_id) {
    if(user->role == 0) {
        printf("View Student Enrolled Courses\n");
    } else {
        int student_id = loop_number_input("Enter user id:", "Please enter a valid student id.");
        printf("View Student Enrolled Courses for student id: %d\n", student_id);
    }
    printf("View Student Enrolled Courses\n");	
}

void view_student_course_score(struct User *user, int student_id) {
    if(user->role == 0) {
        printf("View Student Course Score\n");
    } else {
        int student_id = loop_number_input("Enter student id:", "Please enter a valid student id.");
        printf("View Student Course Score for student: %d\n", student_id);
    }
}

void view_student_cgpa(struct User *user, int student_id) {
    if(user->role == 0) {
        printf("View Student CGPA\n");
    } else {
        int student_id = loop_number_input("Enter student id:", "Please enter a valid student id.");
        printf("View Student CGPA for student: %d\n", student_id);
    }
}

void view_student_attendance_record_option(struct User *user, int student_id) {
    if(user->role == 0) {
        if (!is_student_record_exist(user->user_id)) {
            
            go_back_with_info("You are not a student, how did u get here", user, student_info_menu);
        }
        int student_attandance = get_specific_student_record(user->user_id, 1, 0);
        // ! Need to make it so that the number is made into a string i forgot how to do it waiting for wifi 
        
        go_back_with_info("The attandance of the student for course 0 (defailt)", user, student_info_menu);
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
    printf("Student Management\n");
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

    // ! Code for course validy check

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

void add_course_option(struct User *user) {
    printf("Add Course\n");
}