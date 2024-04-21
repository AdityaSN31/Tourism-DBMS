#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>


#define MAX_LENGTH_LENGTH 50
#define MAX_FIELD_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_MOBILE_LENGTH 15
#define MAX_DESCRIPTION_LENGTH 500
#define MAX_LOCATION_LENGTH 50
#define MAX_PLACES_TO_VISIT 5
#define MAX_DATE_LENGTH 20
#define MAX_REVIEW_LENGTH 200
#define MAX_PACKAGE_NAME_LENGTH 100
#define MAX_REGION_LENGTH 50
#define MAX_ITINERARY_LENGTH 1000
#define MAX_DESTINATIONS 50
#define SALT_SIZE 32
#define HASH_SIZE 64
#define MAX_CSV_LINE_LENGTH 100
#define MAX_PACKAGES 50
#define MAX_DESTINATIONS_PER_PACKAGE 50
#define MAX_FLIGHTS 50
#define MAX_HOTELS 100
#define MAX_FEEDBACK_LENGTH 100
#define MAX_FILE_NAME_LENGTH 50
#define ROTR(x, n) (((x) >> (n)) | ((x) << (64 - (n))))  // Rotate right (circular right shift) operation
#define MAX_ROLE_LENGTH 20
#define MAX_LINE_LENGTH 1024 // Adjust this size as needed
#define USERS_FILE "User.csv" // Define the filename appropriately
#define MAX_DISTANCECC_LENGTH 50
#define MAX_CPN_LENGTH 50
#define MAX_TEXT_LENGTH 50
#define MAX_INPUT_LENGTH 1000
#define MAX_BEST_TIME_TO_VISIT 50
#define MAX_FIELDS 5 // We have 5 fields in our CSV: Name, Description, Location, Places to Visit, Best Time to Visit
#define MAX_NAME_LENGTH 100
#define MAX_CITY_LENGTH 50
#define MAX_AIRLINE_LENGTH 50
#define MAX_FLIGHT_NUMBER_LENGTH 20
#define MAX_TIME_LENGTH 10
#define MAX_COST_LENGTH 20
#define MAX_DETAIL_LENGTH 50



// SHA-512 Constants
const uint64_t K[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

// SHA-512 Initial Hash Values
const uint64_t H[8] = {
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
    0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

typedef enum {
    USER,
    ADMIN,
    INVALID
} UserRole;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char mobile[MAX_MOBILE_LENGTH];
    UserRole role;
} User;

typedef struct {
    char userName[MAX_NAME_LENGTH];
    char password[MAX_EMAIL_LENGTH];
    UserRole role;
} Login;

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
    char price[MAX_CPN_LENGTH];
    struct {
        char name[MAX_PACKAGE_NAME_LENGTH];
        char location[MAX_REGION_LENGTH];
    } destinations[MAX_DESTINATIONS];
} Package;

typedef struct {
    char name[MAX_PACKAGE_NAME_LENGTH];
    char region[MAX_REGION_LENGTH];
    int duration;
    char description[MAX_DESCRIPTION_LENGTH];
    char itinerary[MAX_ITINERARY_LENGTH];
    char price[MAX_CPN_LENGTH];
    char destinations[MAX_DESTINATIONS];
} package;

typedef struct {
    char city[MAX_NAME_LENGTH];
    char airline[MAX_NAME_LENGTH];
    char flightNumber[MAX_NAME_LENGTH];
    char departure[MAX_LOCATION_LENGTH];
    char arrival[MAX_LOCATION_LENGTH];
    char departureTime[MAX_DATE_LENGTH];
    char arrivalTime[MAX_DATE_LENGTH];
    char cost[MAX_CPN_LENGTH];
} Flight;

typedef struct {
    char location[MAX_LOCATION_LENGTH];
    char name[MAX_NAME_LENGTH];
    char costPerNight[MAX_CPN_LENGTH];
    char distance[MAX_DISTANCECC_LENGTH];
} Hotel;


// Define a custom type BookingType to represent different types of bookings
typedef enum {
    FLIGHT_BOOKING,
    HOTEL_BOOKING,
    PACKAGE_BOOKING
} BookingType;

typedef struct {
    BookingType type;
    void *bookingObject; // Pointer to the booked object
    int numPeople;
    char bookingDate[MAX_DATE_LENGTH];
    char userName[MAX_NAME_LENGTH]; // Field to store the username
    char bookingDetail[MAX_DETAIL_LENGTH]; // Additional details, e.g., flight number or hotel name
} Booking;

// Enum to represent feedback types
typedef enum {
    FLIGHT,
    HOTEL,
    PACKAGE
} FeedbackType;

// Define a structure for feedback
typedef struct {
    char username[MAX_NAME_LENGTH];
    char text[MAX_FEEDBACK_LENGTH];
    int rating; // New field for rating out of 5
} Feedback;


/*
clearInputBuffer(): This function clears the input buffer by reading characters until encountering a newline or EOF.
error(const char *message): It prints an error message to the standard error stream and exits the program with a failure status.
validateName(const char *name): This function validates a name string to ensure it's not empty and doesn't exceed the maximum length.
validateEmail(const char *email): It validates an email string, checking if it's not empty, doesn't exceed the maximum length, contains the '@' symbol, and has at least one dot '.' after the '@' symbol.
validateMobile(const char *mobile): This function validates a mobile number string to ensure it's not empty and doesn't exceed the maximum length.
validateDate(const char *date): It validates a date string to ensure it's not empty and doesn't exceed the maximum length.
generateSalt(char *salt): This function generates a random salt of a specified size.
computeHash(const char *password, const char *salt, char *hash): It computes the hash of a password concatenated with a salt using the SHA-512 hashing algorithm.
authenticateUser(const char *username, const char *password): This function performs user authentication by comparing the computed hash of the provided password with the stored hash associated with the username.

addTourist(User *user): Prompts the user to input tourist details (name, email, mobile number) and sets the user's role as "USER".
deleteTourist(User *user): Prints a message indicating the successful deletion of a tourist. Since the data is not persisted beyond program execution, no further action is taken.
addTouristFromFile(User *user): Adds a tourist's details to a CSV file, including name, email, mobile number, and role.
deleteTouristFromFile(User *user): Deletes a tourist's details from the CSV file based on the provided user data.

addDestination(Destination destinations[], int *numDestinations): Allows the user to add a new destination to an array of destinations, including details like name, description, location, and best time to visit.
deleteDestination(Destination destinations[], int *numDestinations): Deletes a destination from the array of destinations based on the user's input index.
addDestinationFromUser(Destination *destination): Prompts the user to input destination details and saves them to a CSV file.
deleteDestinationFromUser(Destination *destination): Deletes a destination from the CSV file based on the provided destination data.
addDestinationFromFile(Destination destinations[], int *numDestinations): Reads destination data from a CSV file and populates an array of destinations.
deleteDestinationFromFile(const char *destinationName): Deletes a destination from the CSV file based on the provided destination name.
viewDestinationsFromFile(): Displays all destinations stored in the CSV file, including their details like name, description, location, places to visit, and best time to visit.

addPackage(Package packages[], int *numPackages): Allows the user to input details for a new package, including name, region, duration, description, itinerary, price, and destinations. The package is added to an array of packages.
deletePackage(Package packages[], int *numPackages): Deletes a package from the array of packages based on the user's input index.
addPackageFromFile(Package packages[], int *numPackages): Prompts the user to input package details and saves them to a CSV file. Destinations associated with the package are also saved.
deletePackageFromFile(int index): Deletes a package from the CSV file based on the provided index.
viewPackagesFromFile(Package packages[], int numPackages): Reads package data from a CSV file and displays it to the user, including details such as name, region, duration, description, itinerary, price, and associated destinations.
savePackagesToFile(Package packages[], int numPackages): Saves package data from an array of packages to a CSV file, including details and associated destinations.
loadPackagesFromFile(Package packages[], int *numPackages): Loads package data from a CSV file into an array of packages.

addFlight(Flight flights[], int *numFlights): Allows the user to input details for a new flight, including airline, flight number, departure, arrival, departure time, arrival time, and cost. The flight is then added to an array of flights.
deleteFlight(Flight flights[], int *numFlights): Deletes a flight from the array of flights based on the user's input index.
addFlightToFile(Flight *flight): Prompts the user to input flight details and saves them to a CSV file.
deleteFlightFromFile(Flight *flight): Deletes a flight from the CSV file based on the provided flight details.
saveFlightsToFile(Flight flights[], int numFlights): Saves flight data from an array of flights to a CSV file.
viewFlightsFromFile(Flight flights[], int numFlights): Reads flight data from a CSV file and displays it to the user.
loadFlightsFromFile(Flight flights[], int *numFlights): Loads flight data from a CSV file into an array of flights.

addHotel(Hotel hotels[], int *numHotels): Allows the user to input details for a new hotel, including name, location, check-in date, check-out date, number of nights, and cost per night. The hotel is then added to an array of hotels.
deleteHotel(Hotel hotels[], int *numHotels): Deletes a hotel from the array of hotels based on the user's input index.
addHotelToFile(Hotel *hotel): Prompts the user to input hotel details and saves them to a CSV file.
deleteHotelFromFile(Hotel *hotel): Deletes a hotel from the CSV file based on the provided hotel details.
saveHotelToFile(Hotel hotels[], int numHotels): Saves hotel data from an array of hotels to a CSV file.
viewHotelFromFile(Hotel hotels[], int numHotels): Reads hotel data from a CSV file and displays it to the user.
loadHotelFromFile(Hotel hotels[], int *numHotels): Loads hotel data from a CSV file into an array of hotels.
*/

