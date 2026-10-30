# C_Project-2_Assignment

typedef struct {
    int id;
    char name[NAME_LEN];
    char course[COURSE_LEN];
    float grade;
} Student;

typedef struct {
    Student *data;
    int count;
    int capacity;
} Roster;

/*I haev only 6 courses, less than 15. SO the % makes i loop around 0 to 5 and repeat
        which means, some students can share the same course. */
    
laterr
// add const to names and courses.
// add the num_courses count and use i % num_courses for the course.

void display_students(Roster *);                                     xx
void add_student(Roster *);                                          xx
void calculate_average(Roster *);                                    xx
void find_top_student(Roster *);                                     xx
void search_student(Roster *);                                       xx
void remove_student(Roster *);                                       xx
void custom_analysis(Roster *);                                      xx
void summary_Report(Roster *);                                       xx
void sort_students(Roster *);
void check_line(char *location, int size);                           xx
int check_int(char *prompt, int min, int max);                       xx
float check_float(char *prompt, float min, float max);               xx
int findIndex_withId(Roster *r, int id);                             xx

Student ID: 54321
Project: Dynamic Academic Performance and Risk Analysis Engine
Custom Analysis: Academic Risk Index


// text mining:
void count_words(Analyzer *);                                        xx
void longest_word(Analyzer *);                                       xx
void most_frequent(Analyzer *);                                      xx
void display_word_statistics(Analyzer *);                            xx
void search_word(Analyzer *);                                        xx
void generate_text_summary(Analyzer *);                              xx
int load_text(Analyzer *);                                           xx
int analyze_text(Analyzer *);                                        xx
int find_word(Analyzer *, const char *);                             xx
void custom_analysis(Analyzer *);                                    xx
sort fuxn


add const to cmpr_name.. compiler complaining