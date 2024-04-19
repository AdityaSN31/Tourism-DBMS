#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_MOBILE_LENGTH 15
#define MAX_DESCRIPTION_LENGTH 100
#define MAX_LOCATION_LENGTH 50
#define MAX_PLACES_TO_VISIT 5
#define MAX_DATE_LENGTH 20
#define MAX_REVIEW_LENGTH 200
#define MAX_PACKAGE_NAME_LENGTH 100
#define MAX_REGION_LENGTH 50
#define MAX_ITINERARY_LENGTH 50
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
} Booking;

// Enum to represent feedback types
typedef enum {
    FLIGHT,
    HOTEL,
    PACKAGE
} FeedbackType;

// Define a structure for feedback
typedef struct {
    FeedbackType type;
    void *object;
    char comment[MAX_FEEDBACK_LENGTH];
    int rating;
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


// Function to authenticate user
bool authenticateUser(const char *username, const char *password) {
    // Here, you would typically retrieve the salt and hash associated with the username
    // For simplicity, we'll use hardcoded values
    char storedSalt[SALT_SIZE + 1] = "RandomSalt123";
    char storedHash[HASH_SIZE + 1] = "c8497363bf0f012fd730ce9d15505e3d0f50f05636780c67c2ec16dfc22ab7d";


    char saltedPassword[MAX_NAME_LENGTH + SALT_SIZE + 1];
    char computedHash[HASH_SIZE + 1];

    // Concatenate password and salt
    strcpy(saltedPassword, password);
    strcat(saltedPassword, storedSalt);

    // Compute the hash
    uint64_t digest[8];
    sha512((unsigned char *)saltedPassword, strlen(saltedPassword), digest);

    // Convert hash to string
    for (int i = 0; i < 8; ++i) {
        sprintf(&computedHash[i * 2], "%02x", (unsigned int)digest[i]);
    }

    // Compare computed hash with stored hash
    if (strcmp(computedHash, storedHash) == 0) {
        return true; // Authentication successful
    } else {
        return false; // Authentication failed
    }
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

    // Open the CSV file in append mode
    FILE *file = fopen("destinations.csv", "a");
    if (file == NULL) {
        perror("Unable to open file for writing");
        exit(EXIT_FAILURE);
    }
    // Write the destination data to the CSV file
    fprintf(file, "%s,%s,%s,%s,", destination->name, destination->description,
            destination->location, destination->bestTimeToVisit);
    for (int i = 0; i < MAX_PLACES_TO_VISIT; ++i) {
        fprintf(file, "%s,", destination->placesToVisit[i]);
    }
    fprintf(file, "\n");
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
void deleteDestinationFromFile(const char *destinationName) {
    FILE *file = fopen("destinations.csv", "r");
    FILE *tempFile = fopen("temp_destinations.csv", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_DESCRIPTION_LENGTH + MAX_LOCATION_LENGTH +
              MAX_DESCRIPTION_LENGTH * MAX_PLACES_TO_VISIT + MAX_DESCRIPTION_LENGTH + 10];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s,", destinationName);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("destinations.csv");
    rename("temp_destinations.csv", "destinations.csv");
}

// Function to view all destinations in the CSV file
void viewDestinationsFromFile() {
    // Open the destinations CSV file for reading
    FILE *file = fopen("destinations.csv", "r");
    if (file == NULL) {
        error("Unable to open destinations file");
    }

    // Print column headers for destinations
    printf("Destinations:\n");
    printf("%-20s | %-50s | %-20s | %-50s | %-50s\n", "Name", "Description", "Location", "Places to Visit", "Best Time to Visit");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    char line[MAX_NAME_LENGTH + MAX_DESCRIPTION_LENGTH + MAX_LOCATION_LENGTH +
              MAX_DESCRIPTION_LENGTH * MAX_PLACES_TO_VISIT + MAX_DESCRIPTION_LENGTH + 10];
    
    // Read each line from the file and print destination details
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH], description[MAX_DESCRIPTION_LENGTH], location[MAX_LOCATION_LENGTH], 
             placesToVisit[MAX_PLACES_TO_VISIT][MAX_DESCRIPTION_LENGTH], bestTimeToVisit[MAX_DESCRIPTION_LENGTH];

        // Extract data from CSV line
        sscanf(line, "%49[^,],%99[^,],%49[^,],%99[^,],%99[^\n]\n", 
               name, description, location, placesToVisit[0], bestTimeToVisit);

        // Print destination details
        printf("%-20s | %-50s | %-20s | %-50s | %-50s\n", name, description, location, placesToVisit[0], bestTimeToVisit);
    }

    // Close the file
    fclose(file);
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
void addPackageFromFile(Package packages[], int *numPackages) {
    if (*numPackages < MAX_PACKAGES) {
        FILE *file = fopen("packages.csv", "a");
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }

        Package newPackage;
        printf("Enter package name: ");
        fgets(newPackage.name, sizeof(newPackage.name), stdin);
        newPackage.name[strcspn(newPackage.name, "\n")] = '\0'; // Remove trailing newline
        printf("Enter region: ");
        fgets(newPackage.region, sizeof(newPackage.region), stdin);
        newPackage.region[strcspn(newPackage.region, "\n")] = '\0'; // Remove trailing newline
        printf("Enter duration (in days): ");
        scanf("%d", &newPackage.duration);
        while (getchar() != '\n'); // Clear input buffer
        printf("Enter description: ");
        fgets(newPackage.description, sizeof(newPackage.description), stdin);
        newPackage.description[strcspn(newPackage.description, "\n")] = '\0'; // Remove trailing newline
        printf("Enter itinerary: ");
        fgets(newPackage.itinerary, sizeof(newPackage.itinerary), stdin);
        newPackage.itinerary[strcspn(newPackage.itinerary, "\n")] = '\0'; // Remove trailing newline
        printf("Enter price: ");
        scanf("%f", &newPackage.price);
        while (getchar() != '\n'); // Clear input buffer

        printf("Enter destinations (name and location):\n");
        for (int i = 0; i < MAX_DESTINATIONS; i++) {
            printf("Destination %d:\n", i + 1);
            printf("Name: ");
            fgets(newPackage.destinations[i].name, sizeof(newPackage.destinations[i].name), stdin);
            newPackage.destinations[i].name[strcspn(newPackage.destinations[i].name, "\n")] = '\0'; // Remove trailing newline
            printf("Location: ");
            fgets(newPackage.destinations[i].location, sizeof(newPackage.destinations[i].location), stdin);
            newPackage.destinations[i].location[strcspn(newPackage.destinations[i].location, "\n")] = '\0'; // Remove trailing newline
        }

        fprintf(file, "%s,%s,%d,%s,%s,%.2f",
                newPackage.name, newPackage.region, newPackage.duration,
                newPackage.description, newPackage.itinerary, newPackage.price);
        for (int i = 0; i < MAX_DESTINATIONS; i++) {
            if (strlen(newPackage.destinations[i].name) > 0 && strlen(newPackage.destinations[i].location) > 0) {
                fprintf(file, ",%s,%s", newPackage.destinations[i].name, newPackage.destinations[i].location);
            }
        }
        fprintf(file, "\n");

        fclose(file);

        (*numPackages)++;
        printf("Package added successfully.\n");
    } else {
        printf("Maximum number of packages reached. Cannot add more packages.\n");
    }
}

