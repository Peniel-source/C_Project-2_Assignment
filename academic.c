#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 40 
#define COURSE_LEN 30
#define I_CAPACITY 20 //initial capacity or number of records
#define PASS_MARK 50.0
#define MY_ID 469420

//structure for a student
typedef struct {
    int id;
    char name[NAME_LEN];
    char course[COURSE_LEN];
    float grade;
} Student;

//collection of students
typedef struct {
    Student *data;
    int count;
    int capacity;
} Roster;

//my funxn pointers
void display_students(Roster *);
void add_student(Roster *);
void calculate_average(Roster *);
void find_top_student(Roster *);
void search_student(Roster *);
void remove_student(Roster *);
void custom_analysis(Roster *);
void summary_Report(Roster *);
void sort_students(Roster *);
void check_line(char *location, int size);
int check_int(char *prompt, int min, int max);
float check_float(char *prompt, float min, float max);
int findIndex_withId(Roster *, int id);

//input validator. this safely reads a line from the keyboard
void check_line(char *location, int size)
{
    if (fgets(location, size, stdin) == NULL)
        location[0] = '\0';
    location[strcspn(location, "\n")] = '\0';
}

//input validator to read int values
int check_int(char *prompt, int min, int max)
{
    char line[32];
    int value;

    while (1)
    {
        printf("%s", prompt);
        check_line(line, sizeof(line));

        if (sscanf(line, "%d", &value)== 1 && value >= min && value <= max)
            return (value);

        printf("Enter a number between %d and %d\n", min, max);
    }
}

//input validator to read float values
float check_float(char *prompt, float min, float max)
{
    char line[32];
    float value;

    while(1)
    {
        printf("%s", prompt);
        check_line(line, sizeof(line));

        if (sscanf(line, "%f", &value)== 1 && value >= min && value <= max)
        return (value);

        printf("Enter a float value between %.2f and %.2f\n", min, max);
    }
}

int main()
{
    printf("===============================================================\n");
    printf("Student ID: %d\n", MY_ID);
    printf("Project: Dynamic Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index\n");
    printf("===============================================================\n");
    printf("\n");


    //dataset for 15 students 
    int ids[] = {21, 22, 23, 31, 32, 33, 41, 42, 43, 51, 52, 53, 61, 62, 63};
    char *names[] = {"Irene", "Igihozo", "Wisdom", "Manuelle", "Paul", "Lisa", 
        "Gideon", "Liata", "Robert", "Helen", "Jefferson", "Grace", "Menes", 
        "Kethia", "Peniel"};
    char *courses[] = {"CProg", "linux", "DSA", "Blockchain", "Math", "IoT"};  // I only used 6 courses
    float grades[] = {42.3, 78.5, 31.1, 92.4, 67.8, 48.9, 5.0, 
        24.6, 71.3, 59.1, 45.2, 88.7, 63.4, 39.8, 76.2};

    int n = sizeof(ids) / sizeof(ids[0]);

    Roster roster;

    //allocate memory for initial capacity
    roster.data = malloc(I_CAPACITY * sizeof(Student));
    if (roster.data == NULL)
        return (1);


    //copying the dataset into the array
    for (int i = 0; i < n; i++)
    {
        roster.data[i].id = ids[i];
        strcpy(roster.data[i].name, names[i]);
        strcpy(roster.data[i].course, courses[i % 6]);
        roster.data[i].grade = grades[i];
    }

    roster.capacity = I_CAPACITY;
    roster.count = n;

    //array for funtion pointers
    void (*funxs[])(Roster *) = {add_student, display_students, sort_students, 
        calculate_average, find_top_student, search_student, remove_student, 
        custom_analysis, summary_Report};

    while (1)
    {
        printf("1.Add student\n2.Display students\n3.Sort\n4.Average grade\n"
            "5.Top student\n6.Search\n7.Remove student\n8.Custom analysis\n"
            "9.Summary report\n10.Exit\n");

        int choice = check_int("Enter your choice: ", 1, 10);
        if (choice == 10)
        {
            break;
        }
        //I'm doing choice-1 so the menu items can match the indexes
        funxs[choice - 1](&roster);    
    }
    printf("\nEnded\n");

    free(roster.data);
    return (0);
}

//print students in a formatted table
void display_students(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students\n");
        return;
    }

    Student *s;
    Student *end = r->data + r->count;

    printf("%-6s %-15s %-12s %6s\n", "Id", "Name", "Course", "Grade");
    printf("==============================================\n");

    for (s = r->data; s < end; s++)
    {
        printf("%-6d %-15s %-12s %6.1f\n", s->id, s->name, 
            s->course, s->grade);
    }
    printf("\n");
}

