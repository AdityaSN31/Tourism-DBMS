# Project Title: Travel Buddy - Tourism Database Management System

## Project Description
Travel Buddy is a tourism database management system developed in C. It manages tourism-related data and customer bookings.

## Installation
To install Travel Buddy, follow these steps:
1. **Clone the Repository**: Clone the project repository to your local machine.
   ```bash
   git clone https://github.com/AdityaSN31/Tourism-DBMS.git
2. **Compile the Code:** Navigate to the project directory and compile the source code using a C compiler.
   ```bash
   cd travel-buddy
   gcc -o travel_buddy main.c
3.Run the Program: After compiling, execute the compiled binary to run the Travel Buddy program.
   ```bash
   ./travel_buddy
   ```
4. Run the program.

## Usage
Travel Buddy is a command-line interface (CLI) application. The system allows both regular users and administrators to interact with the tourism database management system. Depending on the user role, the available functionalities differ.

### User Role Operations
When logged in as a user, you can perform the following operations:

1. **View Destinations**: Display all available destinations in the system.
2. **View Tour Packages**: Display a list of available tour packages with details like cost and duration.
3. **View Hotels**: Show information about hotels associated with the destinations.
4. **View Flights**: List all available flights in the system.
5. **Book Flights or Hotels**: Allows users to book flights or hotels through the system.
6. **Provide Feedback**: Submit feedback on the system or travel experiences.
7. **Logout**: Exit the user session and return to the main menu.

To use these operations, follow the on-screen prompts. Select the desired menu option, and the system will guide you through the corresponding actions.

### Admin Role Operations
When logged in as an administrator, you can perform the following operations:

1. **Add Destination**: Add a new travel destination to the system.
2. **View All Destinations**: List all destinations in the system.
3. **Delete a Destination**: Remove a specific destination from the system.
4. **Add a Flight**: Add new flight details to the system.
5. **View All Flights**: Display all available flights.
6. **Delete a Flight**: Remove specific flight details from the system.
7. **Add a Hotel**: Add new hotel information to the system.
8. **View All Hotels**: List all hotels in the system.
9. **Delete a Hotel**: Remove specific hotel details.
10. **Add a Package**: Add a new tour package to the system.
11. **View All Packages**: List all available packages.
12. **Delete a Package**: Remove specific packages from the system.
13. **View Feedback**: Display customer feedback received by the system.
14. **View Bookings**: Display all existing bookings in the system (this option might be commented out).
15. **Logout**: Exit the admin session and return to the main menu.

As with the user role, follow the on-screen prompts to select an option and carry out the corresponding operation.

To perform administrative operations, you must authenticate with an admin account. Ensure you have the correct login credentials to access these features.


## Features
Travel Buddy provides a range of features to support tourism database management and customer booking operations. These features are divided into user and admin functionalities.

### User Features
As a regular user, you have access to the following features:

- **View Destinations**: Get a list of all available destinations in the system.
- **View Tour Packages**: View details of tour packages, including cost, duration, and destination.
- **View Hotels**: Access information about hotels in various destinations.
- **View Flights**: See a list of available flights for travel planning.
- **Book Flights or Hotels**: Allows users to book flights and hotels.
- **Provide Feedback**: Users can submit feedback about their travel experience or the system's functionality.
- **Logout**: End the user session.

### Admin Features
As an admin, you have additional control and management capabilities, including:

- **Add Destination**: Add new destinations to the system.
- **View All Destinations**: Get a comprehensive list of destinations in the system.
- **Delete a Destination**: Remove existing destinations.
- **Add Flight**: Add new flight information to the system.
- **View All Flights**: List all available flights.
- **Delete a Flight**: Remove specific flights from the system.
- **Add Hotel**: Add new hotel information.
- **View All Hotels**: List all hotels in the system.
- **Delete a Hotel**: Remove specific hotels from the system.
- **Add Package**: Add new tour packages to the system.
- **View All Packages**: View a complete list of available packages.
- **Delete a Package**: Remove existing packages from the system.
- **View Feedback**: Admins can view all customer feedback.
- **View Bookings**: Access customer bookings (currently commented out in code).
- **Logout**: End the admin session.

These features aim to provide a comprehensive tourism management system with user and admin operations. The user role is designed to facilitate travel planning and booking, while the admin role supports database management and system administration.


## Contributors
- Aditya Nerlekar
- Raj Khedkar
- Neer Modi
- Rudraksh Bavisa

## License
This project is licensed under the MIT License.