// Function to delete a package from the CSV file
void deletePackageFromFile(int index) {
    FILE *file = fopen("packages.csv", "r");
    FILE *tempFile = fopen("temp_packages.csv", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening files for deletion.\n");
        return;
    }

    char line[MAX_CSV_LINE_LENGTH];
    int lineNumber = 0;
    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        if (lineNumber != index) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("packages.csv");
    rename("temp_packages.csv", "packages.csv");

    printf("Package deleted successfully.\n");
}

// Function to view packages from a CSV file
void viewPackagesFromFile(Package packages[], int numPackages) {
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
        packages[numPackages].price = atof(token);

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

// Function to add a new flight (for admin) to a CSV file
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

    FILE *file = fopen("flights.csv", "a");
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    fprintf(file, "%s,%s,%s,%s,%s,%s,%.2f\n", flight->airline, flight->flightNumber, flight->departure,
            flight->arrival, flight->departureTime, flight->arrivalTime, flight->cost);
    fclose(file);
}

// Function to delete a flight from a CSV file
void deleteFlightFromFile(Flight *flight) {
    printf("Flight %s %s deleted successfully.\n", flight->airline, flight->flightNumber);

    FILE *file = fopen("flights.csv", "r");
    FILE *tempFile = fopen("temp_flights.csv", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_NAME_LENGTH + MAX_LOCATION_LENGTH * 2 + MAX_DATE_LENGTH * 2 + 20];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s,%s,%s,%s,%s,%s,%.2f", flight->airline, flight->flightNumber, flight->departure, flight->arrival, flight->departureTime, flight->arrivalTime, flight->cost);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("flights.csv");
    rename("temp_flights.csv", "flights.csv");
}