//this funxn returns the position of a student. It helps in search, add and remove funxs
int findIndex_withId(Roster *r, int id)
{
    
    for (int i = 0; i < r->count; i++)
    {
        if (id == r->data[i].id)
            return (i);
    }
    return -1;
}

//add a student
void add_student(Roster *r)
{
    //first check if the capacity is full. If it's full, allocate new memory
    int isFull = r->count == r->capacity;

    if (isFull == 1)
    {
        int new_capacity = 2 * r->capacity;
        Student *new_memory = realloc(r->data, new_capacity * sizeof(Student));
        if (new_memory == NULL)
        {
            printf("Realloc didn't work\n");
            return;
        } 
        r->data = new_memory;
        r->capacity = new_capacity;
        
    }

    Student s;
    s.id = check_int("Enter the student's ID: ", 1, 999999);
    if (findIndex_withId(r, s.id) != -1)
    {
        printf("This ID already exists\n");
        return;
    }
    printf("Enter the name: "); check_line(s.name, NAME_LEN);
    if (s.name[0] == '\0')
    {
        printf("You entered no name\n");
        return;
    }
    
    printf("Enter the course: "); check_line(s.course, COURSE_LEN);
    if (s.course[0] == '\0')
    {
        printf("You did not enter a course\n");
        return;
    }
    s.grade = check_float("Enter grade: ", 0, 100);

    r->data[r->count] = s;
    r->count++;
    printf("Student Added Sucessfully\n");
    printf("\n");      
}

//remove student by id
void remove_student(Roster *r)
{
    if (r->count == 0)
    {
        printf("well, there are no students to remove\n");
        return;
    }

    int id = check_int("Enter id of student: ", 1, 999999);
    int index = findIndex_withId(r, id);

    if (index == -1)
    {
        printf("There's no student with this id\n");
        return;
    }

    //cover up the space by shifting the array items left
    for (int i = index; i < r->count-1; i++)
    {
        r->data[i] = r->data[i + 1];
    }
    r->count--;


    //shrink the size if only it is bigger than its starting size
    if (r->capacity > I_CAPACITY && r->count < r->capacity / 4)
    {
        int new_capacity = r->capacity / 2;
        Student *shrink = realloc(r->data, new_capacity * sizeof(Student));

        if (shrink != NULL)
        {
            r->data = shrink;
            r->capacity = new_capacity;
        }
    }
    printf("Student removed\n");
    printf("\n");
}


//average grade, with pointer traversal
void calculate_average(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students\n");
        return;
    }
    float sum = 0;

    
    Student *s;
    Student *end = r->data + r->count;

    for (s = r->data; s < end; s++)
    {
        sum += s->grade;
    }
    float average = sum / r->count;

    printf("Average grade: %.2f\n", average);
    printf("\n");
}


//find student with the highest grade
void find_top_student(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students\n");
        return;
    }
    Student *top_stdnt = r->data;
    Student *s;
    Student *end = r->data + r->count;

    //keep comparing until you run thruough the entire list of students. save the one with the highest grade
    for (s = r->data; s < end; s++)
    {
        if (s->grade > top_stdnt->grade)
            top_stdnt = s;
    }
    printf("The top student is %s with %.2f in %s\n", top_stdnt->name, top_stdnt->grade, top_stdnt->course);
    printf("\n");
}

//search a student by id or by name
void search_student(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students\n");
        return;
    }

    int choice = check_int("Search by 1.Id, 2.Name: ", 1, 2);

    if (choice == 1)
    {
        int id = check_int("Enter ID: ", 1, 999999);
        int index = findIndex_withId(r, id);
        if (index == -1)
        {
            printf("There's no student with that ID\n");
            return;
        }
        printf("%-6d %-15s %-12s %6.1f\n", r->data[index].id, r->data[index].name, 
            r->data[index].course, r->data[index].grade);
    } else {
        char input[NAME_LEN];
        printf("Enter name: "); check_line(input, NAME_LEN);

        int counter = 0;
        Student *s;
        Student *end = r->data + r->count;
        for (s = r->data; s < end; s++)
        {
            if (strcmp(s->name, input) == 0)
            {
                printf("Id: %-6d name: %-15s course: %-12s grade: %6.1f%%\n", s->id, s->name, s->course, s->grade);
                counter++;
            }
        }
        if (counter == 0)
        {
            printf("There's no student with this name\n");
        } else {
            printf("%d people found\n", counter);
        }
    }
    printf("\n");
}


