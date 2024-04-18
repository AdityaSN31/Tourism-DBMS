#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_MOBILE_LENGTH 15
#define MAX_DESCRIPTION_LENGTH 100
#define MAX_LOCATION_LENGTH 50
#define MAX_PLACES_TO_VISIT 5
#define MAX_DATE_LENGTH 20
#define MAX_REVIEW_LENGTH 200

typedef enum {
    USER,
    ADMIN
} UserRole;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char mobile[MAX_MOBILE_LENGTH];
    UserRole role;
} User;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char location[MAX_LOCATION_LENGTH];
    char placesToVisit[MAX_PLACES_TO_VISIT][MAX_DESCRIPTION_LENGTH];
    char bestTimeToVisit[MAX_DESCRIPTION_LENGTH];
} Destination;

typedef struct {
    char airline[MAX_NAME_LENGTH];
    char flightNumber[MAX_NAME_LENGTH];
    char departure[MAX_LOCATION_LENGTH];
    char arrival[MAX_LOCATION_LENGTH];
    char departureTime[MAX_DATE_LENGTH];
    char arrivalTime[MAX_DATE_LENGTH];
    float cost;
} Flight;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char location[MAX_LOCATION_LENGTH];
    char checkInDate[MAX_DATE_LENGTH];
    char checkOutDate[MAX_DATE_LENGTH];
    int numNights;
    float costPerNight;
} Hotel;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

bool validateName(const char *name) {
    return strlen(name) > 0 && strlen(name) < MAX_NAME_LENGTH;
}

bool validateEmail(const char *email) {
    // Simple email validation, can be improved for stricter validation
    return strlen(email) > 0 && strlen(email) < MAX_EMAIL_LENGTH && strchr(email, '@') != NULL;
}

bool validateMobile(const char *mobile) {
    // Simple mobile validation, can be improved for stricter validation
    return strlen(mobile) > 0 && strlen(mobile) < MAX_MOBILE_LENGTH;
}

bool validateDate(const char *date) {
    // Simple date validation, can be improved for stricter validation
    return strlen(date) > 0 && strlen(date) < MAX_DATE_LENGTH;
}

// Function to authenticate user
UserRole authenticateUser() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove trailing newline
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        return ADMIN;
    } else {
        return USER;
    }
}

// Function to add a new tourist
void addTourist(User *user) {
    printf("Enter tourist name: ");
    fgets(user->name, sizeof(user->name), stdin);
    user->name[strcspn(user->name, "\n")] = '\0'; // Remove trailing newline
    printf("Enter email: ");
    fgets(user->email, sizeof(user->email), stdin);
    user->email[strcspn(user->email, "\n")] = '\0'; // Remove trailing newline
    printf("Enter mobile number: ");
    fgets(user->mobile, sizeof(user->mobile), stdin);
    user->mobile[strcspn(user->mobile, "\n")] = '\0'; // Remove trailing newline
    user->role = USER;

    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    fprintf(file, "%s %s %s %d\n", user->name, user->email, user->mobile, user->role);
    fclose(file);
}

// Function to delete a tourist
void deleteTourist(User *user) {
    printf("Tourist %s deleted successfully.\n", user->name);

    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp_users.txt", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_EMAIL_LENGTH + MAX_MOBILE_LENGTH + 10];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s %s %s %d", user->name, user->email, user->mobile, user->role);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("users.txt");
    rename("temp_users.txt", "users.txt");
}