// Function to save flights to a CSV file
void saveFlightsToFile(Flight flights[], int numFlights) {
    FILE *file = fopen("flights.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < numFlights; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%.2f\n", flights[i].airline, flights[i].flightNumber,
                flights[i].departure, flights[i].arrival, flights[i].departureTime,
                flights[i].arrivalTime, flights[i].cost);
    }

    fclose(file);
}

// Function to view flights from a CSV file
void viewFlightsFromFile(Flight flights[], int numFlights) {
    FILE *file = fopen("flights.csv", "r");
    if (file == NULL) {
        printf("No flights found in the database.\n");
        return;
    }

    char line[MAX_CSV_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int i = 0;

        token = strtok(line, ",");
        strcpy(flights[numFlights].airline, token);

        token = strtok(NULL, ",");
        strcpy(flights[numFlights].flightNumber, token);

        token = strtok(NULL, ",");
        strcpy(flights[numFlights].departure, token);

        token = strtok(NULL, ",");
        strcpy(flights[numFlights].arrival, token);

        token = strtok(NULL, ",");
        strcpy(flights[numFlights].departureTime, token);

        token = strtok(NULL, ",");
        strcpy(flights[numFlights].arrivalTime, token);

        token = strtok(NULL, ",");
        flights[numFlights].cost = atof(token);

        numFlights++;
    }

    fclose(file);

    if (numFlights > 0) {
        printf("List of Flights:\n");
        for (int i = 0; i < numFlights; i++) {
            printf("\nFlight %d:\n", i + 1);
            printf("Airline: %s\n", flights[i].airline);
            printf("Flight Number: %s\n", flights[i].flightNumber);
            printf("Departure: %s\n", flights[i].departure);
            printf("Arrival: %s\n", flights[i].arrival);
            printf("Departure Time: %s\n", flights[i].departureTime);
            printf("Arrival Time: %s\n", flights[i].arrivalTime);
            printf("Cost: %.2f\n", flights[i].cost);
        }
    } else {
        printf("No flights to display.\n");
    }
}

// Function to load flights from a CSV file
void loadFlightsFromFile(Flight flights[], int *numFlights) {
    FILE *file = fopen("flights.csv", "r");
    if (file == NULL) {
        printf("No flights found in the database.\n");
        return;
    }

    while (!feof(file)) {
        fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%f\n",
               flights[*numFlights].airline, flights[*numFlights].flightNumber,
               flights[*numFlights].departure, flights[*numFlights].arrival,
               flights[*numFlights].departureTime, flights[*numFlights].arrivalTime,
               &flights[*numFlights].cost);

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

    FILE *file = fopen("hotels.csv", "a");
    if (file == NULL) {
        error("Unable to open file for writing");
    }
    fprintf(file, "%s,%s,%s,%s,%d,%.2f\n", hotel->name, hotel->location, hotel->checkInDate,
            hotel->checkOutDate, hotel->numNights, hotel->costPerNight);
    fclose(file);
}

// Function to delete a hotel
void deleteHotelFromFile(Hotel *hotel) {
    printf("Hotel %s deleted successfully.\n", hotel->name);

    FILE *file = fopen("hotels.csv", "r");
    FILE *tempFile = fopen("temp_hotels.csv", "w");
    if (file == NULL || tempFile == NULL) {
        error("Unable to open files for deletion");
    }

    char line[MAX_NAME_LENGTH + MAX_LOCATION_LENGTH + MAX_DATE_LENGTH * 2 + 15];
    char nameToDelete[MAX_NAME_LENGTH];
    sprintf(nameToDelete, "%s,%s,%s,%s,%d,%.2f", hotel->name, hotel->location, hotel->checkInDate, hotel->checkOutDate, hotel->numNights, hotel->costPerNight);

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, nameToDelete) == NULL) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("hotels.csv");
    rename("temp_hotels.csv", "hotels.csv");
}


// Function to save hotels to a CSV file
void saveHotelToFile(Hotel hotels[], int numHotels) {
    FILE *file = fopen("hotels.csv", "w");
    if (file == NULL) {
        error("Error opening file for writing");
    }

    for (int i = 0; i < numHotels; i++) {
        fprintf(file, "%s,%s,%s,%s,%d,%.2f\n", hotels[i].name, hotels[i].location,
                hotels[i].checkInDate, hotels[i].checkOutDate, hotels[i].numNights, hotels[i].costPerNight);
    }

    fclose(file);
}

