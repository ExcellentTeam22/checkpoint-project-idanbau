#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define NAME_LENGTH 31
#define PHONE_LENGTH 15

struct contact {
    char fname[NAME_LENGTH];
    char lname[NAME_LENGTH];
    char telep[PHONE_LENGTH];
    char cellp[PHONE_LENGTH];
    struct contact *next;
};

struct contact *A = NULL;

char *ALLOCATE_MEMORY = NULL;
size_t ALLOCATE_SIZE = 0;

void clrscr(void);

void gotoxy(int x, int y);

void insert(void);

void delet(void);


void edit(void);

void search(void);

void searchBy(int (*check)(const struct contact* current,const char *value));

int fNameMatch(const struct contact* current, const char* pattern);

int lNameMatch(const struct contact* current, const char* pattern);

int telpMatch(const struct contact* current, const char* pattern);

int cellpMatch(const struct contact* current, const char* pattern);

void list(void);

void list2(void);

void sort(void);

void sortf(void);

void sortl(void);

void sortp(void);

void sortc(void);

void help(void);

void get_input(int len, char str[]);

void exitFreeMemory();

int last;

int main() {
    int count = 1;
    char n;
    while (count) {
        clrscr();
        printf("\n|Phone Book12<::>Home|\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Welcome to PHONE BOOK2022!\nYou have inserted ( %d ) contacts.\n\n", last);
        printf("\t[1] |--> Insert\n");
        printf("\t[2] |--> Delete\n");
        printf("\t[3] |--> Edit\n");
        printf("\t[4] |--> Search\n");
        printf("\t[5] |--> Show All\n");
        printf("\t[6] |--> Sort\n");
        printf("\t[7] |--> Help\n");
        printf("\t[8] |--> Exit\n");
        printf("\n\tPlease Enter Your Choice (1-8): ");
        n = getc(stdin);
        getc(stdin);
        switch (n) {
            case '1':
                insert();
                break;
            case '2':
                delet();
                break;
            case '3':
//                edit();
                break;
            case '4':
                search();
                break;
            case '5':
                list();
                break;
            case '6':
                sort();
                break;
            case '7':
                help();
                break;
            case '8':
                exitFreeMemory();
                break;
            default:
                printf("\nThere is no item with symbol \"%c\".Please enter a number between 1-8!\nPress any key to continue...",
                       n);
                getc(stdin);
                getc(stdin);
                break;
        }//End of swicth
    }
    return 0;
}//End of main function!

void insert(void) {
    char ans = 'y';
    clrscr();
    printf("\nPhone Book12<::>Insert Contacts");
    printf("\n--------------------------------------------------------------------------------");
    while (ans == 'y') {
        struct contact *new_contact = (struct contact *) malloc(sizeof(struct contact));
        printf("\n\nData of Contact %2.2d{\n", last + 1);
        printf("\n\t  1-F.Name: ");
        get_input(NAME_LENGTH, new_contact->fname);
        printf("\t  2-L.Name: ");
        get_input(NAME_LENGTH, new_contact->lname);
        printf("\t  3-Tele.P: ");
        get_input(PHONE_LENGTH, new_contact->telep);
        printf("\t  4-Cell.P: ");
        get_input(PHONE_LENGTH, new_contact->cellp);
        printf("\n|-->Data Recorded!}");
        printf("\n\t\t\tNext Contact?(y/n) Answer:");
        new_contact->next = A;
        A = new_contact;
        ans = getc(stdin);
        getc(stdin);
        last++;
    }

    printf("\n\nYou have inserted ( %d ) contact!\nPress a key to return main page & continue program|-->", last);
    getc(stdin);
}