// Function to add a new destination (for admin)
void addDestination(Destination *destination) {
    printf("Enter destination name: ");
    fgets(destination->name, sizeof(destination->name), stdin);
    destination->name[strcspn(destination->name, "\n")] = '\0'; // Remove trailing newline
    printf("Enter description: ");
    fgets(destination->description, sizeof(destination->description), stdin);
    destination->description[strcspn(destination->description, "\n")] = '\0'; // Remove trailing newline
    printf("Enter location: ");
    fgets(destination->location, sizeof(destination->location), stdin);
    destination->location[strcspn(destination->location, "\n")] = '\0'; // Remove trailing newline
    printf("Enter best time to visit: ");
    fgets(destination->bestTimeToVisit, sizeof(destination->bestTimeToVisit), stdin);
    destination->bestTimeToVisit[strcspn(destination->bestTimeToVisit, "\n")] = '\0'; // Remove trailing newline
    printf("Enter places to visit (max %d):\n", MAX_PLACES_TO_VISIT);
    for (int i = 0; i < MAX_PLACES_TO_VISIT; ++i) {
        printf("Place %d: ", i + 1);
        fgets(destination->placesToVisit[i], sizeof(destination->placesToVisit[i]), stdin);
        destination->placesToVisit[i][strcspn(destination->placesToVisit[i], "\n")] = '\0'; // Remove trailing newline
    }

    FILE *file = fopen("destinations.txt", "a");
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    fprintf(file, "%s %s %s %s ", destination->name, destination->description, destination->location,
            destination->bestTimeToVisit);
    for (int i = 0; i < MAX_PLACES_TO_VISIT; ++i) {
        fprintf(file, "%s ", destination->placesToVisit[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

// Function to delete a destination
void deleteDestination(Destination *destination) {
    printf("Destination %s deleted successfully.\n", destination->name);

    FILE *file = fopen("destinations.txt", "r");
    FILE *tempFile = fopen("temp_destinations.txt", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_DESCRIPTION_LENGTH + MAX_LOCATION_LENGTH + MAX_PLACES_TO_VISIT * (MAX_DESCRIPTION_LENGTH + 1) + MAX_DESCRIPTION_LENGTH + 10];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s %s %s %s ", destination->name, destination->description, destination->location, destination->bestTimeToVisit);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("destinations.txt");
    rename("temp_destinations.txt", "destinations.txt");
}

// Function to add a new flight (for admin)
void addFlight(Flight *flight) {
    printf("Enter airline: ");
    fgets(flight->airline, sizeof(flight->airline), stdin);
    flight->airline[strcspn(flight->airline, "\n")] = '\0'; // Remove trailing newline
    printf("Enter flight number: ");
    fgets(flight->flightNumber, sizeof(flight->flightNumber), stdin);
    flight->flightNumber[strcspn(flight->flightNumber, "\n")] = '\0'; // Remove trailing newline
    printf("Enter departure: ");
    fgets(flight->departure, sizeof(flight->departure), stdin);
    flight->departure[strcspn(flight->departure, "\n")] = '\0'; // Remove trailing newline
    printf("Enter arrival: ");
    fgets(flight->arrival, sizeof(flight->arrival), stdin);
    flight->arrival[strcspn(flight->arrival, "\n")] = '\0'; // Remove trailing newline
    printf("Enter departure time: ");
    fgets(flight->departureTime, sizeof(flight->departureTime), stdin);
    flight->departureTime[strcspn(flight->departureTime, "\n")] = '\0'; // Remove trailing newline
    printf("Enter arrival time: ");
    fgets(flight->arrivalTime, sizeof(flight->arrivalTime), stdin);
    flight->arrivalTime[strcspn(flight->arrivalTime, "\n")] = '\0'; // Remove trailing newline
    printf("Enter cost: ");
    scanf("%f", &flight->cost);
    clearInputBuffer();

    FILE *file = fopen("flights.txt", "a");
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    fprintf(file, "%s %s %s %s %s %s %.2f\n", flight->airline, flight->flightNumber, flight->departure,
            flight->arrival, flight->departureTime, flight->arrivalTime, flight->cost);
    fclose(file);
}

// Function to delete a flight
void deleteFlight(Flight *flight) {
    printf("Flight %s %s deleted successfully.\n", flight->airline, flight->flightNumber);

    FILE *file = fopen("flights.txt", "r");
    FILE *tempFile = fopen("temp_flights.txt", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_NAME_LENGTH + MAX_LOCATION_LENGTH * 2 + MAX_DATE_LENGTH * 2 + 20];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s %s %s %s %s %s %.2f", flight->airline, flight->flightNumber, flight->departure, flight->arrival, flight->departureTime, flight->arrivalTime, flight->cost);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("flights.txt");
    rename("temp_flights.txt", "flights.txt");
}

// Function to add a new hotel (for admin)
void addHotel(Hotel *hotel) {
    printf("Enter hotel name: ");
    fgets(hotel->name, sizeof(hotel->name), stdin);
    hotel->name[strcspn(hotel->name, "\n")] = '\0'; // Remove trailing newline
    printf("Enter location: ");
    fgets(hotel->location, sizeof(hotel->location), stdin);
    hotel->location[strcspn(hotel->location, "\n")] = '\0'; // Remove trailing newline
    printf("Enter check-in date: ");
    fgets(hotel->checkInDate, sizeof(hotel->checkInDate), stdin);
    hotel->checkInDate[strcspn(hotel->checkInDate, "\n")] = '\0'; // Remove trailing newline
    printf("Enter check-out date: ");
    fgets(hotel->checkOutDate, sizeof(hotel->checkOutDate), stdin);
    hotel->checkOutDate[strcspn(hotel->checkOutDate, "\n")] = '\0'; // Remove trailing newline
    printf("Enter number of nights: ");
    scanf("%d", &hotel->numNights);
    clearInputBuffer();
    printf("Enter cost per night: ");
    scanf("%f", &hotel->costPerNight);
    clearInputBuffer();

    FILE *file = fopen("hotels.txt", "a");
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    fprintf(file, "%s %s %s %s %d %.2f\n", hotel->name, hotel->location, hotel->checkInDate,
            hotel->checkOutDate, hotel->numNights, hotel->costPerNight);
    fclose(file);
}

// Function to delete a hotel
void deleteHotel(Hotel *hotel) {
    printf("Hotel %s deleted successfully.\n", hotel->name);

    FILE *file = fopen("hotels.txt", "r");
    FILE *tempFile = fopen("temp_hotels.txt", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_LOCATION_LENGTH + MAX_DATE_LENGTH * 2 + 15];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s %s %s %s %d %.2f", hotel->name, hotel->location, hotel->checkInDate, hotel->checkOutDate, hotel->numNights, hotel->costPerNight);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("hotels.txt");
    rename("temp_hotels.txt", "hotels.txt");
}

// Function to read data from file and display destinations
void displayDestinationsFromFile() {
    FILE *file = fopen("destinations.txt", "r");
    if (file == NULL) {
        error("Unable to open destinations file");
    }

    Destination destination;
    while (fscanf(file, "%s %s %s %s", destination.name, destination.description,
                  destination.location, destination.bestTimeToVisit) != EOF) {
        printf("Destination Name: %s\n", destination.name);
        printf("Description: %s\n", destination.description);
        printf("Location: %s\n", destination.location);
        printf("Best Time to Visit: %s\n", destination.bestTimeToVisit);
    }
    fclose(file);
}

int main() {
    UserRole role = authenticateUser();

    if (role == ADMIN) {
        printf("Welcome, Admin!\n");

        // Admin functionalities
        Destination destination;
        addDestination(&destination);

        Flight flight;
        addFlight(&flight);

        Hotel hotel;
        addHotel(&hotel);

        User user;
        addTourist(&user);
    } else {
        printf("Welcome, User!\n");

        // User functionalities
        displayDestinationsFromFile();
    }

    return 0;
}