// Function to view hotels from a CSV file
void viewHotelFromFile(Hotel hotels[], int numHotels) {
    FILE *file = fopen("hotels.csv", "r");
    if (file == NULL) {
        error("No hotels found in the database");
        return;
    }

    printf("List of Hotels:\n");
    printf("%-20s%-20s%-15s%-15s%-10s%-10s\n", "Name", "Location", "Check-in Date", "Check-out Date", "Nights", "Cost/Night");
    printf("-----------------------------------------------------------------\n");
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%d,%f\n", hotels[numHotels].name, hotels[numHotels].location,
                  hotels[numHotels].checkInDate, hotels[numHotels].checkOutDate, &hotels[numHotels].numNights, &hotels[numHotels].costPerNight) != EOF) {
        printf("%-20s%-20s%-15s%-15s%-10d%-10.2f\n", hotels[numHotels].name, hotels[numHotels].location,
               hotels[numHotels].checkInDate, hotels[numHotels].checkOutDate, hotels[numHotels].numNights, hotels[numHotels].costPerNight);
        numHotels++;
    }

    fclose(file);
}

// Function to load hotels from a CSV file
void loadHotelFromFile(Hotel hotels[], int *numHotels) {
    FILE *file = fopen("hotels.csv", "r");
    if (file == NULL) {
        error("No hotels found in the database");
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%d,%f\n", hotels[*numHotels].name, hotels[*numHotels].location,
                  hotels[*numHotels].checkInDate, hotels[*numHotels].checkOutDate, &hotels[*numHotels].numNights, &hotels[*numHotels].costPerNight) != EOF) {
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

// Function to provide feedback
void provideFeedback(FeedbackType type, void *object, char *comment, int rating) {
    Feedback feedback;

    // Initialize feedback
    feedback.type = type;
    feedback.object = object;
    strncpy(feedback.comment, comment, MAX_FEEDBACK_LENGTH - 1);
    feedback.comment[MAX_FEEDBACK_LENGTH - 1] = '\0'; // Ensure null-terminated string
    feedback.rating = rating;

    // Save feedback to file
    saveFeedbackToFile("feedback.txt", feedback);

    // Process feedback as needed
    printf("Feedback provided:\n");
    switch (type) {
        case FLIGHT:
            printf("Type: Flight\n");
            printf("Comment: %s\n", feedback.comment);
            printf("Rating: %d\n", feedback.rating);
            // Additional processing specific to flight feedback
            break;
        case HOTEL:
            printf("Type: Hotel\n");
            printf("Comment: %s\n", feedback.comment);
            printf("Rating: %d\n", feedback.rating);
            // Additional processing specific to hotel feedback
            break;
        case PACKAGE:
            printf("Type: Package\n");
            printf("Comment: %s\n", feedback.comment);
            printf("Rating: %d\n", feedback.rating);
            // Additional processing specific to package feedback
            break;
        default:
            printf("Unknown feedback type.\n");
            break;
    }
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




int main() {
    // Initialize variables
    int choice;
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    UserRole role;
    Flight flights[MAX_FLIGHTS]; // Declare an array to store flights
    int numFlights = 0; // Variable to keep track of the number of flights
    Hotel hotels[MAX_HOTELS]; // Declare an array to store hotels
    int numHotels = 0; // Variable to keep track of the number of hotels
    Package packages[MAX_PACKAGES]; // Declare an array to store packages
    int numPackages = 0; // Variable to keep track of the number of packages
    Destination destinations[MAX_DESTINATIONS]; // Declare an array to store destinations
    int numDestinations = 0; // Variable to keep track of the number of destinations
    Booking booking; // Assuming Booking is a struct type defined somewhere in your code
    int numPeople;
    char bookingDate[MAX_DATE_LENGTH]; // Assuming MAX_DATE_LENGTH is defined somewhere in your code



    // Authenticate user
    printf("Welcome to the Travel Management System\n");
    printf("Please login to continue.\n");
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove trailing newline
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

    // Authenticate user based on username and password
    if (authenticateUser(username, password)) {
        // If authentication successful, assign role
        if (strcmp(username, "admin") == 0) {
            role = ADMIN;
            printf("Welcome Admin!\n");
        } else {
            role = USER;
            printf("Welcome User!\n");
        }
    } else {
        printf("Authentication failed. Exiting...\n");
        exit(1);
    }

    // User Interface
    while (1) {
        // Display options based on role
        if (role == ADMIN) {
            printf("\nAdmin Menu:\n");
            printf("1. Manage Flights\n");
            printf("2. Manage Hotels\n");
            printf("3. Manage Packages\n");
            printf("4. Manage Destinations\n");
            printf("5. Exit\n");
        } else if (role == USER) {
            printf("\nUser Menu:\n");
            printf("1. View Flights\n");
            printf("2. View Hotels\n");
            printf("3. View Packages\n");
            printf("4. View Destinations\n");
            printf("5. Book Flight/Hotel/Package\n");
            printf("6. Provide Feedback\n");
            printf("7. Exit\n");
        }

        // Get user choice
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        // Perform actions based on choice
        switch (choice) {
            case 1:
                if (role == ADMIN) {
                    int adminChoice;
                    printf("\nAdmin Flight Management Menu:\n");
                    printf("1. Add Flight\n");
                    printf("2. Delete Flight\n");
                    printf("3. View Flights\n");
                    printf("4. Exit\n");
            
                    // Get admin's choice for flight management
                    printf("Enter your choice: ");
                    scanf("%d", &adminChoice);
                    getchar(); // Consume newline character
            
                    switch (adminChoice) {
                        case 1:
                            // Add new flight
                            addFlight(flights, &numFlights);
                            saveFlightsToFile(flights, numFlights); // Save flights to file after addition
                            break;
                        case 2:
                            // Delete flight
                            deleteFlight(flights, &numFlights);
                            saveFlightsToFile(flights, numFlights); // Save flights to file after deletion
                            break;
                        case 3:
                            // View flights
                            viewFlightsFromFile(flights, numFlights);
                            break;
                        case 4:
                            printf("Exiting Admin Flight Management...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                } else if (role == USER) {
                    printf("\nView Flights Menu:\n");
                    printf("1. View All Flights\n");
                    printf("2. Exit\n");
            
                    int userChoice;
                    printf("Enter your choice: ");
                    scanf("%d", &userChoice);
                    getchar(); // Consume newline character
            
                    switch (userChoice) {
                        case 1:
                            // View all flights
                            viewFlightsFromFile(flights, numFlights);
                            break;
                        case 2:
                            printf("Exiting View Flights Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 2:
                if (role == ADMIN) {
                    int adminHotelChoice;
                    printf("\nAdmin Hotel Management Menu:\n");
                    printf("1. Add Hotel\n");
                    printf("2. Delete Hotel\n");
                    printf("3. View Hotels\n");
                    printf("4. Exit\n");
            
                    // Get admin's choice for hotel management
                    printf("Enter your choice: ");
                    scanf("%d", &adminHotelChoice);
                    getchar(); // Consume newline character
            
                    switch (adminHotelChoice) {
                        case 1:
                            // Add new hotel
                            addHotel(hotels, &numHotels);
                            saveHotelToFile(hotels, numHotels); // Save hotels to file after addition
                            break;
                        case 2:
                            // Delete hotel
                            deleteHotel(hotels, &numHotels);
                            saveHotelToFile(hotels, numHotels); // Save hotels to file after deletion
                            break;
                        case 3:
                            // View hotels
                            viewHotelFromFile(hotels, numHotels);
                            break;
                        case 4:
                            printf("Exiting Admin Hotel Management...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                } else if (role == USER) {
                    printf("\nView Hotels Menu:\n");
                    printf("1. View All Hotels\n");
                    printf("2. Exit\n");
            
                    int userHotelChoice;
                    printf("Enter your choice: ");
                    scanf("%d", &userHotelChoice);
                    getchar(); // Consume newline character
            
                    switch (userHotelChoice) {
                        case 1:
                            // View all hotels
                            viewHotelFromFile(hotels, numHotels);
                            break;
                        case 2:
                            printf("Exiting View Hotels Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 3:
                if (role == ADMIN) {
                    int adminPackageChoice;
                    printf("\nAdmin Package Management Menu:\n");
                    printf("1. Add Package\n");
                    printf("2. Delete Package\n");
                    printf("3. View Packages\n");
                    printf("4. Exit\n");
            
                    // Get admin's choice for package management
                    printf("Enter your choice: ");
                    scanf("%d", &adminPackageChoice);
                    getchar(); // Consume newline character
            
                    switch (adminPackageChoice) {
                        case 1:
                            // Add new package
                            addPackage(packages, &numPackages);
                            savePackagesToFile(packages, numPackages); // Save packages to file after addition
                            break;
                        case 2:
                            // Delete package
                            deletePackage(packages, &numPackages);
                            savePackagesToFile(packages, numPackages); // Save packages to file after deletion
                            break;
                        case 3:
                            // View packages
                            viewPackagesFromFile(packages, numPackages);
                            break;
                        case 4:
                            printf("Exiting Admin Package Management...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                } else if (role == USER) {
                    printf("\nView Packages Menu:\n");
                    printf("1. View All Packages\n");
                    printf("2. Exit\n");
            
                    int userPackageChoice;
                    printf("Enter your choice: ");
                    scanf("%d", &userPackageChoice);
                    getchar(); // Consume newline character
            
                    switch (userPackageChoice) {
                        case 1:
                            // View all packages
                            viewPackagesFromFile(packages, numPackages);
                            break;
                        case 2:
                            printf("Exiting View Packages Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 4:
                if (role == ADMIN) {
                    int adminDestinationChoice;
                    printf("\nAdmin Destination Management Menu:\n");
                    printf("1. Add Destination\n");
                    printf("2. Delete Destination\n");
                    printf("3. View Destinations\n");
                    printf("4. Exit\n");
            
                    // Get admin's choice for destination management
                    printf("Enter your choice: ");
                    scanf("%d", &adminDestinationChoice);
                    getchar(); // Consume newline character
            
                    switch (adminDestinationChoice) {
                        case 1:
                            // Add new destination
                            addDestinationFromUser(destinations); 
                            break;
                        case 2:
                            // Delete destination
                            deleteDestinationFromUser(destinations);
                            break;
                        case 3:
                            // View destinations
                            viewDestinationsFromFile();
                            break;
                        case 4:
                            printf("Exiting Admin Destination Management...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                } else if (role == USER) {
                    printf("\nView Destinations Menu:\n");
                    printf("1. View All Destinations\n");
                    printf("2. Exit\n");
            
                    int userDestinationChoice;
                    printf("Enter your choice: ");
                    scanf("%d", &userDestinationChoice);
                    getchar(); // Consume newline character
            
                    switch (userDestinationChoice) {
                        case 1:
                            // View all destinations
                            viewDestinationsFromFile();
                            break;
                        case 2:
                            printf("Exiting View Destinations Menu...\n");
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 5:
                if (role == USER) {
                    int userBookingChoice;
                    printf("\nBooking Menu:\n");
                    printf("1. Book a Flight\n");
                    printf("2. Book a Hotel\n");
                    printf("3. Book a Package\n");
                    printf("4. Exit\n");
            
                    printf("Enter your choice: ");
                    scanf("%d", &userBookingChoice);
                    getchar(); // Consume newline character
            
                    switch (userBookingChoice) {
                      case 1:
                          // Book a flight
                          bookFlight(flights, numFlights, &booking, numPeople, bookingDate);
                          break;
                      case 2:
                          // Book a hotel
                          bookHotel(hotels, numHotels, &booking, numPeople, bookingDate);
                          break;
                      case 3:
                          // Book a package
                          bookPackage(packages, numPackages, &booking, numPeople, bookingDate);
                          break;
                      case 4:
                          printf("Exiting Booking Menu...\n");
                          break;
                      default:
                          printf("Invalid choice. Please try again.\n");
                  }
                  break;



            case 6:
                if (role == USER) {
                    int userFeedbackChoice;
                    printf("\nFeedback Menu:\n");
                    printf("1. Provide Feedback for a Flight\n");
                    printf("2. Provide Feedback for a Hotel\n");
                    printf("3. Provide Feedback for a Package\n");
                    printf("4. Exit\n");

                    printf("Enter your choice: ");
                    scanf("%d", &userFeedbackChoice);
                    getchar(); // Consume newline character

                    switch (userFeedbackChoice) {
                      case 1:
                          // Provide feedback for a flight
                          provideFlightFeedback(flights, numFlights, "flight_feedback.txt");
                          break;
                      case 2:
                          // Provide feedback for a hotel
                          provideHotelFeedback(hotels, numHotels, "hotel_feedback.txt");
                          break;
                      case 3:
                          // Provide feedback for a package
                          providePackageFeedback(packages, numPackages, "package_feedback.txt");
                          break;
                      case 4:
                          printf("Exiting Feedback Menu...\n");
                          break;
                      default:
                          printf("Invalid choice. Please try again.\n");
                  }
                }
                break;


            case 7:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
  }
}