void delet(void) {
    char dfname_string[NAME_LENGTH], dlname_string[NAME_LENGTH];
    struct contact *front, *rear, *temp;
    register int find = 0;
    char ch;
    clrscr();
    printf("\nPhone Book12<::>Delete Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\n::Enter data of the contact that you want delete it,please:");
    printf("\n\n  ::Enter first name: ");
    get_input(NAME_LENGTH, dfname_string);
    printf("\n  ::Enter last name: ");
    get_input(NAME_LENGTH, dlname_string);
    if (A == NULL)
        return;
    if (strcmp(dfname_string, A->fname) == 0 && strcmp(dlname_string, A->lname) == 0) {
        printf("\nContact was found! Details:");
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }", A->fname, A->lname,
               A->telep, A->cellp);
        printf("\n\nAre you sure you want to delete this contact?(y/n)");
        ch = getc(stdin);
        getc(stdin);
        if (ch == 'y') {
            temp = A;
            A = A->next;
            free(temp);
            find = 1;
        }
    }
    if (find == 0) {
        rear = A;
        front = rear->next;
        while (front != NULL) {
            if (strcmp(dfname_string, front->fname) == 0 && strcmp(dlname_string, front->lname) == 0) {
                printf("\nContact was found! Details:");
                printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }", front->fname,
                       front->lname,
                       front->telep, front->cellp);
                printf("\n\nAre you sure you want to delete this contact?(y/n)");
                ch = getc(stdin);
                getc(stdin);
                if (ch == 'y') {
                    find = 1;
                    rear->next = front->next;
                    free(front);
                    break;
                }
            }
            rear = front;
            front = front->next;
        }
    }
    printf(find == 0 ? "\t\t\n<<This contact does not exist in this list or program can not delete it.>>"
                     : "\t\t\n<<Target contact was successfully deleted from list!>>");
    printf("\n\n\nPress a key to return main page & continue program|-->");
    getc(stdin);
}

void edit() {
    char dfname[NAME_LENGTH], dlname[NAME_LENGTH];
    struct contact *temp = A;
    register int find = 0;
    char ch;
    clrscr();
    printf("\nPhone Book12<::>Edit Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\n::Enter data of the contact that you want edit it,please:");

    printf("\n\n  ::Enter first name: ");
    get_input(NAME_LENGTH, dfname);
    printf("\n  ::Enter last name: ");
    get_input(NAME_LENGTH, dlname);
    while (temp != NULL) {
        if (strcmp(dfname, temp->fname) == 0 && strcmp(dlname, temp->lname) == 0) {
            printf("\nContact found! Details:");
            printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }", temp->fname, temp->lname,
                   temp->telep, temp->cellp);
            printf("\n\nDo you want edit it?(y/n) ");
            ch = getc(stdin);
            getc(stdin);
            if (ch == 'y') {
                printf("\n::Enter NEW data for this contact...");
                printf("\n >|Enter new first name: ");
                get_input(NAME_LENGTH, temp->fname);
                printf(" >|Enter new last name: ");
                get_input(NAME_LENGTH, temp->lname);
                printf(" >|Enter new telephone number: ");
                get_input(PHONE_LENGTH, temp->telep);
                printf(" >|Enter new cellphone number: ");
                get_input(PHONE_LENGTH, temp->cellp);
                find = 1;
                break;
            }
            temp = temp->next;
        }
    }
    printf(find == 0 ? "\t\t\n<<This contact does not exist or you chose not to Edit it.>>"
                     : "\t\t\n<<Target contact was successfully updated!>>");
    printf("\n\n\n   ::Press a key to return main page & continue program|-->");
    getc(stdin);
}

void search(void) {
    char ch;
    while(1) {
        clrscr();
        printf("\nPhone Book12<::>$earch Contacts");
        printf("\n--------------------------------------------------------------------------------");
        printf("\nChoose search type,please:\n\n");
        printf("\t[1] |--> Search by first name\n");
        printf("\t[2] |--> Search by last name\n");
        printf("\t[3] |--> Search by phone number\n");
        printf("\t[4] |--> Search by cellphone number\n");
        printf("\t[5] |--> Main Menu\n");
        printf("\n\t::Enter a number (1-5): ");
        ch = getc(stdin);
        getc(stdin);
        printf("\n--------------------------------------------------------------------------------");
        switch (ch) {
            case '1':
                searchBy(&fNameMatch);
                break;
            case '2':
                searchBy(&lNameMatch);
                break;
            case '3':
                searchBy(&telpMatch);
                break;
            case '4':
                searchBy(&cellpMatch);
                break;
            case '5':
            default:
                return;
        }
    }
}

