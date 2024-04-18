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
    char name[MAX_PACKAGE_NAME_LENGTH];
    char region[MAX_REGION_LENGTH];
    int duration;
    char description[MAX_DESCRIPTION_LENGTH];
    char itinerary[MAX_ITINERARY_LENGTH];
    float price;
    struct {
        char name[MAX_PACKAGE_NAME_LENGTH];
        char location[MAX_REGION_LENGTH];
    } destinations[MAX_DESTINATIONS];
} Package;

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
void addTouristFromFile(User *user) {
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
void deleteTouristFromFile(User *user) {
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

void addDestination(Destination destinations[], int *numDestinations) {
    if (*numDestinations < MAX_DESTINATIONS) {
        Destination newDestination;
        printf("Enter destination name: ");
        fgets(newDestination.name, sizeof(newDestination.name), stdin);
        newDestination.name[strcspn(newDestination.name, "\n")] = '\0'; // Remove trailing newline
        printf("Enter description: ");
        fgets(newDestination.description, sizeof(newDestination.description), stdin);
        newDestination.description[strcspn(newDestination.description, "\n")] = '\0'; // Remove trailing newline
        printf("Enter location: ");
        fgets(newDestination.location, sizeof(newDestination.location), stdin);
        newDestination.location[strcspn(newDestination.location, "\n")] = '\0'; // Remove trailing newline
        printf("Enter best time to visit: ");
        fgets(newDestination.bestTimeToVisit, sizeof(newDestination.bestTimeToVisit), stdin);
        newDestination.bestTimeToVisit[strcspn(newDestination.bestTimeToVisit, "\n")] = '\0'; // Remove trailing newline

        destinations[*numDestinations] = newDestination;
        (*numDestinations)++;
        printf("Destination added successfully.\n");
    } else {
        printf("Maximum number of destinations reached. Cannot add more.\n");
    }
}

void deleteDestination(Destination destinations[], int *numDestinations) {
    if (*numDestinations > 0) {
        printf("Enter the index of the destination to delete (1 to %d): ", *numDestinations);
        int index;
        scanf("%d", &index);
        clearInputBuffer();
        if (index >= 1 && index <= *numDestinations) {
            for (int i = index - 1; i < *numDestinations - 1; i++) {
                destinations[i] = destinations[i + 1];
            }
            (*numDestinations)--;
            printf("Destination deleted successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No destinations to delete.\n");
    }
}

// Function to add a new destination (for admin)
void addDestinationFromFile(Destination *destination) {
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
void deleteDestinationFromFile(Destination *destination) {
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

void addPackage(Package packages[], int *numPackages) {
    if (*numPackages < MAX_PACKAGES) {
        Package newPackage;
        printf("Enter package name: ");
        fgets(newPackage.name, sizeof(newPackage.name), stdin);
        newPackage.name[strcspn(newPackage.name, "\n")] = '\0'; // Remove trailing newline
        printf("Enter region: ");
        fgets(newPackage.region, sizeof(newPackage.region), stdin);
        newPackage.region[strcspn(newPackage.region, "\n")] = '\0'; // Remove trailing newline
        printf("Enter duration (in days): ");
        scanf("%d", &newPackage.duration);
        clearInputBuffer();
        printf("Enter description: ");
        fgets(newPackage.description, sizeof(newPackage.description), stdin);
        newPackage.description[strcspn(newPackage.description, "\n")] = '\0'; // Remove trailing newline
        printf("Enter itinerary: ");
        fgets(newPackage.itinerary, sizeof(newPackage.itinerary), stdin);
        newPackage.itinerary[strcspn(newPackage.itinerary, "\n")] = '\0'; // Remove trailing newline
        printf("Enter price: ");
        scanf("%f", &newPackage.price);
        clearInputBuffer();

        printf("Enter destinations (name and location):\n");
        for (int i = 0; i < MAX_DESTINATIONS_PER_PACKAGE; i++) {
            printf("Destination %d:\n", i + 1);
            printf("Name: ");
            fgets(newPackage.destinations[i].name, sizeof(newPackage.destinations[i].name), stdin);
            newPackage.destinations[i].name[strcspn(newPackage.destinations[i].name, "\n")] = '\0'; // Remove trailing newline
            printf("Location: ");
            fgets(newPackage.destinations[i].location, sizeof(newPackage.destinations[i].location), stdin);
            newPackage.destinations[i].location[strcspn(newPackage.destinations[i].location, "\n")] = '\0'; // Remove trailing newline
        }

        packages[*numPackages] = newPackage;
        (*numPackages)++;
        printf("Package added successfully.\n");
    } else {
        printf("Maximum number of packages reached. Cannot add more packages.\n");
    }
}

void deletePackage(Package packages[], int *numPackages) {
    if (*numPackages > 0) {
        printf("Enter the index of the package to delete (1 to %d): ", *numPackages);
        int index;
        scanf("%d", &index);
        clearInputBuffer();
        if (index >= 1 && index <= *numPackages) {
            for (int i = index - 1; i < *numPackages - 1; i++) {
                packages[i] = packages[i + 1];
            }
            (*numPackages)--;
            printf("Package deleted successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No packages to delete.\n");
    }
}


void addPackageFromFile(Package packages[], int *numPackages) {
    if (*numPackages < 10) {
        Package newPackage;
        printf("Enter package name: ");
        scanf(" %[^\n]", newPackage.name);
        printf("Enter region: ");
        scanf(" %[^\n]", newPackage.region);
        printf("Enter duration (in days): ");
        scanf("%d", &newPackage.duration);
        printf("Enter description: ");
        scanf(" %[^\n]", newPackage.description);
        printf("Enter itinerary: ");
        scanf(" %[^\n]", newPackage.itinerary);
        printf("Enter price: ");
        scanf("%f", &newPackage.price);

        printf("Enter destinations (name and location):\n");
        for (int i = 0; i < MAX_DESTINATIONS; i++) {
            printf("Destination %d:\n", i + 1);
            printf("Name: ");
            scanf(" %[^\n]", newPackage.destinations[i].name);
            printf("Location: ");
            scanf(" %[^\n]", newPackage.destinations[i].location);
        }

        packages[*numPackages] = newPackage;
        (*numPackages)++;
        printf("Package added successfully.\n");
    } else {
        printf("Maximum number of packages reached. Cannot add more packages.\n");
    }
}

void deletePackageFromFile(Package packages[], int *numPackages) {
    if (*numPackages > 0) {
        printf("Enter the index of the package to delete (1 to %d): ", *numPackages);
        int index;
        scanf("%d", &index);
        if (index >= 1 && index <= *numPackages) {
            for (int i = index - 1; i < *numPackages - 1; i++) {
                packages[i] = packages[i + 1];
            }
            (*numPackages)--;
            printf("Package deleted successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No packages to delete.\n");
    }
}

void viewPackagesFromFile(Package packages[], int numPackages) {
    if (numPackages > 0) {
        printf("List of Packages:\n");
        for (int i = 0; i < numPackages; i++) {
            printf("\nPackage %d:\n", i + 1);
            printf("Name: %s\n", packages[i].name);
            printf("Region: %s\n", packages[i].region);
            printf("Duration: %d days\n", packages[i].duration);
            printf("Description: %s\n", packages[i].description);
            printf("Itinerary: %s\n", packages[i].itinerary);
            printf("Price: %.2f\n", packages[i].price);
            printf("Destinations:\n");
            for (int j = 0; j < MAX_DESTINATIONS; j++) {
                printf("%d. %s, %s\n", j + 1, packages[i].destinations[j].name, packages[i].destinations[j].location);
            }
        }
    } else {
        printf("No packages to display.\n");
    }
}

void savePackagesToFile(Package packages[], int numPackages) {
    FILE *file = fopen("packages.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < numPackages; i++) {
        fprintf(file, "%s|%s|%d|%s|%s|%.2f|", packages[i].name, packages[i].region,
                packages[i].duration, packages[i].description, packages[i].itinerary, packages[i].price);

        for (int j = 0; j < MAX_DESTINATIONS; j++) {
            fprintf(file, "%s,%s|", packages[i].destinations[j].name, packages[i].destinations[j].location);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

void loadPackagesFromFile(Package packages[], int *numPackages) {
    FILE *file = fopen("packages.txt", "r");
    if (file == NULL) {
        printf("No packages found in the database.\n");
        return;
    }

    while (!feof(file)) {
        fscanf(file, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%f|",
               packages[*numPackages].name, packages[*numPackages].region,
               &packages[*numPackages].duration, packages[*numPackages].description,
               packages[*numPackages].itinerary, &packages[*numPackages].price);

        for (int i = 0; i < MAX_DESTINATIONS; i++) {
            fscanf(file, "%[^,],%[^|]|", packages[*numPackages].destinations[i].name,
                   packages[*numPackages].destinations[i].location);
        }

        (*numPackages)++;
    }

    fclose(file);
}

void addFlight(Flight flights[], int *numFlights) {
    if (*numFlights < MAX_FLIGHTS) {
        Flight newFlight;
        printf("Enter airline: ");
        fgets(newFlight.airline, sizeof(newFlight.airline), stdin);
        newFlight.airline[strcspn(newFlight.airline, "\n")] = '\0'; // Remove trailing newline
        printf("Enter flight number: ");
        fgets(newFlight.flightNumber, sizeof(newFlight.flightNumber), stdin);
        newFlight.flightNumber[strcspn(newFlight.flightNumber, "\n")] = '\0'; // Remove trailing newline
        printf("Enter departure: ");
        fgets(newFlight.departure, sizeof(newFlight.departure), stdin);
        newFlight.departure[strcspn(newFlight.departure, "\n")] = '\0'; // Remove trailing newline
        printf("Enter arrival: ");
        fgets(newFlight.arrival, sizeof(newFlight.arrival), stdin);
        newFlight.arrival[strcspn(newFlight.arrival, "\n")] = '\0'; // Remove trailing newline
        printf("Enter departure time: ");
        fgets(newFlight.departureTime, sizeof(newFlight.departureTime), stdin);
        newFlight.departureTime[strcspn(newFlight.departureTime, "\n")] = '\0'; // Remove trailing newline
        printf("Enter arrival time: ");
        fgets(newFlight.arrivalTime, sizeof(newFlight.arrivalTime), stdin);
        newFlight.arrivalTime[strcspn(newFlight.arrivalTime, "\n")] = '\0'; // Remove trailing newline
        printf("Enter cost: ");
        scanf("%f", &newFlight.cost);
        clearInputBuffer();

        flights[*numFlights] = newFlight;
        (*numFlights)++;
        printf("Flight added successfully.\n");
    } else {
        printf("Maximum number of flights reached. Cannot add more flights.\n");
    }
}

void deleteFlight(Flight flights[], int *numFlights) {
    if (*numFlights > 0) {
        printf("Enter the index of the flight to delete (1 to %d): ", *numFlights);
        int index;
        scanf("%d", &index);
        clearInputBuffer();
        if (index >= 1 && index <= *numFlights) {
            for (int i = index - 1; i < *numFlights - 1; i++) {
                flights[i] = flights[i + 1];
            }
            (*numFlights)--;
            printf("Flight deleted successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No flights to delete.\n");
    }
}


// Function to add a new flight (for admin)
void addFlightToFile(Flight *flight) {
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
void deleteFlightFromFile(Flight *flight) {
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

void addHotel(Hotel hotels[], int *numHotels) {
    if (*numHotels < MAX_HOTELS) {
        Hotel newHotel;
        printf("Enter hotel name: ");
        fgets(newHotel.name, sizeof(newHotel.name), stdin);
        newHotel.name[strcspn(newHotel.name, "\n")] = '\0'; // Remove trailing newline
        printf("Enter location: ");
        fgets(newHotel.location, sizeof(newHotel.location), stdin);
        newHotel.location[strcspn(newHotel.location, "\n")] = '\0'; // Remove trailing newline
        printf("Enter check-in date: ");
        fgets(newHotel.checkInDate, sizeof(newHotel.checkInDate), stdin);
        newHotel.checkInDate[strcspn(newHotel.checkInDate, "\n")] = '\0'; // Remove trailing newline
        printf("Enter check-out date: ");
        fgets(newHotel.checkOutDate, sizeof(newHotel.checkOutDate), stdin);
        newHotel.checkOutDate[strcspn(newHotel.checkOutDate, "\n")] = '\0'; // Remove trailing newline
        printf("Enter number of nights: ");
        scanf("%d", &newHotel.numNights);
        clearInputBuffer();
        printf("Enter cost per night: ");
        scanf("%f", &newHotel.costPerNight);
        clearInputBuffer();

        hotels[*numHotels] = newHotel;
        (*numHotels)++;
        printf("Hotel added successfully.\n");
    } else {
        printf("Maximum number of hotels reached. Cannot add more hotels.\n");
    }
}

void deleteHotel(Hotel hotels[], int *numHotels) {
    if (*numHotels > 0) {
        printf("Enter the index of the hotel to delete (1 to %d): ", *numHotels);
        int index;
        scanf("%d", &index);
        clearInputBuffer();
        if (index >= 1 && index <= *numHotels) {
            for (int i = index - 1; i < *numHotels - 1; i++) {
                hotels[i] = hotels[i + 1];
            }
            (*numHotels)--;
            printf("Hotel deleted successfully.\n");
        } else {
            printf("Invalid index.\n");
        }
    } else {
        printf("No hotels to delete.\n");
    }
}


// Function to add a new hotel (for admin)
void addHotelToFile(Hotel *hotel) {
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
void deleteHotelFromFile(Hotel *hotel) {
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