// Function to trim leading and trailing whitespace from a string
char *trim(char *str) {
    // Trim leading whitespace
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}


// Function to clear the input buffer
void clearInputBuffer() {
    int c;
    // Read characters from input buffer until newline or EOF is encountered
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to print an error message and exit the program
void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Function to validate a name string
bool validateName(const char *name) {
    // Check if the name is not empty and does not exceed the maximum length
    return strlen(name) > 0 && strlen(name) < MAX_NAME_LENGTH;
}

// Function to validate an email string
bool validateEmail(const char *email) {
    // Check if the email is not empty and does not exceed the maximum length
    if (strlen(email) == 0 || strlen(email) >= MAX_EMAIL_LENGTH) {
        return false;
    }
    // Check if the email contains the '@' symbol
    char *at = strchr(email, '@');
    if (at == NULL || at == email) {
        return false;
    }
    // Check if there is at least one dot '.' after the '@' symbol
    char *dot = strchr(at, '.');
    if (dot == NULL || dot == at + 1) {
        return false;
    }
    // Check if there is at least one character between '@' and '.'
    if (dot - at <= 1) {
        return false;
    }
    return true;
}

// Function to validate a mobile number string
bool validateMobile(const char *mobile) {
    // Check if the mobile number is not empty and does not exceed the maximum length
    return strlen(mobile) > 0 && strlen(mobile) < MAX_MOBILE_LENGTH;
}

// Function to validate a date string
bool validateDate(const char *date) {
    // Check if the date is not empty and does not exceed the maximum length
    return strlen(date) > 0 && strlen(date) < MAX_DATE_LENGTH;
}

// Function to compute the SHA-512 hash
void sha512(const uint8_t *message, size_t len, uint64_t hash[8]) {
    // Initialize variables
    uint64_t a, b, c, d, e, f, g, h;
    uint64_t w[80];
    uint64_t temp1, temp2;
    uint64_t maj, ch;

    // Initialize hash values
    for (int i = 0; i < 8; i++) {
        hash[i] = H[i];
    }

    // Pre-processing
    size_t orig_len = len;
    len += 128; // Padding
    while (len % 128 != 112) len++;
    uint8_t *padded_message = malloc(len + 16);
    memset(padded_message, 0, len + 16);
    memcpy(padded_message, message, orig_len);
    padded_message[orig_len] = 0x80; // Padding: append a single bit (1)

    // Append length of message
    uint64_t bitlen = orig_len * 8;
    memcpy(padded_message + len, &bitlen, 8);

    // Process message in 1024-bit chunks
    for (int i = 0; i < len; i += 128) {
        // Prepare message schedule
        for (int t = 0; t < 16; t++) {
            w[t] = ((uint64_t)padded_message[i + t * 8] << 56) |
                   ((uint64_t)padded_message[i + t * 8 + 1] << 48) |
                   ((uint64_t)padded_message[i + t * 8 + 2] << 40) |
                   ((uint64_t)padded_message[i + t * 8 + 3] << 32) |
                   ((uint64_t)padded_message[i + t * 8 + 4] << 24) |
                   ((uint64_t)padded_message[i + t * 8 + 5] << 16) |
                   ((uint64_t)padded_message[i + t * 8 + 6] << 8) |
                   ((uint64_t)padded_message[i + t * 8 + 7]);
        }
        for (int t = 16; t < 80; t++) {
            w[t] = w[t - 16] + ROTR(w[t - 15], 1) + w[t - 7] + ROTR(w[t - 2], 19);
        }

        // Initialize working variables
        a = hash[0];
        b = hash[1];
        c = hash[2];
        d = hash[3];
        e = hash[4];
        f = hash[5];
        g = hash[6];
        h = hash[7];

        // Compression function main loop
        for (int t = 0; t < 80; t++) {
            temp1 = h + ((ROTR(e, 14)) ^ (ROTR(e, 18)) ^ (ROTR(e, 41))) + ((e & f) ^ ((~e) & g)) + K[t] + w[t];
            temp2 = ((ROTR(a, 28)) ^ (ROTR(a, 34)) ^ (ROTR(a, 39))) + ((a & b) ^ (a & c) ^ (b & c));
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // Update hash values
        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
        hash[5] += f;
        hash[6] += g;
        hash[7] += h;
    }

    // Free memory
    free(padded_message);
}


// Function to generate a random salt
void generateSalt(char *salt) {
    for (int i = 0; i < SALT_SIZE; ++i) {
        salt[i] = 'A' + rand() % 26;
    }
    salt[SALT_SIZE] = '\0';
}

// Function to compute the hash of a password with salt
void computeHash(const char *password, const char *salt, uint64_t hash[8]) {
    char saltedPassword[MAX_NAME_LENGTH + SALT_SIZE + 1];
    strcpy(saltedPassword, password);
    strcat(saltedPassword, salt);

    sha512((const uint8_t *)saltedPassword, strlen(saltedPassword), hash);
}


UserRole authenticateUser(const char *username, const char *password) {
    FILE *file = fopen("User.csv", "r");
    if (file == NULL) {
        printf("Error opening User.csv file\n");
        exit(EXIT_FAILURE);
    }

    UserRole role = INVALID; // Default role

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Tokenize the line to extract username, password, and role
        char *token = strtok(line, ",");
        if (token == NULL) continue; // Skip empty lines

        char storedUsername[MAX_NAME_LENGTH];
        strcpy(storedUsername, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue; // Skip lines with missing fields
        char storedPassword[MAX_NAME_LENGTH];
        strcpy(storedPassword, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue; // Skip lines with missing fields
        char storedRole[MAX_ROLE_LENGTH];
        strcpy(storedRole, token);

        // Compare stored username and password with input
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            if (strcmp(trim(storedRole), "admin") == 0) {
                role = ADMIN;
            } else {
                role = USER;
            }
            break; // Authentication successful, exit loop
        }
    }

    fclose(file);

    return role;
}

// Function to signup
void signup() {
    FILE *usersFile = fopen("User.csv", "a+");
    if (usersFile == NULL) {
        printf("Error opening User.csv file.\n");
        exit(EXIT_FAILURE);
    }

    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];

    printf("\nSignup\n");
    printf("Username: ");
    scanf("%s", username);

    // Check if the username already exists in the file
    rewind(usersFile);
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), usersFile) != NULL) {
        if (strlen(line) <= 1) continue; // Skip empty lines
        char savedUsername[MAX_NAME_LENGTH];
        sscanf(line, "%[^,],", savedUsername);
        if (strcmp(username, savedUsername) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            fclose(usersFile);
            return;
        }
    }

    printf("Password: ");
    scanf("%s", password);

    // Move file pointer to the end of the file
    fseek(usersFile, 0, SEEK_END);

    // Check if the file is empty
    fseek(usersFile, 0, SEEK_END);
    long fileSize = ftell(usersFile);
    if (fileSize > 0) {
        // If file is not empty, add a newline character before appending
        fprintf(usersFile, "\n");
    }

    fprintf(usersFile, "%s,%s,user\n", username, password);
    printf("Signup successful. You can now login.\n");

    fclose(usersFile);
}

