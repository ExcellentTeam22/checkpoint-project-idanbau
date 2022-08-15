#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

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

void searchBy(int (*check)(const struct contact *current, const char *value));

int fNameMatch(const struct contact *current, const char *pattern);

int lNameMatch(const struct contact *current, const char *pattern);

int telpMatch(const struct contact *current, const char *pattern);

int cellpMatch(const struct contact *current, const char *pattern);

void list(void);

void list2(void);

void sort(void);

struct contact *mergeSort(struct contact *head, int (*compare)(const struct contact *r, const struct contact *l));

void split(struct contact *list, struct contact **list1, struct contact **list2);

struct contact *
merge(struct contact *list1, struct contact *list2, int (*compare)(const struct contact *r, const struct contact *l));

int fNameCmp(const struct contact *r, const struct contact *l);

int lNameCmp(const struct contact *r, const struct contact *l);

int telpCmp(const struct contact *r, const struct contact *l);

int cellpCmp(const struct contact *r, const struct contact *l);

void help(void);

void get_input(int len, char str[]);

void exitFreeMemory();

int last;

int main() {
    int count = 1;
    char n;
    while (1) {
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
        n = (char) getc(stdin);
        getc(stdin);
        switch (n) {
            case '1':
                insert();
                break;
            case '2':
                delet();
                break;
            case '3':
                edit();
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
                return EXIT_SUCCESS;
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
    ssize_t line_size;
    FILE *fp = fopen("phonebookFile.txt", "r");
    if (!fp) {
        {
            fprintf(stderr, "Error opening file '%s'\n", "phonebookfile.txt");
            exit(EXIT_FAILURE);
        }
    }
    clrscr();
    line_size = getline(&ALLOCATE_MEMORY, &ALLOCATE_SIZE, fp);
    while (line_size >= 0) {
        struct contact *new_contact = (struct contact *) malloc(sizeof(struct contact));
        new_contact->next = A;
        sscanf(ALLOCATE_MEMORY, "%s\t%s\t%s\t%s", new_contact->fname, new_contact->lname, new_contact->telep, new_contact->cellp);
        A = new_contact;
        last++;
        line_size = getline(&ALLOCATE_MEMORY, &ALLOCATE_SIZE, fp);
    }
    fclose(fp);
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
        ch = (char) fgetc(stdin);
        fgetc(stdin);
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
                ch = (char) fgetc(stdin);
                fgetc(stdin);
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
            ch = (char) fgetc(stdin);
            fgetc(stdin);
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
        }
        temp = temp->next;
    }
    printf(find == 0 ? "\t\t\n<<This contact does not exist or you chose not to Edit it.>>"
                     : "\t\t\n<<Target contact was successfully updated!>>");
    printf("\n\n\n   ::Press a key to return main page & continue program|-->");
    getc(stdin);
}

void search(void) {
    char ch;
    while (1) {
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
        ch = (char) fgetc(stdin);
        fgetc(stdin);
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

void searchBy(int (*check)(const struct contact *current, const char *value)) {
    register int find = 0;
    struct contact *temp = A;
    printf("Enter a first pattern to search:");
    size_t size = getline(&ALLOCATE_MEMORY, &ALLOCATE_SIZE, stdin);
    ALLOCATE_MEMORY[size - 1] = '\0';
    while (temp != NULL) {
        if (check(temp, ALLOCATE_MEMORY) == 0) {
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

int fNameMatch(const struct contact *current, const char *pattern) {
    return strcmp(current->fname, pattern);
}

int lNameMatch(const struct contact *current, const char *pattern) {
    return strcmp(current->lname, pattern);
}

int telpMatch(const struct contact *current, const char *pattern) {
    return strcmp(current->telep, pattern);
}

int cellpMatch(const struct contact *current, const char *pattern) {
    return strcmp(current->cellp, pattern);
}

void sort(void) {
    char ch;
    while (1) {
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
        ch = (char) fgetc(stdin);
        fgetc(stdin);
        printf("\n--------------------------------------------------------------------------------");
        switch (ch) {
            case '1':
                A = mergeSort(A, &fNameCmp);
                break;
            case '2':
                A = mergeSort(A, &lNameCmp);
                break;
            case '3':
                A = mergeSort(A, &telpCmp);
                break;
            case '4':
                A = mergeSort(A, &cellpCmp);
            case '5':
            default:
                return;
        }
    }
}

struct contact *mergeSort(struct contact *head, int (*compare)(const struct contact *r, const struct contact *l)) {
    struct contact *list1 = NULL, *list_2 = NULL, *list;
    if (A == NULL || A->next == NULL) return head;
    split(head, &list1, &list_2);
    list1 = mergeSort(list1, compare);
    list_2 = mergeSort(list_2, compare);
    list = merge(list1, list_2, compare);
    printf("\nplease wait... .Contacts will be sorted.");
    list2();
    printf("\n   ::Press any key to sort contact by another form... ");
    getc(stdin);
    return list;

}

void split(struct contact *list, struct contact **list1, struct contact **list2) {
    bool to_list1 = false;
    struct contact *temp;
    while (list != NULL) {
        temp = list;
        list = list->next;
        if (to_list1) {
            temp->next = *list1;
            *list1 = temp;
        } else {
            temp->next = *list2;
            *list2 = temp;
        }
        to_list1 = !to_list1;
    }
}

struct contact *
merge(struct contact *list1, struct contact *list2, int (*compare)(const struct contact *r, const struct contact *l)) {
    struct contact *list = NULL, *last_contact, *temp;
    while (list1 != NULL && list2 != NULL) {
        if (compare(list1, list2) <= 0) {
            temp = list1;
            list1 = list1->next;
        } else {
            temp = list2;
            list2 = list2->next;
        }
        temp->next = NULL;
        if (list == NULL) {
            last_contact = list = temp;
        } else {
            last_contact->next = temp;
            last_contact = temp;
        }
    }
    last_contact->next = list1 == NULL ? list2 : list1;
    return list;
}

int fNameCmp(const struct contact *r, const struct contact *l) {
    return strcmp(r->fname, l->fname);
}

int lNameCmp(const struct contact *r, const struct contact *l) {
    return strcmp(r->lname, l->lname);
}

int telpCmp(const struct contact *r, const struct contact *l) {
    return strcmp(r->telep, l->telep);
}

int cellpCmp(const struct contact *r, const struct contact *l) {
    return strcmp(r->cellp, l->cellp);
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
        printf("%s\n", b->cellp);
        i++;
    }

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t    **********End Of list!**********");
    printf("\n\n\n   ::Press a key to return main page & continue program.|-->");
    getc(stdin);
}

void list2(void) {
    register int i = 0;
    struct contact *temp;
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
    for (temp = A; temp != NULL; temp = temp->next) {
        gotoxy(1, i + 21);
        printf("%3.3d", i + 1);
        gotoxy(9, i + 21);
        printf("%s", temp->fname);
        gotoxy(27, i + 21);
        printf("%s", temp->lname);
        gotoxy(44, i + 21);
        printf("%s", temp->telep);
        gotoxy(60, i + 21);
        printf("%s\n", temp->cellp);
        i++;
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