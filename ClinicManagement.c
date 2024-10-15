#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef signed int s32;

// Structure to hold patient information
typedef struct Patient {
    u8 name[50];
    u8 gender[10];
    s32 age;
    s32 id;
    struct Patient* next;
} Patient;

// Structure to hold appointment information
typedef struct Appointment {
    s32 patientId;
    u8 timeSlot[20];
    struct Appointment* next;
} Appointment;

// Head pointers for linked lists
Patient* patientList = NULL;
Appointment* appointmentList = NULL;

// Function prototypes
void addPatient();
void editPatient();
void reserveSlot();
void cancelReservation();
void viewPatientRecord();
void viewTodaysReservations();
u8 checkPassword();

int main() {
    s32 choice, isAdmin;

    printf("Welcome to Clinic Management System\n");
    printf("Choose mode: 1. Admin 2. User\n");
    scanf("%d", &isAdmin);

    if (isAdmin == 1) {
        if (!checkPassword()) {
            printf("Access Denied. Exiting...\n");
            return 0;
        }
    }

    while (1) {
        if (isAdmin == 1) {
            printf("\nAdmin Menu:\n");
            printf("1. Add Patient\n2. Edit Patient\n3. Reserve Slot\n4. Cancel Reservation\n5. Exit\n");
            printf("Choose an option: ");
            scanf("%d", &choice);

            switch(choice) {
                case 1: addPatient(); break;
                case 2: editPatient(); break;
                case 3: reserveSlot(); break;
                case 4: cancelReservation(); break;
                case 5: exit(0);
                default: printf("Invalid choice.\n");
            }
        } else {
            printf("\nUser Menu:\n");
            printf("1. View Patient Record\n2. View Today's Reservations\n3. Exit\n");
            printf("Choose an option: ");
            scanf("%d", &choice);

            switch(choice) {
                case 1: viewPatientRecord(); break;
                case 2: viewTodaysReservations(); break;
                case 3: exit(0);
                default: printf("Invalid choice.\n");
            }
        }
    }
    return 0;
}

// Function to check the password
u8 checkPassword() {
    u8 password[10];
    const u8 correctPassword[] = "1234";
    s32 attempts = 0;

    while (attempts < 3) {
        printf("Enter password: ");
        scanf("%s", password);
        if (strcmp(password, correctPassword) == 0) {
            return 1; // Access granted
        }
        attempts++;
        printf("Incorrect password. Attempts left: %d\n", 3 - attempts);
    }
    return 0; // Access denied
}

// Function to add a patient
void addPatient() {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    printf("Enter patient name: ");
    scanf("%s", newPatient->name);
    printf("Enter patient age: ");
    scanf("%d", &newPatient->age);
    printf("Enter patient gender: ");
    scanf("%s", newPatient->gender);
    printf("Enter patient ID: ");
    scanf("%d", &newPatient->id);
    
    // Check for unique ID
    Patient* temp = patientList;
    while (temp) {
        if (temp->id == newPatient->id) {
            printf("ID already exists. Patient not added.\n");
            free(newPatient);
            return;
        }
        temp = temp->next;
    }

    newPatient->next = patientList;
    patientList = newPatient;
    printf("Patient added successfully.\n");
}

// Function to edit a patient
void editPatient() {
    s32 id;
    printf("Enter patient ID to edit: ");
    scanf("%d", &id);
    
    Patient* temp = patientList;
    while (temp) {
        if (temp->id == id) {
            printf("Editing patient record...\n");
            printf("Enter new name: ");
            scanf("%s", temp->name);
            printf("Enter new age: ");
            scanf("%d", &temp->age);
            printf("Enter new gender: ");
            scanf("%s", temp->gender);
            printf("Patient record updated.\n");
            return;
        }
        temp = temp->next;
    }
    printf("Patient ID not found.\n");
}

// Function to reserve a slot
void reserveSlot() {
    s32 id;
    u8 timeSlot[20];
    printf("Enter patient ID to reserve a slot: ");
    scanf("%d", &id);
    
    printf("Available slots:\n");
    const char* slots[] = {"2pm to 2:30pm", "2:30pm to 3pm", "3pm to 3:30pm", "4pm to 4:30pm", "4:30pm to 5pm"};
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, slots[i]);
    }
    
    printf("Enter slot number: ");
    int slotNum;
    scanf("%d", &slotNum);

    if (slotNum < 1 || slotNum > 5) {
        printf("Invalid slot number.\n");
        return;
    }

    snprintf(timeSlot, sizeof(timeSlot), "%s", slots[slotNum - 1]);

    Appointment* newAppointment = (Appointment*)malloc(sizeof(Appointment));
    newAppointment->patientId = id;
    snprintf(newAppointment->timeSlot, sizeof(newAppointment->timeSlot), "%s", timeSlot);
    newAppointment->next = appointmentList;
    appointmentList = newAppointment;

    printf("Slot reserved successfully.\n");
}

// Function to cancel a reservation
void cancelReservation() {
    s32 id;
    printf("Enter patient ID to cancel reservation: ");
    scanf("%d", &id);

    Appointment** current = &appointmentList;
    while (*current) {
        if ((*current)->patientId == id) {
            Appointment* temp = *current;
            *current = (*current)->next;
            free(temp);
            printf("Reservation cancelled successfully.\n");
            return;
        }
        current = &(*current)->next;
    }
    printf("No reservation found for patient ID.\n");
}

// Function to view patient record
void viewPatientRecord() {
    s32 id;
    printf("Enter patient ID: ");
    scanf("%d", &id);

    Patient* temp = patientList;
    while (temp) {
        if (temp->id == id) {
            printf("Patient Record:\n");
            printf("Name: %s\nAge: %d\nGender: %s\nID: %d\n", temp->name, temp->age, temp->gender, temp->id);
            return;
        }
        temp = temp->next;
    }
    printf("Patient ID not found.\n");
}

// Function to view today's reservations
void viewTodaysReservations() {
    Appointment* temp = appointmentList;
    if (!temp) {
        printf("No reservations for today.\n");
        return;
    }

    printf("Today's Reservations:\n");
    while (temp) {
        printf("Patient ID: %d, Time Slot: %s\n", temp->patientId, temp->timeSlot);
        temp = temp->next;
    }
}