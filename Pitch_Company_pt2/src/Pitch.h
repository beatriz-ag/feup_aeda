#ifndef PITCH_H
#define PITCH_H

#include "Utils.h"
#include "Lift.h"
#include "Client.h"
#include "Employee.h"
#include "bst.h"
#include <queue>
#include <unordered_set>
#include <fstream>

using namespace std;

typedef priority_queue<ServiceProviderType> heapEmployeesSP;
typedef priority_queue<ContractedEmployeeType> heapEmployeesCE;

struct ClientHash {
    int operator()(const Client *c) const {
        int v = 0;
        for (int i = 0; i < 1; i++) {
            v = 37 * v + c->getNif();
        }
        return v;
    }

    bool operator()(const Client *c1, const Client *c2) const {
        return c1->getNif() == c2->getNif();
    }
};

typedef unordered_set<Client *, ClientHash, ClientHash> HashTableClient;

/**
 * @class Pitch
 */
class Pitch {
    unsigned int nif = 555555555;
    string clientsFile; /** file from which will be loaded and to which will be saved the clients information */
    string employeesFile; /** file from which will be loaded and to which will be saved the employees information */
    string liftsFile; /** file from which will be loaded and to which will be saved the lifts information */

    HashTableClient clients; /** hash table with all clients organized by their NIFs */

    vector<Lift *> lifts; /** vector of pointers to the lifts owned by the Pitch's clients */
    vector<Lift *> ownedLifts; /** vector of pointer to lifts owned by Pitch */

    BST<MaintenanceRecord> maintenances; /** binary search tree of all maintenances done by Pitch */

    vector<Employee *> employees; /** vector of pointers to the Pitch employees */
    heapEmployeesSP serviceProviders; /** priority_queue of all Pitch service providers */
    heapEmployeesCE contractedEmployees; /** priority_queue of all Pitch contracted employees */


public:
    /**
     * Constructor for class Pitch
     * @param clientsFile has default value "clients.txt"
     * @param employeesFile has default value "employees.txt"
     * @param liftsFile has default value "lifts.txt"
     */
    Pitch(string clientsFile = "clients.txt", string employeesFile = "employees.txt", string liftsFile = "lifts.txt");

    /**
     * Loads from clientsFile the information about clients
     */
    void loadClients();

    /**
     * Loads from employeesFile the information about employees
     */
    void loadEmployees();

    /**
     * Loads from liftsFile the information about lifts
     */
    void loadLifts();

    /**
     * Saves to clientsFile the information about clients
     */
    void saveClients();

    /**
     * Saves to employeesFile the information about employees
     */
    void saveEmployees();

    /**
     * Saves to liftsFile the information about lifts
     */
    void saveLifts();

    /**
     * Saves information concerning the lifts, clients and employees to the correspondent files
     */
    void saveData();

    /**
     * Searches for a certain client given a NIF
     * @param searchedNIF NIF of the client to be searched
     * @return reference to the client searched
     * @throws ClientDiesNotExist
     */
    Client *searchClient(unsigned int searchedNIF);

    /**
      * Searches for a certain employee given a NIF
      * @param searchedNIF NIF of the employee to be searched
      * @return reference to the employee searched
      * @throws EmployeeDoesNotExist
      */
    Employee *searchEmployee(unsigned int searchedNIF);

    /**
     * Searches for the employee associated to the client whose NIF is clientNIF
     * @param clientNIF NIF of the client whose associated employee is to be obtained
     * @return reference to the employee searched
     * @throws ClientDoesNotHaveAnEmployeeAssociated
     * @throws NotAnEntrepreneur
     */
    Employee *searchClientsEmployee(unsigned int clientNIF);

    /**
     * Searches for a certain lift given a code
     * @param searchedCode code of the lift to be searched
     * @return reference to the lift searched
     * @throws LiftDoesNotExist
     */
    Lift *searchLift(unsigned int searchedCode);

    /**
     * Allows to know whether or not an employee exists
     * @param nif NIF of the searched employee
     * @return true, if exception is not thrown, meaning the employee exists, false otherwise
     * @throw EmployeeDoesNotExist
     */
    bool employeeExists(unsigned int nif);

    /**
     * Adds a client to vector clients
     * @param toAdd pointer to the client to be added
     */
    void addClient(Client *toAdd);

    void addServiceProvider(ServiceProviderType spt);

    void addContractedEmployee(ContractedEmployee *e);

    /**
     * Adds an employee to vector employees, if a service provider, adds him to serviceProviders as well or, if a contracted employee, adds him to contractedEmployees as well
     * @param name name of the employee to be added
     * @param nif NIF of the employee to be added
     * @param age age of the employee to be added
     * @param function function of the employee to be added
     * @param salary salary of the employee to be added
     */
    void addEmployee(string name, unsigned int nif, unsigned int age, double salary);

    /**
     * Adds a lift to vector lifts
     * @param toAdd pointer to the lift to be added
     */
    void addLift(Lift *toAdd);

    void addMaintenance(Maintenance *maintenance);

    /**
    * @return NIF of the Pitch Company itself
    */
    unsigned int getNif() const;

    /**
     * @return number of employees currently in vector employees
     */
    unsigned int getNumEmployees() const;

    /**
     * @return number of lifts currently in vector lifts
     */
    unsigned int getNumLift() const;

    /**
     * @return vector lifts
     */
    vector<Lift *> getLifts();


    /**
     * Schedules the asked maintenance to the next available sp
     */
    bool scheduleMaintenance(Lift *lift, Time schedule, Date today);

    void updateMaintenances(Time actualTime, Date actualDate);

