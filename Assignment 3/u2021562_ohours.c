#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

/* Constants that define parameters of the simulation */
#define MAX_SEATS 3           /* Number of seats in TA's office */
#define TA_LIMIT 10           /* Number of students a TA can help before he needs a break */
#define MAX_STUDENTS 1000     /* Maximum number of students in the simulation */

#define CLASS_OS "OS"
#define CLASS_PP "PP"


/* Add your synchronization variables here */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ta_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t class_os_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t class_pp_cond = PTHREAD_COND_INITIALIZER;

/* Basic information about the simulation. They are printed/checked at the end
 * and in assert statements during execution.
 */
static int students_in_office;
static int class_os_inoffice;
static int class_pp_inoffice;
static int students_since_break = 0;

typedef struct {
    int arrival_time;
    int question_time;
    char student_class[5];
    int student_id;
} student_info;

/* Called at the beginning of the simulation. Create/initialize all synchronization
 * variables and other global variables that you add.
 */
static int initialize(student_info *si, char *filename) {
    students_in_office = 0;
    class_os_inoffice = 0;
    class_pp_inoffice = 0;
    students_since_break = 0;

    /* Initialize your synchronization variables (and
     * other variables you might use) here
     */

    /* Read in the data file and initialize the student array */
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Cannot open input file %s for reading.\n", filename);
        exit(1);
    }
    int i = 0;
    while ((fscanf(fp, "%d%d%s\n", &(si[i].arrival_time), &(si[i].question_time), si[i].student_class) != EOF) && i < MAX_STUDENTS) {
        i++;
    }
    fclose(fp);
    return i;
}

/* Code executed by TA to simulate taking a break
 * You do not need to add anything here.
 */
static void take_break() {
    sleep(5);
    printf("The TA is taking a break now.\n");
}

/* Code for the TA thread. This is fully implemented except for synchronization
 * with the students.  See the comments within the function for details.
 */