void searchBy(int (*check)(const struct contact* current,const char *value)){
    register int find = 0;
    struct contact *temp = A;
    printf("Enter a first pattern to search:");
    size_t size = getline(&ALLOCATE_MEMORY, &ALLOCATE_SIZE, stdin);
    ALLOCATE_MEMORY[size - 1] = '\0';
    while(temp != NULL){
        if(check(temp, ALLOCATE_MEMORY) == 0){
            find = 1;
            break;
        }
        temp = temp->next;
    }
    if (find == 0)
        printf("\t\n<<Not Find!There is no contact with this pattern in phone book.>>");
    else {
        printf("\t\n<<Target contact found! Details:>>");
        printf("\n\t   F.Name:%s\n\t   L.name:%s\n\t   Tele.P:%s\n\t   Cell.P:%s\n\t   }", temp->fname, temp->lname,
               temp->telep, temp->cellp);
    }
    printf("\nPress a key to search another contact.");
    getc(stdin);
}

int fNameMatch(const struct contact* current, const char* pattern){
    return strcmp(current->fname, pattern);
}

int lNameMatch(const struct contact* current, const char* pattern){
    return strcmp(current->lname, pattern);
}

int telpMatch(const struct contact* current, const char* pattern){
    return strcmp(current->telep, pattern);
}

int cellpMatch(const struct contact* current, const char* pattern){
    return strcmp(current->cellp, pattern);
}

void sort(void) {
    char ch;
    clrscr();
    printf("\nPhone Book12<::>$earch Contacts");
    printf("\n--------------------------------------------------------------------------------");
    printf("\nChoose sort type,please:\n\n");
    printf("\t[1] |--> Sort by first name\n");
    printf("\t[2] |--> Sort by last name\n");
    printf("\t[3] |--> Sort by phone number\n");
    printf("\t[4] |--> Sort by cellphone number\n");
    printf("\t[5] |--> Main Menu\n");
    printf("\n\t::Enter a number (1-5): ");
    ch = getc(stdin);
    getc(stdin);
    printf("\n--------------------------------------------------------------------------------");
    switch (ch) {
        case '1':
            sortf();
            break;
        case '2':
            sortl();
            break;
        case '3':
            sortp();
            break;
        case '4':
            sortc();
        case '5':
        default:
            return;
    }
}

void sortf(void) {
    struct contact B;
    register int i, j;
    for (i = last - 1; i > 0; i--)
        for (j = 0; j < i; j++)
            if (strcmp(A[j].fname, A[j + 1].fname) > 0) {
                B = A[j];
                A[j] = A[j + 1];
                A[j + 1] = B;
            }
    printf("\nplease wait... .Contacts will be sorted by first names.");
    list2();
    printf("\n   ::Press any key to sort contact by another form... ");
    getc(stdin);
    sort();
}

void sortl(void) {
    struct contact B;
    register int i, j;
    for (i = last - 1; i > 0; i--)
        for (j = 0; j < i; j++)
            if (strcmp(A[j].lname, A[j + 1].lname) > 0) {
                B = A[j];
                A[j] = A[j + 1];
                A[j + 1] = B;
            }
    printf("\nplease wait... .Contacts will be sorted by last names.");
    list2();
    printf("\n   ::Press any key to sort contact by another form... ");
    getc(stdin);
    sort();
}

void sortp(void) {
    struct contact B;
    register int i, j;
    for (i = last - 1; i > 0; i--)
        for (j = 0; j < i; j++)
            if (strcmp(A[j].telep, A[j + 1].telep) > 0) {
                B = A[j];
                A[j] = A[j + 1];
                A[j + 1] = B;
            }
    printf("\nplease wait... .Contacts will be sorted by telephone numbers.");
    list2();
    printf("\n   ::Press any key to sort contact by another form... ");
    getc(stdin);
    sort();
}