    /**
     * @return pointer to the most reviewed employee, either a service provider or a contracted employee
     */
    Employee *getMostReviewedEmployee();

    /**
     * @return pointer to the least reviewed employee, either a service provider or a contracted employee
     */
    Employee *getLeastReviewedEmployee();

    /**
     * @return pointer to the employee with the highest evaluation, either a service provider or a contracted employee
     */
    Employee *getBestEvaluatedEmployee();

    /**
     * @return pointer to the employee with the lowest evaluation, either a service provider or a contracted employee
     */
    Employee *getWorstEvaluatedEmployee();

    /**
     * @return pointer to the employee with less clients associated, either a service provider or a contracted employee
     */
    Employee *getEmployeeWithLessClientsAssociated();

    /**
     * @return pointer to the client who has purchased his first lift at the longest time ago
     */
    Client *getOldestClient();

    /**
     * @return pointer to the client who has purchased his first lift most recently
     */
    Client *getMostRecentClient();

    /**
     * @return pointer to the client who has the highest number of lifts purchased
     */
    Client *getClientWithMostLiftsPurchased();

    /**
     * @param l pointer to the lift from which the associated client is to be obtained
     * @return pointer to the client associated with the lift given as parameter
     */
    Client *getClientAssociatedToLift(Lift *l);

    /**
     * @param e pointer to the entrepreneur whose associated employee is to be obtained
     * @return pointer to the associated employee of the entrepreneur given as parameter
     */
    ContractedEmployee *getEmployeeAssociatedToEntrepreneur(Entrepreneur *e);

    /**
     * Removes an employee from the vector employees
     * if the employee to be removed is of type ContractedEmployee, his associated clients are automatically associated to the employee that has less clients associated
     * @param nif NIF of the employee to be removed
     * @throws EmployeeDoesNotExist
     */
    void temporarilyRemoveEmployee(unsigned int nif);

    /**
     *
     * @param nif NIF of the employee to be removed
     */
    void removeEmployee(unsigned int nif);

    /**
     * Removes an employee from the vector employees
     * if the client to be removed is of type entrepreneur, meaning he has a contracted employee associated, the first is also removed from the last's vector
     * if the client to be removed has any lifts on his name, those lifts are transfered to Pitch Company's name
     * @param nif
     */
    void removeClient(unsigned int nif);

    /**
     * Associates to the client given as parameter the employee that has currently less clients associated
     * @param c pointer to the client the employee will be associated to
     * @throws NotAnEntrepreneur
     */
    void associateEmployeeToClient(Client *c);

    /**
     * Reviews service provider after the maintenance is executed
     * @param reviewed service provider to be reviewed
     * @param eva evaluation given by the client
     */
    void reviewMaintenance(ServiceProvider *reviewed, unsigned int eva);

    /**
     * Reviews contracted employee
     * @param reviewed contracted employee to be reviewed
     * @param eva evaluation given by the client
     */
    void reviewContractedEmployee(ContractedEmployee *reviewed, unsigned int eva);

    /**
     * Shows on the screen the information about the lift given as parameter
     * @param toShow pointer to the lift to be shown
     */
    void showLift(Lift *toShow);

    /*
     * Executes maintenance without being scheduled by user
     * @param lift to excute maintenance on
     * @param Today to register date
     * @param time to register time
     */
    void automaticMaintenance(Lift *lift, Date Today, Time time);

    /**
     * Shows on the screen the information concerning all the clients of Pitch
     */
    void showAllClients();

    /**
     * Shows on the screen the information concerning all the employees of Pitch
     */
    void showAllEmployees();

    /**
     * Shows on the screen the information concerning all the lifts of Pitch
     */
    void showAllLifts();

    /**
     * Shows on the screen the most reviewed employee
     */
    void showMostReviewedEmployee();

    /**
     * Shows on the screen the least reviewed employee
     */
    void showLeastReviewedEmployee();

    /**
     * Shows on the screen the best evaluated employee
     */
    void showBestEvaluatedEmployee();

    /**
     * Shows on the screen the worst evaluated employee
     */
    void showWorstEvaluatedEmployee();

    /**
     * Shows on the screen the client who has purchased his first lift at the longest time ago
     */
    void showOldestClient();

    /**
     * Shows on screen the client who has purchased his first lift most recently
     */
    void showMostRecentClient();

    /**
     * Shows on the screen the client who has the highest number of lifts purchased
     */
    void showClientWithMostLiftsPurchased();

    /**
     * Shows on the screen the client associated with the lift given as parameter
     * @param l pointer to the lift of which the associated client is to be shown
     */
    void showClientAssociatedToLift(Lift *l);

    /**
     * Shows on the screen the installation date of the lift given as parameter
     * @param l pointer to the lift of which the installation date is to be shown
     */
    void showLiftInstallDate(Lift *l);

    /**
     * Shows on the screen the last move of the lift given as parameter
     * @param l pointer to the lift of which the last move is to be shown
     */
    void showLiftLastMove(Lift *l);

    /**
     * Shows on the screen the last maintenance of the lift given as parameter
     * @param l pointer to the lift of which the last maintenance is to be shown
     */
    void showLiftLastMaintenance(Lift *l);

    /**
     * Shows on the screen the the maintenance history of the lift given as parameter
     * @param l pointer to the lift of which the maintenance history is to be shown
     */
    void showLiftMaintenanceHist(Lift *l);

    /**
     * Registers a given by parameter lift's move
     * @param lift lift of which move will be registered
     * @param op2 go to floor
     * @param today current date
     * @param now current time
     * @return 0
     */
    int registerMove(Lift &lift, int op2, Date today, Time now);
};


#endif