// Function to add a new tourist
void addUser(User *user) {
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
}

// Function to delete a tourist
void deleteUser(User *user) {
    printf("Tourist %s deleted successfully.\n", user->name);
    // No action needed since the data will not be persisted after the program exits
}


// Function to add a new tourist to the CSV file
void addUserFromFile(User *user) {
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

    FILE *file = fopen("users.csv", "a"); // Open the CSV file in append mode
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    // Write user details to the CSV file
    fprintf(file, "%s,%s,%s,%d\n", user->name, user->email, user->mobile, user->role);
    fclose(file);
}

// Function to delete a tourist from the CSV file
void deleteUserFromFile(User *user) {
    printf("Tourist %s deleted successfully.\n", user->name);

    FILE *file = fopen("users.csv", "r"); // Open the CSV file in read mode
    FILE *tempFile = fopen("temp_users.csv", "w"); // Open a temporary file for writing
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_EMAIL_LENGTH + MAX_MOBILE_LENGTH + 10];
    char nameToDelete[MAX_NAME_LENGTH];
    // Create a string representing the user's data to delete
    sprintf(nameToDelete, "%s,%s,%s,%d", user->name, user->email, user->mobile, user->role);

    // Read each line from the CSV file
    while (fgets(line, sizeof(line), file)) {
        // Check if the line does not contain the data to delete
        if (strstr(line, nameToDelete) == NULL) {
            // Write the line to the temporary file
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original CSV file
    remove("users.csv");
    // Rename the temporary file to the original CSV file
    rename("temp_users.csv", "users.csv");
}

//function to add destination
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

//function to add destination
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
// Function to add a new destination from user input and save to CSV file
void addDestinationFromUser(Destination *destination) {

    printf("Press enter to continue:");
    getchar();
    
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

    // Collect all places to visit as a single comma-separated string
    printf("Enter places to visit, separated by commas: ");
    char placesToVisit[MAX_INPUT_LENGTH]; // To store places
    fgets(placesToVisit, sizeof(placesToVisit), stdin);
    placesToVisit[strcspn(placesToVisit, "\n")] = '\0'; // Remove trailing newline

    // Copy the combined places to visit into the destination
    strncpy(destination->placesToVisit[0], placesToVisit, sizeof(destination->placesToVisit[0]));


    // Open the CSV file in append mode
    FILE *file = fopen("Destination.csv", "a");
    if (file == NULL) {
        perror("Unable to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Write the destination data to the CSV file with double quotes for safety
    if (fprintf(file, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                destination->name,
                destination->description,
                destination->location,
                destination->placesToVisit,
                destination->bestTimeToVisit) < 0) {
        perror("Error writing to CSV file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// Function to delete a destination from user input and update the CSV file
void deleteDestinationFromUser(Destination *destination) {
    printf("Destination %s deleted successfully.\n", destination->name);

    // Open the CSV file for reading
    FILE *file = fopen("destinations.csv", "r");
    FILE *tempFile = fopen("temp_destinations.csv", "w");
    if (file == NULL || tempFile == NULL) {
        perror("Unable to open files for deletion");
        exit(EXIT_FAILURE);
    }

    char line[MAX_CSV_LINE_LENGTH];
    // Generate the pattern to match the destination data
    char pattern[MAX_CSV_LINE_LENGTH];
    sprintf(pattern, "%s,%s,%s,%s,", destination->name, destination->description,
            destination->location, destination->bestTimeToVisit);

    // Read each line from the CSV file
    while (fgets(line, sizeof(line), file)) {
        // If the line does not contain the destination data, write it to the temporary file
        if (strstr(line, pattern) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original CSV file with the temporary file
    remove("destinations.csv");
    rename("temp_destinations.csv", "destinations.csv");
}


//add destinations from file destinations.csv
void addDestinationFromFile(Destination destinations[], int *numDestinations) {
    FILE *file = fopen("destinations.csv", "r");
    if (file == NULL) {
        error("Unable to open destinations file");
    }

    while ((*numDestinations) < MAX_DESTINATIONS && fscanf(file, "%49[^,],%99[^,],%49[^,],%99[^,],%99[^\n]\n",
               destinations[*numDestinations].name, destinations[*numDestinations].description,
               destinations[*numDestinations].location, destinations[*numDestinations].placesToVisit[0],
               destinations[*numDestinations].bestTimeToVisit) != EOF) {
        (*numDestinations)++;
    }

    fclose(file);
}

//delete destinations from the file destinations.csv
void deleteDestinationFromFile() {
    printf("Press enter to continue:");
    getchar();
    char destinationName[MAX_NAME_LENGTH];
    printf("Enter the name of the destination to delete: ");
    fgets(destinationName, sizeof(destinationName), stdin);
    destinationName[strcspn(destinationName, "\n")] = '\0'; // Remove newline character

    FILE *file = fopen("Destination.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Destination destinations[MAX_DESTINATIONS];
    char line[MAX_LINE_LENGTH];
    int numDestinations = 0;

    // Read and parse CSV file
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^\n]", destinations[numDestinations].name,
               destinations[numDestinations].description, destinations[numDestinations].location,
               destinations[numDestinations].placesToVisit[0], destinations[numDestinations].bestTimeToVisit);
        numDestinations++;
    }
    fclose(file);

    int index = -1;
    // Search for the destination by name
    for (int i = 0; i < numDestinations; i++) {
        if (strcmp(destinations[i].name, destinationName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Destination not found.\n");
        return;
    }

    // Shift elements to overwrite the deleted entry
    for (int i = index; i < numDestinations - 1; i++) {
        strcpy(destinations[i].name, destinations[i + 1].name);
        strcpy(destinations[i].description, destinations[i + 1].description);
        strcpy(destinations[i].location, destinations[i + 1].location);
        strcpy(destinations[i].placesToVisit[0], destinations[i + 1].placesToVisit[0]);
        strcpy(destinations[i].bestTimeToVisit, destinations[i + 1].bestTimeToVisit);
    }
    numDestinations--;

    // Rewrite the file
    file = fopen("Destination.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numDestinations; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n", destinations[i].name, destinations[i].description,
                destinations[i].location, destinations[i].placesToVisit[0], destinations[i].bestTimeToVisit);
    }

    fclose(file);
    printf("Destination deleted successfully.\n");
}

void removeQuotationMarks(char *str);

void viewAllDestinations(Destination destinations[], int numDestinations) {
    FILE *file = fopen("Destination.csv", "r");
    if (file == NULL) {
        printf("No destinations found in the database.\n");
        return;
    }

    // Read and discard the first line (header) of the CSV file
    char header[MAX_CSV_LINE_LENGTH];
    if (fgets(header, sizeof(header), file) == NULL) {
        fclose(file);
        printf("Failed to read the header from the file.\n");
        return;
    }

    printf("List of Destinations:\n");
    printf(" ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-100s | %-20s | %-90s | %-20s |\n", "Name", "Description", "Location", "Places to Visit", "Best Time to Visit");
    printf(" ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", destinations[numDestinations].name, destinations[numDestinations].description, destinations[numDestinations].location, destinations[numDestinations].placesToVisit, destinations[numDestinations].bestTimeToVisit) == 5) {
        printf("| %-20s | %-100s | %-20s | %-90s | %-20s |\n", destinations[numDestinations].name, destinations[numDestinations].description, destinations[numDestinations].location, destinations[numDestinations].placesToVisit, destinations[numDestinations].bestTimeToVisit);
    
        numDestinations++;
    }
    printf(" ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    fclose(file);

    if (numDestinations == 0) {
        printf("No destinations to display.\n");
    }
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


// Function to add a package from user input to a CSV file
void addPackagesFromFile() {
    Package newPackage;
    printf("Press enter to continue:");
    getchar();

    printf("Enter package name: ");
    fgets(newPackage.name, sizeof(newPackage.name), stdin);
    newPackage.name[strcspn(newPackage.name, "\n")] = '\0';

    printf("Enter region: ");
    fgets(newPackage.region, sizeof(newPackage.region), stdin);
    newPackage.region[strcspn(newPackage.region, "\n")] = '\0';

    printf("Enter duration (in nights): ");
    scanf("%d", &newPackage.duration);
    getchar(); // Consume newline character

    printf("Enter description: ");
    fgets(newPackage.description, sizeof(newPackage.description), stdin);
    newPackage.description[strcspn(newPackage.description, "\n")] = '\0';

    printf("Enter itinerary: ");
    fgets(newPackage.itinerary, sizeof(newPackage.itinerary), stdin);
    newPackage.itinerary[strcspn(newPackage.itinerary, "\n")] = '\0';

    printf("Enter price range (approx.): ");
    fgets(newPackage.price, sizeof(newPackage.price), stdin);
    newPackage.price[strcspn(newPackage.price, "\n")] = '\0';

    // Open the CSV file in append mode
    FILE *file = fopen("Packages.csv", "a+");
    if (file == NULL) {
        perror("Unable to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize > 0) {
        // If file is not empty, add a newline character before appending
        fprintf(file, "\n");
    }

    // Write the package data to the CSV file
    if (fprintf(file, "%s,%s,%d,\"%s\",\"%s\",%s",
                newPackage.name,
                newPackage.region,
                newPackage.duration,
                newPackage.description,
                newPackage.itinerary,
                newPackage.price) < 0) {
        perror("Error writing to CSV file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// Function to delete a package from the CSV file
void deletePackageFromFile() {
    char packageName[MAX_PACKAGE_NAME_LENGTH];
    printf("Press enter to continue:");
    getchar();
    printf("Enter the name of the package to delete: ");
    fgets(packageName, sizeof(packageName), stdin);
    packageName[strcspn(packageName, "\n")] = '\0'; // Remove newline character

    FILE *file = fopen("Packages.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Package packages[MAX_PACKAGES];
    char line[MAX_LINE_LENGTH];
    int numPackages = 0;

    // Read and parse CSV file
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^,],%[^,],%d,\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",%[^\n]", 
               packages[numPackages].name, packages[numPackages].region, 
               &packages[numPackages].duration, packages[numPackages].description,
               packages[numPackages].itinerary, packages[numPackages].price,
               packages[numPackages].destinations[0].name);
        numPackages++;
    }
    fclose(file);

    int index = -1;
    // Search for the package by name
    for (int i = 0; i < numPackages; i++) {
        if (strcmp(packages[i].name, packageName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Package not found.\n");
        return;
    }

    // Shift elements to overwrite the deleted entry
    for (int i = index; i < numPackages - 1; i++) {
        strcpy(packages[i].name, packages[i + 1].name);
        strcpy(packages[i].region, packages[i + 1].region);
        packages[i].duration = packages[i + 1].duration;
        strcpy(packages[i].description, packages[i + 1].description);
        strcpy(packages[i].itinerary, packages[i + 1].itinerary);
        strcpy(packages[i].price, packages[i + 1].price);
        strcpy(packages[i].destinations[0].name, packages[i + 1].destinations[0].name);
    }
    numPackages--;

    // Rewrite the file
    file = fopen("Packages.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numPackages; i++) {
        fprintf(file, "%s,%s,%d,\"%s\",\"%s\",\"%s\",%s\n", 
                packages[i].name, packages[i].region, packages[i].duration,
                packages[i].description, packages[i].itinerary, packages[i].price,
                packages[i].destinations[0].name);
    }

    fclose(file);
    printf("Package deleted successfully.\n");
}
// Function to view packages from a CSV file
void viewallpackages(Package packages[], int numPackages) {
    FILE *file = fopen("packages.csv", "r");
    if (file == NULL) {
        printf("No packages found in the database.\n");
        return;
    }

    char line[MAX_CSV_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int i = 0;

        token = strtok(line, ",");
        strcpy(packages[numPackages].name, token);

        token = strtok(NULL, ",");
        strcpy(packages[numPackages].region, token);

        token = strtok(NULL, ",");
        packages[numPackages].duration = atoi(token);

        token = strtok(NULL, ",");
        strcpy(packages[numPackages].description, token);

        token = strtok(NULL, ",");
        strcpy(packages[numPackages].itinerary, token);

        token = strtok(NULL, ",");
        strcpy(packages[numPackages].price, token);

        for (i = 0; i < MAX_DESTINATIONS; i++) {
            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(packages[numPackages].destinations[i].name, token);
            } else {
                break;
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(packages[numPackages].destinations[i].location, token);
            } else {
                break;
            }
        }

        numPackages++;
    }

    fclose(file);

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
                if (strlen(packages[i].destinations[j].name) > 0 && strlen(packages[i].destinations[j].location) > 0) {
                    printf("%d. %s, %s\n", j + 1, packages[i].destinations[j].name, packages[i].destinations[j].location);
                }
            }
        }
    } else {
        printf("No packages to display.\n");
    }
}

void viewAllPackages(package packages[], int numPackages) {
    printf("Press enter to continue:");
    getchar();
    FILE *file = fopen("Packages.csv", "r");
    if (file == NULL) {
        printf("No packages found in the database.\n");
        return;
    }

    // Read and discard the first line (header) of the CSV file
    char header[MAX_CSV_LINE_LENGTH];
    if (fgets(header, sizeof(header), file) == NULL) {
        fclose(file);
        printf("Failed to read the header from the file.\n");
        return;
    }

    printf("List of Packages:\n");
    printf(" ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-40s | %-12s | %-8s | %-141s | %-90s | %-20s | %-40s|\n", "Name", "Region", "Duration", "Description", "Itinerary", "Price Range","Destinations");
    printf(" ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(file, "%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^\n]\n",
                  packages[numPackages].name, packages[numPackages].region, &packages[numPackages].duration,
                  packages[numPackages].description, packages[numPackages].itinerary, packages[numPackages].price,
                  packages[numPackages].destinations) == 7) {
        printf("| %-40s | %-12s | %-8d | %-141s | %-90s | %-20s | %-40s|\n",
               packages[numPackages].name, packages[numPackages].region, packages[numPackages].duration,
               packages[numPackages].description, packages[numPackages].itinerary, packages[numPackages].price, packages[numPackages].destinations);
        numPackages++;
    }
    printf(" ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    fclose(file);

    if (numPackages == 0) {
        printf("No packages to display.\n");
    }
}

// Function to save packages to a CSV file
void savePackagesToFile(Package packages[], int numPackages) {
    FILE *file = fopen("packages.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < numPackages; i++) {
        fprintf(file, "%s,%s,%d,%s,%s,%.2f", packages[i].name, packages[i].region,
                packages[i].duration, packages[i].description, packages[i].itinerary, packages[i].price);

        for (int j = 0; j < MAX_DESTINATIONS; j++) {
            if (strlen(packages[i].destinations[j].name) > 0 && strlen(packages[i].destinations[j].location) > 0) {
                fprintf(file, ",%s,%s", packages[i].destinations[j].name, packages[i].destinations[j].location);
            }
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to load packages from a CSV file
void loadPackagesFromFile(Package packages[], int *numPackages) {
    FILE *file = fopen("packages.csv", "r");
    if (file == NULL) {
        printf("No packages found in the database.\n");
        return;
    }

    while (!feof(file)) {
        fscanf(file, "%[^,],%[^,],%d,%[^,],%[^,],%f,",
               packages[*numPackages].name, packages[*numPackages].region,
               &packages[*numPackages].duration, packages[*numPackages].description,
               packages[*numPackages].itinerary, &packages[*numPackages].price);

        for (int i = 0; i < MAX_DESTINATIONS; i++) {
            fscanf(file, "%[^,],%[^,],",
                   packages[*numPackages].destinations[i].name,
                   packages[*numPackages].destinations[i].location);
        }

        (*numPackages)++;
    }

    fclose(file);
}

void addFlight(Flight flights[], int *numFlights) {
    if (*numFlights < MAX_FLIGHTS) {
        Flight newFlight;
        printf("Enter city: ");
        fgets(newFlight.city, sizeof(newFlight.city), stdin);
        newFlight.city[strcspn(newFlight.city, "\n")] = '\0';
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
        fgets(newFlight.city, sizeof(newFlight.city), stdin);
        newFlight.city[strcspn(newFlight.city, "\n")] = '\0';

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


// Function to add double quotes if a field contains a comma
void addDoubleQuotes(char *input) {
    if (strchr(input, ',') != NULL) {
        size_t length = strlen(input);
        // Shift the string to the right to make space for quotes
        for (int i = length; i >= 0; i--) {
            input[i + 1] = input[i];
        }
        // Add double quotes at the start and end
        input[0] = '\"';
        input[length + 1] = '\"';
        input[length + 2] = '\0';  // Ensure null-termination
    }
}


void addFlightToFile(Flight *flight) {
    printf("Enter city: ");
    fgets(flight->city, sizeof(flight->city), stdin);
    flight->city[strcspn(flight->city, "\n")] = '\0';
    addDoubleQuotes(flight->city);

    printf("Enter airline: ");
    fgets(flight->airline, sizeof(flight->airline), stdin);
    flight->airline[strcspn(flight->airline, "\n")] = '\0';
    addDoubleQuotes(flight->airline);

    printf("Enter flight number: ");
    fgets(flight->flightNumber, sizeof(flight->flightNumber), stdin);
    flight->flightNumber[strcspn(flight->flightNumber, "\n")] = '\0';
    addDoubleQuotes(flight->flightNumber);

    printf("Enter departure: ");
    fgets(flight->departure, sizeof(flight->departure), stdin);
    flight->departure[strcspn(flight->departure, "\n")] = '\0';
    addDoubleQuotes(flight->departure);

    printf("Enter arrival: ");
    fgets(flight->arrival, sizeof(flight->arrival), stdin);
    flight->arrival[strcspn(flight->arrival, "\n")] = '\0';
    addDoubleQuotes(flight->arrival);

    printf("Enter departure time: ");
    fgets(flight->departureTime, sizeof(flight->departureTime), stdin);
    flight->departureTime[strcspn(flight->departureTime, "\n")] = '\0';
    addDoubleQuotes(flight->departureTime);

    printf("Enter arrival time: ");
    fgets(flight->arrivalTime, sizeof(flight->arrivalTime), stdin);
    flight->arrivalTime[strcspn(flight->arrivalTime, "\n")] = '\0';
    addDoubleQuotes(flight->arrivalTime);

    printf("Enter cost: ");
    fgets(flight->cost, sizeof(flight->cost), stdin);
    flight->cost[strcspn(flight->cost, "\n")] = '\0';
    addDoubleQuotes(flight->cost);

    FILE *file = fopen("Flights.csv", "a+");
    if (file == NULL) {
        perror("Unable to open file for writing");
        return;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {  // The first time writing to the file
        fprintf(file, "\n");  // Add a newline character to initialize the CSV
    }

    // Write the flight information to the CSV file
    fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n",
        flight->city, flight->airline, flight->flightNumber, flight->departure, flight->arrival, flight->departureTime, flight->arrivalTime, flight->cost);

    fclose(file);  // Close the file
}

// Function to delete a flight from a CSV file
void deleteFlightFromFile() {
    char flightNumber[MAX_NAME_LENGTH];
    printf("Press enter to continue:");
    getchar();
    printf("Enter the flight number to delete: ");
    fgets(flightNumber, sizeof(flightNumber), stdin);
    flightNumber[strcspn(flightNumber, "\n")] = '\0'; // Remove newline character

    FILE *file = fopen("Flights.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Flight flights[MAX_FLIGHTS];
    char line[MAX_LINE_LENGTH];
    int numFlights = 0;

    // Read and parse CSV file
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]", 
               flights[numFlights].city, flights[numFlights].airline, 
               flights[numFlights].flightNumber, flights[numFlights].departure, 
               flights[numFlights].arrival, flights[numFlights].departureTime, 
               flights[numFlights].arrivalTime, flights[numFlights].cost);
        numFlights++;
    }
    fclose(file);

    int index = -1;
    // Search for the flight by flight number
    for (int i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].flightNumber, flightNumber) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Flight not found.\n");
        return;
    }

    // Shift elements to overwrite the deleted entry
    for (int i = index; i < numFlights - 1; i++) {
        strcpy(flights[i].city, flights[i + 1].city);
        strcpy(flights[i].airline, flights[i + 1].airline);
        strcpy(flights[i].flightNumber, flights[i + 1].flightNumber);
        strcpy(flights[i].departure, flights[i + 1].departure);
        strcpy(flights[i].arrival, flights[i + 1].arrival);
        strcpy(flights[i].departureTime, flights[i + 1].departureTime);
        strcpy(flights[i].arrivalTime, flights[i + 1].arrivalTime);
        strcpy(flights[i].cost, flights[i + 1].cost);
    }
    numFlights--;

    // Rewrite the file
    file = fopen("Flights.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numFlights; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n", flights[i].city, flights[i].airline, 
                flights[i].flightNumber, flights[i].departure, flights[i].arrival, 
                flights[i].departureTime, flights[i].arrivalTime, flights[i].cost);
    }

    fclose(file);
    printf("Flight deleted successfully.\n");
}

// Function to save flights to a CSV file
void saveFlightsToFile(Flight flights[], int numFlights) {
    FILE *file = fopen("Flights.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < numFlights; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n", flights[i].city, flights[i].airline, flights[i].flightNumber,
                flights[i].departure, flights[i].arrival, flights[i].departureTime,
                flights[i].arrivalTime, flights[i].cost);
    }

    fclose(file);
}

// Function to view flights from a CSV file
void viewAllFlights(Flight flights[], int numFlights) {
    FILE *file = fopen("Flights.csv", "r");
    if (file == NULL) {
        printf("No flights found in the database.\n");
        return;
    }

    // Read and discard the first line (header) of the CSV file
    char header[MAX_CSV_LINE_LENGTH];
    if (fgets(header, sizeof(header), file) == NULL) {
        fclose(file);
        printf("Failed to read the header from the file.\n");
        return;
    }

    printf("List of Flights:\n");
    printf(" ------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-35s | %-12s | %-15s | %-10s | %-8s | %-15s | %-15s | %-6s|\n", "City", "Airline", "Flight Number", "Departure", "Arrival", "Departure Time", "Arrival Time", "Cost");
    printf(" ------------------------------------------------------------------------------------------------------------------------------------------\n");


    while(fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", flights[numFlights].city, flights[numFlights].airline, flights[numFlights].flightNumber, flights[numFlights].departure, flights[numFlights].arrival, flights[numFlights].departureTime, flights[numFlights].arrivalTime, flights[numFlights].cost)==8){
        printf("| %-35s | %-12s | %-15s | %-10s | %-8s | %-15s | %-15s | %-6s|\n", flights[numFlights].city, flights[numFlights].airline, flights[numFlights].flightNumber, flights[numFlights].departure, flights[numFlights].arrival, flights[numFlights].departureTime, flights[numFlights].arrivalTime, flights[numFlights].cost);
    
        numFlights++;
    }
    printf(" ------------------------------------------------------------------------------------------------------------------------------------------\n");

    fclose(file);

    if (numFlights == 0) {
        printf("No flights to display.\n");
    }
}


// Function to load flights from a CSV file
void loadFlightsFromFile(Flight flights[], int *numFlights) {
    FILE *file = fopen("Flights.csv", "r");
    if (file == NULL) {
        printf("No flights found in the database.\n");
        return;
    }

    while (!feof(file)) {
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]\n",
               flights[*numFlights].city, flights[*numFlights].airline, flights[*numFlights].flightNumber,
               flights[*numFlights].departure, flights[*numFlights].arrival,
               flights[*numFlights].departureTime, flights[*numFlights].arrivalTime,
               flights[*numFlights].cost);

        (*numFlights)++;
    }

    fclose(file);
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
        printf("Enter cost per night: ");
        fgets(newHotel.costPerNight, sizeof(newHotel.costPerNight), stdin);
        newHotel.costPerNight[strcspn(newHotel.costPerNight, "\n")] = '\0';
        printf("Enter distance from city center: ");
        fgets(newHotel.distance, sizeof(newHotel.distance), stdin);
        newHotel.distance[strcspn(newHotel.distance, "\n")] = '\0';

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
    printf("Press enter to continue:");
    getchar();
    
    // Read hotel name and apply double quotes if needed
    printf("Enter hotel name: ");
    fgets(hotel->name, sizeof(hotel->name), stdin);
    hotel->name[strcspn(hotel->name, "\n")] = '\0'; // Remove trailing newline
    addDoubleQuotes(hotel->name);

    // Read location and apply double quotes if needed
    printf("Enter location: ");
    fgets(hotel->location, sizeof(hotel->location), stdin);
    hotel->location[strcspn(hotel->location, "\n")] = '\0'; // Remove trailing newline
    addDoubleQuotes(hotel->location);

    // Read distance from city center and apply double quotes if needed
    printf("Enter distance from city center: ");
    fgets(hotel->distance, sizeof(hotel->distance), stdin);
    hotel->distance[strcspn(hotel->distance, "\n")] = '\0';
    addDoubleQuotes(hotel->distance);

    // Read cost per night and apply double quotes if needed
    printf("Enter cost per night: ");
    fgets(hotel->costPerNight, sizeof(hotel->costPerNight), stdin);
    hotel->costPerNight[strcspn(hotel->costPerNight, "\n")] = '\0';
    addDoubleQuotes(hotel->costPerNight);

    // Open the CSV file for appending
    FILE *file = fopen("Hotels.csv", "a");
    if (file == NULL) {
        error("Unable to open file for writing.");
        return;
    }

    // Write to the CSV file with double quotes when needed
    fprintf(file, "%s,%s,%s,%s\n", hotel->name, hotel->location, hotel->costPerNight, hotel->distance);

    // Properly close the file
    fclose(file);
}

// Function to delete a hotel
void deleteHotelFromFile(Hotel *hotel) {
    printf("Hotel %s deleted successfully.\n", hotel->name);

    FILE *file = fopen("Hotels.csv", "r");
    FILE *tempFile = fopen("temp_hotels.csv", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_LOCATION_LENGTH + MAX_DATE_LENGTH * 2 + 15];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s,%s,%s,%s", hotel->name, hotel->location, hotel->costPerNight, hotel->distance);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("Hotels.csv");
    rename("temp_hotels.csv", "Hotels.csv");
}


// Function to save hotels to a CSV file
void saveHotelToFile(Hotel hotels[], int numHotels) {
    FILE *file = fopen("Hotels.csv", "w");
    if (file == NULL) {
        error("Error opening file for writing");
    }

    for (int i = 0; i < numHotels; i++) {
        fprintf(file, "%s,%s,%s,%s\n", hotels[i].name, hotels[i].location,
                  hotels[i].costPerNight, hotels[i].distance);
    }

    fclose(file);
}

// Function to view hotels from a CSV file
void viewAllHotels(Hotel hotels[], int numHotels) {
    printf("Press enter to continue:");
    getchar();
    FILE *file = fopen("Hotels.csv", "r");
    if (file == NULL) {
        printf("No hotels found in the database\n");
        return;
    }

    // Read and discard the first line (header) of the CSV file
    char header[100];
    if (fgets(header, sizeof(header), file) == NULL) {
        fclose(file);
        printf("Failed to read the header from the file\n");
        return;
    }

    printf("List of Hotels:\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-40s | %-15s | %-40s |\n", "City", "Hotel Name", "Cost/Night", "Distance from City Center");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n", hotels[numHotels].location, hotels[numHotels].name,
                  hotels[numHotels].costPerNight, hotels[numHotels].distance) == 4) {
        printf("| %-20s | %-40s | %-15s | %-40s |\n", hotels[numHotels].location, hotels[numHotels].name,
               hotels[numHotels].costPerNight, hotels[numHotels].distance);
        numHotels++;
    }
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    fclose(file);
}


// Function to load hotels from a CSV file
void loadHotelFromFile(Hotel hotels[], int *numHotels) {
    FILE *file = fopen("Hotels.csv", "r");
    if (file == NULL) {
        printf("No hotels found in the database\n");
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n", hotels[*numHotels].location, hotels[*numHotels].name,
                  hotels[*numHotels].costPerNight, hotels[*numHotels].distance) == 4) {
        (*numHotels)++;
    }

    fclose(file);
}

// Function to book a flight
void bookFlight(Flight flights[], int numFlights, Booking *booking, int numPeople, const char *bookingDate) {
    // Check if there are flights available
    if (numFlights > 0) {
        // Let's assume we are booking the first flight in the array
        booking->type = FLIGHT_BOOKING; // Set booking type to flight
        booking->bookingObject = &flights[0]; // Set the booking object to the first flight
        booking->numPeople = numPeople; // Set the number of people for the booking
        strncpy(booking->bookingDate, bookingDate, MAX_DATE_LENGTH - 1); // Copy booking date
        booking->bookingDate[MAX_DATE_LENGTH - 1] = '\0'; // Ensure null-termination
        printf("Flight booked successfully!\n");
    } else {
        printf("No flights available for booking.\n");
    }
}

// Function to book a hotel
void bookHotel(Hotel hotels[], int numHotels, Booking *booking, int numPeople, const char *bookingDate) {
    // Check if there are hotels available
    if (numHotels > 0) {
        // Let's assume we are booking the first hotel in the array
        booking->type = HOTEL_BOOKING; // Set booking type to hotel
        booking->bookingObject = &hotels[0]; // Set the booking object to the first hotel
        booking->numPeople = numPeople; // Set the number of people for the booking
        strncpy(booking->bookingDate, bookingDate, MAX_DATE_LENGTH - 1); // Copy booking date
        booking->bookingDate[MAX_DATE_LENGTH - 1] = '\0'; // Ensure null-termination
        printf("Hotel booked successfully!\n");
    } else {
        printf("No hotels available for booking.\n");
    }
}

// Function to book a package
void bookPackage(Package packages[], int numPackages, Booking *booking, int numPeople, const char *bookingDate) {
    // Check if there are packages available
    if (numPackages > 0) {
        // Let's assume we are booking the first package in the array
        booking->type = PACKAGE_BOOKING; // Set booking type to package
        booking->bookingObject = &packages[0]; // Set the booking object to the first package
        booking->numPeople = numPeople; // Set the number of people for the booking
        strncpy(booking->bookingDate, bookingDate, MAX_DATE_LENGTH - 1); // Copy booking date
        booking->bookingDate[MAX_DATE_LENGTH - 1] = '\0'; // Ensure null-termination
        printf("Package booked successfully!\n");
    } else {
        printf("No packages available for booking.\n");
    }
}

/*
// Function to save feedback to file
void saveFeedbackToFile(const char *fileName, Feedback feedback) {
    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        printf("Error opening file %s\n", fileName);
        return;
    }
    fprintf(file, "Type: ");
    switch (feedback.type) {
        case FLIGHT:
            fprintf(file, "Flight\n");
            break;
        case HOTEL:
            fprintf(file, "Hotel\n");
            break;
        case PACKAGE:
            fprintf(file, "Package\n");
            break;
        default:
            fprintf(file, "Unknown\n");
            break;
    }
    fprintf(file, "Comment: %s\n", feedback.comment);
    fprintf(file, "Rating: %d\n", feedback.rating);
    fprintf(file, "------------------------------------\n");
    fclose(file);
}


// Function to provide feedback for flights
void provideFlightFeedback(Flight flights[], int numFlights, const char *fileName) {
    printf("\nProvide feedback for flights:\n");
    for (int i = 0; i < numFlights; i++) {
        printf("Flight %s - %s\n", flights[i].flightNumber, flights[i].airline);
        char comment[MAX_FEEDBACK_LENGTH];
        int rating;
        printf("Enter your comment for this flight: ");
        fgets(comment, sizeof(comment), stdin);
        comment[strcspn(comment, "\n")] = '\0'; // Remove trailing newline
        printf("Enter your rating (1-5) for this flight: ");
        scanf("%d", &rating);
        getchar(); // Consume newline character
        provideFeedback(FLIGHT, &flights[i], comment, rating);
    }
}

// Function to provide feedback for hotels
void provideHotelFeedback(Hotel hotels[], int numHotels, const char *fileName) {
    printf("\nProvide feedback for hotels:\n");
    for (int i = 0; i < numHotels; i++) {
        printf("-> %s\n", hotels[i].name);
        char comment[MAX_FEEDBACK_LENGTH];
        int rating;
        printf("Enter your comment for this hotel: ");
        fgets(comment, sizeof(comment), stdin);
        comment[strcspn(comment, "\n")] = '\0'; // Remove trailing newline
        printf("Enter your rating (1-5) for this hotel: ");
        scanf("%d", &rating);
        getchar(); // Consume newline character
        provideFeedback(HOTEL, &hotels[i], comment, rating);
    }
}

// Function to provide feedback for packages
void providePackageFeedback(Package packages[], int numPackages, const char *fileName) {
    printf("\nProvide feedback for packages:\n");
    for (int i = 0; i < numPackages; i++) {
        printf("-> %s\n", packages[i].description);
        char comment[MAX_FEEDBACK_LENGTH];
        int rating;
        printf("Enter your comment for this package: ");
        fgets(comment, sizeof(comment), stdin);
        comment[strcspn(comment, "\n")] = '\0'; // Remove trailing newline
        printf("Enter your rating (1-5) for this package: ");
        scanf("%d", &rating);
        getchar(); // Consume newline character
        provideFeedback(PACKAGE, &packages[i], comment, rating);
    }
}
*/

// Function to check if the user has already submitted feedback
int hasSubmittedFeedback(const char *username) {
    FILE *file = fopen("Feedbacks.csv", "r");
    if (file == NULL) {
        return 0; // Assume user has not submitted feedback if file doesn't exist
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            fclose(file);
            return 1; // User has already submitted feedback
        }
    }
    
    fclose(file);
    return 0; // User has not submitted feedback
}