void sortc(void) {
    struct contact B;
    register int i, j;
    for (i = last - 1; i > 0; i--)
        for (j = 0; j < i; j++)
            if (strcmp(A[j].cellp, A[j + 1].cellp) > 0) {
                B = A[j];
                A[j] = A[j + 1];
                A[j + 1] = B;
            }
    printf("\nPlease wait... .Contacts will be sort by cellphone numbers.");
    list2();
    printf("\n   ::Press any key to sort contact by another form... ");
    getc(stdin);
    sort();
}

void list() {
    register int i = 0;
    struct contact *b = A;
    clrscr();
    printf("\nPhone Book12<::>All Contacts List");
    printf("\n--------------------------------------------------------------------------------");
    gotoxy(1, 4);
    printf("Row");
    gotoxy(9, 4);
    printf("First Name");
    gotoxy(27, 4);
    printf("Last Name");
    gotoxy(44, 4);
    printf("Telephone");
    gotoxy(60, 4);
    printf("Cellphone");
    printf("\n--------------------------------------------------------------------------------");
    for (; b != NULL; b = b->next) {
        gotoxy(1, i + 6);
        printf("%3.3d", i + 1);
        gotoxy(9, i + 6);
        printf("%s", b->fname);
        gotoxy(28, i + 6);
        printf("%s", b->lname);
        gotoxy(44, i + 6);
        printf("%s", b->telep);
        gotoxy(60, i + 6);
        printf("%s", b->cellp);
        i++;
    }

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t    **********End Of list!**********");
    printf("\n\n\n   ::Press a key to return main page & continue program.|-->");
    getc(stdin);
}

void list2(void) {
    register int i;
    printf("\n--------------------------------------------------------------------------------");
    gotoxy(1, 18);
    printf("Row");
    gotoxy(9, 18);
    printf("First Name");
    gotoxy(27, 18);
    printf("Last Name");
    gotoxy(44, 18);
    printf("Telephone");
    gotoxy(60, 18);
    printf("Cellphone");
    printf("\n--------------------------------------------------------------------------------");
    for (i = 0; i < last; i++) {
        gotoxy(1, i + 21);
        printf("%3.3d", i + 1);
        gotoxy(9, i + 21);
        printf("%s", A[i].fname);
        gotoxy(27, i + 21);
        printf("%s", A[i].lname);
        gotoxy(44, i + 21);
        printf("%s", A[i].telep);
        gotoxy(60, i + 21);
        printf("%s", A[i].cellp);
    }
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t    **********End Of list!**********");
}

void help(void) {
    clrscr();
    gotoxy(26, 1);
    printf("--------------------------");
    gotoxy(26, 2);
    printf("|Welcome To Phone Book12!|");
    gotoxy(26, 3);
    printf("--------------------------");
    gotoxy(1, 5);
    printf("PhoneBook12>>[Help & Instructions] \n");
    printf("--------------------------------------------------------------------------------\n");
    printf("\nAbout Phonebook12\n *Version: 2.5\n *Valence: 50 Contacts(MAX)\n  *By: Morteza Zakeri (2011-12)");
    printf("\n\nThis program allows you to have a list of your contacts.\nIt also allows you to have your listing added or deleted from your list.\nOther features of this program include:\nSearch,edit,sort & list.\n\n   ::Press a key to return main page & continue program.|-->");
    getc(stdin);
}

void clrscr() {
    system("clear");
}

void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void get_input(int len, char str[]) {
    size_t new_len = getline(&ALLOCATE_MEMORY, &ALLOCATE_SIZE, stdin);
    ALLOCATE_MEMORY[new_len - 1] = '\0';
    strncpy(str, ALLOCATE_MEMORY, len);
}

void exitFreeMemory() {
    struct contact *temp = A;
    while (A != NULL) {
        temp = A;
        A = A->next;
        free(temp);
    }
    free(ALLOCATE_MEMORY);
    exit(1);
}