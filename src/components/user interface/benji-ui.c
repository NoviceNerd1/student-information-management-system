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
        add_option(&menu, "Student Management", lecturer_menu);
    }
    if (user_role >= 2 && user_role <= 3) {
        // Programme Leader and System Admin here
        add_option(&menu,"Programme Administration", programme_admin_menu)
    }
    if (user_role == 3) {
        // System Admin only 
        add_option(&menu, "System Administration", system_admin_menu);
    }

    add_option(&menu, "Logout", logout_menu);
    box_menu(&menu, "Main Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void student_info_menu(struct User *user) {
    enum Role user_role = user->role;
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "View Student Information", view_student_info);
    add_option(&menu, "Enrolled Courses", view_student_enrolled_courses);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Student Info Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void lecturer_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Student Attendance", logout_menu);
    add_option(&menu, "Course Grades", logout_menu);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "Student Management Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void programme_admin_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "Course information", logout_menu);
    add_option(&menu, "Student enrollment", logout_menu);
    add_option(&menu, "Lecturer assignment", logout_menu);
    add_option(&menu, "Student performance reports", logout_menu);
    add_option(&menu, "Return to Main Menu", main_menu);
    
    box_menu(&menu, "Programme Administration Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void system_admin_menu(struct User *user) {
    struct Menu menu;
    menu.num_options = 0;

    add_option(&menu, "User management", logout_menu);
    add_option(&menu, "Grading system parameters", logout_menu);
    add_option(&menu, "Student performance report generation", logout_menu);
    add_option(&menu, "System settings", logout_menu);
    add_option(&menu, "Return to Main Menu", main_menu);

    box_menu(&menu, "System Administration Menu");

    int option = option_input("Enter your option:", &menu);
    option_handler(&menu, option, user);
}

void logout_menu(struct User *user) {
    printf("Logout successful!\n");
    user = NULL;
    welcome_menu();
}