// Function to submit feedback
void provideFeedback(const char *username) {
    printf("\nSubmit Your Feedback:\n");

    // Check if user has already submitted feedback
    if (hasSubmittedFeedback(username)) {
        printf("You have already submitted feedback.\n");
        return;
    }

    // Open feedback file in append mode
    FILE *file = fopen("Feedbacks.csv", "a");
    if (file == NULL) {
        printf("Error: Unable to open feedback file.\n");
        return;
    }

    // Input feedback details
    Feedback feedback;
    strcpy(feedback.username, username);
    printf("Enter your feedback: ");
    fgets(feedback.text, sizeof(feedback.text), stdin);
    feedback.text[strcspn(feedback.text, "\n")] = '\0'; // Remove trailing newline
    printf("Rate your experience (out of 5): ");
    scanf("%d", &feedback.rating);
    getchar(); // Consume newline character

    // Write feedback to file in CSV format with comment within quotation marks
    fprintf(file, "\"%s\",\"%s\",%d\n", feedback.username, feedback.text, feedback.rating);

    // Close file
    fclose(file);

    printf("Thank you for your feedback!\n");
}

// Remove leading and trailing quotation marks from a string
void removeQuotationMarks(char *str) {
    if (str[0] == '\"' && str[strlen(str) - 1] == '\"') {
        memmove(str, str + 1, strlen(str)); // Remove leading quotation mark
        str[strlen(str) - 1] = '\0'; // Remove trailing quotation mark
    }
}