void *TAthread(void *junk) {
    printf("The TA arrived and is starting his office hours\n");

    /* Loop while waiting for students to arrive. */
    while (1) {
        pthread_mutex_lock(&mutex);

        

        // assert(students_in_office <= MAX_SEATS && students_in_office >= 0);

        // /* Check if there are students from both classes */
        // assert(!(class_os_inoffice > 0 && class_pp_inoffice > 0));

        /* Check if the TA needs a break */
        if (students_since_break >= TA_LIMIT) {
            while (students_in_office > 0) {
                pthread_cond_wait(&ta_cond, &mutex);
            }
            take_break();
            students_since_break = 0;
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}



/* Code executed by a OS class student to enter the office.
 * You have to implement this.
 */
void class_os_enter() {
    pthread_mutex_lock(&mutex);

    while (students_in_office == MAX_SEATS || class_pp_inoffice > 0) {
        pthread_cond_wait(&class_os_cond, &mutex);
    }

    students_in_office += 1;
    students_since_break += 1;
    class_os_inoffice += 1;

    pthread_cond_broadcast(&ta_cond);
    pthread_mutex_unlock(&mutex);
}

/* Code executed by a PP class student to enter the office.
 * You have to implement this.
 */
void class_pp_enter() {
    pthread_mutex_lock(&mutex);

    while (students_in_office == MAX_SEATS || class_os_inoffice > 0) {
        pthread_cond_wait(&class_pp_cond, &mutex);
    }

    students_in_office += 1;
    students_since_break += 1;
    class_pp_inoffice += 1;

    pthread_cond_broadcast(&ta_cond);
    pthread_mutex_unlock(&mutex);
}

/* Code executed by a student to simulate the time he spends in the office asking questions
 * You do not need to add anything here.
 */
static void ask_questions(int t) {
    sleep(t);
}

/* Code executed by a OS class student when leaving the office.
 * You need to implement this.
 */
static void class_os_leave() {
    pthread_mutex_lock(&mutex);

    students_in_office -= 1;
    class_os_inoffice -= 1;

    pthread_cond_broadcast(&class_pp_cond);
    pthread_cond_broadcast(&class_pp_cond);
    pthread_cond_broadcast(&ta_cond);
    pthread_mutex_unlock(&mutex);
}

/* Code executed by a PP class student when leaving the office.
 * You need to implement this.
 */
static void class_pp_leave() {
    pthread_mutex_lock(&mutex);

    students_in_office -= 1;
    class_pp_inoffice -= 1;

    pthread_cond_broadcast(&class_pp_cond);
    pthread_cond_broadcast(&class_pp_cond);
    pthread_cond_broadcast(&ta_cond);
    pthread_mutex_unlock(&mutex);
}

/* Main code for OS class student threads.
 * You do not need to change anything here, but you can add
 * debug statements to help you during development/debugging.
 */
void *class_os_student(void *si) {
    student_info *s_info = (student_info *)si;

    /* enter office */
    class_os_enter();

    assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
    assert(class_pp_inoffice >= 0 && class_pp_inoffice <= MAX_SEATS);
    assert(class_os_inoffice >= 0 && class_os_inoffice <= MAX_SEATS);

    /* ask questions  --- do not make changes to the 3 lines below */
    printf("Student %d from OS class starts asking questions for %d minutes\n", s_info->student_id, s_info->question_time);
    ask_questions(s_info->question_time);
    printf("Student %d from OS class finishes asking questions and prepares to leave\n", s_info->student_id);

    /* leave office */
    class_os_leave();

    assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
    assert(class_pp_inoffice >= 0 && class_pp_inoffice <= MAX_SEATS);
    assert(class_os_inoffice >= 0 && class_os_inoffice <= MAX_SEATS);

    pthread_exit(NULL);
}

/* Main code for PP class student threads.
 * You do not need to change anything here, but you can add
 * debug statements to help you during development/debugging.
 */
void *class_pp_student(void *si) {
    student_info *s_info = (student_info *)si;

    /* enter office */
    class_pp_enter();

    assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
    assert(class_pp_inoffice >= 0 && class_pp_inoffice <= MAX_SEATS);
    assert(class_os_inoffice >= 0 && class_os_inoffice <= MAX_SEATS);

    printf("Student %d from PP class starts asking questions for %d minutes\n", s_info->student_id, s_info->question_time);
    ask_questions(s_info->question_time);
    printf("Student %d from PP class finishes asking questions and prepares to leave\n", s_info->student_id);

    /* leave office */
    class_pp_leave();

    assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
    assert(class_pp_inoffice >= 0 && class_pp_inoffice <= MAX_SEATS);
    assert(class_os_inoffice >= 0 && class_os_inoffice <= MAX_SEATS);

    pthread_exit(NULL);
}

/* Main function sets up simulation and prints a report
 * at the end.
 */
int main(int nargs, char **args) {
    int i;
    int result;
    // int student_type;
    int num_students;
    void *status;
    pthread_t ta_tid;
    pthread_t student_tid[MAX_STUDENTS];
    student_info s_info[MAX_STUDENTS];

    if (nargs != 2) {
        printf("Usage: officehour <name of inputfile>\n");
        return EINVAL;
    }

    num_students = initialize(s_info, args[1]);
    if (num_students > MAX_STUDENTS || num_students <= 0) {
        printf("Error: Bad number of student threads. Maybe there was a problem with your input file?\n");
        return 1;
    }

    printf("Starting officehour simulation with %d students ...\n", num_students);

    result = pthread_create(&ta_tid, NULL, TAthread, NULL);
    if (result) {
        printf("officehour: pthread_create failed for TA: %s\n", strerror(result));
        exit(1);
    }

    for (i = 0; i < num_students; i++) {
        s_info[i].student_id = i;
        sleep(s_info[i].arrival_time);

        //student_type = random() % 2;

        if (strcmp(s_info[i].student_class, CLASS_OS) == 0)
            result = pthread_create(&student_tid[i], NULL, class_os_student, (void *)&s_info[i]);
        else // student_type == CLASS_PP. assuming input is all correct!
            result = pthread_create(&student_tid[i], NULL, class_pp_student, (void *)&s_info[i]);

        if (result) {
            printf("officehour: thread_fork failed for student %d: %s\n",
                   i, strerror(result));
            exit(1);
        }
    }

    /* wait for all student threads to finish */
    for (i = 0; i < num_students; i++)
        pthread_join(student_tid[i], &status);

    /* tell the TA to finish. */
    pthread_cancel(ta_tid);

    printf("Office hour simulation done.\n");
    // khataaaam
    return 0;
}