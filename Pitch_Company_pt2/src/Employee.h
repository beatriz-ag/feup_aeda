#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Client.h"
#include <queue>
#include <stack>

using namespace std;

typedef priority_queue<Maintenance *> heapMaint;

/**
 * @class Employee
 */
class Employee {
protected:
    string name;
    unsigned int nif;
    unsigned int age;
    string function;
    unsigned int evaluation; /** can be 1, 2, 3, 4, or 5 */
    unsigned int numReviews; /** number of reviews given to the employee */
public:
    Employee(string name, unsigned int nif, unsigned int age);

    virtual ~Employee() = default;

    /**
     * @return string containing the relevant information about the employee
     */
    virtual string show();

    /**
     * Sends to the output file stream given as parameter information about the employee respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    virtual void printToFile(ofstream &op);

    /**
     * @return NIF of the employee
     */
    unsigned int getNif() const;

    /**
     * @return evaluation of the employee
     */
    unsigned int getEvaluation() const;

    /**
     * @return number of reviews given to the employee
     */
    unsigned int getNumOfReviews() const;

    /**
     * Sets the evaluation and the number of reviews of the employee to be equals to the ones given as parameters
     * @param eva evaluation
     * @param nR number of reviews
     */
    void setOldReviews(unsigned int eva, unsigned nR);

    /**
     * Reviews the employee by updating his evaluation and his number of reviews
     * @param punctuation
     */
    void review(unsigned int punctuation);

    virtual bool operator<(Employee *comp);

    virtual bool operator==(Employee *comp);
};


/**
 * @class ServiceProvider
 */
class ServiceProvider : public Employee {
private:
    double perHourSalary;
    double hoursWorked;
    unsigned int numMaintenances;
    heapMaint schedule;
public:
    ServiceProvider(string name, unsigned int nif, unsigned int age, double perHourSalary,
                    unsigned numMaintenances = 0);

    ~ServiceProvider() = default;

    /**
     * @return string containing the relevant information about the service provider
     */
    string show();

    /**
     * Sends to the output file stream given as parameter information about the service provider respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream &op);

    /**
     * @return total salary, given the per hour salary and the number of hours worked
     */
    double getSalary() const;

    /**
     * @return number of maintenances in schedule
     */
    unsigned int getNumAppointments() const;

    /**
     * @return schedule
     */
    heapMaint getSchedule() const;

    /**
     *
     * @return nr of maintenances done
     */
    int getNumMaintenancesDone() const;

    /**
     * Sets the number of hours worked of the service provider to be equal to the value given as parameter
     * @param h number of hours
     */
    void setHoursWorked(double h);

    /**
     * Sets the number of maintenances executed by the service provider to be equal to the value given as parameter
     * @param num number of maintenances executed
     */
    void setNumMaintenances(unsigned int num);

    /**
     * Tries to schedule a maintenance, returning true if possible, meaning the service provider was available, otherwise returns false and nothing happens
     */
    bool scheduleMaintenance(Maintenance *m);

    /**
     * Executes a maintenance request by updating the number of hours worked, the number of maintenances done and by removing that appointment from the schedule
     */
    void executeMaintenance();

    bool operator<(Employee *comp);

    bool operator==(Employee *comp);
};


/**
 * @class ServiceProviderType
 */
class ServiceProviderType {
private:
    ServiceProvider *sp;
public:
    ServiceProviderType(string name, unsigned int nif, unsigned int age, double perHourSalary,
                        unsigned numMaintenances = 0);

    ServiceProviderType(ServiceProvider *sp);

    ServiceProvider *getServiceProvider() const;

    bool operator<(const ServiceProviderType toCmp) const;

    bool operator==(const ServiceProviderType toCmp) const;
};


/**
 * @class ContractedEmployee
 */
class ContractedEmployee : public Employee {
private:
    double perMonthSalary;
    vector<Client *> clientsAssociated;
public:
    ContractedEmployee(string name, unsigned int nif, unsigned int age, double perMonthSalary);

    ~ContractedEmployee() = default;

    /**
     * @return string containing the relevant information about the contracted employee
     */
    string show();

    /**
     * Sends to the output file stream given as parameter information about the contracted employee respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream &op);

    /**
     * @return number of clients associated with the contracted employee
     */
    unsigned int getNumClientsAssociated() const;

    /**
     * @return vector containing all the clients associated to the contracted employee
     */
    vector<Client *> getClientsAssociated();

    /**
     * Associates the client given as parameter to the contracted employee
     * @param c pointer to the client to be associated to the contracted employee
     */
    void associateToClient(Client *c);

    /**
     * Removes the client given as parameter of the vector of clients associated
     * @param rm pointer to the client to be removed
     */
    void removeClient(Client *rm);

    bool operator<(Employee *comp);

    bool operator==(Employee *comp);
};


/**
 * @class ContractedEmployeeType
 */
class ContractedEmployeeType {
private:
    ContractedEmployee *ce;
public:
    ContractedEmployeeType(string name, unsigned int nif, unsigned int age, double perMonthSalary);

    ContractedEmployeeType(ContractedEmployee *ce);

    ContractedEmployee *getContractedEmployee() const;

    bool operator<(const ContractedEmployeeType toCmp) const;

    bool operator==(const ContractedEmployeeType toCmp) const;
};


/** output operator for objects of the class Employee */
ostream &operator<<(ostream &out, Employee *e);

/** functor to sort the employees by the number of reviews */
bool sortEmployeesByNumReviews(Employee *e1, Employee *e2);

/** functor to sort the employees by the number of clients associated */
bool sortEmployeesByNumClientsAssociated(Employee *e1, Employee *e2);

#endif