// Function to view feedbacks in a tabular format with borders
void viewFeedbacks() {
    printf("\nFeedbacks:\n");

    // Open feedback file in read mode
    FILE *file = fopen("Feedbacks.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open feedback file.\n");
        return;
    }

    // Print top border line
    printf("+---------------------+--------------------------------------------------------------+--------+\n");

    // Print table header
    printf("| %-20s | %-60s | %-6s |\n", "Username", "Feedback", "Rating");

    // Print separator line
    printf("+---------------------+--------------------------------------------------------------+--------+\n");

    // Read and display feedbacks
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *username = strtok(line, ",");
        char *text = strtok(NULL, ","); // Get feedback text
        char *ratingStr = strtok(NULL, ","); // Get rating as string
        int rating = atoi(ratingStr); // Convert rating to integer

        // Remove quotation marks from feedback text
        removeQuotationMarks(text);

        // Print feedback details
        printf("| %-20s | %-60s | %-6d |\n", username, text, rating);
    }

    // Close file
    fclose(file);

    // Print bottom border line
    printf("+---------------------+--------------------------------------------------------------+--------+\n");
}


// Function to create a new booking entry
void createBooking(Booking *booking, BookingType type, void *object, int numPeople, const char *bookingDate, const char *userName, const char *bookingDetail) {
    booking->type = type;
    booking->bookingObject = object; // This could be flight, hotel, or package
    booking->numPeople = numPeople;
    strncpy(booking->bookingDate, bookingDate, MAX_DATE_LENGTH - 1);
    booking->bookingDate[MAX_DATE_LENGTH - 1] = '\0'; // Ensure null-termination
    strncpy(booking->userName, userName, MAX_NAME_LENGTH - 1);
    booking->userName[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    strncpy(booking->bookingDetail, bookingDetail, MAX_DETAIL_LENGTH - 1);
    booking->bookingDetail[MAX_DETAIL_LENGTH - 1] = '\0'; // Ensure null-termination
}

// Function to save the booking to a CSV file with double quotes to handle spaces
void saveBookingToCSV(const char *fileName, Booking *booking) {
    FILE *file = fopen(fileName, "a"); // Open the file in append mode
    if (file == NULL) {
        printf("Error opening the CSV file.\n");
        return;
    }

    fprintf(file, "\"%d\",\"%s\",\"%s\",\"%s\"\n",
            booking->numPeople,
            booking->bookingDate,
            booking->userName,
            booking->bookingDetail);

    fclose(file); // Close the file
}

BookingType getBookingType() {
    int choice;
    printf("Enter booking type (1 = Flight, 2 = Hotel, 3 = Package): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            return FLIGHT_BOOKING;
        case 2:
            return HOTEL_BOOKING;
        case 3:
            return PACKAGE_BOOKING;
        default:
            printf("Invalid choice. Defaulting to Flight booking.\n");
            return FLIGHT_BOOKING;
    }
}

void handleBooking(const char *userName) {
    Booking booking;
    int numPeople;
    char bookingDate[MAX_DATE_LENGTH];
    char bookingDetail[MAX_DETAIL_LENGTH]; // Additional detail like flight number or hotel name

    printf("Enter the number of people: ");
    scanf("%d", &numPeople);

    printf("Enter the booking date (YYYY-MM-DD): ");
    scanf("%10s", bookingDate);

    BookingType bookingType = getBookingType(); // Get booking type from user input

    // Collect additional booking detail based on booking type
    if (bookingType == FLIGHT_BOOKING) {
        printf("Enter the flight number: "); // Ensure input is captured
        scanf(" %[^\n]", bookingDetail); // Capture flight number with spaces
    } else if (bookingType == HOTEL_BOOKING) {
        printf("Enter the hotel name: "); // For hotel booking
        scanf(" %[^\n]", bookingDetail); // Capture hotel name with spaces
    } else if (bookingType == PACKAGE_BOOKING) {
        printf("Enter the package name: "); // For package booking
        scanf(" %[^\n]", bookingDetail); // Capture package name with spaces
    }

    // Create and save the booking
    booking.type = bookingType;
    booking.numPeople = numPeople;
    strncpy(booking.bookingDate, bookingDate, MAX_DATE_LENGTH);
    strncpy(booking.userName, userName, MAX_NAME_LENGTH);
    strncpy(booking.bookingDetail, bookingDetail, MAX_DETAIL_LENGTH);

    saveBookingToCSV("Booking.csv", &booking);

    printf("Booking created and saved to CSV.\n");
}


int main() {
    // Initialize variables
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    UserRole role = INVALID; // Initialize role as invalid

    while (role == INVALID) {
        printf("Welcome to the Travel Management System\n");
        printf("Please login or signup to continue.\n");
        printf("1. Login\n");
        printf("2. Signup\n");
        printf("Enter your choice: ");

        int loginChoice;
        scanf("%d", &loginChoice);
        getchar(); // Consume newline character

        switch (loginChoice) {
            case 1:
                // Login
                printf("Username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0'; // Remove trailing newline
                printf("Password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

                role = authenticateUser(username, password);
                if (role != INVALID) {
                    printf("Welcome %s!\n", (role == ADMIN) ? "Admin" : "User");
                } else {
                    printf("Authentication failed.\n");
                }
                break;
            case 2:
                // Signup
                signup();
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    if (role == USER) {
        // User functionalities
        int choice;
        do {
            printf("\nUser Menu:\n");
            printf("1. View Destinations\n");
            printf("2. View Packages\n");
            printf("3. View Hotels\n");
            printf("4. View Flights\n");
            printf("5. Book Flight or Hotel\n");
            printf("6. Provide Feedback\n");
            printf("7. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                
                case 1:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    Destination destination[MAX_DESTINATIONS];
                    int numDestinations = 0;
                    viewAllDestinations(destination, numDestinations);
                    getchar();
                    break;
                    }
                case 2:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    package packages[MAX_PACKAGES];
                    int numPackages = 0;
                    viewAllPackages(packages, numPackages);
                    getchar();
                    break;
                    }
                case 3:{
                    Hotel hotels[MAX_HOTELS];
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    int numHotels = 0;
                    viewAllHotels(hotels, numHotels);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 4:{
                    Flight flights[MAX_FLIGHTS];
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    int numFlights = 0;
                    viewAllFlights(flights, numFlights);
                    break;
                    }
                case 5:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    handleBooking(username);
                    getchar();
                    break;
                    }
                case 6:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    provideFeedback(username);
                    getchar();
                    break;
                    }
                case 7:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
            }
        } while (choice != 8);
    } else if (role == ADMIN) {
        // Admin functionalities
        int choice;
        do {
            printf("\nAdmin Menu:\n");
            printf("1. Add Destination\n");
            printf("2. View All Destinations\n");
            printf("3. Delete a Destination\n");
            printf("4. Add a Flight\n");
            printf("5. View All Flights\n");
            printf("6. Delete a Flight\n");
            printf("7. Add a Hotel\n");
            printf("8. View All Hotels\n");
            printf("9. Delete a Hotel\n");
            printf("10. Add a Package\n");
            printf("11. View All Packages\n");
            printf("12. Delete a Package\n");
            printf("13. View and Modify Login Credentials\n");
            printf("14. View Feedbacks\n");
            printf("15. View Bookings\n");
            printf("16. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:{
                     Destination destinations;
                     int m;
                     while ((m = getchar()) != '\n' && m != EOF);
                     printf("\n--- Add a Destination ---\n");
   		             addDestinationFromUser(&destinations);
    		         printf("--- Destination '%s' added successfully ---\n", destinations.name);
                     printf("Press enter to continue:");
                     getchar();
                     break;
                    }
                case 2:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    Destination destination[MAX_DESTINATIONS];
                    int numDestinations = 0;
                    viewAllDestinations(destination, numDestinations);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 3:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    deleteDestinationFromFile();
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 4:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    Flight flight[MAX_INPUT_LENGTH];
                    addFlightToFile(flight);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 5:{
                    Flight flights[MAX_FLIGHTS];
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    int numFlights = 0;
                    viewAllFlights(flights, numFlights);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 6:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    deleteFlightFromFile();
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 7:{
                    Hotel hotel[MAX_INPUT_LENGTH];
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    addHotelToFile(hotel);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 8:{
                    Hotel hotels[MAX_HOTELS];
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    int numHotels = 0;
                    viewAllHotels(hotels, numHotels);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                /*
                case 9:
                    deleteHotel();
                    break;
                */
                case 10:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    addPackagesFromFile();
                    printf("--- Package added successfully ---\n");
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 11:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    package packages[MAX_PACKAGES];
                    int numPackages = 0;
                    viewAllPackages(packages, numPackages);
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                case 12:{
                    printf("\n\n");
                    int m;
                    while ((m = getchar()) != '\n' && m != EOF);
                    deletePackageFromFile();
                    printf("Press enter to continue:");
                    getchar();
                    break;
                    }
                /*
                case 13:
                    viewAndModifyCredentials();
                    break;
                */
                case 14:
                    viewFeedbacks();
                    break;
                /*case 15:
                    viewBookings();
                    break;*/
                case 16:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
            }
        } while (choice != 16);
    }

    return 0;
}