//Academic risk index. This shows how far each student is below a threshold that is inputted by the user
//Formula: if grade >= threshold, risk = 0
// otherwise risk = (threshold - grade) / threshold * 100
//the risk levels are: safe(from threshold and above), Low (25 and below), Moderate(50 and below), High(above 50)
void custom_analysis(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students\n");
        return;
    }

    float threshold = check_float("Enter threshold: ", 1, 100);
    
    int type_safe = 0;
    int type_moderate = 0;
    int  type_high = 0;
    int type_low = 0;

    Student *s;
    Student *end = r->data + r->count;

    printf("\n%-15s %6s %6s %-13s\n", "Name", "Grade", "Risk", "Level");
    printf("==========================================\n");
    printf("L: low risk, M: medium risk, H: high risk\n");


    for (s = r->data; s < end; s++)
    {
        float risk = 0;
        char *level;

        if (s->grade < threshold)
        {
            risk = (threshold - s->grade) / threshold * 100;
        }

        if (s->grade >= threshold){
            level = "S";
            type_safe++;
        } else if (risk <= 25){
            type_low++;
            level = "L";
        } else if (risk <= 50){
            type_moderate++;
            level = "M";
        } else {
            type_high++;
            level = "H";
        }

        printf("%-15s %6.1f %6.1f %-10s\n", s->name, s->grade, risk, level);
    }
    printf("-------------  Summary  -----------------\n");
    printf("\nSafe students: %d\n", type_safe);
    printf("Low Risk students: %d\n", type_low);
    printf("Medium Risk students: %d\n", type_moderate);
    printf("High Risk students: %d\n", type_high);

    float percentage = (type_low + type_moderate + type_high) * 100.0 / r->count;
    printf("\nIn total, %.1f%% of students are at risk\n", percentage);
    printf("\n");
}

void summary_Report(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students here\n");
        return;
    }

    float sum = 0;
    float highest = r->data[0].grade;
    float lowest = r->data[0].grade;
    int pass = 0;
    int type_low = 0;
    int type_moderate = 0;
    int type_high = 0;

    Student *s;
    Student *end = r->data + r->count;

    for (s = r->data; s < end; s++)
    {
        sum += s->grade;

        if (s->grade > highest)
            highest = s->grade;
        if (s->grade < lowest)
            lowest = s->grade;

        if (s->grade >= PASS_MARK)
        {
            pass++;
        }
        else
        {
            float risk = (PASS_MARK - s->grade) / PASS_MARK * 100;

            if (risk <= 25)
                type_low++;
            else if (risk <= 50)
                type_moderate++;
            else
                type_high++;
        }
    }

    printf("\n============ Report Summary  ============\n");
    printf("Total students: %d\n", r->count);
    printf("Average grade: %.2f\n", sum / r->count);
    printf("Highest grade: %.1f\n", highest);
    printf("Lowest grade: %.1f\n", lowest);
    printf("Passed, 50+: %d\n", pass);
    printf("Failed : %d\n", r->count - pass);
    printf("\n--- Custom analysis ---\n");
    printf("Low Risk students: %d\n", type_low);
    printf("Medium risk: %d\n", type_moderate);
    printf("High risk students: %d\n", type_high);
    printf("In total, %d students are at risk\n", type_low + type_moderate + type_high);
    printf("========================================\n");
    printf("\n");
}

//comparison function for qsort. they return negative positive or zero

//sort highest grade first
int by_grade(const void *a, const void *b)     //highest grade first
{
    const Student *x = a;
    const Student *y = b;
    return (y->grade > x->grade) - (y->grade < x->grade);
}

//sort names in alphabetical order
int by_name(const void *a, const void *b)
{
    const Student *x = a;
    const Student *y = b;
    return strcmp(x->name, y->name);
}

//sort smallest id forst
int by_id(const void *a, const void *b)
{
    const Student *x = a;
    const Student *y = b;
    return (x->id > y->id) - (x->id < y->id);
}

void sort_students(Roster *r)
{
    if (r->count == 0)
    {
        printf("There are no students\n");
        return;
    }

    int (*cmp[])(const void *, const void *) = {by_grade, by_name, by_id};

    int choice = check_int("Sort by 1.Grade, 2.Name, 3.ID: ", 1, 3);
    qsort(r->data, r->count, sizeof(Student), cmp[choice - 1]);

    display_students(r);
}
